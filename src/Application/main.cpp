#include "GameUtil/GameUtil.h"

class GameCore:GameUtil::Game{
private:
    GameUtil::SpriteRenderer *Renderer;
    GameUtil::GameObject *Player;
    const GLfloat PLAYER_VELOCITY = 500.0f;

private:
    std::vector<GameUtil::GameLevel> Levels;
    GLuint Level;

public:
    GameCore(GLuint width, GLuint height, std::string resourcePath)
    : Game(width,height,resourcePath){}

    void Initialize()
    {
        Game::SourceManager->LoadShader(
            GetResourceFilePath("shaders/spriteShader.vs").c_str(),
            GetResourceFilePath("shaders/spriteShader.fs").c_str(),
            nullptr, "sprite");
        // 配置着色器
        glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(this->Width),
                                          static_cast<GLfloat>(this->Height), 0.0f, -1.0f, 1.0f);
        SourceManager->GetShader("sprite").Use().setInt("image", 0);
        SourceManager->GetShader("sprite").setMat4("projection", projection);
        // 设置专用于渲染的控制
        Renderer = new GameUtil::SpriteRenderer(SourceManager->GetShader("sprite"));

        // 加载纹理
        SourceManager->LoadTexture(GetResourceFilePath("textures/awesomeface.png").c_str(), GL_TRUE, "face");
        SourceManager->LoadTexture(GetResourceFilePath("textures/background.jpg").c_str(), false, "background");
        SourceManager->LoadTexture(GetResourceFilePath("textures/block.png").c_str(), false, "block");
        SourceManager->LoadTexture(GetResourceFilePath("textures/block_solid.png").c_str(), false, "block_solid");
        SourceManager->LoadTexture(GetResourceFilePath("textures/paddle.png").c_str(), true, "paddle");

        // 初始化并加载角色
        const glm::vec2 PLAYER_SIZE(100.0f, 20.0f);

        glm::vec2 playerPos = glm::vec2(
            this->Width / 2.0f - PLAYER_SIZE.x / 2.0f,
            this->Height - PLAYER_SIZE.y);
        Player = new GameUtil::GameObject(playerPos, PLAYER_SIZE, SourceManager->GetTexture("paddle"));

        // 加载关卡
        GameUtil::GameLevel one;
        one.Load(GetResourceFilePath("levels/standard.lv").c_str(), this->Width, this->Height / 2,SourceManager);
        GameUtil::GameLevel two;
        two.Load(GetResourceFilePath("fewSmallGaps.lv").c_str(), this->Width, this->Height / 2,SourceManager);
        GameUtil::GameLevel three;
        three.Load(GetResourceFilePath("levels/spaceInvader.lv").c_str(), this->Width, this->Height / 2,SourceManager);
        GameUtil::GameLevel four;
        four.Load(GetResourceFilePath("levels/bounceGalore.lv").c_str(), this->Width, this->Height / 2,SourceManager);
        this->Levels.push_back(one);
        this->Levels.push_back(two);
        this->Levels.push_back(three);
        this->Levels.push_back(four);
        this->Level = 0;
    }

    void Update(GLfloat dt)
    {
    }

    void ProcessInput(GLfloat dt)
    {
        if (this->State == GameUtil::GameState::GAME_ACTIVE)
        {
            float velocity = PLAYER_VELOCITY * dt;
            // move playerboard
            if (this->Keys[GLFW_KEY_A])
            {
                if (Player->Position.x >= 0.0f)
                    Player->Position.x -= velocity;
            }
            if (this->Keys[GLFW_KEY_D])
            {
                if (Player->Position.x <= this->Width - Player->Size.x)
                    Player->Position.x += velocity;
            }
        }
    }

    // 渲染
    void Render()
    {
        if (this->State == GameUtil::GameState::GAME_ACTIVE)
        {
            Renderer->DrawSprite(Game::SourceManager->GetTexture("background"),
                                 glm::vec2(0.0f, 0.0f), glm::vec2(this->Width, this->Height), 0.0f);
            // 绘制关卡
            this->Levels[this->Level].Draw(*Renderer);

            // 绘制角色
            Player->Draw(*Renderer);
        }
    }
};

GameUtil::Game *GameUtil::GetGame()
{
    return (GameUtil::Game*)(new GameCore(800, 600, "G:/my_program/cpp/Breakout/resource/"));
}

GameUtil::GameWindow* GameUtil::GetGameWindow(GameUtil::Game *game)
{
    return (GameUtil::GameWindow *)(new GameUtil::GlfwGameWindow(800, 600, "Breakout", game));
}
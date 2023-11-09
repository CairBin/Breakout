#include "GameUtil/GameUtil.h"
#include "Game/Ball.h"

class GameCore:GameUtil::Game{
private:
    const GLfloat PLAYER_VELOCITY = 500.0f; //用户的初始速度
    const GLfloat BALL_RADIUS = 12.5f;      //球的半径
    const glm::vec2 INITIAL_BALL_VELOCITY =
         glm::vec2(100.0f, -350.0f);        //球的初始速度

    GameUtil::SpriteRenderer *Renderer;
    GameUtil::GameObject *Player;
    Breakout::Ball *BallObj;

private:
    std::vector<GameUtil::GameLevel> Levels;
    GLuint Level;

private:
    void DoCollisions(){
        for(GameUtil::GameObject& box:this->Levels[this->Level].Bricks){
            if(!box.Destroyed){
                GameUtil::Collision::CollisionResult coll = CheckCollision(*BallObj, box);
                if(std::get<0>(coll)){
                    if (!box.IsSolid)
                        box.Destroyed = GL_TRUE;

                    GameUtil::DirectState2d dir = std::get<1>(coll);
                    glm::vec2 diffVector = std::get<2>(coll);
                    if (dir == GameUtil::DirectState2d::LEFT || dir==GameUtil::DirectState2d::RIGHT)
                    {
                        BallObj->Velocity.x = -BallObj->Velocity.x;
                        GLfloat penetration = BallObj->Radius - std::abs(diffVector);
                        if(dir==GameUtil::DirectState2d::LEFT)
                            BallObj->Position.x += penetration;
                        else
                            BallObj->Position.x -= penetration;
                    }else{
                        BallObj->Velocity.y = -BallObj->Velocity.y;
                        GLfloat peneration = BallObj->Radius - std::abs(diffVector.y);
                        if(dir==GameUtil::DirectState2d::UP)
                            BallObj->Position.y -= peneration;
                        else
                            BallObj->Position.y += peneration;
                    }
                }
            }
        }

        GameUtil::Collision::CollisionResult result = CheckCollision(*BallObj, *Player);
        if(!BallObj->Stuck && std::get<0>(result)){
            GLfloat centerBoard = Player->Position.x + Player->Size.x / 2;
            GLfloat distance = (BallObj->Position.x + BallObj->Radius) - centerBoard;
            GLfloat percentage = distance / (Player->Size.x / 2);
            // 依据结果移动
            GLfloat strength = 2.0f;
            glm::vec2 oldVelocity = BallObj->Velocity;
            BallObj->Velocity.x = INITIAL_BALL_VELOCITY.x * percentage * strength;
            BallObj->Velocity.y = -1*abs(BallObj->Velocity.y);
            BallObj->Velocity = glm::normalize(BallObj->Velocity) * glm::length(oldVelocity);
        }
    }

    GameUtil::DirectState2d VectorDirection(glm::vec2 target)
    {
        glm::vec2 compass[] = {
            glm::vec2(0.0f, 1.0f),
            glm::vec2(1.0f, 0.0f),
            glm::vec2(0.0f, -1.0f),
            glm::vec2(-1.0f, 0.0f)};
        GLfloat max = 0.0f;
        GLuint bestMatch = -1;
        for (GLuint i = 0; i < 4;i++){
            GLfloat dotProduct = glm::dot(glm::normalize(target), compass[i]);
            if(dotProduct>max){
                max = dotProduct;
                bestMatch = i;
            }

        }

        return (GameUtil::DirectState2d)bestMatch;
    }

    GameUtil::Collision::CollisionResult CheckCollision(Breakout::Ball& a,GameUtil::GameObject& b)
    {
        auto clamp = [](GLfloat value, GLfloat min, GLfloat max)
        {
            return std::max(min, std::min(max, value));
        };

        glm::vec2 center(a.Position + a.Radius);
        glm::vec2 aabb_half_extents(b.Size.x / 2, b.Size.y / 2);
        glm::vec2 aabb_center(
            b.Position.x + aabb_half_extents.x,
            b.Position.y + aabb_half_extents.y);

        glm::vec2 difference = center - aabb_center;
        glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);
        glm::vec2 closest = aabb_center + clamped;
        difference = closest - center;
    
        if(glm::length(difference)<a.Radius)
            return std::make_tuple(GL_TRUE, VectorDirection(difference), difference);

        return std::make_tuple(GL_FALSE, GameUtil::DirectState2d::UP, glm::vec2(0, 0));
    }

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
        two.Load(GetResourceFilePath("levels/fewSmallGaps.lv").c_str(), this->Width, this->Height / 2, SourceManager);
        GameUtil::GameLevel three;
        three.Load(GetResourceFilePath("levels/spaceInvader.lv").c_str(), this->Width, this->Height / 2,SourceManager);
        GameUtil::GameLevel four;
        four.Load(GetResourceFilePath("levels/bounceGalore.lv").c_str(), this->Width, this->Height / 2,SourceManager);
        this->Levels.push_back(one);
        this->Levels.push_back(two);
        this->Levels.push_back(three);
        this->Levels.push_back(four);
        this->Level = 0;


        //初始化小球
        glm::vec2 ballPos = playerPos + glm::vec2(PLAYER_SIZE.x / 2 - BALL_RADIUS,
                                                   -BALL_RADIUS * 2);
        BallObj = new Breakout::Ball(ballPos,
            BALL_RADIUS,
            INITIAL_BALL_VELOCITY,
            SourceManager->GetTexture("face")
        );

    }

    void Update(GLfloat dt)
    {
        //移动小球
        BallObj->Move(dt, this->Width);

        //碰撞检测
        this->DoCollisions();
        if(BallObj->Position.y >= this->Height){
            
        }
    }

    void ProcessInput(GLfloat dt)
    {
        if (this->State == GameUtil::GameState::GAME_ACTIVE)
        {
            float velocity = PLAYER_VELOCITY * dt;
            
            if (this->Keys[GLFW_KEY_A])
            {
                if (Player->Position.x >= 0.0f)
                    Player->Position.x -= velocity;
                if (BallObj->Stuck)
                    BallObj->Position.x -= velocity;
            }
            if (this->Keys[GLFW_KEY_D])
            {
                if (Player->Position.x <= this->Width - Player->Size.x)
                    Player->Position.x += velocity;
                if (BallObj->Stuck)
                    BallObj->Position.x += velocity;
            }

            if (this->Keys[GLFW_KEY_SPACE])
                BallObj->Stuck = false;
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
            BallObj->Draw(*Renderer);
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
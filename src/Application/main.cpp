#include "GameUtil/GameUtil.h"
#include "Game/Ball.h"
#include "Game/PowerUp.h"
#include <algorithm>

class GameCore : GameUtil::Game
{
private:
    const GLfloat PLAYER_VELOCITY = 500.0f; // 用户的初始速度
    const GLfloat BALL_RADIUS = 12.5f;      // 球的半径
    const glm::vec2 INITIAL_BALL_VELOCITY =
        glm::vec2(100.0f, -350.0f);         // 球的初始速度
    const glm::vec2 PLAYER_SIZE = glm::vec2(100.0f, 20.0f); //用户板子大小

    //游戏对象指针
    GameUtil::SpriteRenderer *Renderer;
    GameUtil::GameObject *Player;
    Breakout::Ball *BallObj;
    GameUtil::ParticleGenerator *Particles;
    GameUtil::PostProcessor *Effects;

private:
    std::vector<GameUtil::GameLevel> Levels;    //关卡
    std::vector<Breakout::PowerUp> PowerUps;    //道具
    GLuint Level;                               //当前关卡
    GLfloat ShakeTime = 0.0f;                   //震动事件（当撞击实心方块触发）

public:
    static GLboolean ShouldSpawn(GLuint chance)
    {
        GLuint random = rand() % chance;
        return random == 0;
    }

private:
    void SpawnPowerUps(GameUtil::GameObject &block)
    {
        if (ShouldSpawn(75))
            this->PowerUps.push_back(
                Breakout::PowerUp(Breakout::PowerUpType::SPEED, glm::vec3(0.5f, 0.5f, 1.0f), 0.0f, block.Position, SourceManager->GetTexture("powerup_speed")));
        if (ShouldSpawn(75))
            this->PowerUps.push_back(Breakout::PowerUp(Breakout::PowerUpType::STICKY, glm::vec3(1.0f, 0.5f, 1.0f), 20.0f, block.Position, SourceManager->GetTexture("powerup_sticky")));
        if (ShouldSpawn(75))
            this->PowerUps.push_back(Breakout::PowerUp(Breakout::PowerUpType::PASS_THROUGH, glm::vec3(0.5f, 1.0f, 0.5f), 10.0f, block.Position, SourceManager->GetTexture("powerup_passthrough")));
        if (ShouldSpawn(75))
            this->PowerUps.push_back(Breakout::PowerUp(Breakout::PowerUpType::PAD_SIZE_INCREASE, glm::vec3(1.0f, 0.6f, 0.4), 0.0f, block.Position, SourceManager->GetTexture("powerup_increase")));
        if (ShouldSpawn(15)) 
            this->PowerUps.push_back(Breakout::PowerUp(Breakout::PowerUpType::CONFUSE, glm::vec3(1.0f, 0.3f, 0.3f), 15.0f, block.Position, SourceManager->GetTexture("powerup_confuse")));
        if (ShouldSpawn(15))
            this->PowerUps.push_back(Breakout::PowerUp(Breakout::PowerUpType::CHAOS, glm::vec3(0.9f, 0.25f, 0.25f), 15.0f, block.Position, SourceManager->GetTexture("powerup_chaos")));

    }

    GLboolean IsOtherPowerUpActive(std::vector<Breakout::PowerUp> &powerUps, Breakout::PowerUpType type)
    {
        for (const Breakout::PowerUp &powerUp : powerUps)
        {
            if (!powerUp.Activated)
                continue;
            if (powerUp.Type == type)
                return GL_TRUE;
        }

        return GL_FALSE;
    }
    void UpdatePowerUps(GLfloat dt)
    {
        for(Breakout::PowerUp &powerUp:PowerUps){
            powerUp.Position += powerUp.Velocity * dt;
            if(!powerUp.Activated)
                continue;
            powerUp.Duration -= dt;
            if(powerUp.Duration>0.0f)
                continue;

            powerUp.Activated = false;
            if(powerUp.Type == Breakout::PowerUpType::STICKY){
                if(!IsOtherPowerUpActive(this->PowerUps,Breakout::PowerUpType::STICKY)){
                    BallObj->Sticky = false;
                    Player->Color = glm::vec3(1.0f);
                }
                continue;
            }

            if(powerUp.Type == Breakout::PowerUpType::PASS_THROUGH){
                if (!IsOtherPowerUpActive(this->PowerUps, Breakout::PowerUpType::PASS_THROUGH))
                {
                    BallObj->PassThrough = false;
                    Player->Color = glm::vec3(1.0f);
                }

                continue;
            }

            if(powerUp.Type == Breakout::PowerUpType::CONFUSE){
                if(!IsOtherPowerUpActive(this->PowerUps,Breakout::PowerUpType::CONFUSE)){
                    Effects->Confuse = false;
                }
                continue;
            }

            if(powerUp.Type == Breakout::PowerUpType::CHAOS){
                if (!IsOtherPowerUpActive(this->PowerUps, Breakout::PowerUpType::CHAOS))
                    Effects->Chaos = false;
                continue;
            }

        }

        this->PowerUps.erase(
            std::remove_if(
                this->PowerUps.begin(), this->PowerUps.end(),
                [](const Breakout::PowerUp &powerUp)
                { return powerUp.Destroyed && !powerUp.Activated; }),
            this->PowerUps.end());
    }

    void ActivatePowerUp(Breakout::PowerUp &powerUp)
    {
        if (powerUp.Type == Breakout::PowerUpType::SPEED)
        {
            BallObj->Velocity *= 1.2;
        }
        else if (powerUp.Type == Breakout::PowerUpType::STICKY)
        {
            BallObj->Sticky = GL_TRUE;
            Player->Color = glm::vec3(1.0f, 0.5f, 1.0f);
        }
        else if (powerUp.Type == Breakout::PowerUpType::PASS_THROUGH)
        {
            BallObj->PassThrough = GL_TRUE;
            BallObj->Color = glm::vec3(1.0f, 0.5f, 0.5f);
        }
        else if (powerUp.Type == Breakout::PowerUpType::PAD_SIZE_INCREASE)
        {
            Player->Size.x += 50;
        }
        else if (powerUp.Type == Breakout::PowerUpType::CONFUSE)
        {
            if (!Effects->Chaos)
                Effects->Confuse = GL_TRUE;
        }
        else if (powerUp.Type == Breakout::PowerUpType::CHAOS)
        {
            if (!Effects->Confuse)
                Effects->Chaos = GL_TRUE;
        }
    }

    void Resetlevel()
    {
        if (this->Level == 0)
            Levels[0].Load(GetResourceFilePath("levels/standard.lv").c_str(), this->Width, this->Height * 0.5f, SourceManager);
        else if (this->Level == 1)
            Levels[1].Load(GetResourceFilePath("levels/fewSmallGaps.lv").c_str(), this->Width, this->Height * 0.5f, SourceManager);
        else if (this->Level == 2)
            Levels[2].Load(GetResourceFilePath("levels/spaceInvader.lv").c_str(), this->Width, this->Height / 2, SourceManager);
        else if (this->Level == 3)
            Levels[3].Load(GetResourceFilePath("levels/bounceGalore.lv").c_str(), this->Width, this->Height / 2, SourceManager);
    }

    void ResetPlayer()
    {
        Player->Size = PLAYER_SIZE;
        Player->Position = glm::vec2(this->Width / 2 - PLAYER_SIZE.x / 2,
                                     this->Height - PLAYER_SIZE.y);
        BallObj->Reset(Player->Position + glm::vec2(PLAYER_SIZE.x / 2 - BALL_RADIUS, -(BALL_RADIUS * 2)), INITIAL_BALL_VELOCITY);
    }

    void ResetEffect(){
        Effects->Chaos = false;
        Effects->Confuse = false;
        Effects->Shake = false;
    }

private:
    void DoCollisions()
    {
        for (GameUtil::GameObject &box : this->Levels[this->Level].Bricks)
        {
            if (box.Destroyed)
                continue;
            GameUtil::Collision::CollisionResult coll = CheckCollision(*BallObj, box);
            if (!std::get<0>(coll))
                continue;

            if (!box.IsSolid)
            {
                box.Destroyed = GL_TRUE;
                this->SpawnPowerUps(box);
                SourceManager->GetAudio("bleep").Play();
            }
            else
            {
                // 实心砖块激活shake特效
                ShakeTime = 0.05f;
                Effects->Shake = true;
                SourceManager->GetAudio("solid").Play();    //播放碰撞音效
            }

            GameUtil::DirectState2d dir = std::get<1>(coll);
            glm::vec2 diffVector = std::get<2>(coll);

            if (!(!(BallObj->PassThrough && !box.IsSolid)))
                continue;
            if (dir == GameUtil::DirectState2d::LEFT || dir == GameUtil::DirectState2d::RIGHT)
            {
                BallObj->Velocity.x = -BallObj->Velocity.x;
                GLfloat penetration = BallObj->Radius - std::abs(diffVector.x);
                if (dir == GameUtil::DirectState2d::LEFT)
                    BallObj->Position.x += penetration;
                else
                    BallObj->Position.x -= penetration;
            }
            else
            {
                BallObj->Velocity.y = -BallObj->Velocity.y;
                GLfloat peneration = BallObj->Radius - std::abs(diffVector.y);
                if (dir == GameUtil::DirectState2d::UP)
                    BallObj->Position.y -= peneration;
                else
                    BallObj->Position.y += peneration;
            }

        }

        for (Breakout::PowerUp &powerUp : this->PowerUps)
        {
            if (powerUp.Destroyed)
                continue;
            if (powerUp.Position.y >= this->Height)
                powerUp.Destroyed = true;

            if (GameUtil::Collision::AabbCheckCollision(*Player, powerUp))
            {
                ActivatePowerUp(powerUp);
                powerUp.Destroyed = true;
                powerUp.Activated = true;
                SourceManager->GetAudio("powerup").Play();
            }
        }

        GameUtil::Collision::CollisionResult result = CheckCollision(*BallObj, *Player);
        if (!BallObj->Stuck && std::get<0>(result))
        {
            GLfloat centerBoard = Player->Position.x + Player->Size.x / 2;
            GLfloat distance = (BallObj->Position.x + BallObj->Radius) - centerBoard;
            GLfloat percentage = distance / (Player->Size.x / 2);
            // 依据结果移动
            GLfloat strength = 2.0f;
            glm::vec2 oldVelocity = BallObj->Velocity;
            BallObj->Velocity.x = INITIAL_BALL_VELOCITY.x * percentage * strength;
            BallObj->Velocity.y = -1 * abs(BallObj->Velocity.y);
            BallObj->Velocity = glm::normalize(BallObj->Velocity) * glm::length(oldVelocity);

            BallObj->Stuck = BallObj->Sticky;

            SourceManager->GetAudio("bleep_board").Play();
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
        for (GLuint i = 0; i < 4; i++)
        {
            GLfloat dotProduct = glm::dot(glm::normalize(target), compass[i]);
            if (dotProduct > max)
            {
                max = dotProduct;
                bestMatch = i;
            }
        }

        return (GameUtil::DirectState2d)bestMatch;
    }

    GameUtil::Collision::CollisionResult CheckCollision(Breakout::Ball &a, GameUtil::GameObject &b)
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

        if (glm::length(difference) < a.Radius)
            return std::make_tuple(GL_TRUE, VectorDirection(difference), difference);

        return std::make_tuple(GL_FALSE, GameUtil::DirectState2d::UP, glm::vec2(0, 0));
    }

public:
    GameCore(GLuint width, GLuint height, std::string resourcePath)
        : Game(width, height, resourcePath) {}

    void Initialize()
    {
        /********************初始化音乐**********************/
        SourceManager->LoadAudio(GetResourceFilePath("audios/breakout.ogg").c_str(), "breakout");
        SourceManager->GetAudio("breakout").SetLoop(true).SetDoppler(true).Play();  //设置循环播放，启动多普勒音效并开始播放
        SourceManager->LoadAudio(GetResourceFilePath("audios/bleep.ogg").c_str(), "bleep");
        SourceManager->LoadAudio(GetResourceFilePath("audios/bleep_board.wav").c_str(), "bleep_board");
        SourceManager->LoadAudio(GetResourceFilePath("audios/powerup.wav").c_str(), "powerup");
        SourceManager->LoadAudio(GetResourceFilePath("audios/solid.wav").c_str(), "solid");

        /*********************初始化着色器************************/
        // 精灵着色器
        SourceManager->LoadShader(
            GetResourceFilePath("shaders/spriteShader.vs").c_str(),
            GetResourceFilePath("shaders/spriteShader.fs").c_str(),
            nullptr, "sprite");
        // 粒子着色器
        SourceManager->LoadShader(
            GetResourceFilePath("shaders/particleShader.vs").c_str(),
            GetResourceFilePath("shaders/particleShader.fs").c_str(),
            nullptr, "particle");
        // 反馈特效着色器
        SourceManager->LoadShader(
            GetResourceFilePath("shaders/processShader.vs").c_str(),
            GetResourceFilePath("shaders/processShader.fs").c_str(),
            nullptr, "postprocessing");
        /********************配置着色器******************************/
        glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(this->Width),
                                          static_cast<GLfloat>(this->Height), 0.0f, -1.0f, 1.0f);
        SourceManager->GetShader("sprite").Use().setInt("image", 0);
        SourceManager->GetShader("sprite").setMat4("projection", projection);

        SourceManager->GetShader("particle").Use().setInt("sprite", 0);
        SourceManager->GetShader("particle").setMat4("projection", projection);

        /*************************配置纹理*************************/
        // 加载纹理
        SourceManager->LoadTexture(GetResourceFilePath("textures/awesomeface.png").c_str(), true, "face");
        SourceManager->LoadTexture(GetResourceFilePath("textures/background.jpg").c_str(), false, "background");
        SourceManager->LoadTexture(GetResourceFilePath("textures/block.png").c_str(), false, "block");
        SourceManager->LoadTexture(GetResourceFilePath("textures/block_solid.png").c_str(), false, "block_solid");
        SourceManager->LoadTexture(GetResourceFilePath("textures/paddle.png").c_str(), true, "paddle");
        // 加载粒子纹理
        SourceManager->LoadTexture(GetResourceFilePath("textures/particle.png").c_str(), true, "particle");
        // 加载道具纹理
        SourceManager->LoadTexture(GetResourceFilePath("textures/powerup_speed.png").c_str(), true, "powerup_speed");
        SourceManager->LoadTexture(GetResourceFilePath("textures/powerup_sticky.png").c_str(), true, "powerup_sticky");
        SourceManager->LoadTexture(GetResourceFilePath("textures/powerup_increase.png").c_str(), true, "powerup_increase");
        SourceManager->LoadTexture(GetResourceFilePath("textures/powerup_confuse.png").c_str(), true, "powerup_confuse");
        SourceManager->LoadTexture(GetResourceFilePath("textures/powerup_chaos.png").c_str(), true, "powerup_chaos");
        SourceManager->LoadTexture(GetResourceFilePath("textures/powerup_passthrough.png").c_str(), true, "powerup_passthrough");

        //初始化配置对象
        Renderer = new GameUtil::SpriteRenderer(SourceManager->GetShader("sprite"));
        Particles = new GameUtil::ParticleGenerator(SourceManager->GetShader("particle"), SourceManager->GetTexture("particle"), 3000);
        Effects = new GameUtil::PostProcessor(SourceManager->GetShader("postprocessing"), this->Width, this->Height);

        // 加载关卡
        GameUtil::GameLevel one;
        one.Load(GetResourceFilePath("levels/standard.lv").c_str(), this->Width, this->Height / 2, SourceManager);
        GameUtil::GameLevel two;
        two.Load(GetResourceFilePath("levels/fewSmallGaps.lv").c_str(), this->Width, this->Height / 2, SourceManager);
        GameUtil::GameLevel three;
        three.Load(GetResourceFilePath("levels/spaceInvader.lv").c_str(), this->Width, this->Height / 2, SourceManager);
        GameUtil::GameLevel four;
        four.Load(GetResourceFilePath("levels/bounceGalore.lv").c_str(), this->Width, this->Height / 2, SourceManager);
        this->Levels.push_back(one);
        this->Levels.push_back(two);
        this->Levels.push_back(three);
        this->Levels.push_back(four);
        this->Level = 0;

        // 初始化并加载角色
        glm::vec2 playerPos = glm::vec2(
            this->Width / 2.0f - PLAYER_SIZE.x / 2.0f,
            this->Height - PLAYER_SIZE.y);
        Player = new GameUtil::GameObject(playerPos, PLAYER_SIZE, SourceManager->GetTexture("paddle"));

        // 初始化小球
        glm::vec2 ballPos = playerPos + glm::vec2(PLAYER_SIZE.x / 2 - BALL_RADIUS,
                                                  -BALL_RADIUS * 2);
        BallObj = new Breakout::Ball(ballPos,
                                     BALL_RADIUS,
                                     INITIAL_BALL_VELOCITY,
                                     SourceManager->GetTexture("face"));
    }

    

    void Update(GLfloat dt)
    {
        // 移动小球
        BallObj->Move(dt, this->Width);
        // 碰撞检测
        this->DoCollisions();
        // 更新粒子
        Particles->Update(dt, *BallObj, 2, glm::vec2(BallObj->Radius / 2.0f));
        // 更新道具
        this->UpdatePowerUps(dt);

        if (BallObj->Position.y >= this->Height)
        {
            this->Resetlevel();
            this->ResetPlayer();
            this->ResetEffect();
        }

        if (ShakeTime > 0.0f)
        {
            ShakeTime -= dt;
            if (ShakeTime <= 0.0f)
                Effects->Shake = false;
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
            Effects->BeginRender();
            Renderer->DrawSprite(Game::SourceManager->GetTexture("background"),
                                 glm::vec2(0.0f, 0.0f), glm::vec2(this->Width, this->Height), 0.0f);
            // 绘制角色
            Player->Draw(*Renderer);
            // 绘制关卡
            this->Levels[this->Level].Draw(*Renderer);

            // 绘制道具
            for (Breakout::PowerUp &powerUp : this->PowerUps)
                if (!powerUp.Destroyed)
                    powerUp.Draw(*Renderer);

            // 绘制粒子特效
            Particles->Draw();
            // 绘制小球
            BallObj->Draw(*Renderer);

            Effects->EndRender();
            Effects->Render(glfwGetTime());
        }
    }

    ~GameCore(){
        // 回收内存
        delete Renderer;
        delete Player;
        delete BallObj;
        delete Particles;
        delete Effects;
    }
};

GameUtil::Game *GameUtil::GetGame()
{
    return (GameUtil::Game *)(new GameCore(800, 600, "G:/my_program/cpp/Breakout/resource/"));
}

GameUtil::GameWindow *GameUtil::GetGameWindow(GameUtil::Game *game)
{
    return (GameUtil::GameWindow *)(new GameUtil::GlfwGameWindow(800, 600, "Breakout", game));
}
#define STB_IMAGE_IMPLEMENTATION
#include "GameUtil/Game.h"
#include "GameUtil/stb_image.h"
#include "GameUtil/ResourceManager.h"
#include "GameUtil/SpriteRenderer.h"
namespace GameUtil{
    SpriteRenderer *Renderer;
    Game::Game(GLuint width, GLuint height)
        : State(GameState::GAME_ACTIVE), Keys(), Width(width), Height(height)
    {
    }

    Game::~Game()
    {
    }

    void Game::Initialize()
    {
        ResourceManager::LoadShader("G:/my_program/cpp/Breakout/resource/shaders/spriteShader.vs", "G:/my_program/cpp/Breakout/resource/shaders/spriteShader.fs", nullptr, "sprite");
        // 配置着色器
        glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(this->Width),
                                          static_cast<GLfloat>(this->Height), 0.0f, -1.0f, 1.0f);
        ResourceManager::GetShader("sprite").Use().setInt("image", 0);
        ResourceManager::GetShader("sprite").setMat4("projection", projection);
        // 设置专用于渲染的控制
        Renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));
        // 加载纹理
        ResourceManager::LoadTexture("G:/my_program/cpp/Breakout/resource/textures/awesomeface.png", GL_TRUE, "face");
    }

    void Game::Update(GLfloat dt)
    {
    }

    void Game::ProcessInput(GLfloat dt)
    {
    }

    void Game::Render()
    {
        Renderer->DrawSprite(ResourceManager::GetTexture("face"),
                             glm::vec2(200, 200), glm::vec2(300, 400), 45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    }
}
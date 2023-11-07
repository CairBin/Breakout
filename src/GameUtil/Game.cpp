#include "GameUtil/Game.h"
#include "GameUtil/stb_image.h"
#include "GameUtil/ResourceManager.h"
#include "GameUtil/SpriteRenderer.h"
#include "GLFW/glfw3.h"

namespace GameUtil
{
    Game::Game(GLuint width, GLuint height, std::string resourcePath)
        : State(GameState::GAME_ACTIVE), Keys(), Width(width), Height(height), ResourcePath(resourcePath), SourceManager(nullptr)
    {
    }

    Game::~Game()
    {
    }

    std::string Game::GetResourceFilePath(const std::string &filename)
    {
        return this->ResourcePath + filename;
    }

    void Game::SetResourceManager(ResourceManager &sourceManager)
    {
        this->SourceManager = &sourceManager;
    }

}
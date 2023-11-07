#ifndef _GAME_WINDOW_H_
#define _GAME_WINDOW_H_

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "GameUtil/Game.h"
#include "ResourceManager.h"
#include <string>
#include <map>

namespace GameUtil{
    class GameWindow{
    public:
        GLuint Width, Height;
        std::string Title;
        Game* GameCore;
        GLuint64 Id;
        ResourceManager SourceManager;
        static std::map<GLuint64, GameWindow *> Windows;
        std::string Type;

    public:
        GameWindow(GLuint width,GLuint height,std::string title,Game* gameCore);
        ~GameWindow();
        void SetResourceManager(ResourceManager &resourceManager);
        virtual void MainLoop(void *param) = 0;
        
    public:
        static void LoadWindow(GameWindow &window);
        static void Clear();
        static GLuint64 IdCnt;
    };

    class GlfwGameWindow:GameWindow{
    public:
        GLFWwindow *Window;
        GlfwGameWindow(GLuint width, GLuint height, std::string title, Game *gameCore,GLFWwindow *window = nullptr, GLFWmonitor *monitor = nullptr, GLFWwindow *share = nullptr);
        ~GlfwGameWindow();

        virtual void Initialize();
        virtual void Config();
        virtual void MainLoop(void *param);
    };

    GameWindow *GetGameWindow(Game* game);
}


#endif
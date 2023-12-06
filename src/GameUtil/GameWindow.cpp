#include "GameUtil/GameWindow.h"
#include "GameUtil/ResourceManager.h"
#include "GameUtil/Audio.h"
#include <iostream>

namespace GameUtil{
    std::map<GLuint64, GameWindow *> GameWindow::Windows;

    GameWindow::GameWindow(GLuint width, GLuint height, std::string title, Game *gameCore)
        : Width(width), Height(height), Title(title), GameCore(gameCore)
    {
        Id = IdCnt++;
        GameCore->SetResourceManager(SourceManager);
        GameWindow::LoadWindow(*this);
    }
    GLuint64 GameWindow::IdCnt = 0;
    GameWindow::~GameWindow(){
        Windows[Id] = nullptr;
        SourceManager.Clear();
        if(GameCore)
            delete GameCore;
    }

    void GameWindow::Clear(){
        for(auto iter:Windows)
        {
            delete iter.second;
            iter.second = nullptr;
        }
    }

    void GameWindow::SetResourceManager(ResourceManager& resourceManager){
        this->SourceManager = resourceManager;
        GameCore->SetResourceManager(SourceManager);
    }

    void GameWindow::LoadWindow(GameWindow& window){
        Windows[window.Id] = &window;
    }

    GlfwGameWindow::GlfwGameWindow(GLuint width, GLuint height, std::string title, Game *gameCore, GLFWwindow *window, GLFWmonitor *monitor, GLFWwindow *share)
        : GameWindow(width, height, title, gameCore), Window(window)
    {
        GameWindow::Type = std::string("glfw");
        this->Initialize();
        if(!window){
            Window = glfwCreateWindow(Width, Height, Title.c_str(), monitor, share);
        }
        if(!Window){
            std::cout << "Failed to initialize GLFW window" << std::endl;
            exit(EXIT_FAILURE);
        }
        glfwMakeContextCurrent(Window);
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cout << "Failed to initialize GLAD" << std::endl;
            exit(EXIT_FAILURE);
        }
        this->Config();
    }

    void GlfwGameWindow::Initialize(){
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
        glfwWindowHint(GLFW_RESIZABLE, false);
    }

    void GlfwGameWindow::Config(){
        glfwSetKeyCallback(Window, [](GLFWwindow *window, int key, int scancode, int action, int mode)
        {
            GlfwGameWindow* gameWin = nullptr;
            for(auto iter:GameWindow::Windows){
                if (iter.second->Type == "glfw" && static_cast<GlfwGameWindow *>(iter.second)->Window == window)
                {
                    gameWin = static_cast<GlfwGameWindow *>(iter.second);
                }
            }
            if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
                glfwSetWindowShouldClose(window, true);
            if(!gameWin)
                return;

            if (key >= 0 && key < Game::MAX_KEYS_SIZE)
            {
                if (action == GLFW_PRESS)
                    gameWin->GameCore->Keys[key] = true;
                else if (action == GLFW_RELEASE)
                {
                    gameWin->GameCore->Keys[key] = false;
                    gameWin->GameCore->KeysProcessed[key] = false;
                }
            }
            
        });
        glfwSetFramebufferSizeCallback(Window, [](GLFWwindow *window, int width, int height)
                                       { glViewport(0, 0, width, height); });
        glViewport(0, 0, Width, Height);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        Audio::Init();
        GameCore->Initialize();
        
    }

    void GlfwGameWindow::MainLoop(void* param){
        GLfloat deltaTime = 0.0f;
        GLfloat lastFrame = 0.0f;

        while (!glfwWindowShouldClose(Window))
        {
            float currentFrame = glfwGetTime();
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;
            glfwPollEvents();

            GameCore->ProcessInput(deltaTime);
            GameCore->Update(deltaTime);
            Audio::Update();

            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            GameCore->Render();

            glfwSwapBuffers(Window);
        }
    }

    GlfwGameWindow::~GlfwGameWindow(){
        Audio::Close();
        glfwTerminate();
    }
}
# Breakout

## 项目介绍
这是一个基于OpenGL的2D打砖块游戏，代码参考了LearnOpenGL教程的2D游戏实战部分。本项目对原项目做了一些结构性调整。
原文地址: [LearnOpenGL-Breakout](https://learnopengl.com/In-Practice/2D-Game/Breakout)

该项目除了着色器渲染以及纹理贴图外还实现了如下功能：
* 音乐播放
* 碰撞检测
* 游戏状态切换
* 游戏道具(Buff,Debuff)
* 地图加载/关卡切换

## 项目结构

项目主要分为三个部分，分别为GameUtil, Game, Application。

* GameUtil为游戏工具类，包括一些游戏相关的辅助函数。
* Game包含具体的游戏对象，它一般继承自GameUtil中的一些类型。
* Application为游戏应用类，初始化一些基本文件路径以及一些游戏运行时需要的变量，并确定游戏逻辑。

对于GameUtil部分，游戏与窗口之间的设计是分离的。对于游戏窗口，默认是使用的glfw创建，在`GameUtil/GameWindow.h`文件中定义了`GameUtil::GlfwGameWindow`类型，它继承自`GameUtil::GameWindow`并封装了一些glfw所用到的基本函数。如果你不想使用glfw，可以继承`GameUtil::GameWindow`类型并重载它的虚方法，并在`Application/main.cpp`的`GameUtil::GameWindow GetGameWindow(GameUtil::Game* game)`方法中返回该类型。
默认情况下，它应该是这样的。
```cpp
GameUtil::GameWindow *GameUtil::GetGameWindow(GameUtil::Game *game)
{
    return (GameUtil::GameWindow *)(new GameUtil::GlfwGameWindow(800, 600, "Breakout", game));
}
```

对于游戏类型，它被声明在`GameUtil/Game.h`中，它主要被用于创建游戏本体，并指定各项参数，如游戏状态等。你应该在`Application/main.cpp`中定义你的继承自`GameUtil::Game`的游戏类型，并在`GameUtil::Game *GameUtil::GetGame()`函数中返回它。

```cpp
GameUtil::Game *GameUtil::GetGame()
{
    return (GameUtil::Game *)(new GameCore(800, 600, "G:/my_program/cpp/Breakout/resource/"));
}
```

本项目有一个有趣的地方，你并不能在`Application/main.cpp`中看到main函数，它实际上是被隐藏到`GameUtil/EntryPoint.h`中了。在设计上，我们希望它能够像一个游戏引擎一样，一些基本功能如纹理、着色器、精灵等仅通过调用`GameUtil`模块的接口来加载，而游戏逻辑交给外部实现。


## 编译
目前仅在Windows下测试过，不过所用的库都是跨平台的，理论上解决依赖问题后可以在Linux和OSX下通过编译。

另外需要注意，本项目使用了YSE音频库，它通过MSVC编译。由于MSVC与其它编译器的ABI有所差异，这要求在Windows下你同样需要用MSVC来编译本项目。

除了YSE库之外，还需要FreeType库。你可以通过本项目的git子模块从相应的仓库获取它们的源代码，然后手动编译它们或者从网络上下载已经编译好的库文件，并将它们放到与项目相同的目录下。

关于其它依赖项，如GLFW和glad，构建系统已经将它们添加到编译过程中，一般情况下不需要手动安装它们。

### Windows
在Windows下你需要执行以下命令，创建Visual Studio解决方案后使用VS的MSVC进行编译。
```bat
mkdir build
cd build
cmake ..
```

### Linux/OSX
未经过测试。
```shell
mkdir build
cd build
cmake ..
make
```

## 素材
图片与音乐素材来源自[LearnOpenGL文档](https://learnopengl.com/In-Practice/2D-Game/Breakout)，并对部分文件格式做了转换（如`.mp3`转换成了`.ogg`）。
字体下载自下载自网络。
请遵循素材的版权许可，本项目并不对这些内容提供额外授权。

## 效果展示

### 菜单
![菜单](https://picdl.sunbangyan.cn/2023/12/07/6f4e9ce563c224ddc92bc81350201ad8.jpeg)

![切换地图](https://picdl.sunbangyan.cn/2023/12/07/58da6957d974d57d2fe654770bb31579.jpeg)

### 游戏

![颠倒](https://picst.sunbangyan.cn/2023/12/07/5889b324db7e69385bb6eee28d25076a.jpeg)

![混乱](https://picdm.sunbangyan.cn/2023/12/07/41d796690eb8babe3a0d99f0274da3f1.jpeg)

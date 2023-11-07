#ifndef _ENTRY_POINT_H_
#define _ENTRY_POINT_H_

extern GameUtil::Game *GameUtil::GetGame();
extern GameUtil::GameWindow *GameUtil::GetGameWindow(Game* game);

int main(int argc,char** argv){
    auto win = GameUtil::GetGameWindow(GameUtil::GetGame());
    win->MainLoop(nullptr);
    delete win;
    return 0;
}

#endif
#ifndef SCREENMANAGER_H
#define SCREENMANAGER_H

enum class Screen { MENU, PLAYING, WIN_SCREEN };

class ScreenManager {
public:
    static Screen current;
    static void Change(Screen screen);
};

#endif
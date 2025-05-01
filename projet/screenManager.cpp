#include "ScreenManager.h"

Screen ScreenManager::current = Screen::MENU;

void ScreenManager::Change(Screen s) {
    current = s;
}
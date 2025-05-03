#ifndef MENU_H
#define MENU_H

#include <vector>
#include "StageButton.h"

class Menu {
    std::vector<StageButton> buttons;
    int selectedStage;
public:
    Menu();
    void Update();
    void Draw();
    bool StageChosen(int &stage);
};

#endif
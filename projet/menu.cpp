#include "Menu.h"
#include "ScreenManager.h"
#include "raylib.h"


Menu::Menu()
{
    for (int i = 1; i <= 7; i++)
        buttons.emplace_back(i, 50 + ((i - 1) % 4) * 140, 100 + ((i - 1) / 4) * 100);
    selectedStage = 0;
}

void Menu::Update()
{
    Vector2 m = GetMousePosition();
    for (auto &b : buttons)
    {
        if (b.Clicked(m) || b.ReloadClicked(m))
        {
            selectedStage = b.index;
            ScreenManager::Change(Screen::PLAYING);
        }
    }
}

void Menu::Draw()
{
    for (auto &b : buttons)
        b.Draw();
}

bool Menu::StageChosen(int &stage)
{
    if (ScreenManager::current == Screen::PLAYING)
    {
        stage = selectedStage;
        return true;
    }
    return false;
}
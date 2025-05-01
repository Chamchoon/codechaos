#ifndef SCOREMANAGER_H
#define SCOREMANAGER_H

class ScoreManager {
public:
    static int Get(int stage);
    static void Set(int stage, int score);
};

#endif
#ifndef BINARY_H
#define BINARY_H

#include "raylib.h"
#include <vector>
using namespace std;
class Binary {
public:
    vector<bool> answer;
    vector<bool> correct_answer;


    Binary(vector<bool> a, vector<bool> ca);
    bool solved(vector<bool> a);
};
#endif
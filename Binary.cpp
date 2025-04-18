#include "Binary.h"
#include <vector>
using namespace std;

Binary::Binary(vector<bool> a, vector<bool> ca){
    answer = a;
    correct_answer = ca;
}


bool Binary::solved(vector<bool> a){
    answer = a;
    return answer==correct_answer;
}

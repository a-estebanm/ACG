

#include <iostream>
#include <cfloat>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <chrono>
#include <random>
#include <vector>
#include <algorithm>

using namespace std::chrono;
using namespace std;
using namespace std;


std::vector <float> randomize_flt(std::vector <float> vect, int size){
    for (int i = 0; i < size; ++i) {
        vect.push_back((float) rand()/RAND_MAX);
    }
    return vect;
}

std::vector <int> randomize_int(std::vector <int> vect, int range, int size){
    for (int i = 0; i < range; ++i) {
        vect.push_back(i + 1);
    }
    std::shuffle(vect.begin(), vect.end(), std::mt19937(std::random_device()()));
    auto start = vect.begin();
    auto end = vect.begin() + size + 1;

    // To store the sliced vector
    std::vector<int> result(size + 1);

    // Copy vector using copy function()
    copy(start, end, result.begin());

    // Return the final sliced vector
    return result;
}

int dist_int(int x1, int x2){
    return abs(x2-x1);
}

float dist_flt(float x1, float x2){
    return abs(x2-x1);
}

int bruteForce_int(std::vector <int> vect, int n)
{
    int min  = INT32_MAX;
    int d;
    for (int i = 1; i < n ; i++) {
        d = dist_int(vect.at(i-1), vect.at(i));
        if(d < min) min = d;
    }
    return min;
}

float bruteForce_flt(std::vector <float> vect, int n)
{
    float min  = FLT_MAX;
    float d;
    for (int i = 1; i < n ; i++) {
        d = dist_flt(vect.at(i-1), vect.at(i));
        if(d < min) min = d;
    }
    return min;
}

int main(){
    srand((unsigned)time(nullptr));

    std::vector <float> points;
    points = randomize_flt(points,20);
    cout << "the min is " << bruteForce_flt(points, 20);

}



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


std::vector <int> randomize(std::vector <int> vect, int range, int size){
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

int dist(int x1, int x2){
    return abs(x2-x1);
}

int bruteForce(std::vector <int> vect, int n)
{
    int min  = INT32_MAX;
    int d;
    for (int i = 1; i < n ; i++) {
        d = dist(vect.at(i-1), vect.at(i));
        if(d < min) min = d;
    }
    return min;
}

int main(){

    std::vector <int> points;
    points = randomize(points,100,20);
    cout << "the min is " << bruteForce(points, 20);

}

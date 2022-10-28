// A divide and conquer program in C++ to find the smallest distance from a
// given set of points.

#include <iostream>
#include <cfloat>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <chrono>
#include <vector>

#define SIZE 1000000

using namespace std::chrono;
using namespace std;
using namespace std;

// A structure to represent a Point in 2D plane
struct Point
{
    int x, y;
};

Point * pointynazer(int xArr[], int yArr[]){
    static Point p[((sizeof(xArr))/(sizeof(xArr[0])))];
    for (int i = 0; i < sizeof p; i++) {
        p[i] = {xArr[i], yArr[i]};
    }
    return p;
}

/* Following two functions are needed for library function qsort().
Refer: http://www.cplusplus.com/reference/clibrary/cstdlib/qsort/ */

// Needed to sort array of points according to X coordinate
int compareX(const void* a, const void* b)
{
    auto *p1 = (Point *)a, *p2 = (Point *)b;
    return (p1->x != p2->x) ? (p1->x - p2->x) : (p1->y - p2->y);
}
// Needed to sort array of points according to Y coordinate
int compareY(const void* a, const void* b)
{
    auto *p1 = (Point *)a, *p2 = (Point *)b;
    return (p1->y != p2->y) ? (p1->y - p2->y) : (p1->x - p2->x);
}

// A utility function to find the distance between two points
float dist(Point p1, Point p2)
{
    if(p1.x == p2.x && p1.y == p2.y) return FLT_MAX;
    return sqrt( (p1.x - p2.x)*(p1.x - p2.x) +
                 (p1.y - p2.y)*(p1.y - p2.y)
    );
}

// A Brute Force method to return the smallest distance between two points
// in P[] of size n
float bruteForce(Point P[], int n)
{
    float min = FLT_MAX;
    for (int i = 0; i < n; ++i)
        for (int j = i+1; j < n; ++j)
            if (dist(P[i], P[j]) < min)
                min = dist(P[i], P[j]);
    return min;
}

// A utility function to find a minimum of two float values
float min(float x, float y)
{
    return (x < y)? x : y;
}


// A utility function to find the distance between the closest points of
// strip of a given size. All points in strip[] are sorted according to
// y coordinate. They all have an upper bound on minimum distance as d.
// Note that this method seems to be a O(n^2) method, but it's a O(n)
// method as the inner loop runs at most 6 times
float stripClosest(Point strip[], int size, float d)
{
    float min = d; // Initialize the minimum distance as d

    // Pick all points one by one and try the next points till the difference
    // between y coordinates is smaller than d.
    // This is a proven fact that this loop runs at most 6 times
    for (int i = 0; i < size; ++i)
        for (int j = i+1; j < size && (strip[j].y - strip[i].y) < min; ++j)
            if (dist(strip[i],strip[j]) < min)
                min = dist(strip[i], strip[j]);

    return min;
}

// A recursive function to find the smallest distance. The array Px contains
// all points sorted according to x coordinates and Py contains all points
// sorted according to y coordinates
float closestUtil(Point Px[], Point Py[], int n, bool *end)
{
    if(end) return 1;
    // If there are 2 or 3 points, then use brute force
    if (n <= 3)
        return bruteForce(Px, n);

    // Find the middle point
    int mid = n/2;
    Point midPoint = Px[mid];


    // Divide points in y sorted array around the vertical line.
    // Assumption: All x coordinates are distinct.
    auto *Pyl = new Point[mid]; // y sorted points on left of vertical line
    auto *Pyr = new Point[n-mid]; // y sorted points on right of vertical line
    int li = 0, ri = 0; // indexes of left and right subarrays
    for (int i = 0; i < n; i++)
    {
        if ((Py[i].x < midPoint.x || (Py[i].x == midPoint.x && Py[i].y < midPoint.y)) && li<mid)
            Pyl[li++] = Py[i];
        else
            Pyr[ri++] = Py[i];
    }

    // Consider the vertical line passing through the middle point
    // calculate the smallest distance dl on left of middle point and
    // dr on right side
    float dl = closestUtil(Px, Pyl, mid, end);
    float dr = closestUtil(Px + mid, Pyr, n-mid, end);

    // Find the smaller of two distances
    float d = min(dl, dr);
    if (d == 1) end = reinterpret_cast<bool *>(true);

    // Build an array strip[] that contains points close (closer than d)
    // to the line passing through the middle point
    auto *strip = new Point[n];
    int j = 0;
    for (int i = 0; i < n; i++)
        if (abs(Py[i].x - midPoint.x) < d)
            strip[j] = Py[i], j++;

    // Find the closest points in strip. Return the minimum of d and closest
    // distance is strip[]
    return stripClosest(strip, j, d);
}

// The main function that finds the smallest distance
// This method mainly uses closestUtil()
float closest(Point P[], int n)
{
    static Point Px[SIZE];
    static Point Py[SIZE];
    bool * end = new bool;
    end = reinterpret_cast<bool *>(false);
    for (int i = 0; i < n; i++)
    {
        Px[i] = P[i];
        Py[i] = P[i];
    }

    qsort(Px, n, sizeof(Point), compareX);
    qsort(Py, n, sizeof(Point), compareY);

    // Use recursive function closestUtil() to find the smallest distance
    return closestUtil(Px, Py, n, end);
}

// Driver program to test above functions

void randomArray(int xarray[], int yarray[], int size) {
    int i;

    for (i = 0; i <= size - 1; i++) {
        xarray[i] = rand() % 1000;
        yarray[i] = rand() % 1000;
    }
}

int main()
{
    srand( (unsigned)time( NULL ) );

    int * arrX = new int [SIZE];
    int * arrY = new int [SIZE];
    auto *P = new Point[SIZE];

    randomArray(arrX, arrY,SIZE);
    for (int j = 0; j < SIZE; j++) {
        P[j] = {arrX[j], arrY[j]};
    }
    auto start2 = high_resolution_clock::now();
    //cout << "\nThe smallest distance with BF is " << bruteForce(P, n);
    auto stop2 = high_resolution_clock::now();
    auto duration2 = duration_cast<microseconds>(stop2 - start2);
    cout << " with time of computation of: " << duration2.count() << " microseconds" << endl;
    auto start1 = high_resolution_clock::now();
    cout << "The smallest distance with DQ is " << closest(P, SIZE);
    auto stop1 = high_resolution_clock::now();
    auto duration1 = duration_cast<microseconds>(stop1 - start1);
    cout << " with time of computation of: " << duration1.count() << " microseconds" << endl;


    return 0;
}




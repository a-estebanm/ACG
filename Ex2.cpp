

#include <iostream>
#include <cfloat>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <chrono>
#include <random>
#include <vector>
#include <algorithm>
#include <list>

using namespace std::chrono;
using namespace std;
using namespace std;


std::vector<float> randomize_flt(std::vector<float> vect, long long size) {
    std::random_device rd;
    std::default_random_engine gen(rd());
    std::uniform_real_distribution<float> dist(0.0000000000001, 10000);


//    std::normal_distribution<float> dist(1, 0);
    for (long long i = 0; i < size; ++i) {
        vect.at(i) = (dist(gen)); //cambiado para evitar añadir al final
    }
    return vect;
}

float dist_flt(float x1, float x2) {
    return abs(x2 - x1);
}

float bruteForce_flt(std::vector<float> vect, long long n) {
    float min = FLT_MAX;
    float x1;
    float x2;
    float d;
    for (int i = 1; i < n; i++) {
        for (int j = i; j < n; j++) {
            d = dist_flt(vect.at(i), vect.at(j));
            if (d < min && d != 0) {
                min = d;
                x1 = vect.at(i);
                x2 = vect.at(j);
            }
        }
    }
    return min;
}

float sorted_cpp(std::vector<float> vect, long long n) {
    float min = FLT_MAX;
    float x1;
    float x2;
    float d;
    std::sort(vect.begin(), vect.end());
    for (int i = 1; i < n-1; i++) {
            d = dist_flt(vect.at(i), vect.at(i+1));
            if (d < min && d != 0) {
                min = d;
                x1 = vect.at(i);
                x2 = vect.at(i+1);
            }
    }
    return min;
}

int compare (const void * a, const void * b){
    return ( *(int*)a - *(int*)b );
}

int Partition(vector<float> &v, int start, int end){

    int pivot = end;
    int j = start;
    for(int i=start;i<end;++i){
        if(v[i]<v[pivot]){
            swap(v[i],v[j]);
            ++j;
        }
    }
    swap(v[j],v[pivot]);
    return j;

}

void Quicksort(vector<float> &v, int start, int end ){

    if(start<end){
        int p = Partition(v,start,end);
        Quicksort(v,start,p-1);
        Quicksort(v,p+1,end);
    }

}

float sorted_quick(std::vector<float> vect, long long n) {
    float min = FLT_MAX;
    float x1;
    float x2;
    float d;
    Quicksort(vect, 0, vect.size());
    for (int i = 1; i < n-1; i++) {
        d = dist_flt(vect.at(i), vect.at(i+1));
        if (d < min && d != 0) {
            min = d;
            x1 = vect.at(i);
            x2 = vect.at(i+1);
        }
    }
    return min;
}

void shellSort(std::vector<float> &arr, int n){
    n = arr.size();
    for (int gap = n/2; gap > 0; gap /= 2){
        for (int i = gap; i < n; i += 1){
            float temp = arr.at(i);
            int j;
            for (j = i; j >= gap && arr.at(j - gap) > temp; j -= gap)
                arr.at(j) = arr.at(j - gap);
            arr.at(j) = temp;
        }
    }
}

float sorted_shell(std::vector<float> vect, long long n) {
    float min = FLT_MAX;
    float x1;
    float x2;
    float d;
    shellSort(vect, n);
    for (int i = 1; i < n-1; i++) {
        d = dist_flt(vect.at(i), vect.at(i+1));
        if (d < min && d != 0) {
            min = d;
            x1 = vect.at(i);
            x2 = vect.at(i+1);
        }
    }
    return min;
}

vector<vector<float>> bucketer(const vector<float> &vect, int size) {
    vector<vector<float>> buckets(256);
    int exp = 0;
    int size_ap = size; //size aproximated of the bucket with the given exponent
    int bucket_size[256] = {0};
    int z = 0;
//    printf("-----------Tamaños aproximados:\n");
    for (int i = 255; i >= 0; i--) {
        size_ap = size_ap / 2;
//        printf("%d\n",size_ap);
        vector<float> bucket(size_ap + size_ap * 0.25);
        buckets.at(i) = bucket;
    }
    for (float i: vect) {
//        frexp(i, &exp);
        try {
            buckets.at(exp + 127).at(bucket_size[exp + 127]++) = i;
        } catch (...) {
            z++;
            buckets.at(exp + 127).push_back(i);
        }
    }
    printf("catch called %d times\n", z);
    return buckets;
}

void print_buckets(vector<vector<float>> buckets) {
    printf("-------------PRINTING BUCKETS:\n");
    for (vector<float> bucket: buckets) {
        printf("%zu\n", bucket.size());
    }
}


vector<vector<float>>
bucketer1(const vector<float> &vect) { //Old function that doesn't take into account the size of buckets
    vector<vector<float>> buckets(256);
    int exp;
    for (float i: vect) {
        frexp(i, &exp);
        buckets.at(exp + 127).push_back(i);
    }
    return buckets;
}

vector<vector<float>>
bucketer2(vector<float> &vect) {
    vector<vector<float>> buckets(256);
    int exp = 0;
    vector<float> vectErased;

    bool twoElements = false;

    auto min = std::min_element(vect.begin(), vect.end());

    float minN = *min;//TODO:si borro el elemento minimo cambia el número de buckets
    frexp(minN, &exp);
    int minExp = exp;

    buckets.at(exp + 127).push_back(minN);
    vectErased.push_back(minN);
    vect.erase(min);

    while (!twoElements) {
        auto min2 = std::min_element(vect.begin(), vect.end());
        float minN2 = *min2;
        frexp(minN2, &exp);
        if (exp <= minExp + 24)
            twoElements = true;
        else {
            minExp = exp;
            vectErased.push_back(*min2);
            vect.erase(min2);
        }

    }

    vect.insert(vect.end(), vectErased.begin(), vectErased.end());

    for (float i: vect) {
        frexp(i, &exp);
        if (exp > minExp + 24)
            continue;
        buckets.at(exp + 127).push_back(i);
    }

    return buckets;
}


float dist_b(const vector<float> &bucket) {
    float d = FLT_MAX;
    float t;
    int x, y;
    for (int i = 1; i < bucket.size(); i++) {
        t = dist_flt(bucket.at(i), bucket.at(i - 1));
        if (t < d) {
            d = t;
            x = i;
            y = i - 1;
        };
    }
    return d;
}


int nextBucket(const vector<vector<float>> &buckets, int bucketNum) {

    int num = 0;

    for (const vector<float> &bucket: buckets)
        if (num <= bucketNum) {
            num++;
            continue;
        } else {
            if (!bucket.empty()) return num;
            num++;
        }
    return 0;
}


float bucketSort(vector<vector<float>> buckets) {
    float min = FLT_MAX;
    int bucket_num = 0;
    float d;
    for (vector<float> bucket: buckets) {
        bucket_num++;
        if (bucket.size() > 1) {
            std::sort(bucket.begin(), bucket.end());
            d = dist_b(bucket);
            if (d < min && d != 0) min = d;
            if (bucket_num < buckets.size() && !buckets.at(bucket_num).empty()) {
                std::sort(buckets[bucket_num].begin(), buckets[bucket_num].end());
                d = dist_flt(bucket.back(), buckets.at(bucket_num).front());
                if (d < min && d != 0) min = d;
            }
        } else if (bucket.size() == 1) {
            int next = nextBucket(buckets, bucket_num - 1);
            if (next != 0) {
                std::sort(buckets[next].begin(), buckets[next].end());
                d = dist_flt(bucket.back(), buckets.at(next).front());
                if (d < min && d != 0) min = d;
            }
        }
    }
    return min;
}

float bucketNoWindow(vector<float> vect) {
    vector<vector<float>> buckets = bucketer1(vect);
    return bucketSort(buckets);
}

float bucketWindow(vector<float> vect) {
    vector<vector<float>> buckets = bucketer2(vect);
    return bucketSort(buckets);
}


int main() {
//    freopen("results.txt", "w", stdout);
    //srand(time(nullptr)) ;
    long long sizes[10] = {10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000, 2000000000};
    int repeat = 1;
    for (long long size: sizes) {
        std::vector<float> points2(size);
        const std::vector<float> points = randomize_flt(points2, size);
        printf("----------- Size %lld  --------------\n", size);
        long long time1 = 0;
        float d1;
        for (int i = 0; i < repeat; i++) {

            auto start1 = high_resolution_clock::now();
            d1 = bucketWindow(points);
            auto stop1 = high_resolution_clock::now();
            auto duration1 = duration_cast<microseconds>(stop1 - start1);
            time1 += duration1.count();
        }
        printf("The smallest distance with BucketWindow is %.27f", d1);
        cout << " with time of computation of: " << time1 / repeat << " microseconds" << endl;


        long long time2 = 0;
        float d2;
        for (int i = 0; i < repeat; i++) {


            auto start2 = high_resolution_clock::now();
            d2 = bucketNoWindow(points);
            auto stop2 = high_resolution_clock::now();
            auto duration2 = duration_cast<microseconds>(stop2 - start2);
            time2 += duration2.count();
        }
        printf("The smallest distance with BucketNoWindow is %.27f", d2);
        cout << " with time of computation of: " << time2 / repeat << " microseconds" << endl;

        long long time4 = 0;
        float d4;
        for (int i = 0; i < repeat; i++) {


            auto start4 = high_resolution_clock::now();
            d4 = sorted_cpp(points, size);
            auto stop4 = high_resolution_clock::now();
            auto duration4 = duration_cast<microseconds>(stop4 - start4);
            time4 += duration4.count();
        }
        printf("The smallest distance with sorted_cpp is %.27f", d4);
        cout << " with time of computation of: " << time4 / repeat << " microseconds" << endl;


        long long time5 = 0;
        float d5;
        for (int i = 0; i < repeat; i++) {


            auto start5 = high_resolution_clock::now();
            d5 = sorted_quick(points, size);
            auto stop5 = high_resolution_clock::now();
            auto duration5 = duration_cast<microseconds>(stop5 - start5);
            time5 += duration5.count();
        }
        printf("The smallest distance with sorted_quick is %.27f", d5);
        cout << " with time of computation of: " << time5 / repeat << " microseconds" << endl;


        long long time6 = 0;
        float d6;
        for (int i = 0; i < repeat; i++) {


            auto start6 = high_resolution_clock::now();
            d6 = sorted_shell(points, size);
            auto stop6 = high_resolution_clock::now();
            auto duration6 = duration_cast<microseconds>(stop6 - start6);
            time6 += duration6.count();
        }
        printf("The smallest distance with sorted_shell is %.27f", d6);
        cout << " with time of computation of: " << time6 / repeat << " microseconds" << endl;

        if (size < 100000000) {
            long long time3 = 0;
            float d3;
            for (int i = 0; i < repeat; i++) {


                auto start3 = high_resolution_clock::now();
                d3 = bruteForce_flt(points, size);
                auto stop3 = high_resolution_clock::now();
                auto duration3 = duration_cast<microseconds>(stop3 - start3);
                time3 += duration3.count();
            }
            printf("The smallest distance with BruteForce is %.27f", d3);
            cout << " with time of computation of: " << time3 / repeat << " microseconds" << endl;


        }

    }
}
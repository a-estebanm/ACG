

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


std::vector <float> randomize_flt(std::vector <float> vect, double size){
    std::random_device rd;
    std::default_random_engine gen(rd());
    std::uniform_real_distribution<float> dist(0.0, 10000);
//    std::normal_distribution<float> dist(1, 0);
    for (int i = 0; i < size; ++i) {
        vect.at(i) = (dist(gen)); //cambiado para evitar añadir al final
    }
    return vect;
}

float dist_flt(float x1, float x2){
    return abs(x2-x1);
}

float bruteForce_flt(std::vector <float> vect, int n)
{
    float min  = FLT_MAX;
    float x1;
    float x2;
    float d;
    for (int i = 1; i < n ; i++) {
        for (int j = i; j < n; j++) {
            d = dist_flt(vect.at(i), vect.at(j));
            if(d < min && d != 0) {
                min = d;
                x1 = vect.at(i);
                x2 = vect.at(j);
            }
        }
    }
    printf("---------Puntos:\nX1: %.26f\nX2: %.26f\n", x1, x2);
    return min;
}

vector<vector<float>> bucketer(const vector<float>& vect, int size){
    vector<vector<float>> buckets(256);
    int exp = 0;
    int size_ap = size; //size aproximated of the bucket with the given exponent
    int  bucket_size[256] ={0};
    int z = 0;
//    printf("-----------Tamaños aproximados:\n");
    for (int i = 255; i>=0; i--){
        size_ap = size_ap/2;
//        printf("%d\n",size_ap);
        vector<float> bucket (size_ap + size_ap*0.25);
        buckets.at(i) = bucket;
    }
    for(float i : vect){
//        frexp(i, &exp);
        try {
            buckets.at(exp + 127).at(bucket_size[exp + 127]++) = i;
        }catch (...){
            z++;
            buckets.at(exp + 127).push_back(i);
        }
    }
    printf("catch called %d times\n", z);
    return buckets;
}

void print_buckets(vector<vector<float>> buckets){
    printf("-------------PRINTING BUCKETS:\n");
    for (vector<float> bucket : buckets){
        printf("%zu\n", bucket.size());
    }
}



vector<vector<float>> bucketer1(const vector<float>& vect, int size){ //Old function that doesn't take into account the size of buckets
    vector<vector<float>> buckets(256);
    int exp;
    for(float i : vect){
        frexp(i, &exp);
        buckets.at(exp+127).push_back(i);
    }
    return buckets;
}

vector<vector<float>> bucketer2(const vector<float>& vect, int size){ //Old function that doesn't take into account the size of buckets
    vector<vector<float>> buckets(256);
    int exp = 0;
    for(float i : vect){
//        frexp(i, &exp);
        buckets.at(exp+127).push_back(i);
    }
    return buckets;
}


float dist_b(const vector<float>& bucket){
    float d = FLT_MAX;
    float t;
    int x,y;
    for (int i = 1; i < bucket.size(); i++) {
        t = dist_flt(bucket.at(i), bucket.at(i - 1));
        if(t < d) {d = t;
            x=i;
            y=i-1;
        };
    }
    return d;
}


int nextBucket(const vector<vector<float>>& buckets, int bucketNum){

    int num = 0;

    for (const vector<float>& bucket : buckets)
        if(num <= bucketNum){
            num++;
            continue;
        }else{
            if(!bucket.empty()) return num;
            num++;
        }
    return 0;
}

float bucketSort(const vector<float>& vect, int size){

    vector<vector<float>> buckets = bucketer1(vect, size);
//    print_buckets(buckets);
    float min = FLT_MAX;
    int bucket_num = 0;
    float d;
    for(vector<float> bucket : buckets) {
        bucket_num ++;
        if (bucket.size() > 1) {
            std::sort(bucket.begin(), bucket.end());
            d = dist_b(bucket);
            if(d < min && d!=0) min = d;
            if(bucket_num < buckets.size() && !buckets.at(bucket_num).empty()){
                std::sort(buckets[bucket_num].begin(), buckets[bucket_num].end());
                d = dist_flt(bucket.back(),buckets.at(bucket_num).front());
                if(d < min && d!= 0) min = d;
            }
        }else if (bucket.size() == 1){
            int next = nextBucket(buckets,bucket_num-1);
            if(next!=0){
                d = dist_flt(bucket.back(),buckets.at(next).front());
                if(d < min && d!= 0) min = d;
            }
        }
    }
    return min;
}

float bucketSort2(const vector<float>& vect, int size){

    vector<vector<float>> buckets = bucketer1(vect, size);
//    print_buckets(buckets);
    float min = dist_b(buckets.at(1));
    int bucket_num = 0;
    float d;
    float x1;
    float x2;
    for(vector<float> bucket : buckets) {
        bucket_num ++;
        if (bucket.size() > 1) {
            std::sort(bucket.begin(), bucket.end());
            float d1 = FLT_MAX;
            float t;
            for (int i = 1; i < bucket.size(); i++) {
                t = dist_flt(bucket.at(i), bucket.at(i - 1));
                if(t < d1) {
                    d1 = t;
                    x1 = bucket.at(i);
                    x2 = bucket.at(i - 1);
                }
            }
            d = d1;
            if(d < min && d!=0) min = d;
            if(bucket_num < buckets.size() && !buckets.at(bucket_num).empty()){
                std::sort(buckets[bucket_num].begin(), buckets[bucket_num].end());
                d = dist_flt(bucket.back(),buckets.at(bucket_num).front());
                if(d < min && d!= 0) {
                    min = d;
                    x1 = bucket.back();
                    x2 = buckets.at(bucket_num).front();
                }
            }
        }else if (bucket.size() == 1){
            int next = nextBucket(buckets,bucket_num-1);
            if(next!=0){
                d = dist_flt(bucket.back(),buckets.at(next).front());
                if(d < min && d!= 0) {
                    min = d;
                    x1 = bucket.back();
                    x2 = buckets.at(next).front();
                }
            }
        }
    }
    printf("---------Puntos:\nX1: %.26f\nX2: %.26f\n", x1, x2);
    return min;
}

float bucketSortW2(const vector<float>& vect, int size){
    vector<vector<float>> buckets = bucketer1(vect, size);
//    print_buckets(buckets);
    float min = FLT_MAX;
    int bucket_num = 0;
    float d;
    float actual_min;
    int cc;
    cc = 0;
    float minp=FLT_MAX;
    for(int i=0;i<buckets.size() && cc<25;i++) {
        bucket_num ++;
        if (buckets[i].size() > 1) {
            std::sort(buckets[i].begin(), buckets[i].end());

            actual_min=buckets[i][0];
            if(minp>actual_min){
                cc=0;
                minp=actual_min;
            }else{
                cc++;
            }
            d = dist_b(buckets[i]);
            if(d < min && d!=0) min = d;
            if(bucket_num < buckets.size() && !buckets.at(bucket_num).empty()){
                std::sort(buckets[i+1].begin(), buckets[i+1].end());
                d = dist_flt(buckets[i].back(),buckets.at(bucket_num).front());
                if(d < min && d!= 0) min = d;
            }
        }else if (buckets[i].size() == 1){
            int next = nextBucket(buckets,bucket_num-1);
            if(next!=0){
                d = dist_flt(buckets[i].back(),buckets.at(next).front());
                if(d < min && d!= 0) min = d;
            }
        }
    }
    return min;
}

float bucketSortW(const vector<float>& vect, int size){

    vector<vector<float>> buckets = bucketer(vect, size);
//    print_buckets(buckets);
    float min = FLT_MAX;
    int bucket_num = 0;
    float d;
    int cc = 0;
    for(int i=0;i<buckets.size() && cc <25;i++) {
        bucket_num ++;
        if (buckets[i].size() > 1) {
            std::sort(buckets[i].begin(), buckets[i].end());

            d = dist_b(buckets[i]);
            if(d < min && d!=0) {
                min = d;
                cc = 0;
            }
            if(bucket_num < buckets.size() && !buckets.at(bucket_num).empty()){
                std::sort(buckets[i+1].begin(), buckets[i+1].end());
                d = dist_flt(buckets[i].back(),buckets.at(bucket_num).front());
                if(d < min && d!= 0){
                    min = d;
                    cc = 0;
                }else{
                    cc++;
                }
            }else{
                cc++;
            }
        }else if (buckets[i].size() == 1){
            int next = nextBucket(buckets,bucket_num-1);
            if(next!=0){
                d = dist_flt(buckets[i].back(),buckets.at(next).front());
                if(d < min && d!= 0) {
                    min = d;
                    cc=0;
                }else{
                    cc++;
                }
            }
        }
    }
    return min;
}



float bucketSort_brute(const vector<float>& vect, int size){
    vector<vector<float>> buckets = bucketer(vect, size);
    float min = FLT_MAX;
    int bucket_num = 0;
    float d;
    for(vector<float> bucket : buckets) {
        bucket_num ++;
        if (bucket.size() > 1) {
            d = bruteForce_flt(bucket,bucket.size());
            if(d < min) min = d;
            if(bucket_num < buckets.size() && !buckets.at(bucket_num).empty()){
                d = dist_flt(bucket.back(),buckets.at(bucket_num).front());
                if(d < min && d!= 0) min = d;
            }
        }else if (bucket.size() == 1){
            int next = nextBucket(buckets,bucket_num-1);
            if(next!=0){
                d = dist_flt(bucket.front(),buckets.at(next).front());
                if(d < min && d!= 0) min = d;
            }
        }
    }
    return min;
}

int main(){
    //srand(time(nullptr)) ;
    double size = 1e10;
    std::vector <float> points (size); //With this we initialize it as an array with size elements avoiding copies
    points = randomize_flt(points,size);
//    points.at(1) = 0.0000000000000001;
//    points.at(2) = 0.0000000000000002;
//    auto start2 = high_resolution_clock::now();
//    cout << "\nThe smallest distance with BF is " << bruteForce_flt(points, size);
//    auto stop2 = high_resolution_clock::now();
//    auto duration2 = duration_cast<microseconds>(stop2 - start2);
//    cout << " with time of computation of: " << duration2.count() << " microseconds" << endl;
    int repeat = 1;
    long long time1 = 0;
    float d1;
    for(int i = 0; i<repeat; i++){
        time_t a = time(0);

        char* b = ctime(&a);

        cout <<"The local date and time is: " <<b <<endl;
        auto start1 = high_resolution_clock::now();
//        bucketer(points,size);
        d1 = bucketSortW2(points, size);
        auto stop1 = high_resolution_clock::now();
        auto duration1 = duration_cast<microseconds>(stop1 - start1);
        time1 += duration1.count();
    }
    printf("The smallest distance with Window is %.27f",d1);
    cout << " with time of computation of: " << time1/repeat << " microseconds" << endl;
    time_t a = time(0);

    char* b = ctime(&a);

    cout <<"The local date and time is: " <<b <<endl;
    long long time2 = 0;
    float d2;
    for(int i = 0; i<repeat; i++){
        time_t a = time(0);

        char* b = ctime(&a);

        cout <<"The local date and time is: " <<b <<endl;
        auto start2 = high_resolution_clock::now();
//        bucketer1(points, size);
        d2 = bucketSort(points, size);
        auto stop2 = high_resolution_clock::now();
        auto duration2 = duration_cast<microseconds>(stop2 - start2);
        time2 += duration2.count();
    }
    printf("The smallest distance with Bucket is %.27f",d2);
    cout << " with time of computation of: " << time2/repeat << " microseconds" << endl;
    time_t a2 = time(0);

    char* b2 = ctime(&a2);

    cout <<"The local date and time is: " <<b2 <<endl;


    /*
    auto start4 = high_resolution_clock::now();
    cout << "The smallest distance with BucketSortW2 is " << bucketSortW2(points);
    auto stop4 = high_resolution_clock::now();
    auto duration4 = duration_cast<microseconds>(stop4 - start4);
    cout << " with time of computation of: " << duration4.count() << " microseconds" << endl;
    */


}

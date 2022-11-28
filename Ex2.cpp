

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
    std::random_device rd;
    std::default_random_engine gen(rd());
    std::uniform_real_distribution<float> dist(0.0, FLT_MAX);
    for (int i = 0; i < size; ++i) {
        vect.push_back(dist(gen));
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
        for (int j = i; j < n; j++) {
            d = dist_flt(vect.at(i), vect.at(j));
            if(d < min && d != 0) min = d;
        }
    }
    return min;
}

vector<vector<float>> bucketer(const vector<float>& vect){
    vector<vector<float>> buckets(254);
    int exp;
    for(float i : vect){
        frexp(i, &exp);
        buckets.at(exp+125).push_back(i);
    }
    return buckets;
}

float dist_b(const vector<float>& bucket){
    float d = FLT_MAX;
    float t;
    for (int i = 1; i < bucket.size(); i++) {
        t = dist_flt(bucket.at(i), bucket.at(i - 1));
        if(t < d) d = t;
    }
    return d;
}

float dist_b2(const vector<float>& bucket, const vector<float>& bucket2){
    float d = FLT_MAX;
    float t;
    for (int i = 1; i < bucket.size()+bucket2.size(); i++) {
        if(bucket.size()<i){
            t = dist_flt(bucket.at(i), bucket.at(i - 1));
        }
        else if(bucket.size()==i){
            t = dist_flt(bucket.at(bucket.size()-1), bucket2.at(0));
        }
        else{
            t = dist_flt(bucket2.at(i-bucket.size()), bucket2.at(i - 1 - bucket.size()));
        }
        if(t < d) d = t;
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

float bucketSortW(const vector<float>& vect){
    vector<vector<float>> buckets = bucketer(vect);
    float min = FLT_MAX;
    int bucket_num = 0;
    float d;
    int actual_min, cc;
    int minp=INT32_MAX;
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
                d = dist_flt(buckets[i].back(),buckets.at(bucket_num).front());
                if(d < min && d!= 0) min = d;
            }
        }else if (buckets[i].size() == 1){
            int next = nextBucket(buckets,bucket_num);
            if(next!=0){
                d = dist_flt(buckets[i].front(),buckets.at(next).front());
                if(d < min && d!= 0) min = d;
            }
        }
    }
    return min;
}



float bucketSort_brute(const vector<float>& vect){
    vector<vector<float>> buckets = bucketer(vect);
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
            int next = nextBucket(buckets,bucket_num);
            if(next!=0){
                d = dist_flt(bucket.front(),buckets.at(next).front());
                if(d < min && d!= 0) min = d;
            }
        }
    }
    return min;
}

float minimum(const vector<float>& bucket){
    float min=FLT_MAX;
    for(float v : bucket){
        if(min>v){
            min=v;
        }
    }
    min=min && ((2^23)-1);
    return min;
}
int minimum2(vector<vector<float>> buckets){
    int minBucket;
    float min=FLT_MAX;
    int x;
    for(int i=0;i<buckets.size();i++){
        x=minimum(buckets[i]);
        if(min>x){
            min=x;
            minBucket=i;
        }
    }
    return minBucket;
}
float bucketSortW2(const vector<float>& vect){
    vector<vector<float>> buckets = bucketer(vect);
    float min = FLT_MAX;
    float d,dnext;
    float minp=FLT_MAX;
    float actual_min;
    int cc;
    /*int x=minimum2(buckets);*/
    for(int i=0;i<buckets.size() && cc<=25;i++) {
        if (buckets[i].size() > 1) {
            std::sort(buckets[i].begin(), buckets[i].end());
            actual_min=buckets[i][0];
            if(minp>actual_min){
                cc=0;
                minp=actual_min;
            }else{
                cc+=1;
            }
            d = dist_b(buckets[i]);
            if(i!=buckets.size()-1) {
                dnext= dist_flt(buckets[i].back(),buckets[i].front());
            }
            if(d < min && d!=0 && d < dnext) min = d;
            else if(dnext < min && dnext!=0 && dnext < d) min = dnext;
        }
    }
    return min;
}


float bucketSort(const vector<float>& vect){
    vector<vector<float>> buckets = bucketer(vect);
    float min = FLT_MAX;
    float d;
    for(vector<float> bucket : buckets) {
        if (bucket.size() > 1) {
            std::sort(bucket.begin(), bucket.end());
            d = dist_b(bucket);
            if(d < min && d!=0) min = d;
        }
    }
    return min;
}

float bucketSort_brute2(const vector<float>& vect){
    vector<vector<float>> buckets = bucketer(vect);
    float min = FLT_MAX;
    int bucket_num = 0;
    float d;
    for(vector<float> bucket : buckets) {
        bucket_num ++;
        if (bucket.size() > 1) {
            d = bruteForce_flt(bucket, bucket.size());
            if (d < min) min = d;
        }
    }
    return min;
}



int main(){
    //srand(time(nullptr));
    int size = 1000;
    std::vector <float> points;
    points = randomize_flt(points,size);
    auto start2 = high_resolution_clock::now();
    cout << "\nThe smallest distance with BF is " << bruteForce_flt(points, size);
    auto stop2 = high_resolution_clock::now();
    auto duration2 = duration_cast<microseconds>(stop2 - start2);
    cout << " with time of computation of: " << duration2.count() << " microseconds" << endl;
    auto start1 = high_resolution_clock::now();
    cout << "The smallest distance with BucketSort is " << bucketSort(points);
    auto stop1 = high_resolution_clock::now();
    auto duration1 = duration_cast<microseconds>(stop1 - start1);
    cout << " with time of computation of: " << duration1.count() << " microseconds" << endl;
    auto start3 = high_resolution_clock::now();
    cout << "The smallest distance with BucketSortW is " << bucketSortW(points);
    auto stop3 = high_resolution_clock::now();
    auto duration3 = duration_cast<microseconds>(stop3 - start3);
    cout << " with time of computation of: " << duration3.count() << " microseconds" << endl;
    /*
    auto start4 = high_resolution_clock::now();
    cout << "The smallest distance with BucketSortW2 is " << bucketSortW2(points);
    auto stop4 = high_resolution_clock::now();
    auto duration4 = duration_cast<microseconds>(stop4 - start4);
    cout << " with time of computation of: " << duration4.count() << " microseconds" << endl;
    */


}

#include "thread-pool.h"
#include <iostream>
#include <vector>
#include <numeric>  // For std::accumulate
#include <functional>

using namespace std;

// Function to compute the sum of a subvector
void computeSum(const vector<int>& data, int start, int end, int* result) {

    *result = accumulate(data.begin() + start, data.begin() + end, 0);
}

int main() {
    // Sample data
    vector<int> data = {100, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    int numThreads = 3;
    ThreadPool pool(numThreads);

    // Results vector to hold the sums computed by each thread
    vector<int> results(numThreads, 0);

    // Determine the size of each chunk of data to process
    int n = data.size();
    int chunkSize = (n + numThreads - 1) / numThreads;  // Ensure all data is covered

    // Schedule tasks in the ThreadPool
    for (int i = 0; i < numThreads; ++i) {
        int start = i * chunkSize;
        int end = min(start + chunkSize, n);
        if (start < n) {
            // schedule the task with lambda function
            /* lambdas : [capture list] (parameters) -> return type {function body} */
            pool.schedule([start, end, i, &data, &results](void)-> void {computeSum(data, start, end, &results[i]);});
            //pool.schedule([start, end, i, &data, &results](){computeSum(data, start, end, &results[i]);});
        }
    }

    // Wait for all threads to finish
    pool.wait();

    // Calculate total sum
    int totalSum = accumulate(results.begin(), results.end(), 0);
    cout << "Total sum of elements: " << totalSum << endl;

    return 0;
}

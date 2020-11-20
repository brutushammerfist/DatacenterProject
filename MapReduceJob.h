#pragma once

using namespace std;

#include <random>

class MapReduceJob {
    private:
        default_random_engine generator;
        int inputSize;
        int intermediateDataSize;
        int completionTime;
        bool isComplete;
    public:
        MapReduceJob();
        ~MapReduceJob();
};
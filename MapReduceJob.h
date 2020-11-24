#pragma once

#include <random>
#include <list>

#include "Vehicle.h"

class Vehicle;

class MapReduceJob {
    private:
        int id;
        std::default_random_engine generator;
        int inputSize;
        int intermediateDataSize;
        int estimatedCompletionTime;
        int completionTime;
        bool isComplete;
        std::list<Vehicle*> repositories;
    public:
        MapReduceJob();
        ~MapReduceJob();
};
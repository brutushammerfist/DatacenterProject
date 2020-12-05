#pragma once

#include <random>
#include <list>

#include "Vehicle.h"
#include "SubJob.h"

class Vehicle;
class SubJob;

class MapReduceJob {
    private:
        int id;
        std::default_random_engine generator;
        int intermediateDataSize;

        int inputSize;
        int estimatedCompletionTime;
        int completionTime;

        bool isComplete;

        std::list<Vehicle*> repositories;

        int numSubJobs;
        SubJob* mapJob;
        std::list<SubJob*> reduceJobs;
        std::list<int> uploadedResults;
    public:
        MapReduceJob();
        MapReduceJob(int id, int numReducers);
        ~MapReduceJob();

        void addRepo(Vehicle* repo);

        void removeRepo(Vehicle* repo);
};
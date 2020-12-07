#pragma once

#include <random>
#include <list>

#include "SubJob.h"

class SubJob;
class Vehicle;

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

        int getIntermediateSize();

        SubJob* getFirstReduceJob();

        std::list<SubJob*> getReduceJobs();

        SubJob* getMapJob();

        bool complete();
};
#pragma once

#include <random>
#include <list>

#include "Vehicle.h"
#include "SubJob.h"

class Vehicle;

class MapReduceJob {
    private:
        int id;
        std::default_random_engine generator;
        int intermediateDataSize;

        int estimatedCompletionTime;
        int actualCompletionTime;

        bool isComplete;

        std::list<Vehicle*> repositories;

        int numSubJobs;
        SubJob* mapJob;
        std::list<SubJob*> reduceJobs;
        std::list<int> uploadedResults;
    public:
        MapReduceJob();
        MapReduceJob(int id, int numSubJobs);
        ~MapReduceJob();

        void work();

        void receiveUpload(SubJob* job);

        void removeRepo(Vehicle* repo);

        void addRepo(Vehicle* repo);

        int getIntermedSize();
};
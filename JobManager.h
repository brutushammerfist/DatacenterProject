#pragma once

#include <list>

#include "MapReduceJob.h"

class JobManager {
    private:
        std::list<MapReduceJob*> completedJobs;
        std::list<MapReduceJob*> runningJobs;
        int numReducers;
        int nextID;
    public:
        JobManager();
        JobManager(int numReducers);
        ~JobManager();

        MapReduceJob* newJob();
};
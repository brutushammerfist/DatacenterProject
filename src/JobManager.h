#pragma once

#include <list>

#include "MapReduceJob.h"

class DatacenterController;
class MapReduceJob;

class JobManager {
    private:
        std::list<MapReduceJob*> completedJobs;
        std::list<MapReduceJob*> runningJobs;
        int numReducers;
        int numSimulJobs;
        int nextID;
    public:
        JobManager();
        JobManager(int numReducers);
        ~JobManager();

        MapReduceJob* newJob();

        int getNumSimulJobs();

        int getNumCurrRunning();

        void checkJobs(DatacenterController* dcController);

        int getNumCompleted();

        void displayRunningJobStatus();
};
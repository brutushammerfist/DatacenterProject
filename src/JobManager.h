#pragma once

#include <list>
#include <map>

#include "MapReduceJob.h"

class DatacenterController;
class MapReduceJob;

struct Stats {
    int id;
    int completionTime;
    int timesMigrated;
    int timesRestarted;
};

class JobManager {
    private:
        std::list<MapReduceJob*> completedJobs;
        std::list<MapReduceJob*> runningJobs;
        int numReducers;
        int numSimulJobs;
        int nextID;
    public:
        JobManager();
        explicit JobManager(int numReducers);
        ~JobManager();

        MapReduceJob* newJob();

        int getNumSimulJobs();

        int getNumCurrRunning();

        void checkJobs(DatacenterController* dcController);

        int getNumCompleted();

        void displayRunningJobStatus();

        std::list<Stats> gatherJobStats();
};
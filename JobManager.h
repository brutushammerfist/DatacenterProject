#pragma once

#include <queue>

#include "MapReduceJob.h"

class JobManager {
    private:
        std::queue<MapReduceJob> jobQ;
        int numSimulJobs;
    public:
        JobManager();
        ~JobManager();
};
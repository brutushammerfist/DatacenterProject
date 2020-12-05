#pragma once

#include "MapReduceJob.h"
#include "Vehicle.h"

class MapReduceJob;

class SubJob {
    private:
        MapReduceJob* mainJob;

        bool map;

        int inputSize;
        int currInputDownloaded;
        bool inputDownloaded;

        int estimatedCompletionTime;
        int actualCompletionTime;
        int workingTimeToCompletion;

        bool workComplete;
        bool uploaded;
        bool complete;
    public:
        SubJob();
        SubJob(MapReduceJob* mainJob, int estimated, bool map);
        ~SubJob();

        void work();
};
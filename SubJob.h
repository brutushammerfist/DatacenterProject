#pragma once

#include "Vehicle.h"

class SubJob {
    private:
        Vehicle* hostVehicle;

        MapReduceJob* mainJob;

        int inputSize;
        int currInputDownloaded;
        bool inputDownloaded;

        int estimatedCompletionTime;
        int actualCompletionTime;
        int workingTimeToCompletion;

        bool isMap;
        bool isComplete;
        bool isUploaded;
        bool uploadOne;
        bool uploadTwo;
    public:
        SubJob();
        SubJob(MapReduceJob* mainJob, int estimated);
        ~SubJob();

        void work();

        void restart();

        bool complete();

        MapReduceJob* getMainJob();

        void setUploaded();

        void setVehicle(Vehicle* vehicle);

        bool uploaded();

        bool isMapJob();

        bool intermedOneSaved();

        bool intermedTwoSaved();

        void setIntermedOneSaved(bool saved);

        void setIntermedTwoSaved(bool saved);
};
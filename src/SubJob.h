#pragma once

#include "AccessPoint.h"
#include "DatacenterController.h"

class AccessPoint;
class DatacenterController;
class MapReduceJob;

class SubJob {
    private:
        MapReduceJob* mainJob;

        Vehicle* vehicle;
        AccessPoint* ac;

        bool map;
        Vehicle* intermediateTargetOne;
        int currTransferOne;
        bool intermediateOneTransfered;
        Vehicle* intermediateTargetTwo;
        int currTransferTwo;
        bool intermediateTwoTransfered;

        int inputSize;
        int currInputDownloaded;
        bool inputDownloaded;

        int estimatedCompletionTime;
        int actualCompletionTime;
        int workingTimeToCompletion;
        int currUploaded;
        bool isAssigned;

        bool isMigrating;
        int currMigrated;

        bool workComplete;
        bool uploaded;
        bool complete;
    public:
        SubJob();
        SubJob(MapReduceJob* mainJob, int estimated, bool map, int inputSize);
        ~SubJob();

        void work(DatacenterController* dcController, AccessPoint* acPoint, Vehicle* hostVehicle, int time, int migrationType);

        int getInputSize();

        void setAssigned(bool isAssigned);

        void restart();

        bool assigned();

        bool isComplete();

        int timeToCompletion();

        bool migrating();

        void setMigrating(bool migrate);

        int getActualCompletionTime();

        void setCurrMigrated(int migrated);

        int getCurrMigrated();
        
        bool isBandwidthUser();

        void printACQueue();

        Vehicle* getHost();

        bool waitingForAP();

        void incrementMigrate();

        int getUploadSize();
};
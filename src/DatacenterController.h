#pragma once

#include <list>
#include <string>

#include "JobManager.h"
#include "RegionController.h"

class RegionController;
class JobManager;

class DatacenterController {
    private:
        RegionController* regions[4];
        std::list<Vehicle*> vehicles;
        int shiftToReplace;

        JobManager* jobManager;
    public:
        DatacenterController();
        ~DatacenterController();
        DatacenterController(int numReducers);

        bool isFull();

        void initializeParkingLot();

        void fillVehicles(int time);

        void display();

        void shiftChange(int time);

        void work(int currTime, int migrationType);

        Vehicle* getRandomVehicle(bool notBusy);

        void scheduleReduce(MapReduceJob* job);

        void initializeJobs();

        void checkJobs();

        int numCompletedJobs();

        void assignJob(SubJob* job);

        Vehicle* findMigrationMatch(int timeUntilCompletion, int dataSize, int currTime);

        void displayRunningJobStatus();

        void writeStatsToCSV(std::string filename);
};
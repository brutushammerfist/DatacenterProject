#include <iostream>

#include "JobManager.h"

JobManager::JobManager() {

}

JobManager::JobManager(int numReducers) {
    this->numReducers = numReducers;

    int numJobs = 1;

    while ((double)((numJobs * numReducers) / 2560.0) < 0.8) {
        numJobs++;
    }

    this->numSimulJobs = numJobs;
    this->nextID = 0;
}

JobManager::~JobManager() {

}

MapReduceJob* JobManager::newJob() {
    MapReduceJob* job = new MapReduceJob(this->nextID, this->numReducers);
    this->runningJobs.push_back(job);
    this->nextID++;
    return job;
}

int JobManager::getNumSimulJobs() {
    return this->numSimulJobs;
}

int JobManager::getNumCurrRunning() {
    return this->runningJobs.size();
}

void JobManager::checkJobs(DatacenterController* dcController) {
    std::list<MapReduceJob*>::iterator jitr = this->runningJobs.begin();

    //(*jitr)->printStatus();

    while (jitr != this->runningJobs.end()) {
        if ((*jitr)->complete()) {
            this->completedJobs.push_back(*jitr);
            jitr = this->runningJobs.erase(jitr);
        } else {
            if ((*jitr)->getMapJob()->isComplete()) {
                std::list<SubJob*>::iterator sitr = (*jitr)->getReduceJobs().begin();

                while (sitr != (*jitr)->getReduceJobs().end()) {
                    if (!(*sitr)->assigned()) {
                        dcController->assignJob(*sitr);
                    }

                    sitr++;
                }
            }

            jitr++;
        }
    }
}

int JobManager::getNumCompleted() {
    return this->completedJobs.size();
}

void JobManager::displayRunningJobStatus() {
    std::list<MapReduceJob*>::iterator jitr = this->runningJobs.begin();

    while (jitr != this->runningJobs.end()) {
        if ((*jitr)->complete()) {
            std::cout << "true\n";
            
            int temp = 0;
            std::cin >> temp;
        }

        jitr++;
    }
}

std::list<Stats> JobManager::gatherJobStats() {
    if (this->completedJobs.size() > 0) {
        std::list<Stats> stats;

        std::list<MapReduceJob*>::iterator jitr = this->completedJobs.begin();

        while (jitr != this->completedJobs.end()) {
            Stats stat;
            stat.id = (*jitr)->getID();
            stat.completionTime = (*jitr)->getCompletionTime();
            stat.timesMigrated = (*jitr)->getTimesMigrated();
            stat.timesRestarted = (*jitr)->getTimesRestarted();

            stats.push_back(stat);

            jitr++;
        }

        return stats;
    } else {
        std::list<Stats> stats;

        std::list<MapReduceJob*>::iterator jitr = this->runningJobs.begin();

        while (jitr != this->runningJobs.end()) {
            Stats stat;
            stat.id = (*jitr)->getID();
            stat.completionTime = 0;
            stat.timesMigrated = (*jitr)->getTimesMigrated();
            stat.timesRestarted = (*jitr)->getTimesRestarted();

            stats.push_back(stat);

            jitr++;
        }

        return stats;
    }
}
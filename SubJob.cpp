#include <iostream>

#include "SubJob.h"

SubJob::SubJob() {
    this->mainJob = nullptr;

    this->map = false;

    this->currInputDownloaded = 0;
    this->inputDownloaded = false;

    this->estimatedCompletionTime = -1;
    this->actualCompletionTime = 0;
    this->workingTimeToCompletion = -1;
    this->currUploaded = 0;
    
    this->workComplete = false;
    this->uploaded = false;
    this->complete = false;
    this->isAssigned = false;

    this->intermediateTargetOne = nullptr;
    this->intermediateOneTransfered = false;
    this->currTransferOne = 0;
    this->intermediateTargetTwo = nullptr;
    this->intermediateTwoTransfered = false;
    this->currTransferTwo = 0;
}

SubJob::SubJob(MapReduceJob* mainJob, int estimated, bool map, int inputSize) {
    this->mainJob = mainJob;

    this->map = map;

    this->inputSize = inputSize;
    this->currInputDownloaded = 0;
    this->inputDownloaded = false;

    this->estimatedCompletionTime = estimated;
    this->actualCompletionTime = 0;
    this->workingTimeToCompletion = estimated;
    this->currUploaded = 0;

    this->workComplete = false;
    this->uploaded = false;
    this->complete = false;
    this->isAssigned = false;

    this->intermediateTargetOne = nullptr;
    this->intermediateOneTransfered = false;
    this->currTransferOne = 0;
    this->intermediateTargetTwo = nullptr;
    this->intermediateTwoTransfered = false;
    this->currTransferTwo = 0;

    this->isMigrating = false;
}

SubJob::~SubJob() {

}

void SubJob::work(DatacenterController* dcController, AccessPoint* acPoint, Vehicle* hostVehicle, int time, int migrationType) {
    if (!complete) {
        // Job not finished running and uploading
        this->actualCompletionTime++;

        // migrationType == 0, don't migrate

        if (migrationType == 1) {
            // Migrate ASAP
            // Check every hour(shift change) for someone to swap to
            if (time % 3600 == 0) {
                //If vehicle is not already migrating
                if (!hostVehicle->isMigrating()) {
                    // Job runnitime is greater than departure time, vehicle leaves before job can finish
                    if (((acPoint->etaQueueEmpty() + this->workingTimeToCompletion + (int)(this->mainJob->getIntermediateSize() / 54) + 1) > (hostVehicle->getDeparture() - time + 1))) {
                        Vehicle* migrationVehicle = dcController->findMigrationMatch(this->workingTimeToCompletion, this->mainJob->getIntermediateSize(), time);
                        if (migrationVehicle == nullptr) {
                            // No suitable candidate found
                            //std::cout << "No suitable candidate found\n";
                            if ((hostVehicle->getDeparture() - time) <= 3600) {
                                // Only has one hour left in datacenter, migrate anyway to random vehicle
                                migrationVehicle = dcController->getRandomVehicle(true);
                                hostVehicle->setMigrationTarget(migrationVehicle);
                                hostVehicle->setMigrate(true);
                                this->isMigrating = true;
                            }
                        } else {
                            // Suitable candidate found
                            //std::cout << "Suitable candidate found\n";
                            hostVehicle->setMigrationTarget(migrationVehicle);
                            hostVehicle->setMigrate(true);
                            this->isMigrating = true;
                        }
                    }
                }
            }
        } else if (migrationType == 2) {
            // Migrate Last Minute
            if (!hostVehicle->isMigrating()) {
                if (((acPoint->etaQueueEmpty() + this->workingTimeToCompletion + (int)(this->mainJob->getIntermediateSize() / 54) + 1) > (hostVehicle->getDeparture() - time + 1))) {
                    // Job runtime is greater than departure time, meaning vehicle leaves before job finishes
                    
                    if (((int)(hostVehicle->getMigrateSize() / 54) + 1) == (hostVehicle->getDeparture() - time - 1)) {
                        // Job migration time == departure time - 1
                        Vehicle* migrationVehicle = dcController->findMigrationMatch(this->workingTimeToCompletion, this->mainJob->getIntermediateSize(), time);
                        
                        if (migrationVehicle == nullptr) {
                            // No suitable candidate found
                            hostVehicle->setMigrationTarget(dcController->getRandomVehicle(true));
                            hostVehicle->setMigrate(true);
                        } else {
                            // Suitable candidate found
                            hostVehicle->setMigrationTarget(migrationVehicle);
                            hostVehicle->setMigrate(true);
                        }
                    }
                }
            }
        }
        
        if (this->inputDownloaded) {
            // Input downloaded
            if (hostVehicle->isMigrating()) {
                // Migrating
                //std::cout << "Migrating!\n";
                acPoint->addToBandwidthQueue(hostVehicle);
                if (hostVehicle == acPoint->getBandwidthUser()) {
                    // Host vehicle able to use network
                    hostVehicle->migrate();
                }
            } else {
                if (workComplete) {
                    acPoint->addToBandwidthQueue(hostVehicle);
                    // Work completed, uploading now
                    if (hostVehicle == acPoint->getBandwidthUser()) {
                        // Host vehicle able to use network
                        if (this->map) {
                            // Is a Map Job
                            if (this->intermediateOneTransfered) {
                                // First saved, work on second
                                this->currTransferTwo += 54;

                                if (this->currTransferTwo >= this->mainJob->getIntermediateSize()) {
                                    // Transfer complete
                                    this->mainJob->addRepo(this->intermediateTargetTwo);
                                    this->intermediateTargetTwo->saveJob(this->mainJob);
                                    this->intermediateTwoTransfered = true;

                                    this->uploaded = true;
                                    this->complete = true;
                                    acPoint->swapBandwidthUser();
                                    hostVehicle->setJob(this->mainJob->getFirstReduceJob());
                                    dcController->scheduleReduce(this->mainJob);
                                }
                            } else {
                                // First not saved, work on first
                                this->currTransferOne += 54;

                                if (this->currTransferOne >= this->mainJob->getIntermediateSize()) {
                                    this->mainJob->addRepo(this->intermediateTargetOne);
                                    this->intermediateTargetOne->saveJob(this->mainJob);
                                    this->intermediateOneTransfered = true;
                                }
                            }
                        } else {
                            // Is not a Map Job
                            this->currUploaded += 54;

                            if (this->currUploaded >= this->mainJob->getIntermediateSize()) {
                                this->uploaded = true;
                                this->complete = true;
                                acPoint->swapBandwidthUser();
                                hostVehicle->setBusy(false);
                                hostVehicle->setJob(nullptr);
                                this->mainJob->checkComplete();
                            }
                        }
                    }
                } else {
                    // Work Not Complete
                    this->workingTimeToCompletion--;

                    if (this->workingTimeToCompletion <= 0) {
                        this->workComplete = true;

                        if (this->map) {
                            this->intermediateTargetOne = acPoint->getRandomVehicle();
                            this->intermediateTargetTwo = dcController->getRandomVehicle(false);
                        }
                    }
                }
            }
        } else {
            // Input not downloaded
            acPoint->addToBandwidthQueue(hostVehicle);
            if (hostVehicle == acPoint->getBandwidthUser()) {
                // Host vehicle can use network
                this->currInputDownloaded += 54;
                
                if (this->currInputDownloaded >= this->inputSize) {
                    this->inputDownloaded = true;
                    acPoint->swapBandwidthUser();
                }
            }
        }
    }
}

int SubJob::getInputSize() {
    return this->inputSize;
}

void SubJob::setAssigned(bool isAssigned) {
    this->isAssigned = isAssigned;
}

void SubJob::restart() {
    this->workingTimeToCompletion = this->estimatedCompletionTime;
    this->workComplete = false;
    this->uploaded = false;
    this->complete = false;
    this->isAssigned = false;
    this->currUploaded = 0;
    this->currInputDownloaded = 0;
    this->intermediateTargetOne = nullptr;
    this->intermediateTargetTwo = nullptr;
    this->intermediateOneTransfered = false;
    this->intermediateTwoTransfered = false;
    this->currTransferOne = 0;
    this->currTransferTwo = 0;
}

bool SubJob::assigned() {
    return this->isAssigned;
}

bool SubJob::isComplete() {
    return this->uploaded;
}

int SubJob::timeToCompletion() {
    return this->workingTimeToCompletion;
}

bool SubJob::migrating() {
    return this->isMigrating;
}

void SubJob::setMigrating(bool migrate) {
    this->isMigrating = migrate;
}

int SubJob::getActualCompletionTime() {
    return this->actualCompletionTime;
}
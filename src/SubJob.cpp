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

    this->currMigrated = 0;
    this->isMigrating = false;
    this->inputSize = 0;
    this->ac = nullptr;
    this->vehicle = nullptr;
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
    this->currMigrated = 0;

    this->ac = nullptr;
    this->vehicle = nullptr;
}

SubJob::~SubJob() {

}

void SubJob::work(DatacenterController* dcController, AccessPoint* acPoint, Vehicle* hostVehicle, int time, int migrationType) {
    this->ac = acPoint;
    this->vehicle = hostVehicle;
    
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
                            if ((hostVehicle->getDeparture() - time) <= 3600) {
                                // Only has one hour left in datacenter, migrate anyway to random vehicle
                                migrationVehicle = dcController->getRandomVehicle(true);
                                hostVehicle->setMigrationTarget(migrationVehicle);
                                hostVehicle->setMigrate(true);
                                this->isMigrating = true;
                                this->mainJob->incrementMigrate();

                                // Debug
                                this->ac = acPoint;
                                this->vehicle = hostVehicle;
                            }
                        } else {
                            // Suitable candidate found
                            hostVehicle->setMigrationTarget(migrationVehicle);
                            hostVehicle->setMigrate(true);
                            this->isMigrating = true;
                            this->mainJob->incrementMigrate();

                            // Debug
                            this->ac = acPoint;
                            this->vehicle = hostVehicle;
                        }
                    }
                }
            }
        } else if (migrationType == 2) {
            // Migrate Last Minute
            if (!hostVehicle->isMigrating()) {
                // If job is not already migrating
                int jobRuntime = (acPoint->etaQueueEmpty() + this->workingTimeToCompletion + (int)(this->getUploadSize() / 54) + 1);
                int timeToDeparture = (hostVehicle->getDeparture() - time) + 1;
                if (jobRuntime > timeToDeparture) {
                    // Job runtime is greater than departure time, meaning vehicle leaves before job finishes
                    int jobMigrationTime = (acPoint->etaQueueEmpty() + (int)(hostVehicle->getMigrateSize() / 54) + 1);
                    if (jobMigrationTime == (hostVehicle->getDeparture() - time) + 2) {
                        // Job migration time == departure time + 2
                        
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
                acPoint->addToBandwidthQueue(hostVehicle);
                if (hostVehicle == acPoint->getBandwidthUser()) {
                    // Host vehicle able to use network
                    hostVehicle->migrate(acPoint);
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
    this->mainJob->incrementRestart();
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

void SubJob::setCurrMigrated(int migrated) {
    this->currMigrated = migrated;
}

int SubJob::getCurrMigrated() {
    return this->currMigrated;
}

bool SubJob::isBandwidthUser() {
    if (this->ac == nullptr) {
        return false;
    } else {
        return (this->ac->getBandwidthUser() == this->vehicle);
    }
}

void SubJob::printACQueue() {
    if (this->ac != nullptr) {
        this->ac->printQueue();
    }
}

Vehicle* SubJob::getHost() {
    return this->vehicle;
}

bool SubJob::waitingForAP() {
    if (this->inputDownloaded == false || this->isMigrating == true || (this->workComplete && !this->uploaded)) {
        if (this->ac == nullptr) {
            return false;
        } else {
            if (this->ac->getBandwidthUser() == this->vehicle) {
                return false;
            } else {
                return true;
            }
        }
    } else {
        return false;
    } 
}

void SubJob::incrementMigrate() {
    this->mainJob->incrementMigrate();
}

int SubJob::getUploadSize() {
    if (this->map) {
        if (this->intermediateOneTransfered) {
            return this->mainJob->getIntermediateSize();
        }

        return (this->mainJob->getIntermediateSize() * 2);
    } else {
        return this->mainJob->getIntermediateSize();
    }
}
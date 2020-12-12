#include <iostream>
#include <list>
#include <string>

#include "DatacenterController.h"

/*
 * Command line arguments
 * argv[0] = ./datacenter.exe
 * argv[1] = Number of weeks to run simulation
 * argv[2] = Number of reducers to use(i.e. number of subjobs for the second phase of a job)
 * argv[3] = Migration policy to use. 0 = No migration.
 *                                    1 = migrating as soon as possible.
 *                                    2 = migrating last second.
 */
int main(int argc, char** argv) {
    /*DatacenterController dcController = DatacenterController(atoi(argv[2]));

    const long endTime = atoi(argv[1]) * 604800;
    long time = 0;

    while (time < endTime) {
        time += 1;

        dcController.work(time, atoi(argv[3]));
        
        if (time % 3600 == 0) {
            dcController.shiftChange(time);
        }

        dcController.checkJobs();
    }*/

    for (int i = 0; i < 3; i++) {
        DatacenterController dcController = DatacenterController(atoi(argv[2]));

        const long endTime = atoi(argv[1]) * 604800;
        long time = 0;

        while (time < endTime) {
            time += 1;

            dcController.work(time, i);
            
            if (time % 3600 == 0) {
                dcController.shiftChange(time);
            }

            dcController.checkJobs();
        }

        //std::string filename = "data" + std::to_string(i) + ".csv";
        std::string filename = "";

        switch (i) {
            case 0:
                filename = "no-migration.csv";
                break;
            case 1:
                filename = "front-migration.csv";
                break;
            case 2:
                filename = "back-migration.csv";
                break;
            default:
                break;
        }

        dcController.writeStatsToCSV(filename);

        std::cout << "Simulation complete!\n";
        std::cout << dcController.numCompletedJobs() << " jobs have been completed.\n";
    }

    /*std::string filename;

    switch (atoi(argv[3])) {
        case 0:
            filename = "no-migration.csv";
            break;
        case 1:
            filename = "front-migration.csv";
            break;
        case 2:
            filename = "back-migration.csv";
            break;
        default:
            break;
    }

    dcController.writeStatsToCSV(filename);

    std::cout << "Simulation complete!\n";
    std::cout << dcController.numCompletedJobs() << " jobs have been completed.\n";*/

    return 0;
}
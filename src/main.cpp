#include <iostream>
#include <list>
#include <string>

#include "DatacenterController.h"

/*
 * Command line arguments
 * argv[0] = ./datacenter.exe
 * argv[1] = Number of weeks to run simulation
 * argv[2] = Number of reducers to use(i.e. number of subjobs for the second phase of a job)
 */
int main(int argc, char** argv) {
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

    return 0;
}
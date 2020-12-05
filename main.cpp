#include <iostream>
#include <list>
#include <string>

#include "DatacenterController.h"

/*
 * Command line arguments
 * argv[0] = ./datacenter.exe
 * argv[1] = Number of weeks to run simulation
 * argv[2] = Percentage of cars to have busy(values from 0.0 to 1.0, i.e. 0.7 for 70%)
 * argv[3] = Number of reducers to use(i.e. number of subjobs for the second phase of a job)
 */
int main(int argc, char** argv) {
    const long endTime = atoi(argv[1]) * 604800; // Convert number of weeks to number of seconds and set as end time.
    //const long endTime = 1209600; // Two weeks end time.
    long time = 0;

    DatacenterController dcController = DatacenterController(std::stof(argv[2]), std::atoi(argv[3]));

    while (time < endTime) {
        time += 1;

        // Check if it's 15 minutes before a shift change...
        /*if (time % 3600 == 2700) {
            dcController.migrateLeaving(time);
        }*/

        dcController.work();

        // Check if it's time for a shift change...
        if (time % 3600 == 0) {
            dcController.shiftChange(time);
        }
    }

    std::cout << "Simulation complete!\n";

    return 0;
}
# Datacenter Project

Datacenter Simulation where vehicles in a parking lot are used as resources to host virtual machines.

## Usage
To run the program, you'll simple need to clone the repository and follow these steps once inside the folder.

1. ```cd src```
1. ```make```
1. ```./datacenter.exe {numWeeks} {numReducers}```

Where numWeeks is the number of weeks that you want the simulation to run and numReducers is the number of reduce jobs you want to run for each job. All MapReduceJobs are assumed to split run time 50/50 between the map portion and the reduce portion.

The program will print ```Simulation Complete! {numJobs} have been completed.``` three times in total, once for each migration strategy. numJobs will, of course, be the number of jobs that the simulation was able to complete. Output files will also be generated: ```no-migration.csv```, ```front-migration.csv```, and ```back-migration.csv```, and house the following information: Job ID, Job Completion Time, Times Migrated, Times Restarted. Times Migrated and Times Restarted are the sum of how many times a subjob of that job was either migrated or restarted, respectively.

To generate the graphs, I used ```plots.py```, which takes the output files from the simulation and generates the graphs in the report using plotly and pandas. This program does not require any command line arguments, just the files be in the same directory. To run this, you would simply enter ```python plots.py```. If you're using the ODU CS Linux Server, you may need to use ```python3 plots.py``` instead, as well as installing dependencies: ```python3 -m pip install plotly pandas kaleido```. The graphs will be saved as ```CompletionTime.png``` and ```MigrateRestart.png``` in the same directory that you ran the script.

As a word of warning, when attempting to run this script, the output files from the simulation to need to be the same length, so I manually went through and added ",,," to each line until all files were equal to the length of the longest. Doing this ensures that every dataframe used to create the plots are the same length and don't have any issues.
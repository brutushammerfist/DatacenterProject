all: datacenter

datacenter: main.cpp MapReduceJob.h MapReduceJob.cpp VirtualMachine.h VirtualMachine.cpp AccessPoint.h AccessPoint.cpp DatacenterController.h DatacenterController.cpp GroupController.h GroupController.cpp RegionController.h RegionController.cpp Vehicle.h Vehicle.cpp 
			g++ -o datacenter.exe main.cpp MapReduceJob.h MapReduceJob.cpp VirtualMachine.h VirtualMachine.cpp AccessPoint.h AccessPoint.cpp DatacenterController.h DatacenterController.cpp GroupController.h GroupController.cpp RegionController.h RegionController.cpp Vehicle.h Vehicle.cpp
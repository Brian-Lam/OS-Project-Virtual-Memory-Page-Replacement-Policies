#include "vmsim.h"

int main(int argc, char *argv[]) {
	// Check usage
	if (argc != 4) {
		usage();
		return 1;
	}

	// Parse page count
	std::istringstream iss(argv[1]);
    int pages;
    if (iss >> pages)
    {
        std::cout << "Using " << pages << " pages." << std::endl;
    } else {
    	std::cout << "Please enter an integer for the page count." << std::endl;
    	return 2;
    }

	// Read from file
	std::ifstream referenceFile;
	referenceFile.open(argv[2]);

	// Parse usage choice
	std::string policy = argv[3];
	std::transform(policy.begin(), policy.end(), policy.begin(), ::tolower);

	if (policy == "lru") {
		lru_policy(pages, referenceFile);
	}
	

}

void opt_policy(int pages, std::ifstream& referenceFile){

}
void fifo_policy(int pages, std::ifstream& referenceFile){

}
void lru_policy(int pages, std::ifstream& referenceFile){

}
void clock_policy(int pages, std::ifstream& referenceFile){
}

void usage(){
	std::cout << "Usage:" << std::endl;
	std::cout << "vmsim must accept three command line arguments in the following order: (a) the total number of physical memory frames (maximum 10), (b) an input filename where a sequence of page references is stored, (c) the chosen algorithm (OPT, LRU, FIFO, or CLOCK)" << std::endl;
}
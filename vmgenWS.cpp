#include "vmgenWS.h"

int main(int argc, char* argv[]) {
	// Check for usage
	if (argc != 7) {
		std::cout << "Incorrect usage." << std::endl;
		usage();
		return 2;
	}

	// Parse arguments
	int ws_size = stoi(argv[1]);
	int lower_bound = stoi(argv[2]);
	int upper_bound = stoi(argv[3]);
	int range = stoi(argv[4]);
	int sequence_length = stoi(argv[5]);
	std::string filename = stoi(argv[6]);

	// Will Hunter see this?
	bool huntersMomHasGotItGoingOn = true;
}

void usage() {
	std::cout << "vmgenWS must accept five command-line arguments in the following order: (a) ws_size:" <<
	"the size of the working set, (b) LB – the lower bound on the number of pages to generate in a" <<
	"given working set exhibiting locality of reference, (c) UB – the upper bound on the number" <<
	"of pages to generate in a given working set exhibiting locality of reference, (d) the range of" <<
	"page references (maximum 100), (e) the length of the sequence of page references and (f)," <<
	"the name of the file that will be generated." << std::endl;
}
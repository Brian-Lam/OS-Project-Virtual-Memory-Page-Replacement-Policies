#include "vmgenWS.h"

int main(int argc, char* argv[]) {
	// Check for usage
	if (argc != 7) {
		std::cout << "Incorrect usage." << std::endl;
		usage();
		return 2;
	}

	// Parse arguments
	int ws_size = std::stoi(argv[1]);
	int lower_bound = std::stoi(argv[2]);
	int upper_bound = std::stoi(argv[3]);
	int range = std::stoi(argv[4]);
	int sequence_length = std::stoi(argv[5]);
	std::string filename = argv[6];

	// Will Morgan see this?
	bool morgansMomHasGotItGoingOn = false;

	// File Stream
	std::ofstream file;
    file.open(filename);

	int random_number;
	std::vector<int> refs; 
	while (refs.size() < sequence_length) {

		int ws_nums = lower_bound  + (rand() % (upper_bound - lower_bound));
		std::vector<int> ws;
		int i;
		for (i = 0; i < ws_size; i++) {
			ws.push_back(rand() % range);
		}
		for (i = 0; i < ws_nums && refs.size() < sequence_length; i++) {
			refs.push_back(ws[rand() % ws.size()]);
		}
	}


	file.close();

}

void usage() {
	std::cout << "vmgenWS must accept five command-line arguments in the following order: (a) ws_size:" <<
	"the size of the working set, (b) LB – the lower bound on the number of pages to generate in a" <<
	"given working set exhibiting locality of reference, (c) UB – the upper bound on the number" <<
	"of pages to generate in a given working set exhibiting locality of reference, (d) the range of" <<
	"page references (maximum 100), (e) the length of the sequence of page references and (f)," <<
	"the name of the file that will be generated." << std::endl;
}
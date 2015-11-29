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

	// TODO: put in try-catch
	std::string fileText = readFile(argv[2]);
	std::vector<int> pageRequests = stringSplit(fileText, '\n');

	// Parse usage choice
	std::string policy = argv[3];
	std::transform(policy.begin(), policy.end(), policy.begin(), ::tolower);

	if (policy == "lru") {
		lru_policy(pages, pageRequests);
	}
}

void opt_policy(int pages, std::vector<int>& pageRequests) {

}
void fifo_policy(int pages,std::vector<int>& pageRequests) {

}
void lru_policy(int pages, std::vector<int>& pageRequests) {
	std::vector<int> frames;

	for (int ref: frames) {
		// Check if frames contains this reference
		std::vector <int>::iterator i = find (frames.begin (),frames.end (), ref);

		if (i != frames.end ()) {
			// Page hit
			int nPosition = distance (frames.begin (), i);
			std::cout << "Value "<< *i;
			std::cout << " found in the vector at position: " << nPosition << std::endl;

			// Refresh reference by moving it to the front of the vector
		} else {
			// Page miss
			frames.push_back(ref);
		}
	}
}
void clock_policy(int pages, std::vector<int>& pageRequests) {
}

void usage(){
	std::cout << "Usage:" << std::endl;
	std::cout << "vmsim must accept three command line arguments in the following order: (a) the total number of physical memory frames (maximum 10), (b) an input filename where a sequence of page references is stored, (c) the chosen algorithm (OPT, LRU, FIFO, or CLOCK)" << std::endl;
}

#include <fstream>

std::string readFile(const std::string& fileName) {
    std::string fileText = "";
    std::string line;
    std::ifstream file (fileName);
    if (file.is_open()) {
        while(getline(file,line)) {
            fileText += line + '\n';
        }
        file.close();
    }
    else {
        throw std::runtime_error("Unable to open file");
    }
    return fileText;
}

std::vector<int> stringSplit(const std::string& text, char delimiter) {
	std::vector<int> r;
	size_t last = 0;
	for (size_t i = 0; i < text.size(); i++) {
		if (text[i] == delimiter) {
			r.push_back(stoi(text.substr(last, i - last)));
			last = i + 1;
		}
	}
	return r;
}







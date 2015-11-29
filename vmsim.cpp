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
	else if (policy == "opt") {
		opt_policy(pages, pageRequests);
	}
	else if (policy == "fifo") {
		fifo_policy(pages, pageRequests);
	}
	else if (policy == "clock") {
		clock_policy(pages, pageRequests);
	}
}

void opt_policy(const int pages, std::vector<int>& pageRequests) {

	const size_t maxNumberOfPrograms = 100;
	const size_t n = pageRequests.size();

	size_t** nextUse = new size_t*[maxNumberOfPrograms];
	for (size_t i = 0; i < maxNumberOfPrograms; i++) {
		nextUse[i] = nullptr;
	}

	std::vector< int > programs;
	for (size_t i = n - 2; i < n; i--) {
		if (nextUse[pageRequests[i]] == nullptr) {
			nextUse[pageRequests[i]] = new size_t[n];
			for (size_t j = i + 1; j < n; j++) {
				nextUse[pageRequests[i]][j] = 4294967295;
			}
		}
		for (size_t j = 0; j < programs.size(); j++) {
			size_t p = programs[j];
			nextUse[p][i] = nextUse[p][i + 1] + 1;
		}
		programs.push_back(pageRequests[i]);
		nextUse[pageRequests[i]][i] = 0;
	}

	size_t* mem;
	mem = new size_t[pages];
	for (size_t i = 0; i < pages; i++) {
		mem[i] = maxNumberOfPrograms;
	}
	for (size_t i = 0; i < pageRequests.size(); i++) {

		// TODO
		bool satisfied = false;
		size_t j;
		for (j = 0; j < pages; j++) {
			if (mem[j] == maxNumberOfPrograms) {
				break;
			}
			if (mem[j] == pageRequests[i]) {
				satisfied = true;
				break;
			}
		}

		if (!satisfied) {
			if (j < pages) {
				mem[j] = pageRequests[i];
			}
			else {
				// find the program in mem[] with the biggest value in nextUse[]
				size_t max_index = 0;
				for (size_t j = 1; j < pages; j++) {
					if (nextUse[mem[j]][i] > nextUse[mem[max_index]][i]) {
						max_index = j;
					}
				}
				mem[max_index] = pageRequests[i];
			}
		}

		std::cout << pageRequests[i] << ": [";
		if (std::to_string(mem[0]).size() == 1) {
			std::cout << " " << std::to_string(mem[0]);
		}
		else {
			std::cout << std::to_string(mem[0]);
		}
		for (size_t k = 1; k < pages; k++) {
			if (mem[k] == maxNumberOfPrograms) {
				std::cout << "|  ";
			}
			else {
				std::string s = std::to_string(mem[k]);
				if (s.size() == 1) {
					std::cout << "| " << s;
				}
				else {
					std::cout << "|" << s;
				}
			}
		}
		std::cout << "]" << std::endl;
	}

	for (size_t i = 0; i < maxNumberOfPrograms; i++) {
		if (nextUse[i] != nullptr) {
			// std::cout << i << ": ";
			// for (size_t j = 0; j < n; j++) {
			// 	std::cout << nextUse[i][j] << ", ";
			// }
			// std::cout << std::endl;
			delete[] nextUse[i];
		}
	}
	delete[] nextUse;
}
void fifo_policy(int pages,std::vector<int>& pageRequests) {
	// TODO
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
	// TODO
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







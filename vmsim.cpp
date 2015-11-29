#include "vmsim.h"

/*
*	Vmsim.cpp
*
* 	This is the source code file for the Virtual Memory Policy 
* 	Simulator, written for the Operating Systems Course (CSE422)
*	at Washington University in St Louis, which is different from
*	University of Washington. 
*
*	Contributors: Brian Lam, Morgan Redding, Hunter Wills, Alex Tran
*
*/


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
	} else if (policy == "fifo") {
		fifo_policy(pages, pageRequests);
	} else if (policy == "clock") {
		clock_policy(pages, pageRequests);
	} else if (policy == "opt") {
		opt_policy(pages, pageRequests);
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
	bool* pageFault = new bool[pageRequests.size()];
	mem = new size_t[pages];
	for (size_t i = 0; i < pages; i++) {
		mem[i] = maxNumberOfPrograms;
	}
	for (size_t i = 0; i < pageRequests.size(); i++) {

		// TODO
		pageFault[i] = true;
		size_t j;
		for (j = 0; j < pages; j++) {
			if (mem[j] == maxNumberOfPrograms) {
				break;
			}
			if (mem[j] == pageRequests[i]) {
				pageFault[i] = false;
				break;
			}
		}

		if (pageFault[i]) {
			if (j < pages) {
				mem[j] = pageRequests[i];
				pageFault[i] = false;
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
		std::cout << "]";
		if (pageFault[i]) {
			std::cout << " F";
		}
		std::cout << std::endl;
	}

	for (size_t i = 0; i < maxNumberOfPrograms; i++) {
		if (nextUse[i] != nullptr) {
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

	for (int ref: pageRequests) {
		// Check if frames contains this reference
		std::vector <int>::iterator i = find (frames.begin (),frames.end (), ref);

		if (i != frames.end ()) {
			// Page hit
			int nPosition = distance (frames.begin (), i);

			// Refresh reference by moving it to the front of the vector
			std::vector<int> newFrames;
			newFrames.push_back(ref);
			for (int pageFrame: frames) {
				if (pageFrame != ref) {
					newFrames.push_back(pageFrame);
				}
			}
			frames = newFrames;

			// Show status of the pages
			printPages(ref, frames, pages, false);
		} else {
			// Page miss
			// Check if page frames are full
			if (frames.size() >= pages) {
				frames.pop_back();
			}
			// Create a new vector with the new ref in the front
			std::vector<int> newFrames;
			newFrames.push_back(ref);
			for (int pageFrame: frames) {
				if (pageFrame != ref) {
					newFrames.push_back(pageFrame);
				}
			}
			frames = newFrames;

			// Show status of the pages
			printPages(ref, frames, pages, true);

		}
	}
}
void clock_policy(int pages, std::vector<int>& pageRequests) {
    std::vector<int> cached;
    std::vector<int> usage;
    for (auto it = pageRequests.begin(); it != pageRequests.end(); it++) {
        auto inCacheIterator = std::find(cached.begin(), cached.end(), *it);
        bool hadToReplace = false;
        if (inCacheIterator == cached.end()) { //request is NOT contained in current cache
            if (cached.size() < pages) { //room on the cache to just push current page
                cached.push_back(*it);
                usage.push_back(1);
            }
            else { //time to replace!
                hadToReplace = true;
                int i;
                for (i = 0; i < cached.size(); i++) {
                    if (usage[i] == 0) { //found a page to replace
                        cached[i] = *it;
                        usage[i] = 1;
                        break;
                    }
                    else {
                        usage[i] = 1;
                    }
                }
                if (i == cached.size()) { //looping through no page was replaced so we must replace the first entry
                    cached[0] = *it;
                    usage[0] = 1;
                }
            }
        }
        else {
            int indexInCache;
            for (indexInCache = 0; cached[indexInCache] != *inCacheIterator; indexInCache++);
            usage[indexInCache] = 1;
        }
        printPages(*it, cached, pages, hadToReplace);
    }
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

/*
*	Print the status of the pages in memory after a request. 
*	@args:
*		int request: The page number being requested
*		std::vector<int>& pages: The vector representing pages already in memory
*		int numPages: The capacity of pages in memory
*		bool pageFault: True if there was a page fault for this request
*/
void printPages(int request,std::vector<int>& pages, int numPages, bool pageFault) {
	// Pad with a space if request is 1 digit
	if (request < 10) {
		std::cout << " ";
	}

	std::cout << request << ": [";
	for (int cachedPage: pages) {
		if (cachedPage < 10) {
			// Pad with a space if page is 1 digit
			std::cout << " ";
		}
		std::cout << cachedPage << "|";
	}

	// Pad empty spots in memory
	int emptySpots = numPages - pages.size();
	if (emptySpots) {
		for (int _i = 0; _i < emptySpots; _i++) {
			std::cout << "  |";
		}
	}

	// Show whether there was a page fault at this step
	if (pageFault) {
		std::cout << "] F" << std::endl;
	} else {
		std::cout << "]" << std::endl;
	}

}

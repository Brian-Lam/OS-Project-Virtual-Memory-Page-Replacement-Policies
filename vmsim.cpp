#include "vmsim.h"
#include <map>

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

void opt_policy(const int numberOfPages, std::vector<int>& pageRequests) {

	const size_t maxNumberOfPrograms = 100;
	const size_t n = pageRequests.size();

	size_t** nextUse = new size_t*[maxNumberOfPrograms];
	for (size_t i = 0; i < maxNumberOfPrograms; i++) {
		nextUse[i] = nullptr;
	}

	// for every program p and every request t, set nextUse[p][t] to how
	// many requests away the next request for "p" is
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

	// represents the frames in memory
	// each mem[i] is program id
	size_t* mem;
	mem = new size_t[numberOfPages];

	// initialize all elements in mem[] to "empty"
	const size_t empty = maxNumberOfPrograms;
	for (size_t i = 0; i < numberOfPages; i++) {
		mem[i] = empty;
	}

	// for every page request, satisfy it and print out the state of mem[]
	for (size_t i = 0; i < pageRequests.size(); i++) {

		// if the request is already in mem[], skip it
		// otherwise indicate a page fault
		bool pageFault = true;
		size_t j;
		for (j = 0; j < numberOfPages; j++) {
			if (mem[j] == empty) {
				break;
			}
			if (mem[j] == pageRequests[i]) {
				pageFault = false;
				break;
			}
		}

		if (pageFault) {
			// if there is an empty slot in mem[], just use that
			if (j < numberOfPages) {
				mem[j] = pageRequests[i];
				pageFault = false;
			}
			// otherwise find the optimal program to replace
			else {
				// find the program in mem[] with the biggest value in nextUse[]
				// replace this program's spot in mem[] with the current requesting program
				size_t max_index = 0;
				for (size_t j = 1; j < numberOfPages; j++) {
					if (nextUse[mem[j]][i] > nextUse[mem[max_index]][i]) {
						max_index = j;
					}
				}
				mem[max_index] = pageRequests[i];
			}
		}

		// print out the state of mem[]
		if (std::to_string(pageRequests[i]).size() == 1) {
                    std::cout << " ";
                }
                std::cout << pageRequests[i] << ": [";
		if (std::to_string(mem[0]).size() == 1) {
			std::cout << " " << std::to_string(mem[0]);
		}
		else {
			std::cout << std::to_string(mem[0]);
		}
		for (size_t k = 1; k < numberOfPages; k++) {
			if (mem[k] == empty) {
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
		if (pageFault) {
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
   std::vector<int> frame;
   int replaceIndex= 0;
   for (std::vector<int>::iterator it=pageRequests.begin(); it != pageRequests.end(); ++it)
   {
      //print out page requested
      if (std::find(frame.begin(), frame.end(), *it) != frame.end())
      {
         //hit, print out frame
         printPages(*it, frame, pages, false);
      }
      else 
      {
         if (frame.size()< pages) 
         {
            //frame is not full
            frame.push_back(*it);

            //print out frame
            printPages(*it, frame, pages, false);
         }
         else 
         {
            //page fault, replace a page in FIFO order
            frame[replaceIndex++]= *it;
            if (replaceIndex==pages){
               replaceIndex=0;
            }

            //print out frame
            printPages(*it, frame, pages, true);
         }
      }
   }
}
void lru_policy(int pages, std::vector<int>& pageRequests) {
    std::vector<int> cached;
    std::map<int,int> usage;
    for (int i = 0; i < pageRequests.size(); ++i) {
        auto inCacheIterator = std::find(cached.begin(), cached.end(), pageRequests[i]);
        bool hadToReplace = false;
        if (inCacheIterator == cached.end()) { //request is NOT contained in current cache
            if (cached.size() < pages) { //room on the cache to just push current page
                cached.push_back(pageRequests[i]);
                usage[pageRequests[i]] = i;
            }
            else { //time to replace!
                hadToReplace = true;
                int indexToReplace = 0;
                for (int j = 1; j < cached.size(); ++j) {
                    if (usage[cached[j]] < usage[cached[indexToReplace]]) {
                        indexToReplace = j;
                    }
                }
                cached[indexToReplace] = pageRequests[i];
                usage[pageRequests[i]] = i;
            }
        }
        else { //update the last use of this variable
            usage[pageRequests[i]] = i;
        }
        printPages(pageRequests[i], cached, pages, hadToReplace);
    }
}

void clock_policy(int pages, std::vector<int>& pageRequests) {
    std::vector<int> cached;
    std::vector<int> usage;
    int i = 0;
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
                while (true) {
                    if (usage[i] == 0) { //found a page to replace
                        cached[i] = *it;
                        usage[i] = 1;
                        i = (i + 1) % pages;
                        break;
                    }
                    else {
                        usage[i] = 0;
                    }
                    i = (i + 1) % pages;
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
    int pageIndex = 0;

    for (std::vector<int>::iterator it = pages.begin() ; it != pages.end(); ++it) {
	    int cachedPage = *it;
	    if (cachedPage && cachedPage < 10) {
			// Pad with a space if page is 1 digit
			std::cout << " ";
		}
		if (std::next(it) != pages.end()) {
	        std::cout << cachedPage << "|";
		} else {
			std::cout << cachedPage;
			if (pages.size() < numPages) {
				std::cout << "|";
			}
		}
    }

	// Pad empty spots in memory
	int emptySpots = numPages - pages.size();
	if (emptySpots) {
		for (int _i = 1; _i < emptySpots; _i++) {
			std::cout << "  |";
		}
		std::cout << "  ";
	}

	// Show whether there was a page fault at this step
	if (pageFault) {
		std::cout << "] F" << std::endl;
	} else {
		std::cout << "]" << std::endl;
	}

}

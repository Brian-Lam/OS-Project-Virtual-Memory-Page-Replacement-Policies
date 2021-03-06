#include <iostream>
#include <algorithm>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <stdexcept>
#include <map>

void opt_policy(int pages, std::vector<int>& pageRequests);
void fifo_policy(int pages, std::vector<int>& pageRequests);
void lru_policy(int pages, std::vector<int>& pageRequests);
void clock_policy(int pages, std::vector<int>& pageRequests);
void printPages(int request,std::vector<int>& pages, int numPages, bool pageFault);
void usage();
std::string readFile(const std::string& fileName);
std::vector<int> stringSplit(const std::string& text, char delimiter);
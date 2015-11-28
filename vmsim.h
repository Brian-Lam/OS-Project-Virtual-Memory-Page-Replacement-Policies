#include <iostream>
#include <algorithm>
#include <string>
#include <sstream>
#include <fstream>

void opt_policy(int pages, std::ifstream & referenceFile);
void fifo_policy(int pages, std::ifstream & referenceFile);
void lru_policy(int pages, std::ifstream & referenceFile);
void clock_policy(int pages, std::ifstream & referenceFile);
void usage();
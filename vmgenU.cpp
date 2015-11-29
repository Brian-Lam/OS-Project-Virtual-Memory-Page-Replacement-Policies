#include <iostream>
#include <string>
#include <time.h>
#include <fstream>

void writeFile(const std::string& fileName, const std::string& text) {
    std::ofstream file;
    file.open(fileName);
    file << text;
    file.close();
}

int main(int argc, char *argv[]) {

	if (argc != 4) {
		std::cout << "This program takes 3 arguments; you only gave it " << (argc - 1) << std::endl;
		std::cout << "It needs: \"range of page references\", \"length of sequence\", and \"output file name\"" << std::endl;
		return 1;
	}

	int rangeOfPageReferences = atoi(argv[1]);
	int lengthOfSequence = atoi(argv[2]);
	std::string outputFileName = argv[3];
	if (rangeOfPageReferences < 1 || rangeOfPageReferences > 100) {
		std::cout << "Range of page references (argument 1) must be an integer between 1 and 100" << std::endl;
		return 1;
	}

	srand(time(NULL));

	std::string t = "";
	for (size_t i = 0; i < lengthOfSequence; i++) {
		t += std::to_string(rand() % rangeOfPageReferences);
		t += '\n';
	}

	writeFile(outputFileName, t);

	return 0;
}
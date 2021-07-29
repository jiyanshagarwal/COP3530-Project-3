#include <fstream>
#include "DataReader.h"

bool DataReader::read(std::string filename, unsigned int num_lines) {
	std::ifstream file;
	file.open(filename);

	if (file.is_open()) {
		std::string line;
		getline(file, line);		//Skip the first line since it is just column headers

		for (unsigned int i = 0; i < num_lines && getline(file, line); i++) {
			unsigned int comma_index;
			std::vector<std::string> vec;

			while (comma_index = line.find(',')) {
				std::string field = line.substr(0, comma_index);
				line = line.substr(comma_index + 1);
				vec.push_back(field);
			}
		}

		file.close();
		return true;
	}

	return false;
}
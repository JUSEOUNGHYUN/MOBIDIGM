#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <Windows.h>
#include <vector>

using namespace std;


int main() {

	string filename("C:\\test\\outputSF.txt");
	vector<string> lines;
	string savefile;

	ifstream input_file(filename);
	if (!input_file.is_open()) {
		cerr << "No FIle" << filename << " " << "\n";
		return EXIT_FAILURE;
	}

	while (getline(input_file, savefile)) {
		lines.push_back(savefile);
	}

	for (const auto& i : lines)
		cout << i << "\n";

	input_file.close();
	return EXIT_SUCCESS;


}


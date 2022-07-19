#include "IOfstream.h";

using namespace std;

char ch[1000] = { 0 };

int main() {

	IOfstream* Write_file = new IOfstream("C:\\test\\outputSF.txt");
	WriteNMEAData* writeNMEAData = new WriteNMEAData("C:\\test\\write.nmea");
	int cnt = 0;
	string* readData = NULL;
	while ((readData = Write_file->GetNextData()) != NULL) {
		if (readData != NULL) {
			printf("[%d] [%s]\n", cnt++, readData->c_str());
			writeNMEAData->WriteData(*readData);
			delete readData;

		}
	}
	if (Write_file != NULL) {
		delete Write_file;
	}

	if (writeNMEAData != nullptr) {
		delete writeNMEAData;
	}

	return 0;

}
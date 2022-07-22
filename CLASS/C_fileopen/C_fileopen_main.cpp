#include "C_fileopen.h"

int main() {

    C_fileopen* str_file = new C_fileopen("C:\\test\\outputSF.txt");
    WriteNMEAData* writeNMEAData = new WriteNMEAData("C:\\test\\write.txt"); // write class
    int cnt = 0;
    std::string* readData = NULL;
    while ((readData = str_file->GetNextData()) != NULL) {
        //printf readData
        if (readData != NULL) {
            writeNMEAData->WriteData(*readData); // write data
            printf("[%d] [%s]\n", cnt++, readData->c_str());
            delete readData;
        }
    }
    if (str_file != NULL) {
        delete str_file;
    }

    if (writeNMEAData != nullptr) {
        delete writeNMEAData;
    }

    return 0;

	return 0;
}
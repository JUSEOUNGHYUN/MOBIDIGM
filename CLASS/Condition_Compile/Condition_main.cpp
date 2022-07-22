#include "condition.h"



int main() {

    M_File* str_file = new M_File("C:\\test\\output.nmea");
    WriteNMEAData* writeNMEAData = new WriteNMEAData("C:\\test\\write.nmea"); // write class
    int cnt = 0;
    string* readData = NULL;
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

}

// /home/jsh/output.txt
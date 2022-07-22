#include "M_file.h"

int main() {


    M_File* str_file = new M_File("C:\\test\\output.nmea");
    //ofstream fout;
    //fout.open("C:\\test\\write.nmea");

    // write class
    WriteNMEAData* writeNMEAData = new WriteNMEAData("C:\\test\\write.nmea");

    
    int cnt = 0;
    string* readData = NULL;
    while ((readData = str_file->GetNextData()) != NULL) {
        //printf readData
        if (readData != NULL) {
            //printf("[%d] [%s]\n", cnt++, readData->c_str());
            // write data
            writeNMEAData->WriteData(*readData);
            //fout << *readData;

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

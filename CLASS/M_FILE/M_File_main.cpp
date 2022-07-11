#include "M_File.h"

int main() {


    M_File* str_file = new M_File("C:\\test\\output.nmea");

    int cnt = 0;
    string* readData = NULL;
    while ((readData = str_file->GetNextData()) != NULL) {
        //printf readData
        if (readData != NULL) {
            printf("[%d] [%s]\n", cnt++, readData->c_str());

            delete readData;
        }
    }


    if (str_file != NULL) {
        delete str_file;
    }
    return 0;

}

// /home/jsh/output.txt
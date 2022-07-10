#include <iostream>
#include <string>
#include <cstring>
#include <stdlib.h>
#include <cstdio>

using namespace std;

class File_open
{
    private:
        string Data_str;
        int Nmea_checksum;
        int check_made;
    public:
        File_open();
        ~File_open();
        bool IsCorrect_getData(string str);
    };



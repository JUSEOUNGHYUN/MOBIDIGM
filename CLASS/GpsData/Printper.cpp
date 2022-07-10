#include "Printper.h"

File_open::File_open() {
    cout << "File is called" << "\n";
    Data_str.clear();
    Nmea_checksum = 0;
    check_made = 0;
    }

File_open::~File_open() {
    cout << "FIle is closed" << "\n";
}

    bool File_open::IsCorrect_getData(string str) {
        Data_str = str;

        int pos = Data_str.rfind('*');
        string Second_data = Data_str.substr(pos + 1);

        int nMea_checksum = strtol(Second_data.c_str(), NULL, 16);

        int slen = str.length();

        int check_made = 0;

        for (int i = 0; i < slen; i++) {
            char value = Data_str.at(i);
            if (value == '$') {
                continue;
            }
            else if (value == '*') {
                break;
            }
            else {
                check_made ^= value;    // Exclusive 완료
            }
        }

        return Nmea_checksum = check_made ? true : false;
    }

    int main() {

        File_open file;
        cout << file.IsCorrect_getData("$GPRMC,215300.800,A,4730.429,N,01901.933,E,278.0,281.5,020821,000.0,W*72") << "\n";
}

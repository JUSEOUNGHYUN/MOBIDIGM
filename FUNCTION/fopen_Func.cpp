#include <iostream>
#include <string>
#include <stdlib.h>
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <Windows.h>
#include <sstream>

using namespace std;

FILE* fp = NULL;

// declar
void GetNextNumber_open(string strPath);
string intToString(int n);
bool IsCorrectChecksum(string strNMEA);


void GetNextNumber_open(string strPath)
{
    //fp = fopen("C:\\test\\output.nmea", "r");
    fp = fopen(strPath.c_str(), "r");
}

void GeTNextNumber_close() {
    printf("File Close\n");
    if (fp != NULL) {
        fclose(fp);
    }
    fp = NULL;
}

string GetNextNumber_nextData() {  // FILE open
    string nmea_data;
    nmea_data.clear();
    char Char_data[1000] = { 0 };
    if (fp != nullptr) {
        fgets(Char_data, 1000, fp);
        nmea_data = Char_data;
        if (!IsCorrectChecksum(nmea_data)) {
            nmea_data.clear();
        }
    }
    return nmea_data;
}

string intToString(int n) {  // to_string 함수 구현
    stringstream s;
    s << n;
    return s.str();
}

int StringToint(string str) {   // stoi 함수 구현
    int n;
    stringstream(str) >> n;
    return n;
}

bool IsCorrectChecksum(string f_nmea_data) {   // exclusive한 숫자 리턴 (string)

    string Finish_num;  // 애스터리스크 오른쪽 숫자
    int pos = f_nmea_data.rfind('*');
    string First_num = f_nmea_data.substr(0, pos);
    Finish_num = f_nmea_data.substr(pos + 1);

    int slen = f_nmea_data.length();
    int check_made = 0; // 애스터리스크 왼쪽 문자열 Exclusive

    for (int i = 0; i < slen; i++) {
        char value = f_nmea_data.at(i);
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

    int nmea_checksum = strtol(Finish_num.c_str(), NULL, 16);

    
    if (nmea_checksum == check_made) {
        return true;
    }
    else {
        return false;
    }

}

int main() {

    string Data_address = "C:\\test\\output.nmea";
    GetNextNumber_open(Data_address);
    string n_nmea_data;
    
     while (!n_nmea_data.empty()){
        n_nmea_data = GetNextNumber_nextData();
        printf("%s", n_nmea_data);
        GeTNextNumber_close();
    }
        return 0;

}
// fclose(fp);
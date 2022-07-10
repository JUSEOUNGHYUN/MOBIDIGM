#include <iostream>
#include <string>
#include <stdlib.h>
#include <cstdio>
#include <cstring>

using namespace std;

class Wkf{
    private:
        string Data_str;    // GPS 데이터
        int nMea_checksum;  // 체크섬 데이터
        int check_made;     // 메세지 데이터
    public:
        int CheckSum_data();    // 체크섬 데이터 출력
        int Exclu_getData();    // 메세지 데이터 출력
        bool IsCorrect_getData();   // 체크섬 메세지 같은지 출력
        void setData(string str);   // GPS 데이터 입력
};

void Wkf::setData(string str){  // GPS 데이터 입력
    Data_str = str;
}

int Wkf::CheckSum_data(){     // CheckSum 데이터
    int pos = Data_str.rfind('*');
    string Second_data = Data_str.substr(pos+1);

    nMea_checksum = strtol(Second_data.c_str(), NULL, 16);
    return nMea_checksum;

}

int Wkf::Exclu_getData(){   // Message 데이터
    int slen = Data_str.length();

    check_made = 0;

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
    return check_made;
}

bool Wkf::IsCorrect_getData(){  // CheckSum == Message 데이터 비교
    return nMea_checksum = check_made ? true : false;
}

int main() {    // 구현 단계

    Wkf w;
    w.setData("$GPRMC,215300.800,A,4730.429,N,01901.933,E,278.0,281.5,020821,000.0,W*72");
    cout<<w.CheckSum_data()<<" "<<w.Exclu_getData()<<" ";
    cout<<w.IsCorrect_getData();
}


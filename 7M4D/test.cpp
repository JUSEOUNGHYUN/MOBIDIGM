#include <iostream>
#include <string>
#include <list>
#include <windows.h>
#include <algorithm>
#include <sstream>
#include <cstring>

using namespace std;

int main() {

    FILE *fp;
    char sstr[1000];
    int len;
    int cnt;

    fp = fopen("C:\\test\\output.nmea", "r");

    while(fgets(sstr, 100, fp)!=NULL){
        

        string str(sstr);           // char[] -> string
        istringstream ss(str);     // 문자열 파싱 함수 (**)
        list <string> gps;          // string 문자열을 넣을 list 배열 생성
        //list <int>::iterator it; // 순방향 반복자
        string gp;                  // 기존 문자열을 짜를 문자열 생성
        int len = strlen(sstr);     
        if(sstr[len-1]=='\n'){
            Sleep(50);
        }
        printf("%s", sstr);
        while(getline(ss, gp, ',')){    // (**) 콤마 단위로 문자열을 짜른다.    
            gps.push_back(gp);          // 짜른 문자열들을 list(gp)에 넣는다.        
        }

        len=gp.length();
        cnt=gps.size()-1;
        list<string>::reverse_iterator riter(gps.rbegin()); // 역방향 반복자


        for(; riter != gps.rend(); ++riter){
            
            cout<<"["<<cnt<<" : " << (*riter).length()<<" : "<<*riter<<"]";
            cnt--;
        }
    
        /*  순방향
        for(it=gps.begin(); it!=gps.end(); it++){
            cout<<*it;
            cnt++;
        }
        */
    }
        
        fclose(fp);

        return 0;
}

/*
list
istringstream
getline
*/
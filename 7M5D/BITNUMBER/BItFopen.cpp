#include <iostream>
#include <string>
#include <stdlib.h>
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <Windows.h>

using namespace std;

int main() {

    FILE *fp;
    char ch[1000];

    fp = fopen("C:\\test\\outputSF.txt","r");

    while(fgets(ch, 1000, fp)!=NULL){
        string str = ch;
        int clen = strlen(ch);

        if(ch[clen-1]=='\n'){
            Sleep(50);
        }
        printf("%s", ch);

        int pos = str.rfind('*');
        string second = str.substr(pos+1);

        int len = str.length();

        int checksum = 0;

        for(int i=0; i<len; i++){
            char value = str.at(i);
            if(value=='$'){
                continue;
            }
            else if(value == '*'){
                break;
            }
            else{
                checksum ^= value;
            }
        }

        char hexadecimal[20] = { 0, };    
        int position = 0;

        while (1){      // 16진수 변환 구현
            int mod = checksum % 16;    
            if (mod < 10){
                hexadecimal[position] = 48 + mod;
            }
            else{ 
                hexadecimal[position] = 65 + (mod - 10);
            }
            checksum = checksum / 16;
            position++; 
            if (checksum == 0)
                break;
    }

        int cnt=0;

        for (int i = position - 1; i >= 0; i--){    // 뒷자리수 : second와 exclusive한 수의 비교
            if(second[0]==hexadecimal[1]&&second[1]==hexadecimal[0]){
                cnt++;
            }
            else{
                cnt=0;
            }
        }

            if(cnt>0){
            cout<<"TRUE"<<"\n";
            }
            else{
                cout<<"FALSE"<<"\n";
            }

            cout<<"\n";
    }

    fclose(fp);

    return 0;
}
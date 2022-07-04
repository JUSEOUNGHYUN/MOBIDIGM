#include <stdio.h>
#include <windows.h>

int main() {

    FILE *fp;
    char *str;

    fp = fopen("C:\\test\\output.nmea", "r");

    while(fgets(str, 100, fp)!=NULL){
        int len = strlen(str);
        if(str[len-1]=='\n'){
            Sleep(50);
        }
        printf("%s", str);
    }

    fclose(fp);

    return 0;
}

/*
fgets 함수로 한줄씩 출력


*/
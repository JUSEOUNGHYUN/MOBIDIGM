#include <stdio.h>
#include <Windows.h>    // Sleep함수를 쓰기위한 헤더파일

int main() {

    FILE *fp;
    int pinter;

    for(int i=0; i<2; i++){
        fp = fopen("C:\\test\\asdf.txt", "r"); // 파일 절대 경로 포인터에 저장

        while(1){
            pinter = fgetc(fp);
            if(pinter == '\n'){
                Sleep(50);
            }
            if(feof(fp)){
                break;
            }

            printf("%c", pinter);
        }
    }
    fclose(fp);

    return 0;
}

//C:\\test\\asdf
/* 
fgetc : 파일에 문자를 하나씩 읽는다.

fgets : 개방된 파일에서 텍스트 문자열을 한 줄씩 읽는다.
- 파일 끝이나 개행 문자까지 읽는다.
- 라인 끝을 읽으면 개행 문자 '\n'으로 변환
- string 끝에 NULL문자 추가

feof : 파일의 끝에 도달했는지 여부를 확인할 때 사용

fclose : 반환형은 int 인데 정상적으로 파일을 닫았을 땐 0을 반환하고 
그렇지 않을땐 EOF (-1)을 반환합니다.

EOF : End Of File
*/
#include <iostream>

using std::cout;

int main() {

    int x = 10;
    int y;

    int* px; // 선언문, 포인터 px 선언

    px = &x; // 포인터 px에 x 주소 대입
    y = *px; // 실행문, px의 주소로 가서 값을 가져옴;

    cout<<y;

    return 0;
}
// 이항 연산자 오버로딩 == operator +

#include <iostream>

using std::cout;

class Point{
    private:
        int x;
        int y;
    public:
        Point(){
            x=0;
            y=0;
        }
        Point(int xx, int yy){
            x=xx;
            y=yy;
        }
        int getX(){
            return x;
        }
        int getY(){
            return y;
        }
        Point operator +(Point ob){ // (operator +) = 함수 이름
            Point temp;
            temp.x = x+ob.x;
            temp.y = y+ob.y;    // ob1.y + ob2.y
            return temp;
        }
        // + 연산자는 두 수를 더하는 연산자
        // Point 클래스형 객체에서는 맴버 변수 x,y를 각각 더하는 연산자로 재정의
        // 이름은 + 그대로 쓴다.
        
        std::string operator ==(Point ob){
            return (x==ob.x && y == ob.y) ? "true":"false";
        }
};

int main() {
    Point ob1(3,5), ob2(4,6), ob3;
    // ob1 = Point() | ob2 = Point(int xx, int yy) | ob3 Point operator + (Point temp)
    cout<<ob3.getX()<<" "<<ob3.getY()<<" ";
    ob3 = ob1 + ob2;
    cout<<ob3.getX()<<" "<<ob3.getY();
    cout<<(ob1==ob2);
    return 0;
}

// Point의 객체들끼리 더하는 것을 함수로 만든것이다. == operator + 그냥 이름임
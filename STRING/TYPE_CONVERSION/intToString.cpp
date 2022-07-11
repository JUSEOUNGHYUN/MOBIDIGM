#include <iostream>
#include <string>
#include <sstream>

using namespace std;

string intToString(int n) {  // to_string 함수 구현
    stringstream s;
    s << n;
    return s.str();
}
#include <iostream>
#include <list>
#include <sstream>
#include <string>
#include <algorithm>

using namespace std;

int main() {

    int len;
    int cnt=0;

    string str;
    cin>>str;
    istringstream ss(str);
    list<string> gps;
    string gp;

    while(getline(ss, gp, ',')){
        gps.push_back(gp);
        len = gp.length();

        cout<<"["<<cnt<<" : "<<len<<" : "<<gp<<"]\0";
        cnt++;
    }

    return 0;

    }


#include <iostream>
#include <cstdio>
#include <string>
#include <cstring>
#include <string.h>

// ex) C:\\test\\Butter.mp3 -> C:\\test\\Butter.
std::string strFileFormatSubstr(std::string strTemp) {

	int pos;

	pos = strTemp.rfind('.');
	std::string first = strTemp.substr(0, pos + 1);
	return  first;
}
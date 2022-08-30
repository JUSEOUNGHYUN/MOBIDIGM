#include <iostream>
#include <cstdio>
#include <string>
#include <stdlib.h>
#include <cstring>
#include <string.h>
#include <Windows.h>

// ex) C:\\test\\Butter.mp3 -> C:\\test\\Butter
std::string strFileFormatDotSubstr(std::string strTemp) {

	int pos;

	pos = strTemp.rfind('.');
	std::string first = strTemp.substr(0, pos);
	return  first;
}
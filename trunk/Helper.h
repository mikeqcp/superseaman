#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

class Helper
{
public:
	Helper(void);
	~Helper(void);

	string ReadFile(string fileName);
	char* ReadBinaryFile(char* fileName);

	vector<string> SplitString(string str);
};


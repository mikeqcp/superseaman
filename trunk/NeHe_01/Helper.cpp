#include "StdAfx.h"
#include "Helper.h"
#include <sstream>

Helper::Helper(void)
{
}


Helper::~Helper(void)
{
}

vector<string> Helper::SplitString(string str){

	vector<string> result;

	istringstream iss(str);
	string sub;
	while(!iss.eof()){
		iss >> sub;
		result.push_back(sub);
	}

	return result;

}
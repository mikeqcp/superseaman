#include "Helper.h"


Helper::Helper(void)
{
}


Helper::~Helper(void)
{
}

string Helper::ReadFile(string fileName){

	string input, result = "";
	char readC;
	ifstream file;
	file.open(fileName.c_str(), ios_base::beg);

	while(!file.eof()){

		readC = file.get();
		result += readC;

	}

	file.close();

	return result;
}

char* Helper::ReadBinaryFile(char* fileName) {
	int filesize;
	FILE *plik;
	char* result;

	plik=fopen(fileName,"rb");
	fseek(plik,0,SEEK_END);
	filesize=ftell(plik);
	fseek(plik,0,SEEK_SET);
	result=new char[filesize+1];
	fread(result,1,filesize,plik);
	result[filesize]=0;
	fclose(plik);

	return result;
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
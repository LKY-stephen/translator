#pragma once
#include <fstream>
using namespace std;
class htmlbuilder {
private:
	static FILE* htmlOutputFile;//out put file
public:
	static void Init(const char* formulaOutput);
	//fontStyle = 0 when wants normal
	static int PrintToken(int left, int top, int fontSize, std::string token,bool IDflag=false);
	static void Finish();
};

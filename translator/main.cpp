#pragma once
#include "compiler.h"
#include "parsers.h"
#include "builder.h"
#include "resource.h"


using namespace std;
int main(){
	string inputpath = "C:/Users/Stephen/Desktop/sample06.txt";//input file
	cout << "Init done." << endl;
	compiler compile(inputpath);
	int result = compile.analysis();
	if (result == SUCCESS){
	cout << "The result has been written in \"output/result.html\"." << endl;
	cout << "Finished." << endl;
	}
	return 0;
}
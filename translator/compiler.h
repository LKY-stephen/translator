#pragma once
#include "parsers.h"
#include "builder.h"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

struct ID_struct{
	//内容长度
	int length;
	//内容数组
	char value[IDLEN];
	//内容类型
	int type;
};

class compiler{
private:
	//输入存储表
	struct ID_struct table[IDNUM];
	//输入表达式
	string express;
	//文法分析函数
	int ID_class();
public:
	compiler(string filepath);
	//唯一接口用于编译
	int analysis();
};

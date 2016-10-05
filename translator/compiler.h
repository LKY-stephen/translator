#pragma once
#include "parsers.h"
#include "builder.h"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

struct ID_struct{
	//���ݳ���
	int length;
	//��������
	char value[IDLEN];
	//��������
	int type;
};

class compiler{
private:
	//����洢��
	struct ID_struct table[IDNUM];
	//������ʽ
	string express;
	//�ķ���������
	int ID_class();
public:
	compiler(string filepath);
	//Ψһ�ӿ����ڱ���
	int analysis();
};

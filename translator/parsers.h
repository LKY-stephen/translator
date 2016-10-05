#pragma once
#include <vector>
#include <fstream>
#include <map>
#include "resource.h"
#include "builder.h"
#include <algorithm>
#include <queue>  
#include <iostream>
using namespace std;
struct Grammar {
	// ��ǰջֵ
	int derivationNumber;
	// ջ
	vector< vector<int> > derivation;
	// ӳ���
	map<int, string> translate;
	// ��ʼ������
	void Init(const char* grammarInput, const char* mapInput);
	// ��ѯ����
	int FetchSign(int k, int i);
	// ��ӡ�ķ�����
	void Print(int k);
};

struct LLTable {
	// ӳ���
	map<pair<int, int>, int> table;
	//��ʼ������
	void Init(const char* LLTableInput);
	//�����
	int Derivate(int mark, int token);
};

struct Node {// �﷨�ڵ�
	//����
	int type;
	//λ��
	int matchCursor;
	// ���ڵ�
	Node* parent;
	// �˽ڵ���������ķ��ĺ����ڵ�ָ���б�
	vector<Node*> next;
	// ������
	int printCursor;
	// ������
	int top;
	// �����С
	int fontSize;
	// �ڵ�ֵ
	string* lexname;
	//����1����ʼ���ڵ�
	Node(int type, Node* parent) : type(type), parent(parent) {
		matchCursor = 1;
		fontSize = 0;
		lexname = NULL;
	}
	//����2������λ������
	void SetPosition(int newCursor, int newTop) {
		printCursor = newCursor;
		top = newTop;
	}
};
class Parser{
private:
	Grammar grammar;
	LLTable table;
	struct Node* root;
	Node* mCurrentNode;
public:
	Parser(const char* grammarInput, const char* mapInput, const char* LLTableInput);
	int match(int token);
	void assignvalue(string lexname);
	string getmarkname(int type);
	void Depth(Node* node);
	void DeterminDepth();
	int onesidescript(int pos, Node* currentNode, int nextNumber, int typenumber);
	int twosidescript(Node* currentNode, int Script_type);
	int printnode(Node* currentNode);
	void Print();
};


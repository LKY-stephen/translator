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
	// 当前栈值
	int derivationNumber;
	// 栈
	vector< vector<int> > derivation;
	// 映射表
	map<int, string> translate;
	// 初始化函数
	void Init(const char* grammarInput, const char* mapInput);
	// 查询函数
	int FetchSign(int k, int i);
	// 打印文法函数
	void Print(int k);
};

struct LLTable {
	// 映射对
	map<pair<int, int>, int> table;
	//初始化方法
	void Init(const char* LLTableInput);
	//查表方法
	int Derivate(int mark, int token);
};

struct Node {// 语法节点
	//类型
	int type;
	//位置
	int matchCursor;
	// 父节点
	Node* parent;
	// 此节点所代表的文法的后续节点指针列表
	vector<Node*> next;
	// 左坐标
	int printCursor;
	// 上坐标
	int top;
	// 字体大小
	int fontSize;
	// 节点值
	string* lexname;
	//方法1：初始化节点
	Node(int type, Node* parent) : type(type), parent(parent) {
		matchCursor = 1;
		fontSize = 0;
		lexname = NULL;
	}
	//方法2：设置位置坐标
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


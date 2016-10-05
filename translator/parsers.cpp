#pragma once
#include "parsers.h"

using namespace std;

void Grammar::Init(const char* grammarInput, const char* mapInput) {
	derivationNumber = 0;
	derivation.clear();//clean the list
	FILE* fp;
	fopen_s(&fp,grammarInput, "r");
	while (true) {
		string currentSentence = "";
		char ch;
		// use the \n and EOF to seperate each derivation
		while ((ch = fgetc(fp)) != '\n' && ch != EOF)
			currentSentence += ch;
		if (currentSentence.size() == 0) break;
		derivationNumber++;
		derivation.resize(derivationNumber);
		for (int i = 0; i < currentSentence.size(); i++) {
			int code;
			// rematch the signs with single char
			switch (currentSentence[i]) {
			case 'S': code = SMARK;			break;
			case 'B': code = BMARK;			break;
			case 'F': code = FMARK;			break;
			case '$': code = DOLLAR;		break;
			case '&': code = ALLSCRIPT;		break;
			case '^': code = SUPERSCRIPT;	break;
			case '_': code = SUBSCRIPT;		break;
			case 'i': code = INTEGRITY;		break;
			case 's': code = SIGMA;			break;
			case 'd': code = ID;			break;
			case 'n': code = NUMBER;		break;
			case 'b': code = BLANK;			break;
			case '(': code = LEFTBRACKET;	break;
			case ')': code = RIGHTBRACKET;	break;
			case '{': code = LEFTCURLY;		break;
			case '}': code = RIGHTCURLY;	break;
			case 'e': code = EMPTY;			break;
			}
			derivation.back().push_back(code);//input the code
		}
	}
	fclose(fp);

	// to generate readable debug for grammar init
	fopen_s(&fp,mapInput, "r");
	int sign;
	char* lexname2 = new char[10];
	while (fscanf_s(fp, "%d %s", &sign,lexname2,15) != EOF) {
		translate[sign] = string(lexname2);
	}
	fclose(fp);

	// output all the derivation uesd in process
	cout<< "Left most derivation:" << endl;
}
string Parser::getmarkname(int type){
	return grammar.translate[type];
}
int Grammar::FetchSign(int k, int i) {
	if (k >= derivationNumber || i >= derivation[k].size()) {
		return ARRAYOVER;
	}
	// F -> emplty
	if (derivation[k][i] == EMPTY) return ARRAYOVER;
	return derivation[k][i];
}
void Grammar::Print(int k) {
	cout << translate[derivation[k][0]].c_str()<< " -> ";
	for (int i = 1; i < derivation[k].size(); i++) {
		cout << translate[derivation[k][i]].c_str();
	}
	cout << endl;
}
void LLTable::Init(const char* LLTableInput) {
	// read in LL table
	FILE* fp;
	fopen_s(&fp,LLTableInput, "r");
	int mark=0, token=0, derivation=0;
	while (fscanf_s(fp, "%d %d %d", &mark, &token, &derivation) != EOF) {
		pair<int, int> mtp = make_pair(mark, token);
		if (table.count(mtp) && table[mtp] != derivation) {//if there has the rule and the derivation is not the same
			cout << "Collision rules in LL table." << endl;
			return;
		}
		table[mtp] = derivation;
	}
	fclose(fp);
}

// query LL table term
int LLTable::Derivate(int mark, int token) {
	pair<int, int> mtp = make_pair(mark, token);
	if (table.count(mtp)) {
		return table[mtp];
	}
	return NO_LL_RULE;
}

Parser::Parser(const char* grammarInput, const char* mapInput, const char* LLTableInput) {
	grammar.Init(grammarInput, mapInput);
	table.Init(LLTableInput);
	// first derivation
	grammar.Print(1);
	mCurrentNode = root = new Node(1, NULL);
}

// read in token and decide to derivate or move on
int Parser::match(int token) {
	int error = SUCCESS;
	while (true) {
		int sign = grammar.FetchSign(mCurrentNode->type, mCurrentNode->matchCursor);
		//如果当前文法处理完成
		if (sign == ARRAYOVER) {
			mCurrentNode = mCurrentNode->parent;
			if (mCurrentNode != NULL) {
				mCurrentNode->matchCursor++;
				continue;
			}
			else {
				return SUCCESS;
			}
		}
		//如果匹配当前终结符
		if (token == sign) {
			mCurrentNode->matchCursor++;
			return SUCCESS;
		}
		// 如果需要一个未终结符
		if (sign >= 2000) {
			int derivation = table.Derivate(sign, token);
			// no match rule
			if (derivation == NO_LL_RULE) {
				return MISMATCH_TOKEN;
			}
			// used for debug
			grammar.Print(derivation);
			mCurrentNode->next.push_back(new Node(derivation, mCurrentNode));
			mCurrentNode = mCurrentNode->next.back();
		}
		//如果未匹配的终结符为括号类
		else if(sign == 1006 || sign == 1007 || sign == 1008 || sign == 1009){
				mCurrentNode->matchCursor++;
				error = FIXERROR;
			}
		//未匹配非可纠错
		else return MISMATCH_TOKEN;
	}
	return error;
}

// only leaf node can have lexname
void Parser::assignvalue(string lexname) {
	// lexname is a pointer to string
	// must assign a new storage area
	mCurrentNode->lexname = new string(lexname);
}

// recursive determine minimum size
void Parser::Depth(Node* node) {
	// 初始默认为10
	node->fontSize = 10;
	for (int i = 0; i < node->next.size(); i++) {
		Depth(node->next[i]);
		// 决定收缩倍率
		int ratio = 0;
		// 数字或标识符
		if (node->type >= 5 && node->type <= 6)
			ratio = 2*(i < 2);
		// 算符
		if (node->type == 4 || node->type >= 7 && node->type <= 8)
			ratio = 3*(i < 4);
		node->fontSize = max(node->fontSize,node->next[i]->fontSize*ratio);
	}
}

// first find the minimum
// second adjust the minimum
// third update descendant size from root use bfs
void Parser::DeterminDepth() {
	// 递归获取全部字体阶级
	Depth(root);
	// 决定根字体大小
	root->fontSize = max(root->fontSize, 50);
	queue<Node*> nodeQueue;
	nodeQueue.push(root);
	//队列处理全部非终结符的字体大小
	while (nodeQueue.size()) {
		Node* now = nodeQueue.front();
		nodeQueue.pop();
		for (int i = 0; i < now->next.size(); i++) {
			// indicate whether to shrink
			int ratio = 0;
			// have only supscript or subscript
			if (now->type >= 5 && now->type <= 6)
				ratio = 1.7*(i < 2);
			// have both supscript and subscript
			if (now->type == 4 || now->type >= 7 && now->type <= 8)
				ratio = 1.2*(i < 4);
			now->next[i]->fontSize = now->fontSize - now->fontSize*IDTIMES* ratio;
			nodeQueue.push(now->next[i]);
		}
	}

	//check result
}

// current node has only subscript or supscript
// pos indicate the sub(1) or the sup(0)
// print only script
int Parser::onesidescript(int pos, Node* currentNode, int nextNumber,int typenumber) {
	//current info tmp
	int currentTop = currentNode->top;
	int currentCursor = currentNode->printCursor;
	int currentSize = currentNode->fontSize;
	//next info tmp
	Node* nextNode = currentNode->next[nextNumber];
	// pos = -1 when solve superscript vise verse subscript pos = 1
	int nextTop = currentTop + (pos + 1)*currentSize*0.30 + (pos - 1 - typenumber)*currentSize*0.1;
	int nextsize = nextNode->fontSize;
	nextNode->SetPosition(currentCursor, nextTop);
	return printnode(nextNode);
}

// print scripts and others
int Parser::twosidescript(Node* currentNode,int Script_type) {
	int currentTop = currentNode->top;
	int originCursor = currentNode->printCursor;
	int& currentCursor = currentNode->printCursor;
	//print subscript
	currentCursor = originCursor;
	currentCursor = onesidescript(1, currentNode, 0, Script_type);
	currentCursor = onesidescript(1, currentNode, 1, Script_type);
	//print superscript
	currentCursor = originCursor;
	currentCursor = onesidescript(-1, currentNode, 2, Script_type);
	currentCursor = onesidescript(-1, currentNode, 3, Script_type);
	//push cursor and print suffix
	currentCursor = max(currentNode->next[1]->printCursor, currentNode->next[3]->printCursor);
	currentNode->next[4]->SetPosition(currentCursor+0.25*currentNode->fontSize, currentTop);
	currentCursor = printnode(currentNode->next[4]);
	if (currentNode->type >= 7) {
		currentNode->next[5]->SetPosition(currentCursor, currentTop);
		currentCursor = printnode(currentNode->next[5]);
	}
	return currentCursor;
}

// print all kinds of formulas
int Parser::printnode(Node* currentNode) {
	// current node info tmp
	int currentTop = currentNode->top;
	int currentType = currentNode->type;
	int currentSize = currentNode->fontSize;
	int& currentCursor = currentNode->printCursor;
	// F -> empty
	if (currentType == 3) return currentCursor;
	// terminal tokens directly print
	if (currentType >= 9 && currentType <= 11) {
		string token = *(currentNode->lexname);
		bool IDstyle = currentType == 9 ? true : false;
		currentCursor = htmlbuilder::PrintToken(currentCursor, currentTop, currentSize, token, IDstyle);
		return currentCursor;
	}
	int pos;
	string token;
	switch (currentNode->type) {
	case 1:
	case 2:
		for (int i = 0; i < currentNode->next.size(); i++) {
			currentNode->next[i]->SetPosition(currentCursor, currentTop);
			currentCursor = printnode(currentNode->next[i]);
		}
		break;
	case 4:
		currentCursor = twosidescript(currentNode,0);
		break;
	case 5:
	case 6:
		pos = 2*((currentNode->type + 1) & 1)-1;
		currentCursor = onesidescript(pos, currentNode, 0,0);
		currentCursor = onesidescript(pos, currentNode, 1,0);
		currentNode->next[2]->SetPosition(currentCursor, currentTop);
		currentCursor = printnode(currentNode->next[2]);
		break;
	case 7:
	case 8:
		token = (currentNode->type & 1) ? "∫" : "∑";
		currentCursor = htmlbuilder::PrintToken(currentCursor, currentTop, currentSize, token);
		if (token == "∑")
		currentNode->printCursor-=0.4*currentNode->fontSize;
		else
		currentNode->printCursor -= 0.8*currentNode->fontSize;
		currentCursor = twosidescript(currentNode,1);
		break;
	case 12:
		currentCursor = htmlbuilder::PrintToken(currentCursor, currentTop, currentSize, string("("));
		for (int i = 0; i < currentNode->next.size(); i++) {
			currentNode->next[i]->SetPosition(currentCursor, currentTop);
			currentCursor = printnode(currentNode->next[i]);
		}
		currentCursor = htmlbuilder::PrintToken(currentCursor, currentTop, currentSize, string(")"));
		break;
	default:
		//ERROR
		break;
	}
	return currentNode->printCursor;
}

// outer interface
void Parser::Print() {
	// initialize root position
	root->printCursor = 100;
	root->top = 100;
	printnode(root);
}

#pragma once
#include "builder.h"
FILE* htmlbuilder::htmlOutputFile;

void htmlbuilder::Init(const char* formulaOutput) {

	fopen_s(&htmlOutputFile,formulaOutput, "w");
	fprintf(htmlOutputFile, "<html>\n");
	fprintf(htmlOutputFile, "\t<head>\n");
	fprintf(htmlOutputFile, "\t\t<META content=\"text/html; charset=gb2312\">\n");
	fprintf(htmlOutputFile, "\t</head>\n\t<body>\n");
}

int htmlbuilder::PrintToken(int left, int top, int fontSize, string token, bool IDflag) {
	if (token[0] == ' ')
		return int(left + fontSize * 2.8 / 5);
	fprintf(htmlOutputFile, "\t\t<div style=\"position: absolute; left:%dpx; top:%dpx;\>\n"\
		,left, top);
	fprintf(htmlOutputFile, "\t\t\t<span style=\"font-family:Times New Roman;font-size"
		":%dpx;font-style:%s; line-height:100%%;\">", fontSize, IDflag ? "oblique" : "normal");
	fprintf(htmlOutputFile, "%s</span>\n\t\t</div>\n", token.c_str());
	return int(left + token.size() * fontSize * 2.8 / 5);
}
void htmlbuilder::Finish() {
	fprintf(htmlOutputFile, "\t</body>\n</html>\n");
	fclose(htmlOutputFile);
}
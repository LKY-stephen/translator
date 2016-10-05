#pragma once

#define SENTENCE "C:/Users/Stephen/Desktop/translator/translator/sample/input/sentence.in"
#define MAP "C:/Users/Stephen/Desktop/translator/translator/sample/input/map.in"
#define TABLE "C:/Users/Stephen/Desktop/translator/translator/sample/input/LLTable.in"
#define OUTPUT "C:/Users/Stephen/Desktop/translator/translator/sample/output/result.html"

#define IDNUM 100
#define IDLEN 10
#define EXP_LEN 10
#define DELTA_SIZE	20

//error return
#define FIXERROR 1
#define SUCCESS 0
#define UNKNOWNERROR -1
#define UNKNOWN_CHARACTER -2
#define NUMBERBEHINDID -3
#define ARRAYOVER -4
#define INPUT_ERROR -5
#define INPUT_LOSS -6
#define NO_LL_RULE -7
#define MISMATCH_TOKEN -8
//depth
#define IDTIMES 0.35
#define SIGNALTIMES 0.7


//[1000, 2000) token name
#define NOTYPE          0000
#define INTEGRITY 		1000
#define SIGMA			1001
#define BLANK			1002
#define SUBSCRIPT		1003
#define ALLSCRIPT		1004
#define SUPERSCRIPT		1005
#define LEFTCURLY		1006
#define RIGHTCURLY		1007
#define LEFTBRACKET		1008
#define RIGHTBRACKET	1009
#define NUMBER			1010
#define ID				1011
#define DOLLAR			1012
#define EMPTY			1013

//[2000, 3000) mark name
#define SMARK			2000
#define FMARK			2001
#define BMARK			2002
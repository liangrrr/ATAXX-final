#define MY_API __declspec(dllexport)
#include <bits/stdc++.h>
#include "jsoncpp/json.h"
using namespace std;
#define grid pair<bitset<49>, bitset<49> >
// !    1 BLACK X second     -1 WHITE O first
#define BLACK 1
#define WHITE -1
#define TIME_LIMIT 0.95
#define black_count(g) ((g).second.count())
#define white_count(g) ((g).first.count())
#define empty_spaces(g) (49-black_count(g)-white_count(g))
#define STEP2I(x0,y0,x1,y1) ((x0) + ((y0)<<3) + ((x1)<<6) + ((y1)<<9))
#define SETTYPE(i,t) ((i)+=((t)<<12))
#define GETX0(i) ((i)&0x7)
#define GETY0(i) (((i)&0x38)>>3)
#define GETX1(i) (((i)&0x1C0)>>6)
#define GETY1(i) (((i)&0xE00)>>9)
#define GETTYPE(i) (((i)&0x3000)>>12)
#define STEP unsigned short
int mycolor;

extern Json::Value mcts(Json::Value input);
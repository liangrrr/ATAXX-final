#include "mcts-short.hpp"
inline void grid_set(grid & g, int x, int y, int color);
inline bool grid_delete (grid & g, int x, int y, int color);
inline int grid_get (const grid & g, int x, int y);
inline void proc_step_grid(grid & g, const STEP & st);
int delta[24][2] = { { 1,1 },{ 0,1 },{ -1,1 },{ -1,0 },
{ -1,-1 },{ 0,-1 },{ 1,-1 },{ 1,0 },
{ 2,0 },{ 2,1 },{ 2,2 },{ 1,2 },
{ 0,2 },{ -1,2 },{ -2,2 },{ -2,1 },
{ -2,0 },{ -2,-1 },{ -2,-2 },{ -1,-2 },
{ 0,-2 },{ 1,-2 },{ 2,-2 },{ 2,-1 } };

inline bool in_grid(int x,int y)
{
	return x>=0 && x<7 && y>=0 && y<7;
}

inline int set_judge_valid(STEP & st, const grid & g, const int & color)
{
	int x0 = GETX0(st), x1 = GETX1(st), y0 = GETY0(st), y1 = GETY1(st);
	if((in_grid (x0,y0) && in_grid(x1,y1)) == 0)  return 0;
	int dx = abs((x0 - x1)), dy = abs((y0 - y1));
	if ((dx == 0 && dy == 0) || dx > 2 || dy > 2) 
	return 0;
	if(grid_get(g,x0,y0) != color) return 0;
	if(grid_get(g,x1,y1) != 0) return 0;
	if(dx > 1||dy > 1)SETTYPE(st,2);
	else SETTYPE(st,1);
	return GETTYPE(st);
}


grid gen_board(Json::Value input)
{
	int turnID = input["responses"].size();
	int x0,y0,x1,y1;
	grid ng;
	ng.second.flip(0);
	ng.second.flip(48);
	ng.first.flip(6);
	ng.first.flip(42);
	mycolor = input["requests"][(Json::Value::UInt) 0]["x0"].asInt() < 0 ? 1 : -1; // 第一回合收到坐标是-1, -1，说明我是黑方
	for (int i = 0; i < turnID; i++)
	{
		// 根据这些输入输出逐渐恢复状态到当前回合
		x0 = input["requests"][i]["x0"].asInt();
		y0 = input["requests"][i]["y0"].asInt();
		x1 = input["requests"][i]["x1"].asInt();
		y1 = input["requests"][i]["y1"].asInt();
		if (x1 >= 0)
		{
			STEP s = STEP2I(x0,y0,x1,y1);
			set_judge_valid(s,ng,-mycolor);
			proc_step_grid(ng,s);
		}
		x0 = input["responses"][i]["x0"].asInt();
		y0 = input["responses"][i]["y0"].asInt();
		x1 = input["responses"][i]["x1"].asInt();
		y1 = input["responses"][i]["y1"].asInt();
		if (x1 >= 0)
		{
			STEP s = STEP2I(x0,y0,x1,y1);
			set_judge_valid(s,ng,mycolor);
			proc_step_grid(ng,s);
		}
	}

	if(input["requests"].size()>=turnID)
	{
		x0 = input["requests"][turnID]["x0"].asInt();
		y0 = input["requests"][turnID]["y0"].asInt();
		x1 = input["requests"][turnID]["x1"].asInt();
		y1 = input["requests"][turnID]["y1"].asInt();
		if (x1 >= 0)
		{
			STEP s = STEP2I(x0,y0,x1,y1);
			set_judge_valid(s,ng,-mycolor);
			proc_step_grid(ng,s);
		} 
	}
}

bool withdraw(Json::Value & input)
{
	int HumanSize = input["requests"].size();
	int BotSize = input["responses"].size();
	if(HumanSize>0 && input["requests"][HumanSize-1]["x0"]>=0)
	{
		input["requests"][HumanSize-1].clear();
	}
	else return 0;
	if(BotSize>0 && input["responses"][BotSize-1]["x0"]>=0)
		input["responses"][BotSize-1].clear();
	return 1;
}
int main()
{
	
}
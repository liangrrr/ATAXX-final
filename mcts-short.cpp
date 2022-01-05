#include <bits/stdc++.h>
// #define _BOTZONE_ONLINE
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

int mycolor,MAXD;
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


struct node
{
	node *parent=NULL;
	grid g;int color,relscore;
	STEP comefrom;
	STEP depth, win_theory = 0;
	struct cmp
	{
		bool operator ()(node *lhs, node *rhs)
		{
			double lu = lhs->uct(), ru = rhs->uct();
			if(abs(lu-ru)<1e-3)
			{
				return lhs-> relscore > rhs->relscore;
			}
			return lu < ru;
		}
	};
	priority_queue <node*, vector<node *>, cmp> child_list;
	int visited=0; double win=0;

	//node (grid _g, node *p):g(_g), parent(p){}
	node (node *p, STEP s):g(p->g),color(-p->color),comefrom(s),parent(p),depth(p->depth + 1)
	{
		proc_step_grid(g,s);
		relscore = (black_count(g) - white_count(g))*color;
	}
	node (grid _g):g(_g),color(mycolor),comefrom(0),depth(0){}

	/*inline int relscore() const
	{
		return (black_count(g) - white_count(g))*color;
	}*/

	inline double uct() const
	{
		if (visited == 0)return 2147483647;
		return (double)(visited - win)/visited + sqrt( 200.0 * log2(parent->visited) / visited );
	}
	inline bool fully_expanded()const
	{
		return child_list.top()->visited != 0;
	}


	bool find_valid_moves()
	{
		if(empty_spaces(g) == 0)return 0;
		for(int i=0;i<7;++i)for(int j=0;j<7;++j)
		{
			if(grid_get(g,i,j)!=0);
			else for(int k=0;k<24;++k)
			{
				STEP st = STEP2I(i+delta[k][0], j+delta[k][1],i,j);
				if(set_judge_valid(st,g,color) == 0)continue;
				node *temp = new node(this, st); bool flag = 1;

				grid ng = temp->g;
				if(black_count(ng)+white_count(ng)<(temp->depth>>2) + (temp->depth>>3))flag = 0;
				if(flag && parent && parent->parent)
				{
					if(color == -1 && parent->parent->g.first == ng.first)flag = 0;
					else if(parent->parent->g.second == ng.second)flag = 0;
				}

				if(flag)child_list.push(temp);
				if(k<8)break;
			}
		}
		return child_list.size()!=0;
	}
	//void expand()
	/*{
		for(auto it = valid_moves.begin(); it!=valid_moves.end(); ++it)
		{
			node *temp = new node(this, *it);
			grid ng = temp->g;
			if(black_count(ng)+white_count(ng)<(temp->depth>>2) + (temp->depth>>3))continue;
			if(parent && parent->parent)
			{
				if(color == -1 && parent->parent->g.first == ng.first)continue;
				else if(parent->parent->g.second == ng.second)continue;
			}
			child_list.push(temp);
		}
	}*/
	pair<int,int> simulate()
	{
		node *choice = child_list.top();
		child_list.pop();
		visited++;
		if(depth == MAXD) return make_pair(0,0);
		if(win_theory)
		{
			win+= (MAXD-depth)*relscore;
			return make_pair(color, (MAXD-depth)*relscore);
		}
		if(choice->visited)
		{
			if(choice -> child_list.size() == 0)
			{
				choice->visited ++;
				win+= (MAXD-depth)*relscore;
				child_list.push(choice);
				return make_pair(color, (MAXD-depth)*relscore);
			}
			pair<int,int> winner = choice->simulate();
			if(winner.first == color) win+=winner.second;
			child_list.push(choice);
			return winner;
		}
		if(choice->find_valid_moves() == 0)
		{
			win_theory = choice->comefrom;
			win+=(MAXD-depth)*relscore;
			choice->visited ++;
			return make_pair(color, (MAXD-depth)*relscore);
		}
		else
		{
			//choice->expand();
			pair<int,int> winner = choice->simulate() ;
			if(winner.first == color) win+=winner.second;
			child_list.push(choice);
			return winner;
		}
	}

	/*~node()
	{
		while(!child_list.empty())
		{
			delete child_list.top();
			child_list.pop();
		}
	}*/
}
*MCTSRoot;


inline int grid_get (const grid & g, int x, int y)
{
	int loc = x*7 + y;
	#ifndef _BOTZONE_ONLINE
	if(g.first[loc]&&g.second[loc])
		cerr<<"grid info error.\n";
	#endif
	if(g.first[loc]) return -1;
	if(g.second[loc]) return 1;
	return 0;
}

inline bool grid_delete (grid & g, int x, int y, int color = 0)
{
	if(color == 0)
	{
		color = grid_get(g,x,y);
	}
	if(color == -1)
	{
		g.first.flip(x*7+y);
		return 1;
	}
	else if(color == 1)
	{
		g.second.flip(x*7+y);
		return 1;
	}
	else return 0;
}

inline void grid_set(grid & g, int x, int y, int color)
{

#ifndef _BOTZONE_ONLINE
	if(grid_get(g, x, y))
	{
		cerr<<"grid_set: location error."<<endl;
		return;
	}
#endif

	if(color == -1)
	{
		g.first.flip(7*x+y);
	}
	else if(color == 1)
	{
		g.second.flip(7*x+y);
	}
	else 
	{
		cerr<<"grid_set: color error."<<endl;
	}
}

inline void proc_step_grid(grid & g, const STEP & st)
{

#ifndef _BOTZONE_ONLINE
	if(GETTYPE(st) == 0)
	{
		cerr<<"proc_step: invalid step.\n";
		return;
	}
#endif
	int x0 = GETX0(st), x1 = GETX1(st), y0 = GETY0(st), y1 = GETY1(st);
	int color = grid_get(g, x0, y0);

	grid_set(g, x1, y1, color);
	if(GETTYPE(st) == 2)
		grid_delete(g, x0, y0, color);
	for(int dir = 0; dir < 8 ; dir ++)
	{
		int dx = x1 + delta[dir][0], dy = y1 + delta[dir][1];
		if(in_grid(dx,dy) && grid_get(g,dx,dy) == -color)
		{
			grid_delete(g,dx,dy,-color);
			grid_set(g,dx,dy,color);
		}
	}
}

/*void print_grid(const grid & g)
{
	#ifndef _BOTZONE_ONLINE
	for(int i=0;i<7;++i)
	{
		for(int j=0;j<7;++j)
		{
			int loc = 7*i+j;
			if(g.first[loc])
			{
				if(g.second[loc])
					cerr<<"grid error.\n";
				else 
					cout<<"O";
			}
			else if(g.second[loc])cout<<"X";
			else cout<<" ";
		}
		cout<<endl;
	}
	cout<<endl;
	#endif
}*/


int main()
{	
	bool first_round = 1;
	Json::Reader reader;
	Json::Value input;
	string str;
	getline (cin,str);
	//cout<<"woshishabi";
	clock_t tik = clock();
	reader.parse(str, input);

	int turnID = input["responses"].size();
	MAXD = 50;
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

	// 看看自己本回合输入
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

	//print_grid(ng);
	MCTSRoot = new node(ng);
	MCTSRoot->find_valid_moves();
	while(1)
	{
		MCTSRoot->simulate();
		clock_t tok = clock();
		if(MCTSRoot->win_theory)
		{
			Json::Value ret;
			ret["response"]["x0"] = GETX0(MCTSRoot->win_theory);
			ret["response"]["y0"] = GETY0(MCTSRoot->win_theory);
			ret["response"]["x1"] = GETX1(MCTSRoot->win_theory);
			ret["response"]["y1"] = GETY1(MCTSRoot->win_theory);
			Json::FastWriter writer;
			cout << writer.write(ret) << endl;
			return 0;
		}
		if((double)(tok-tik) / CLOCKS_PER_SEC > TIME_LIMIT)break;
	}

	

		node *bestchild = MCTSRoot->child_list.top();
		MCTSRoot->child_list.pop();
		while(!MCTSRoot->child_list.empty())
		{
			if(MCTSRoot->child_list.top()->visited>bestchild->visited)
			{
				//delete bestchild;
				bestchild = MCTSRoot->child_list.top();
			}
			//else delete MCTSRoot->child_list.top();
			MCTSRoot->child_list.pop();
		}
		//if(bestchild->visited == 0)cerr<<"how can you do this?\n";
		Json::Value ret;
		ret["response"]["x0"] = GETX0(bestchild->comefrom);
		ret["response"]["y0"] = GETY0(bestchild->comefrom);
		ret["response"]["x1"] = GETX1(bestchild->comefrom);
		ret["response"]["y1"] = GETY1(bestchild->comefrom);
		Json::FastWriter writer;
		cout << writer.write(ret) << endl;
		MCTSRoot = bestchild;
		//print_grid(MCTSRoot->g);
}		
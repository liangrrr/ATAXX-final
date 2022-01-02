#include <bits/stdc++.h>
#include "jsoncpp/json.h"
using namespace std;
#define grid pair<bitset<49>, bitset<49> >
// !    1 BLACK X second     -1 WHITE O first
#define BLACK 1
#define WHITE -1
#define TIME_LIMIT 0.88
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
inline void grid_set(grid & g, int x, int y, int color);
inline bool grid_delete (grid & g, int x, int y, int color);
inline int grid_get (const grid & g, int x, int y);

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
	if ((dx == 0 && dy == 0) || dx > 2 || dy > 2) // 保证不会移动到原来位置，而且移动始终在5×5区域内
	return 0;
	if(grid_get(g,x0,y0) != color) return 0;
	if(grid_get(g,x1,y1) != 0) return 0;
	if(dx > 1||dy > 1)SETTYPE(st,2);
	else SETTYPE(st,1);
	return GETTYPE(st);
}

struct cmp
{
	bool operator ()(node *lhs, node *rhs)
	{
		double lu = lhs->uct(), ru = rhs->uct();
		if(abs(lu-ru)<1e-5)
		{
			return lhs-> relscore() < rhs->relscore();
		}
		return lu < ru;
	}
};

struct node
{
	node *parent=NULL;
	grid g;int color;
	STEP comefrom;
	priority_queue <node*, vector<node *>, cmp> child_list;
	vector <STEP> valid_moves;
	int visited=0, win=0;

	//node (grid _g, node *p):g(_g), parent(p){}
	node (node *p, STEP s):g(p->g),color(-p->color),comefrom(s)
	{
		proc_step_grid(g,s);
	}

	inline int relscore() const
	{
		return (black_count(g) - white_count(g))*color;
	}
	inline double uct() const
	{
		if (visited == 0)return 2147483647;
		return (double)win/visited + sqrt( 2 * log2(parent->visited) / visited );
	}
	inline bool fully_expanded()const
	{
		return child_list.top()->visited != 0;
	}
	bool find_valid_moves()
	{
		if(empty_spaces(g) == 0)return 0;
		for(int i=0;i<7;++i)for(int j=0;j<7;++i)
		{
			if(grid_get(g,i,j)!=color)continue;
			for(int k=0;k<24;++k)
			{
				STEP st = STEP2I(i,j,i+delta[k][0], j+delta[k][1]);
				if(set_judge_valid(st,g,color) == 0)continue;
				valid_moves.push_back(st);
				if(k<8)break;
			}
		}
		return valid_moves.size()==0;
	}
	void expand()
	{
		for(auto it = valid_moves.begin(); it!=valid_moves.end(); ++it)
		{
			child_list.push(new node(this,*it));
		}
	}
	int simulate()
	{
		node *choice = child_list.top();
		child_list.pop();
		visited++;
		if(choice->visited)
		{
			int winner = choice->simulate();
			if(winner == color) win++;
			child_list.push(choice);
			return winner;
		}
		if(choice->find_valid_moves() == 0)
		{
			win++;
			return color;
		}
		else
		{
			choice->expand();
			int winner = choice->simulate() ;
			if(winner == color) win++;
			child_list.push(choice);
			return winner;
		}
	}
}
*MCTSRoot;

inline int grid_get (const grid & g, int x, int y)
{
	int loc = x*7 + y;
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
		if(grid_get(g,dx,dy) == -color)
		{
			grid_delete(g,dx,dy,-color);
			grid_set(g,dx,dy,color);
		}
	}
}

void print_grid(const grid & g)
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
}


int main()
{	
	bool first_round = 1;
	while(1)
	{
		Json::Reader reader;
		Json::Value input;
		string str;
		getline (cin,str);
		clock_t tik = clock();
		reader.parse(str, input);
		if(first_round)
		{
			first_round = 0;
			mycolor = input["requests"][(Json::Value::UInt) 0]["x0"].asInt() < 0 ? 1 : -1;
			grid ng;
			ng.second.flip(0);
			ng.second.flip(48);
			ng.first.flip(6);
			ng.first.flip(42);
			if(mycolor == -1)
			{
				input = input["requests"][(Json::Value::UInt) 0];
				int x0 = input["x0"].asInt();
				int y0 = input["y0"].asInt();
				int x1 = input["x1"].asInt();
				int y1 = input["y1"].asInt();
				STEP fs = STEP2I(x0,y0,x1,y1);
				if(set_judge_valid(fs,ng,-mycolor) == 0)
					cerr<<"First step invalid.\n";
				proc_step_grid(ng,fs);
			}
			MCTSRoot = new node(ng,);
			MCTSRoot->g = ng;
		}
		else
		{
			input = input["requests"][(Json::Value::UInt) 0];
			int x0 = input["x0"].asInt();
			int y0 = input["y0"].asInt();
			int x1 = input["x1"].asInt();
			int y1 = input["y1"].asInt();
			step oppst = step(x0,y0,x1,y1,-mycolor,-1);
			auto it = MCTSRoot->mp.find(oppst);
			if(it != MCTSRoot->mp.end())
			MCTSRoot = it->second;
			else
			// !
			MCTSRoot = new node();
			MCTSRoot->g = 
		}
		

		while(1){
		clock_t tok = clock();
		if((double)(tok-tik) / CLOCKS_PER_SEC > TIME_LIMIT)break;
		}
		cout;
		cout<<"\n>>>BOTZONE_REQUEST_KEEP_RUNNING<<<\n";
	}
	
}		
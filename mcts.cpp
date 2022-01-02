#include <bits/stdc++.h>
#include "jsoncpp/json.h"
using namespace std;
#define grid pair<bitset<49>, bitset<49> >
// !    1 BLACK X second     -1 WHITE O first
#define BLACK 1
#define WHITE -1
#define black_count(g) (g.second.count())
#define white_count(g) (g.first.count())
#define empty_spaces(g) (49-black_count(g)-white_count(g))

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

struct step
{
	int x0,y0,x1,y1;
	step(int _x0,int _y0, int _x1=-1, int _y1=-1):x0(_x0),y0(_y0),x1(_x1),y1(_y1){}
	inline bool valid(const grid & g, int color) const
	{
		if((in_grid (x0,y0) && in_grid(x1,y1)) == 0) return 0;
		int dx = abs((x0 - x1)), dy = abs((y0 - y1));
		if ((dx == 0 && dy == 0) || dx > 2 || dy > 2) // 保证不会移动到原来位置，而且移动始终在5×5区域内
		return false;
		if(grid_get(g,x0,y0) != color) return 0;
		if(grid_get(g,x0,y0) != 0) return 0;
		return 1;
	}
};

void proc_step(grid & g, const step & st)
{
	
}

struct node
{
	node *parent=NULL;
	grid g;
	vector <node*> child_list;
	int visited=0, win=0;
}
MCTSRoot;

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
	else if(color ==1)
	{
		g.second.flip(x*7+y);
		return 1;
	}
	else return 0;
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
					cerr<<"grid error.";
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
	
}		
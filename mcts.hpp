#define MY_API __declspec(dllexport)
#include <bits/stdc++.h>
using namespace std;
extern "C"
{
	MY_API int mcts(int grid0, int grid1, int color);

	// * wrapper find valid: return winner color if game terminates, 
	// * return 0 otherwise
	MY_API int wr_find_valid(int grid0, int grid1, int color);
}
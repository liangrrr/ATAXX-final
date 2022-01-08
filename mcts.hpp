#define MY_API __declspec(dllexport)
#include <bits/stdc++.h>
using namespace std;
extern "C"
{
	MY_API int mcts(unsigned long long grid0, unsigned long long grid1, int color);

	// * wrapper find valid: return winner color if game terminates, 
	// * return 0 otherwise
	MY_API int wr_find_valid(unsigned long long grid0, unsigned long long grid1, int color);
}
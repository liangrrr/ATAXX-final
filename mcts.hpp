#define MY_API __declspec(dllexport)
#include <bits/stdc++.h>
using namespace std;
extern "C"
{
	MY_API int mcts(int grid0, int grid1);
}
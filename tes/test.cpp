#include <bits/stdc++.h>
using namespace std;
extern "C" __declspec(dllimport) string mcts(string str);
int main()
{
	string s;
	cin>>s;
	cout<<mcts(s);
}
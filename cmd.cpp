#define CMD
#include "ataxx.hpp"
using namespace std;

void interfere()
{
	int x0,y0,x1,y1;
	printGrid();
	cout<<endl;
	cout<<"─────────────────────────────────────────────────────"<<endl;
	cout<<"输入【行 列】选择要移动的棋子  按【m】进入菜单："<<endl;
	string s;
	getinput:
	getline(cin,s);
	if(s=="m")
	{
		printGrid();
		cout<<endl;
		cout<<"─────────────────────────────────────────────────────"<<endl;
		cout<<"菜单：n-新游戏   s-保存   l-读取   r-悔棋   e-结束游戏   其他-返回"<<endl;
		getline(cin,s);
		if(s=="e")exit(0);
		else if(s=="s")
		{
			printGrid();
			int sver;
			cout<<endl;
			cout<<"─────────────────────────────────────────────────────"<<endl;
			if(loaded!=-1)
			{
				cout<<"当前存档号是"<<loaded<<"，按【y】覆盖当前存档，按【n】创建新存档：";
				getline(cin,s);
				if(s=="y")
				{
					save(loaded);
					cout<<"保存成功！";
					system("sleep(1)");
					goto getinput;
				}
			}
			save(version+1);
			cout<<"保存成功，存档号"<<version;

		}
	}
	else if(s.length()!=3||s[1]!=' '||s[0]<'0'||s[0]>'6'||s[2]<'0'||s[2]>'6')
	{
		printGrid();
		cout<<"──────────────────────非法指令！─────────────────────"<<endl;
		cout<<"输入【行 列】选择要移动的棋子  按【m】进入菜单："<<endl;
		goto getinput;
	}
	else
	{
		x0 = s[0]-'0'; y0 = s[2]-'0';
		if(gridInfo[x0][y0]!=-currBotColor)
		{
			printGrid();
			cout<<endl;
			cout<<"───────────────────这里没有你的棋子！──────────────────"<<endl;
			cout<<"输入【行 列】选择要移动的棋子  按【m】进入菜单："<<endl;
			goto getinput;
		}
		printGrid();
		cout<<endl;
		cout<<"─────────────────────────────────────────────────────"<<endl;
		cout<<"你选择了【"<<x0<<" "<<y0<<"】，输入【行 列】选择移动到哪里："<<endl;
		getline(cin,s);
		if(s.length()!=3||s[1]!=' '||s[0]<'0'||s[0]>'6'||s[2]<'0'||s[2]>'6')
		{
			printGrid();
			cout<<endl;
			cout<<"──────────────────────非法指令！─────────────────────"<<endl;
			cout<<"输入【行 列】选择要移动的棋子  按【m】进入菜单："<<endl;
			goto getinput;
		}
		else
		{
			x1 = s[0]-'0'; y1 = s[2]-'0';
			if(ProcStep(x0,y0,x1,y1,-currBotColor)==0)
			{
				printGrid();
				cout<<endl;
				cout<<"───────────────────不能移动到这里！──────────────────"<<endl;
				cout<<"输入【行 列】选择要移动的棋子  按【m】进入菜单："<<endl;
				goto getinput;
			}
			else
			{
				printGrid();
			}
		}
	}
}
void printGrid()
{
	system("cls");
	cout<<"\t\t同化棋"<<endl;
	if(currBotColor==1)cout<<"AI\t\t玩家"<<endl;
	else cout<<"玩家\t\tAI"<<endl;
	cout<<"黑● "<<blackPieceCount<<"\t\t"<<whitePieceCount<<" ○白"<<endl;
	cout<<endl;
	cout<<"   0   1   2   3   4   5   6"<<endl;
	cout<<" ╔═══╦═══╦═══╦═══╦═══╦═══╦═══╗"<<endl;
	for(int i=0;i<7;++i)
	{
		cout<<i<<"║";
		for(int j=0;j<7;++j)
		{
			if(gridInfo[i][j]==1)cout<<" ● ║";
			else if(gridInfo[i][j]==-1)cout<<" ○ ║";
			else cout<<"   ║";
		}
		cout<<endl;
		if(i!=6)cout<<" ╠═══╬═══╬═══╬═══╬═══╬═══╬═══╣"<<endl;
		else cout<< " ╚═══╩═══╩═══╩═══╩═══╩═══╩═══╝" << endl;
	}
}

void newGame()
{
	loaded = -1;
	cleartheBoard();
	start:
	cout<<"\t\t同化棋"<<endl;
	cout<<endl;
	cout<<"请选择你的棋子颜色："<<endl;
	cout<<"0：黑方●先手   1：白方○后手"<<endl;
	char chosecolor;
	cin>>chosecolor;
	if(chosecolor=='0')currBotColor=-1;
	else if(chosecolor=='1')currBotColor=1;
	else 
	{
		system("cls");
		goto start;
	}
	string s;
	getline(cin,s);
}
int main()
{
	Json::Value config;
	Json::Reader reader;
	reader.parse("config.ini",config);
	version = config["version"].empty() ? 0 : config["version"].asInt();
	newGame();
	if(currBotColor==1){printGrid();system("sleep(0.5)");AIGamePlay();}
	while(1)
	{	
		if(HumanLoss())break;
		interfere();
		system("sleep(0.5)");
		if(!AIGamePlay())break;
	}
	printGrid();
	cout<<endl;
	cout<<"─────────────────────────────────────────────────────"<<endl;
	if(blackPieceCount>whitePieceCount)cout<<"黑方胜！"<<endl;
	else if(blackPieceCount<whitePieceCount)cout<<"白方胜！"<<endl;
	else cout<<"平局！"<<endl;
	cout<<"菜单：n-新游戏   s-保存   l-读取   r-悔棋   其他-结束游戏"<<endl;
}
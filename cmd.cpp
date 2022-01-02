#define CMD
#include "ataxx.hpp"
using namespace std;

void interfere()
{
	int x0,y0,x1,y1;
	printGrid();
	cout<<endl;
	cout<<"����������������������������������������������������������������������������������������������������������"<<endl;
	cout<<"���롾�� �С�ѡ��Ҫ�ƶ�������  ����m������˵���"<<endl;
	string s;
	getinput:
	getline(cin,s);
	if(s=="m")
	{
		printGrid();
		cout<<endl;
		cout<<"����������������������������������������������������������������������������������������������������������"<<endl;
		cout<<"�˵���n-����Ϸ   s-����   l-��ȡ   r-����   e-������Ϸ   ����-����"<<endl;
		getline(cin,s);
		if(s=="e")exit(0);
		else if(s=="s")
		{
			printGrid();
			int sver;
			cout<<endl;
			cout<<"����������������������������������������������������������������������������������������������������������"<<endl;
			if(loaded!=-1)
			{
				cout<<"��ǰ�浵����"<<loaded<<"������y�����ǵ�ǰ�浵������n�������´浵��";
				getline(cin,s);
				if(s=="y")
				{
					save(loaded);
					cout<<"����ɹ���";
					system("sleep(1)");
					goto getinput;
				}
			}
			save(version+1);
			cout<<"����ɹ����浵��"<<version;

		}
	}
	else if(s.length()!=3||s[1]!=' '||s[0]<'0'||s[0]>'6'||s[2]<'0'||s[2]>'6')
	{
		printGrid();
		cout<<"���������������������������������������������Ƿ�ָ�������������������������������������������"<<endl;
		cout<<"���롾�� �С�ѡ��Ҫ�ƶ�������  ����m������˵���"<<endl;
		goto getinput;
	}
	else
	{
		x0 = s[0]-'0'; y0 = s[2]-'0';
		if(gridInfo[x0][y0]!=-currBotColor)
		{
			printGrid();
			cout<<endl;
			cout<<"������������������������������������������û��������ӣ�������������������������������������"<<endl;
			cout<<"���롾�� �С�ѡ��Ҫ�ƶ�������  ����m������˵���"<<endl;
			goto getinput;
		}
		printGrid();
		cout<<endl;
		cout<<"����������������������������������������������������������������������������������������������������������"<<endl;
		cout<<"��ѡ���ˡ�"<<x0<<" "<<y0<<"�������롾�� �С�ѡ���ƶ������"<<endl;
		getline(cin,s);
		if(s.length()!=3||s[1]!=' '||s[0]<'0'||s[0]>'6'||s[2]<'0'||s[2]>'6')
		{
			printGrid();
			cout<<endl;
			cout<<"���������������������������������������������Ƿ�ָ�������������������������������������������"<<endl;
			cout<<"���롾�� �С�ѡ��Ҫ�ƶ�������  ����m������˵���"<<endl;
			goto getinput;
		}
		else
		{
			x1 = s[0]-'0'; y1 = s[2]-'0';
			if(ProcStep(x0,y0,x1,y1,-currBotColor)==0)
			{
				printGrid();
				cout<<endl;
				cout<<"�������������������������������������������ƶ������������������������������������������"<<endl;
				cout<<"���롾�� �С�ѡ��Ҫ�ƶ�������  ����m������˵���"<<endl;
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
	cout<<"\t\tͬ����"<<endl;
	if(currBotColor==1)cout<<"AI\t\t���"<<endl;
	else cout<<"���\t\tAI"<<endl;
	cout<<"�ڡ� "<<blackPieceCount<<"\t\t"<<whitePieceCount<<" ���"<<endl;
	cout<<endl;
	cout<<"   0   1   2   3   4   5   6"<<endl;
	cout<<" �X�T�T�T�j�T�T�T�j�T�T�T�j�T�T�T�j�T�T�T�j�T�T�T�j�T�T�T�["<<endl;
	for(int i=0;i<7;++i)
	{
		cout<<i<<"�U";
		for(int j=0;j<7;++j)
		{
			if(gridInfo[i][j]==1)cout<<" �� �U";
			else if(gridInfo[i][j]==-1)cout<<" �� �U";
			else cout<<"   �U";
		}
		cout<<endl;
		if(i!=6)cout<<" �d�T�T�T�p�T�T�T�p�T�T�T�p�T�T�T�p�T�T�T�p�T�T�T�p�T�T�T�g"<<endl;
		else cout<< " �^�T�T�T�m�T�T�T�m�T�T�T�m�T�T�T�m�T�T�T�m�T�T�T�m�T�T�T�a" << endl;
	}
}

void newGame()
{
	loaded = -1;
	cleartheBoard();
	start:
	cout<<"\t\tͬ����"<<endl;
	cout<<endl;
	cout<<"��ѡ�����������ɫ��"<<endl;
	cout<<"0���ڷ�������   1���׷������"<<endl;
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
	cout<<"����������������������������������������������������������������������������������������������������������"<<endl;
	if(blackPieceCount>whitePieceCount)cout<<"�ڷ�ʤ��"<<endl;
	else if(blackPieceCount<whitePieceCount)cout<<"�׷�ʤ��"<<endl;
	else cout<<"ƽ�֣�"<<endl;
	cout<<"�˵���n-����Ϸ   s-����   l-��ȡ   r-����   ����-������Ϸ"<<endl;
}
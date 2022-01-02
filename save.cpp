#define CMD
#include "ataxx.hpp"

void analyze()
{
	cleartheBoard();
	int x0,y0,x1,y1;
	// 分析自己收到的输入和自己过往的输出，并恢复状态
	int turnID = input["responses"].size();
	currBotColor = input["requests"][(Json::Value::UInt) 0]["x0"].asInt() < 0 ? 1 : -1; // 第一回合收到坐标是-1, -1，说明我是黑方
	for (int i = 0; i < turnID; i++)
	{
		// 根据这些输入输出逐渐恢复状态到当前回合
		x0 = input["requests"][i]["x0"].asInt();
		y0 = input["requests"][i]["y0"].asInt();
		x1 = input["requests"][i]["x1"].asInt();
		y1 = input["requests"][i]["y1"].asInt();
		if (x1 >= 0)
			ProcStep(x0, y0, x1, y1, -currBotColor); // 模拟对方落子
		x0 = input["responses"][i]["x0"].asInt();
		y0 = input["responses"][i]["y0"].asInt();
		x1 = input["responses"][i]["x1"].asInt();
		y1 = input["responses"][i]["y1"].asInt();
		if (x1 >= 0)
			ProcStep(x0, y0, x1, y1, currBotColor); // 模拟己方落子
	}
	if(input["requests"].size()>=turnID)
	{
		x0 = input["requests"][turnID]["x0"].asInt();
		y0 = input["requests"][turnID]["y0"].asInt();
		x1 = input["requests"][turnID]["x1"].asInt();
		y1 = input["requests"][turnID]["y1"].asInt();
		if (x1 >= 0)
		ProcStep(x0, y0, x1, y1, -currBotColor); // 模拟对方落子
	}
	printGrid();
}
void save(int id)
{
	ofstream of;
	string filename = "save" + to_string(id) + ".json";
	of.open(filename,ios::out);
	Json::FastWriter writer;
	of<<writer.write(input);
	of.close();
	if(id>version)
	{
		version = id;
		Json::FastWriter writer2;
		Json::Value v;
		v["version"] = version;
		of.open("config.ini",ios::out);
		of<<writer.write(v);
		of.close();		
	}
}
bool load(int id)
{
	loaded = id;
	Json::Reader reader;
	string filename = "save" + to_string(id) + ".json";
	if(!reader.parse(filename, input))
		return 0;
	analyze();
	return 1;
}
bool withdraw()
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
	analyze();
	return 1;
}
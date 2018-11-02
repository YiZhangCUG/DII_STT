#include "GCTL-fileFunc.h"
//测试打开输入文件 如果成功则返回0并输出信息 否则返回1
int open_infile(ifstream &infile,char* filename)
{
	infile.open(filename);
	if (!infile)
	{
		cout << BOLDRED << "error ==> " << RESET << "file not found: " << filename << endl;
		return -1;
	}
	return 0;
}
//测试打开输出文件 如果成功则返回0并输出信息 否则返回1
int open_outfile(ofstream &outfile,char* filename)
{
	outfile.open(filename);
	if (!outfile)
	{
		cout << BOLDRED << "error ==> " << RESET << "fail to create the file: " << filename << endl;
		return -1;
	}
	return 0;
}
//替换str中所有lod_value为new_value,返回被替换的old_value的个数
string& replace_all(string& str,const string& old_value,const string& new_value,int &num)
{
	int count = 0;
	for(string::size_type pos(0);pos!=string::npos;pos+=new_value.length())
	{
		if((pos=str.find(old_value,pos))!=string::npos)
		{
			str.replace(pos,old_value.length(),new_value);
			count++;
		}
		else break;
	}
	num = count;
	return str;
}
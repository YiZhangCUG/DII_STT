#ifndef _FILEFUNC_H
#define _FILEFUNC_H
#include "GCTL-sysDefine.h"
// 打开输入文件
int open_infile(ifstream &,char*);
// 打开输出文件
int open_outfile(ofstream &,char*);
// 替换指定字符
string& replace_all(string&,const string&,const string&,int &);
#endif
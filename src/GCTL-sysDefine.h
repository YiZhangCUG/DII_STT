#ifndef _SYSDEFINE_H
#define _SYSDEFINE_H
#include "iostream"
#include "fstream"
#include "sstream"
#include "string.h"
#include "cmath"
#include "iomanip"
#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "vector"
#include "map"
#include "algorithm"
#include "ctime"
#include "omp.h"

using namespace std;
//数学常量
#define BDL_MAX 1e+30
#define BDL_MIN -1e+30
#define ZERO 1e-20
//物理常量
#define WGS84_PoleRadius 6356752.314//WGS84椭球极半径
#define WGS84_EquatorRadius 6378137//WGS84椭球长半径
#define EarthRadius 6371008.8
#define MoonRadius 1738000
#define Pi (4.0*atan(1.0)) //Pi
#define Golden_Mean (sqrt(5.0)+1)/2//黄金比例
#define G0 6.67408e-3 //注意这里本来应该是e-11，考虑到单位转换，取维度单位为m，密度单位为g/cm^3，乘以G0则重力单位即为mGal 2014年推荐值
//#define G0 6.673e-3 //和上面的解释一样 只是这是tesseroids程序使用的默认值
//终端显示控制符
#define BOLDRED "\033[1m\033[31m"
#define BOLDGREEN "\033[1m\033[32m"
#define BOLDBLUE "\033[1m\033[34m"
#define UNDERLINE "\033[1m\033[4m"
#define RESET "\033[0m"
#define MOVEUP(x) printf("\033[%dA", (x))
#define MOVEDOWN(x) printf("\033[%dB", (x))
#define MOVELEFT(x) printf("\033[%dD", (x))
#define MOVERIGHT(x) printf("\033[%dC", (x))
#define MOVETO(y,x) printf("\033[%d;%dH", (y), (x))
#define CLEARLINE "\033[K"
#define CLEARALL "\033[2J"
#endif
#ifndef _MATHFUNC_H
#define _MATHFUNC_H
#include "GCTL-sysDefine.h"
#include "GCTL-dataType.h"
//以度计算的正弦函数
double sind(double);
//以度计算的余弦函数
double cosd(double);
//正负分离的atan函数 正数返回atan 负数返回atan+pi
double arctg(double);
//利用二分法求一个正数的n次方根 注意输入值小于1的情况
double sqrtn(double,int);
//返回范围内的随机浮点值
double randomDouble(double,double);
//椭球或者球在不同纬度位置的半径
double ellipseRadius(double,double,double);
//球面双线性插值函数 以度为单位的版本 注意纬度为余纬度
double SphBiInterp_deg(double,double,double,double,double,double,double,double,double,double);
//规则网络插值 长方形内数据插值 距离平方反比
double RectInterp(double,double,double,double,double,double,double,double,double,double);
//规则网络插值 长方体内数据插值 距离平方反比
double CubeInterp(double,double,double,double,double,double,double,double,double,
					double,double,double,double,double,double,double,double);
//一维线性插值
double LineInterp(double,double,double,double,double);
#endif
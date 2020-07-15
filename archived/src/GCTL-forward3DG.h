#ifndef _FORWARD3DG_H
#define _FORWARD3DG_H
#include "GCTL-sysDefine.h"
#include "GCTL-dataType.h"
#include "GCTL-mathFunc.h"

// 三维重力正演函数
class forward3DG
{
public:
	//直角坐标系块体正演函数
	//参数 块体 使用的块体属性索引 观测点 计算类型G Gx Gy Gz
	double cube_G(cube,int,vertex,char*);
	//直角坐标系球体正演函数
	//参数 球体 使用的块体属性索引 观测点 计算类型G Gx Gy Gz
	double sphere_G(sphere,int,vertex,char*);
};
#endif
#ifndef _STRUCT_H
#define _STRUCT_H
#include "GCTL-sysDefine.h"
#include "GCTL-dataType.h"
#include "GCTL-fileFunc.h"
#include "GCTL-mathFunc.h"
#include "GCTL-progressBar.h"
//整型到整形的映射 用于文件索引到数组索引的映射
typedef map<int,int> IDMAP;
//顶点的相邻关系
struct vertNeigh
{
	int id;
	_1iArray tri; //与顶点相连的三角形面元 也即三棱锥的顶面
	//_1iArray right_tri; //fac数组中每一个面元绕中心顶点逆时针相邻的面元
	//_1iArray vec; //与顶点相连的顶点
};
typedef vector<vertNeigh> vertNeighArray;
//三棱锥 GCTL中有tripy类型 这里为了避免重复 我们将重命名一个结构为 triPyramid
struct triPyramid
{
	int id; //索引值
	int phys; //物理组
	int vec[4][3]; //顶点索引
	double rho; //属性值 这里我们只需要一个密度值
	cpoint E[12]; //边的外法线矢量 这里有12个是因为每条边链接两个面
	cpoint F[4]; //面外法线单位矢量
	double edgelen[12]; //边的长度 这里有12个是因为每条边链接两个面
	//注意三棱锥的侧面与侧面的外法线矢量相对于半径的偏导数为0 所以nf_rj为顶面三角形相对于三个顶点半径的偏导数
	//三棱锥顶面的偏导数矢量
	cpoint nf_rj[3]; //顶面的单位矢量相对于一个标量（顶点半径）的偏导数
	cpoint nj2_rj[3]; //顶面三角形中与顶点相连的逆时针边及其侧面相对于半径的偏导数
	cpoint n2j_rj[3]; //侧面上的偏导数 侧面外法线矢量的偏导数为0
	cpoint n3j_rj[3];
	cpoint nj3_rj[3];
	cpoint n23_rj[3];
};
typedef vector<triPyramid> triPyramidArray;

int local_index(int id, triPyramid t)//给出一个顶点的索引和其所在三角形，循环确定该顶点在三角形顶点中排序的位置
{
	for (int i = 0; i < 3; i++)
		if (id == t.vec[0][i])	return i;
	return -1;
}

cpoint nr_dr(cpoint n, cpoint n_rj)//计算一个矢量的单位矢量相对一个标量（顶点半径）的偏导数矢量，输入值分别为该矢量和其相对于这个标量的偏导数矢量
{
	dataFunc df;
	cpoint out, nn;
	double temp;
	double moduel_n = sqrt(n.x*n.x+n.y*n.y+n.z*n.z);
	nn.x = n.x/moduel_n; nn.y = n.y/moduel_n; nn.z = n.z/moduel_n;

	temp = df.dot(n,n_rj)/moduel_n;
	out.x = (n_rj.x - nn.x*temp)/moduel_n;
	out.y = (n_rj.y - nn.y*temp)/moduel_n;
	out.z = (n_rj.z - nn.z*temp)/moduel_n;

	return out;
}
#endif
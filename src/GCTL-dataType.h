#ifndef _DATATYPE_H
#define _DATATYPE_H
#include "GCTL-sysDefine.h"
//一维整型向量数组
typedef vector<int> _1iArray;
//一维双精度向量数组
typedef vector<double> _1dArray;
//二维整型向量数组
typedef vector<vector<int> > _2iArray;
//二维双精度向量数组
typedef vector<vector<double> > _2dArray;
//直角坐标系点
struct cpoint
{
	//cpoint包含了点的坐标位置
	double x, y, z;
	//类型的构造函数
	cpoint();
	//显示类型信息
	void info();
};
//直角坐标系点数组
typedef vector<cpoint> cpointArray;
//球坐标系点
struct spoint
{
	//spoint包含了点的坐标位置
	double lon,lat,rad;
	//类型的构造函数
	spoint();
	//显示类型信息
	void info();
};
//球坐标系点数组
typedef vector<spoint> spointArray;
//张量3*3矩阵
struct tensor
{
	double val[3][3];
	tensor();
	void info();
};
typedef vector<tensor> tensorArray;
//顶点或者观测点
struct vertex
{
	int id; //观测点索引
	_1dArray val; //观测值数组
	cpoint posic; //观测点位置
	spoint posis;
	vertex(); //构造函数
	void info(); //显示观测点信息
	void set(int); //设置索引
	void set(cpoint); //由cpoint初始化位置
	void set(spoint); //由spoint初始化位置
	void set(_1dArray); //设置观测值
	void clear(); //清空观测值
};
typedef vector<vertex> vertexArray;

//一个通用的模型单元结构 用于模型操作比较实用 在正反演计算中我们还是使用特定的类型比较好
struct modcell
{
	int id;
	int phys;
	int geom;
	int vecNum;
	_1iArray vec;
	_1dArray val;
	modcell();
	void info();
	void clear();
};
typedef vector<modcell> modcellArray;

//三角形
struct triangle
{
	int id;
	int phys;
	int vec[3];
	_1dArray val;
	double edgelen[3];
	tensor E[3];
	tensor F;
	triangle();
	void info();
	void clear();
};
typedef vector<triangle> triangleArray;

struct tripy //三棱锥 第四个点为坐标原点
{
	int id;
	int phys;
	int vec[4][3]; //四个面上的顶点索引
	tensor E[12];
	tensor F[4];
	double val;
	double edgelen[12];
	tripy();
};
typedef vector<tripy> tripyArray;

//三角棱柱
struct prism
{
	int id;
	int phys;
	int vec[6];
	_1dArray val;
	prism();
	void clear();
};
typedef vector<prism> prismArray;

// 直角坐标系下的块体
// z
// |
// |        y
// |       /
// |  z2----------y2
// | /|  /        /|
// |/ | /        / |
// |------------y1 |
// |  z1        |  |
// | /          | /
// |/           |/
// x1-----------x2------> x
struct cube
{
	int id; //块体索引
	double x1,x2,y1,y2,z1,z2; //块体尺寸
	_1dArray val; //块体属性
	cube();
	void info(); //显示块体信息
	void set(int); //设置块体索引
	void set(_1dArray); //设置块体属性
	void clear(); //清空块体属性
};
typedef vector<cube> cubeArray;

struct sphere
{
	int id;
	double x, y, z, rad;
	_1dArray val;
	sphere();
	void info();
	void set(int);
	void set(_1dArray);
	void clear();
};
typedef vector<sphere> sphereArray;

// 针对cpoint类型重新声明简单的数学与布尔运算
bool operator ==(cpoint, cpoint);
bool operator !=(cpoint, cpoint);
cpoint operator -(cpoint, cpoint);
cpoint operator +(cpoint, cpoint);
cpoint operator *(double, cpoint);
tensor operator +(tensor, tensor);
// 针对spoint类型重新声明简单的数学与布尔运算
bool operator ==(spoint, spoint);
bool operator !=(spoint, spoint);
// 自定义数据类型的函数列表
class dataFunc
{
public:
	int angXYZ(cpoint,double&,double&,double&); // cpoint与坐标轴的夹角
	int angXYZ(spoint,double&,double&,double&); // spoint与坐标轴的夹角
	double dot(cpoint, cpoint); //矢量点乘
	double lenCpoint(cpoint); //矢量的模长
	double disCpoint(cpoint, cpoint); //两个矢量之间的距离
	double angCpoint(cpoint, cpoint); //两个矢量以坐标原点为顶点的夹角
	double dotToPlane(cpoint, cpoint, cpoint);//点到平面的距离 平面上一点 平面的法线 计算点
	cpoint cross(cpoint, cpoint); // 矢量叉乘 注意矢量的叉乘是有顺序的 这里默认为第一个矢量叉乘第二个矢量
	cpoint normal(cpoint); // 矢量的单位矢量
	cpoint midCpoint(cpoint, cpoint); // 两个矢量的中点矢量
	cpoint cenCpoint(cpointArray); // 一组矢量的中心
	cpoint scaleCpoint(cpoint, cpoint, double); // 以从第一到第二个矢量为方向 以第一个矢量为起点 返回一个模长为double参数值的矢量
	cpoint rescaleCpoint(cpoint, double); // 以第一个矢量为方向和起点 返回一个模长为double参数的矢量
	cpoint vecMartix(cpoint, tensor); // 矢量乘以张量
	cpoint martixVec(tensor, cpoint); // 张量乘以矢量
	cpoint s2c(spoint); // 直角坐标与球坐标相互转换函数 注意这里使用的球坐标是地理坐标范围 即经度为-180~180 纬度为-90~90
	cpoint rotate(cpoint,double,double,double); // 绕x y z轴旋转cpoint
	cpoint rotate(cpoint,cpoint,double); //绕通过原点的旋转轴旋转cpoint
	cpoint lineOnPlane(cpoint,cpoint,cpoint,cpoint); //计算直线与平面的交点
	spoint c2s(cpoint); // 直角坐标转球坐标 注意与z轴重合的南北极点无法计算得到唯一的经度值 这里直接令其经度为0
	tensor kron(cpoint, cpoint); //两个矢量的张量积
	tensor martixMartix(tensor, tensor); //两个张量的乘积
	tensor martixT(tensor); //张量的转置
	tensor martixDouble(tensor,double);
};
#endif
#include "GCTL-mathFunc.h"

//以度计算的正弦函数
inline double sind(double degree)
{
	return sin(degree*Pi/180.0);
}

//以度计算的余弦函数
inline double cosd(double degree)
{
	return cos(degree*Pi/180.0);
}

//正负分离的atan函数 正数返回atan 负数返回atan+pi
double arctg(double v)
{
	double ang;
	if(v>=0) ang=atan(v);
	else if(v<0) ang=atan(v)+Pi;
	return ang;
}

//利用二分法求一个正数的n次方根 注意输入值小于1的情况
double sqrtn(double d,int n)
{
	double xmin,xmax,halfx;
	double eps = 1e-5; //计算精度
	if (d == 1)
	{
		return d;
	}
	else if (d > 1)
	{
		xmin = 1;
		xmax = d;
		halfx = 0.5*(xmin+xmax);
		while (fabs(d - pow(halfx,n)) > eps)
		{
			if (pow(halfx,n) > d)
			{
				xmax = halfx;
				halfx = 0.5*(xmin+xmax);
			}
			else
			{
				xmin = halfx;
				halfx = 0.5*(xmin+xmax);
			}
		}
	}
	else
	{
		xmin = 0;
		xmax = 1;
		halfx = 0.5*(xmin+xmax);
		while (fabs(d - pow(halfx,n)) > eps)
		{
			if (pow(halfx,n) > d)
			{
				xmax = halfx;
				halfx = 0.5*(xmin+xmax);
			}
			else
			{
				xmin = halfx;
				halfx = 0.5*(xmin+xmax);
			}
		}
	}
	return halfx;
}

//返回范围内的随机浮点值 注意调取函数之前要调用srand(time(0));
double randomDouble(double L,double T)
{
	return (T-L)*rand()*1.0/RAND_MAX + L;
}

//计算一个参考椭球或者参考球在纬度位置的半径
double ellipseRadius(double lati,double refr,double refR)
{
	return refr*refR/sqrt(pow(refr,2)*pow(cos((double) lati*Pi/180.0),2)+pow(refR,2)*pow(sin((double) lati*Pi/180.0),2));
}

/*
lat
|
|
h21----h22
|       |
|       |
h11----h12----> lon
*/
// 球面双线性插值函数 以度为单位的版本 注意纬度为余纬度
double SphBiInterp_deg(double CoLat1,double CoLat2,double Lon1,double Lon2,double CoLat0,double Lon0,double h11,double h12,double h21,double h22)
{
	double Delta=(Lon2-Lon1)*(cosd(CoLat2)-cosd(CoLat1));
	double A=(Lon1*(h12-h22)+Lon2*(h21-h11))/Delta;
	double B=(cosd(CoLat1)*(h21-h22)+cosd(CoLat2)*(h12-h11))/Delta;
	double C=(h11+h22-h21-h12)/Delta;
	double D=(Lon2*cosd(CoLat2)*h11-Lon2*cosd(CoLat1)*h21+Lon1*cosd(CoLat1)*h22-Lon1*cosd(CoLat2)*h12)/Delta;
	double h0=A*cosd(CoLat0)+B*Lon0+C*Lon0*cosd(CoLat0)+D;
	return h0;
}

//规则网络插值 长方形内数据插值 距离平方反比
/*长方体示意图*/
// y
// |
// |
// 3------------2
// |            |
// |            |
// |            |
// 0------------1--->x
// 左下角坐标x0 y0
// 块体尺寸dx dy
// 插值点坐标x y
// 四个角点值
double RectInterp(double x0,double y0,double dx,double dy,double x,double y,
					double d0,double d1,double d2,double d3)
{
	double res = 0;
	double total_dist = 0;
	double dist[4] = {0,0,0,0};
	double val[4];
	val[0] = d0; val[1] = d1; val[2] = d2; val[3] = d3;
	dist[0] = 1.0/(1e-30+(x-x0)*(x-x0)+(y-y0)*(y-y0));
	dist[1] = 1.0/(1e-30+(x-dx-x0)*(x-dx-x0)+(y-y0)*(y-y0));
	dist[2] = 1.0/(1e-30+(x-dx-x0)*(x-dx-x0)+(y-dy-y0)*(y-dy-y0));
	dist[3] = 1.0/(1e-30+(x-x0)*(x-x0)+(y-dy-y0)*(y-dy-y0));
	for (int i = 0; i < 4; i++)
	{
		total_dist += dist[i];
	}
	for (int i = 0; i < 4; i++)
	{
		res += val[i]*dist[i]/total_dist;
	}
	return res;
}

//规则网络插值 六面体内数据插值 距离平方反比
/*六面体示意图*/
//       y
//      /
//     /
//    3------------2
//   /|           /|
//  / |          / |
// 0------------1------> x
// |  7         |  6
// | /          | /
// |/           |/
// 4------------5
// |
// |
// z
// 左上角坐标x0 y0 z0
// 块体尺寸dx dy dz
// 插值点坐标x y z
// 八个角点值
double CubeInterp(double x0,double y0,double z0,double dx,double dy,double dz,double x,double y,double z,
						double d0,double d1,double d2,double d3,double d4,double d5,double d6,double d7)
{
	double res = BDL_MAX;
	double total_dist = 0;
	double dist[8] = {0,0,0,0,0,0,0,0};
	double val[8];
	val[0] = d0; val[1] = d1; val[2] = d2; val[3] = d3;
	val[4] = d4; val[5] = d5; val[6] = d6; val[7] = d7;
	//计算八个角点与待插值点的距离信息
	dist[0] = 1.0/(1e-30+(x-x0)*(x-x0)+(y-y0)*(y-y0)+(z-z0)*(z-z0));
	dist[1] = 1.0/(1e-30+(x-dx-x0)*(x-dx-x0)+(y-y0)*(y-y0)+(z-z0)*(z-z0));
	dist[2] = 1.0/(1e-30+(x-dx-x0)*(x-dx-x0)+(y-dy-y0)*(y-dy-y0)+(z-z0)*(z-z0));
	dist[3] = 1.0/(1e-30+(x-x0)*(x-x0)+(y-dy-y0)*(y-dy-y0)+(z-z0)*(z-z0));
	dist[4] = 1.0/(1e-30+(x-x0)*(x-x0)+(y-y0)*(y-y0)+(z-dz-z0)*(z-dz-z0));
	dist[5] = 1.0/(1e-30+(x-dx-x0)*(x-dx-x0)+(y-y0)*(y-y0)+(z-dz-z0)*(z-dz-z0));
	dist[6] = 1.0/(1e-30+(x-dx-x0)*(x-dx-x0)+(y-dy-y0)*(y-dy-y0)+(z-dz-z0)*(z-dz-z0));
	dist[7] = 1.0/(1e-30+(x-x0)*(x-x0)+(y-dy-y0)*(y-dy-y0)+(z-dz-z0)*(z-dz-z0));
	for (int i = 0; i < 8; i++)
	{
		total_dist += dist[i];
	}
	for (int i = 0; i < 8; i++)
	{
		res += val[i]*dist[i]/total_dist;
	}
	return res;
}

//一维线性插值
double LineInterp(double x1,double x2,double y1,double y2,double x)
{
	if (x1 == x2) return 0.5*(y1 + y2);
	else
	{
		double k = (y2 - y1)/(x2 - x1);
		double b = y1 - k*x1;
		return k*x + b;
	}
}
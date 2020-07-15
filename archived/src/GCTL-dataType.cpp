#include "GCTL-dataType.h"
//cpoint定义
cpoint::cpoint()
{
	x = y = z = BDL_MAX;
}

void cpoint::info()
{
	std::cout << x << " " << y << " " << z << std::endl;
}
//spoint定义
spoint::spoint()
{
	lon = lat = rad = BDL_MAX;
}

void spoint::info()
{
	std::cout << lon << " " << lat << " " << rad << std::endl;
}
//tensor定义
tensor::tensor()
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			val[i][j] = BDL_MAX;
		}
	}
}

void tensor::info()
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			std::cout << val[i][j] << " ";
		}
		std::cout << std::endl;
	}
}
//obs定义
vertex::vertex()
{
	id = -1;
	if (!val.empty()) val.clear();
}

void vertex::info()
{
	std::cout << id << std::endl;
	posic.info();
	posis.info();
	for (int i = 0; i < val.size(); i++)
	{
		std::cout << val.at(i) << " ";
	}
	std::cout << std::endl;
}

void vertex::set(int inId)
{
	id = inId;
}

void vertex::set(cpoint c)
{
	dataFunc df;
	posic = c;
	posis = df.c2s(posic);
}

void vertex::set(spoint s)
{
	dataFunc df;
	posis = s;
	posic = df.s2c(posis);
}

void vertex::set(_1dArray inVal)
{
	val = inVal;
}

void vertex::clear()
{
	if (!val.empty()) val.clear();
}
//模型单元定义
modcell::modcell()
{
	id = phys = geom = vecNum = -1;
	if(!vec.empty()) vec.clear();
}

void modcell::info()
{
	std::cout << id << " " << phys << " " << geom << " " << vecNum << std::endl;
	for (int i = 0; i < vec.size(); i++)
	{
		std::cout << vec.at(i) << " ";
	}
	std::cout << std::endl;
	for (int i = 0; i < val.size(); i++)
	{
		std::cout << val.at(i) << " ";
	}
	std::cout << std::endl;
}

void modcell::clear()
{
	if(!vec.empty()) vec.clear();
	if(!val.empty()) val.clear();
}
//三角形定义
triangle::triangle()
{
	id = -1;
	phys = 0;
	vec[0] = vec[1] = vec[2] = -1;
	edgelen[0] = edgelen[1] = edgelen[2] = BDL_MAX;
}

void triangle::clear()
{
	if(!val.empty()) val.clear();
}

void triangle::info()
{
	std::cout << id << std::endl;
	std::cout << vec[0] << " " << vec[1] << " " << vec[2] << std::endl;
	std::cout << edgelen[0] << " " << edgelen[1] << " " << edgelen[2] << std::endl;
	for (int i = 0; i < 3; i++)
	{
		E[i].info();
	}
	F.info();
	for (int i = 0; i < val.size(); i++)
	{
		std::cout << val.at(i) << " ";
	}
	std::cout << std::endl;
}
//三棱锥
tripy::tripy()
{
	id = -1;
	phys = -1;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			vec[i][j] = -1;
		}
	}
	val = BDL_MAX;
	for (int i = 0; i < 12; i++)
	{
		edgelen[i] = BDL_MAX;
	}
}
//三角棱柱定义
prism::prism()
{
	id = -1;
	phys = 0;
	vec[0] = vec[1] = vec[2] = vec[3] = vec[4] = vec[5] = -1;
}

void prism::clear()
{
	if(!val.empty()) val.clear();
}
//cube定义
cube::cube()
{
	id = -1;
	x1 = x2 = y1 = y2 = z1 = z2 = BDL_MAX;
	if (!val.empty()) val.clear();
}

void cube::info()
{
	std::cout << id << std::endl;
	std::cout << x1 << " " << x2 << " " << y1 << " " << y2 << " " << z1 << " " << z2 << std::endl;
	for (int i = 0; i < val.size(); i++)
	{
		std::cout << val.at(i) << " ";
	}
	std::cout << std::endl;
}

void cube::set(int inId)
{
	id = inId;
}

void cube::set(_1dArray inVal)
{
	val = inVal;
}

void cube::clear()
{
	if (!val.empty()) val.clear();
}
//sphere定义
sphere::sphere()
{
	id = -1;
	x = y = z = rad = BDL_MAX;
	if (!val.empty()) val.clear();
}

void sphere::info()
{
	std::cout << id << std::endl;
	std::cout << x << " " << y << " " << z << " " << rad << std::endl;
	for (int i = 0; i < val.size(); i++)
	{
		std::cout << val.at(i) << " ";
	}
	std::cout << std::endl;
}

void sphere::set(int inId)
{
	id = inId;
}

void sphere::set(_1dArray inVal)
{
	val = inVal;
}

void sphere::clear()
{
	if (!val.empty()) val.clear();
}

bool operator ==(cpoint a, cpoint b) //矢量等于
{
	if(fabs(a.x-b.x)<=ZERO&&fabs(a.y-b.y)<=ZERO&&fabs(a.z-b.z)<=ZERO)
	{
		return true;
	}
	else return false;
}

bool operator !=(cpoint a, cpoint b) //矢量不等于
{
	if(fabs(a.x-b.x)>ZERO||fabs(a.y-b.y)>ZERO||fabs(a.z-b.z)>ZERO)
	{
		return true;
	}
	else return false;
}

cpoint operator -(cpoint a, cpoint b) //矢量减法
{
	cpoint m;
	m.x=a.x-b.x;
	m.y=a.y-b.y;
	m.z=a.z-b.z;
	return m;
}

cpoint operator +(cpoint a, cpoint b) //矢量加法
{
	cpoint m;
	m.x=a.x+b.x;
	m.y=a.y+b.y;
	m.z=a.z+b.z;
	return m;
}

cpoint operator *(double sign,cpoint b) //矢量数乘法
{
	cpoint m;
	m.x=sign*b.x;
	m.y=sign*b.y;
	m.z=sign*b.z;
	return m;
}

tensor operator +(tensor a,tensor b) //矢量数乘法
{
	tensor m;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			m.val[i][j] = a.val[i][j] + b.val[i][j];
		}
	}
	return m;
}


bool operator ==(spoint a, spoint b) //spoint等于
{
	if(fabs(a.lon-b.lon)<=ZERO&&fabs(a.lat-b.lat)<=ZERO&&fabs(a.rad-b.rad)<=ZERO)
	{
		return true;
	}
	else return false;
}

bool operator !=(spoint a, spoint b) //spoint不等于
{
	if(fabs(a.lon-b.lon)>ZERO||fabs(a.lat-b.lat)>ZERO||fabs(a.rad-b.rad)>ZERO)
	{
		return true;
	}
	else return false;
}

int dataFunc::angXYZ(cpoint a,double& angx,double& angy,double& angz)
{
	double length = sqrt(a.x*a.x+a.y*a.y+a.z*a.z);
	if (length <= ZERO) //如果矢量为0则令所有角度均为0 返回1 计算失败
	{
		angx = angy = angz = 0.0;
		return 1;
	}
	else
	{
		angx = acos(a.x/length)*180.0/Pi;
		angy = acos(a.y/length)*180.0/Pi;
		angz = acos(a.z/length)*180.0/Pi;
		return 0;
	}
}

int dataFunc::angXYZ(spoint s,double& angx,double& angy,double& angz)
{
	cpoint a = s2c(s);
	double length = sqrt(a.x*a.x+a.y*a.y+a.z*a.z);
	if (length <= ZERO)
	{
		angx = angy = angz = 0.0;
		return 1;
	}
	else
	{
		angx = acos(a.x/length)*180.0/Pi;
		angy = acos(a.y/length)*180.0/Pi;
		angz = acos(a.z/length)*180.0/Pi;
		return 0;
	}
}

double dataFunc::dot(cpoint a, cpoint b) //矢量点乘
{
	return a.x*b.x+a.y*b.y+a.z*b.z;
}

double dataFunc::lenCpoint(cpoint v) //矢量模
{
	return sqrt(v.x*v.x+v.y*v.y+v.z*v.z);
}

double dataFunc::disCpoint(cpoint a,cpoint b) //两点距离
{
	return sqrt(pow(a.x-b.x,2)+pow(a.y-b.y,2)+pow(a.z-b.z,2));
}

double dataFunc::angCpoint(cpoint a,cpoint b) //向量夹角
{
	return acos((a.x*b.x+a.y*b.y+a.z*b.z)/(sqrt(a.x*a.x+a.y*a.y+a.z*a.z)*sqrt(b.x*b.x+b.y*b.y+b.z*b.z)))*180.0/Pi;
}

double dataFunc::dotToPlane(cpoint c,cpoint nor,cpoint d)
{
	cpoint tmp;
	//确定nor是单位向量
	tmp.x = nor.x/sqrt(nor.x*nor.x + nor.y*nor.y + nor.z*nor.z);
	tmp.y = nor.y/sqrt(nor.x*nor.x + nor.y*nor.y + nor.z*nor.z);
	tmp.z = nor.z/sqrt(nor.x*nor.x + nor.y*nor.y + nor.z*nor.z);

	//点到平面距离等于vec_{cd}在法线方向的投影的绝对值
	return fabs(dot(tmp,d-c));
}

cpoint dataFunc::cross(cpoint a,cpoint b) //矢量叉乘
{
	cpoint v;
	v.x = a.y*b.z-a.z*b.y;
	v.y = a.z*b.x-a.x*b.z;
	v.z = a.x*b.y-a.y*b.x;
	return v;
}

cpoint dataFunc::normal(cpoint v) //求单位矢量
{
	double Length;
	cpoint nor_v;
	Length = sqrt(v.x*v.x+v.y*v.y+v.z*v.z);
	nor_v.x = v.x/Length;
	nor_v.y = v.y/Length;
	nor_v.z = v.z/Length;
	return nor_v;
}

cpoint dataFunc::midCpoint(cpoint a,cpoint b) //两点中点
{
	cpoint c;
	c.x = (a.x + b.x)/2;
	c.y = (a.y + b.y)/2;
	c.z = (a.z + b.z)/2;
	return c;
}

cpoint dataFunc::cenCpoint(cpointArray ca)
{
	cpoint c;
	c.x = 0; c.y = 0; c.z = 0;
	if (!ca.empty())
	{
		for (int i = 0; i < ca.size(); i++)
		{
			c.x += ca.at(i).x;
			c.y += ca.at(i).y;
			c.z += ca.at(i).z;
		}
		c.x /= ca.size();
		c.y /= ca.size();
		c.z /= ca.size();
	}
	return c;
}

//返回两点之间的一个点 以第一个点为参考点 第三个参数为相对于原线段的比例
cpoint dataFunc::scaleCpoint(cpoint a,cpoint b,double scale)
{
	cpoint c;
	c.x = a.x + (b.x - a.x)*scale;
	c.y = a.y + (b.y - a.y)*scale;
	c.z = a.z + (b.z - a.z)*scale;
	return c;
}

cpoint dataFunc::rescaleCpoint(cpoint a,double refr)
{
	cpoint c;
	double m = sqrt(a.x*a.x+a.y*a.y+a.z*a.z);
	c.x = a.x*refr/m;
	c.y = a.y*refr/m;
	c.z = a.z*refr/m;
	return c;
}

cpoint dataFunc::vecMartix(cpoint a,tensor m) //a^T*Tensor
{
	cpoint v;
	v.x = a.x*m.val[0][0]+a.y*m.val[1][0]+a.z*m.val[2][0];
	v.y = a.x*m.val[0][1]+a.y*m.val[1][1]+a.z*m.val[2][1];
	v.z = a.x*m.val[0][2]+a.y*m.val[1][2]+a.z*m.val[2][2];
	return v;
}

cpoint dataFunc::martixVec(tensor m,cpoint a) //Tensor*a
{
	cpoint v;
	v.x = a.x*m.val[0][0]+a.y*m.val[0][1]+a.z*m.val[0][2];
	v.y = a.x*m.val[1][0]+a.y*m.val[1][1]+a.z*m.val[1][2];
	v.z = a.x*m.val[2][0]+a.y*m.val[2][1]+a.z*m.val[2][2];
	return v;
}

cpoint dataFunc::s2c(spoint s)
{
	cpoint c;
	c.x = s.rad*sin((0.5 - s.lat/180.0)*Pi)*cos((2.0 + s.lon/180.0)*Pi);
	c.y = s.rad*sin((0.5 - s.lat/180.0)*Pi)*sin((2.0 + s.lon/180.0)*Pi);
	c.z = s.rad*cos((0.5 - s.lat/180.0)*Pi);
	return c;
}

cpoint dataFunc::rotate(cpoint c,double angx,double angy,double angz)
{
	cpoint cp,temp_cp;
	// 绕x轴旋转
	cp.x = c.x;
	cp.y = cos(angx*Pi/180.0)*c.y - sin(angx*Pi/180.0)*c.z;
	cp.z = sin(angx*Pi/180.0)*c.y + cos(angx*Pi/180.0)*c.z;
	// 绕y轴旋转
	temp_cp.x = cos(angy*Pi/180.0)*cp.x + sin(angy*Pi/180.0)*cp.z;
	temp_cp.y = cp.y;
	temp_cp.z = -1.0*sin(angy*Pi/180.0)*cp.x + cos(angy*Pi/180.0)*cp.z;
	// 绕z轴旋转
	cp.x = cos(angz*Pi/180.0)*temp_cp.x - sin(angz*Pi/180.0)*temp_cp.y;
	cp.y = sin(angz*Pi/180.0)*temp_cp.x + cos(angz*Pi/180.0)*temp_cp.y;
	cp.z = temp_cp.z;
	return cp;
}

cpoint dataFunc::rotate(cpoint c, cpoint axis, double ang)
{
	cpoint cp;
	tensor A, A2, A3, M;
	cpoint nor_axis = normal(axis);
	A.val[0][0] = nor_axis.x*nor_axis.x; A.val[0][1] = nor_axis.x*nor_axis.y; A.val[0][2] = nor_axis.x*nor_axis.z;
	A.val[1][0] = nor_axis.y*nor_axis.x; A.val[1][1] = nor_axis.y*nor_axis.y; A.val[1][2] = nor_axis.y*nor_axis.z;
	A.val[2][0] = nor_axis.z*nor_axis.x; A.val[2][1] = nor_axis.z*nor_axis.y; A.val[2][2] = nor_axis.z*nor_axis.z;

	A2.val[0][0] = 1.0-nor_axis.x*nor_axis.x; A2.val[0][1] = nor_axis.x*nor_axis.y; A2.val[0][2] = nor_axis.x*nor_axis.z;
	A2.val[1][0] = nor_axis.y*nor_axis.x; A2.val[1][1] = 1.0-nor_axis.y*nor_axis.y; A2.val[1][2] = nor_axis.y*nor_axis.z;
	A2.val[2][0] = nor_axis.z*nor_axis.x; A2.val[2][1] = nor_axis.z*nor_axis.y; A2.val[2][2] = 1.0-nor_axis.z*nor_axis.z;

	A3.val[0][0] = 0.0;             A3.val[0][1] = -1.0*nor_axis.z;  A3.val[0][2] = nor_axis.y;
	A3.val[1][0] = nor_axis.z;      A3.val[1][1] = 0.0;              A3.val[1][2] = -1.0*nor_axis.x;
	A3.val[2][0] = -1.0*nor_axis.y; A3.val[2][1] = nor_axis.x;       A3.val[2][2] = 0.0;

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			M.val[i][j] = A.val[i][j] + cos(ang*Pi/180.0)*A2.val[i][j] + sin(ang*Pi/180.0)*A3.val[i][j];
		}
	}

	cp = vecMartix(c,martixT(M));
	return cp;
}

cpoint dataFunc::lineOnPlane(cpoint face_p,cpoint face_nor,cpoint line_p, cpoint line_nor) //平面上的一个点c 平面的外法线矢量 平面外一个点 线方向
{
	cpoint m;
	m = line_p;
	double t;
	if (dot(face_nor,line_nor) != 0)
	{
		t = dot(face_nor,face_p - line_p)/dot(face_nor,line_nor);
		m.x += line_p.x*t;
		m.y += line_p.y*t;
		m.z += line_p.z*t;
	}
	return m;
}

spoint dataFunc::c2s(cpoint c)
{
	spoint s;
	s.rad = sqrt(pow(c.x,2)+pow(c.y,2)+pow(c.z,2));
	if (fabs(s.rad)<ZERO) //点距离原点极近 将点置于原点
	{
		s.lat = s.lon = s.rad = 0.0;
	}
	else
	{
		s.lat = 90.0 - acos(c.z/s.rad)*180.0/Pi;
		s.lon = atan2(c.y,c.x)*180.0/Pi;
	}
	return s;
}

tensor dataFunc::kron(cpoint a,cpoint b) //tensor product of two cpoints
{
	tensor t;
	t.val[0][0]=a.x*b.x; t.val[0][1]=a.x*b.y; t.val[0][2]=a.x*b.z;
	t.val[1][0]=a.y*b.x; t.val[1][1]=a.y*b.y; t.val[1][2]=a.y*b.z;
	t.val[2][0]=a.z*b.x; t.val[2][1]=a.z*b.y; t.val[2][2]=a.z*b.z;
	return t;
}

tensor dataFunc::martixMartix(tensor a,tensor b) //维度为3*3的矩阵相乘
{
	tensor m;
	for(int i=0;i<3;i++)
	{
		for(int j=0;j<3;j++)
		{
			m.val[i][j] = a.val[i][0]*b.val[0][j]+a.val[i][1]*b.val[1][j]+a.val[i][2]*b.val[2][j];
		}
	}
	return m;
}

tensor dataFunc::martixT(tensor a) //维度为3x3的矩阵转置
{
	tensor m;
	for(int i=0;i<3;i++)
	{
		for(int j=0;j<3;j++)
		{
			m.val[i][j] = a.val[j][i];
		}
	}
	return m;
}

tensor dataFunc::martixDouble(tensor a,double d) //维度为3x3的矩阵转置
{
	tensor m;
	for(int i=0;i<3;i++)
	{
		for(int j=0;j<3;j++)
		{
			m.val[i][j] = a.val[j][i]*d;
		}
	}
	return m;
}
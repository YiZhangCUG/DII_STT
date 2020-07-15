#ifndef _CGDY_H
#define _CGDY_H
#include "struct.h"
#include "iterator"
#include "random"

class cgdy
{
private:
	//模型类数组
	vertexArray mshVert; //msh文件顶点数组
	vertexArray modelVert; //模型顶点数组 注意我们会在最后新增一个顶点 在坐标原点
	triPyramidArray modelPyramid; //由模版得到的三棱锥数组
	vertNeighArray vertNeigh; //顶点相邻关系
	//元素在文件中的id与元素数组排序的映射
	IDMAP pyraIdMap; //三棱锥索引映射
	IDMAP VertIdMap; //顶点索引映射
	IDMAP::iterator iim;
	//观测点数组
	vertexArray obsVert; //观测点的第一个值为数据值 第二值为不确定度

	_1dArray baseTopo; //基准面高程
	_1dArray forwardTopo; //正演地形
	_1dArray initTopo; //初始面高程
	_1dArray upperTopo; //上界面高程
	_1dArray bottomTopo; //下界面高程
	_1dArray referTopo; //参考面高程
	_1dArray rhoValue; //界面密度值
	_1dArray invertTopo; //反演面高程
	_1dArray invertTopo_temp;

	_1dArray baseGravity; //基准面的重力响应
	_1dArray varyGravity; //模型面的重力响应
	_1dArray objectDiff;
	_1dArray objectDiff_temp;
	_1dArray gradRad;
	_1dArray gradRad_temp;
	_1dArray dk;

	int debugModel;
	int use_strong;

	double refr,refR;
	double objectF;
	double objectF_temp;
	double gradModule;
	double ak, betak_DY, dkyk, gkyk, betak_HS,betak;
	double c1, c2;
	double fNoise;

	char baseTopoName[1024]; //对应数据在msh文件中的名称
	char forwardTopoName[1024];
	char initTopoName[1024];
	char upperTopoName[1024];
	char bottomTopoName[1024];
	char referTopoName[1024];
	char rhoValueName[1024];

	char runtype[1024];
	char datatype[1024];
	char mshName[1024];
	char xyzName[1024];
	char outMshName[1024];
	char refSysName[1024];
	char obsPara[1024];
	char outObsName[1024];
	char baseTopoFile[1024];

	char dkFile[1024];
	char tempFile[1024];
public:
	cgdy(){
		refr = refR = 0.0;
		strcpy(baseTopoName,"NULL");
		strcpy(forwardTopoName,"NULL");
		strcpy(initTopoName,"NULL");
		strcpy(upperTopoName,"NULL");
		strcpy(bottomTopoName,"NULL");
		strcpy(referTopoName,"NULL");
		strcpy(rhoValueName,"NULL");

		strcpy(runtype,"NULL");
		strcpy(datatype,"gravity"); //默认使用重力数据
		strcpy(mshName,"NULL");
		strcpy(xyzName,"NULL");
		strcpy(outMshName,"NULL");
		strcpy(refSysName,"NULL");
		strcpy(obsPara,"NULL");
		strcpy(outObsName,"NULL");
		strcpy(baseTopoFile,"NULL");

		strcpy(dkFile,"NULL");
		strcpy(tempFile,"NULL");
		use_strong = 0;
	}
	~cgdy(){}
	int runtine(char*); //程序流程
	int readMsh(char*); //读入msh文件 我们所有的模型数据输入均由这个函数完成
	int initRef(char*); //初始化参考椭球
	int initBaseGravity(char*); //从文件读入基准面的重力值
	int initObs(char*); //初始化观测值
	void calObs(double,double);
	int outObs(char*); //输出观测值
	int outVertData(char*,_1dArray);
	int outNodeData(char*,char*,_1dArray);
	int outNodeData(char*,_1dArray); //直接输出模型文件中的顶点属性值的xyz文件
	int outEleData(char*,char*,_1dArray);
	void initModelSetup(); //初始化模型设置
	void changeModelSetupTo(_1dArray); //改变modelVert值并重新计算几何参数
	_1dArray forwardGravity(); //正演计算当前模型的重力响应
	_1dArray forwardGradient();
	_1dArray forwardDiff(double&,char*); //计算模型正演值与观测值的差值 同时返回差值除不确定度的平方和除以观测点数量 这里我们直接调用forwardGravity
	_1dArray forwardRadGrad(_1dArray,char*); //计算差值相对于三棱锥顶面顶点的偏导数
	//反演函数
	int initInversionSetup(); //初始化反演设置
	void boundToBox(_1dArray&); //目标值范围约束
	int linearSearch_Wolfe(); //标准条件的非精确线性搜索
	int linearSearch_strongWolfe(); //强条件的非精确线性搜索
	int invertRuntine();
};

int cgdy::runtine(char* parafile)
{
	string temp_str;

	ifstream paraIn;
	if (open_infile(paraIn,parafile)) return -1;

	while (getline(paraIn,temp_str))
	{
		if (*(temp_str.begin()) == '#') continue;
		else
		{
			if (1==sscanf(temp_str.c_str(),"runtype=%s",runtype))
				cout << "running type:\t" << runtype << endl;
			else if (1==sscanf(temp_str.c_str(),"datatype=%s",datatype))
				cout << "data type:\t" << datatype << endl;
			else if (1==sscanf(temp_str.c_str(),"debugModel=%d",&debugModel))
				cout << "debug model:\t" << debugModel << endl;
			else if (1==sscanf(temp_str.c_str(),"strongCondition=%d",&use_strong))
				cout << "strong wolfe condition:\t" << use_strong << endl;
			else if (1==sscanf(temp_str.c_str(),"model-file=%s",mshName))
				cout << "model file:\t" << mshName << endl;
			else if (1==sscanf(temp_str.c_str(),"dk-file=%s",dkFile))
				cout << "dk file:\t" << dkFile << endl;
			else if (1==sscanf(temp_str.c_str(),"tempRes-file=%s",tempFile))
				cout << "tempRes file:\t" << tempFile << endl;
			else if (1==sscanf(temp_str.c_str(),"basement-topo=%[^\n]",baseTopoName))
				cout << "basement topography:\t" << baseTopoName << endl;
			else if (1==sscanf(temp_str.c_str(),"forward-topo=%[^\n]",forwardTopoName))
				cout << "forward topography:\t" << forwardTopoName << endl;
			else if (1==sscanf(temp_str.c_str(),"basement-file=%s",baseTopoFile))
				cout << "basement file:\t" << baseTopoFile << endl;
			else if (1==sscanf(temp_str.c_str(),"initial-topo=%[^\n]",initTopoName))
				cout << "initial topography:\t" << initTopoName << endl;
			else if (1==sscanf(temp_str.c_str(),"upper-boundary-topo=%[^\n]",upperTopoName))
				cout << "upper boundary topography:\t" << upperTopoName << endl;
			else if (1==sscanf(temp_str.c_str(),"bottom-boundary-topo=%[^\n]",bottomTopoName))
				cout << "bottom boundary topography:\t" << bottomTopoName << endl;
			else if (1==sscanf(temp_str.c_str(),"reference-topo=%[^\n]",referTopoName))
				cout << "reference topography:\t" << referTopoName << endl;
			else if (1==sscanf(temp_str.c_str(),"density-distribution=%[^\n]",rhoValueName))
				cout << "density distribution:\t" << rhoValueName << endl;
			else if (1==sscanf(temp_str.c_str(),"xyz-file=%s",xyzName))
				cout << "xyz file:\t" << xyzName << endl;
			else if (1==sscanf(temp_str.c_str(),"msh-file=%s",outMshName))
				cout << "msh file:\t" << outMshName << endl;
			else if (1==sscanf(temp_str.c_str(),"refer-radius=%s",refSysName))
				cout << "reference radius:\t" << refSysName << endl;
			else if (1==sscanf(temp_str.c_str(),"obserin-para=%s",obsPara))
				cout << "observation para:\t" << obsPara << endl;
			else if (1==sscanf(temp_str.c_str(),"obserout-para=%s",outObsName))
				cout << "observation file:\t" << outObsName << endl;
			else if (1==sscanf(temp_str.c_str(),"c1=%lf",&c1))
				cout << "c1:\t" << c1 << endl;
			else if (1==sscanf(temp_str.c_str(),"c2=%lf",&c2))
				cout << "c2:\t" << c2 << endl;
			else if (1==sscanf(temp_str.c_str(),"ak=%lf",&ak))
				cout << "ak:\t" << ak << endl;
			else if (1==sscanf(temp_str.c_str(),"noise-dev=%lf",&fNoise))
				cout << "noise deviation:\t" << fNoise << endl;
			else
			{
				cout << BOLDRED << "Error ==> " << RESET << "undefined parameter: " << temp_str.c_str() << endl;
				return -1;
			}
		}
	}
	paraIn.close();
	cout << "===============================" << endl;

	if (!strcmp(runtype,"forward"))
	{
		if (initRef(refSysName)) return -1; //首先初始化参考系统
		if (readMsh(mshName)) return -1; //读入模型文件
		if (initObs(obsPara)) return -1; //初始化观测点

		initModelSetup(); //初始化模型设置
		if (initBaseGravity(baseTopoFile)) //尝试从文件读入基准面重力场 若失败则计算 同时输出计算结果
		{
			changeModelSetupTo(baseTopo); //计算基准面重力
			if (!strcmp(datatype,"gravity"))
			{
				baseGravity = forwardGravity();
			}
			else if (!strcmp(datatype,"gradient"))
			{
				baseGravity = forwardGradient();
			}
			outVertData(baseTopoFile,baseGravity);
		}

		changeModelSetupTo(forwardTopo); //计算正演面
		if (!strcmp(datatype,"gravity"))
		{
			varyGravity = forwardGravity();
		}
		else if (!strcmp(datatype,"gradient"))
		{
			varyGravity = forwardGradient();
		}

		for (int i = 0; i < obsVert.size(); i++)
		{
			obsVert[i].val.push_back(varyGravity[i] - baseGravity[i]);
		}
		calObs(0,fNoise);
		outObs(outObsName);
	}
	else if (!strcmp(runtype,"inversion"))
	{
		if (initRef(refSysName)) return -1; //首先初始化参考系统
		if (readMsh(mshName)) return -1; //读入模型文件
		if (initObs(obsPara)) return -1; //初始化观测点

		initModelSetup(); //初始化模型设置
		initInversionSetup(); //初始化反演设置

		if (initBaseGravity(baseTopoFile)) //尝试从文件读入基准面重力场 若失败则计算 同时输出计算结果
		{
			changeModelSetupTo(baseTopo); //计算基准面重力
			if (!strcmp(datatype,"gravity"))
			{
				baseGravity = forwardGravity();
			}
			else if (!strcmp(datatype,"gradient"))
			{
				baseGravity = forwardGradient();
			}
			outVertData(baseTopoFile,baseGravity);
		}

		invertRuntine();
		char dataName[1024];
		strcpy(dataName,"inverted topography (m)");
		outNodeData(outMshName,dataName,invertTopo);
	}
	else if (!strcmp(runtype,"convert"))
	{
		if (readMsh(mshName)) return -1; //读入模型文件
		outNodeData(xyzName,referTopo);
	}
	else
	{
		cout << BOLDRED << "Error ==> " << RESET << "wrong running type" << endl;
		return -1;
	}
	return 0;
}

int cgdy::readMsh(char* filename)
{
	int temp_int,ele_count,ele_type,attri_num,temp_attri,temp_id;
	double temp_val;
	vertex temp_vert;
	triPyramid temp_pyra;
	string temp_str;
	stringstream temp_ss;

	if (!strcmp(filename,"NULL")) return -1;

	ifstream mshin;
	if (open_infile(mshin,filename)) return -1;

	while(getline(mshin,temp_str))
	{
		if (temp_str == "$Nodes")
		{
			getline(mshin,temp_str);
			temp_ss.str("");
			temp_ss.clear();
			temp_ss << temp_str;
			temp_ss >> temp_int;
			ele_count = 0;
			for (int i = 0; i < temp_int; i++)
			{
				getline(mshin,temp_str);
				temp_ss.str("");
				temp_ss.clear();
				temp_ss << temp_str;
				temp_ss >> temp_vert.id >> temp_vert.posic.x >> temp_vert.posic.y >> temp_vert.posic.z;
				temp_vert.set(temp_vert.posic);
				mshVert.push_back(temp_vert);
				//idmap的目的是联系顶点索引到顶点在数组中的索引
				VertIdMap[temp_vert.id] = ele_count; //添加idmap值
				ele_count++;
			}
			//顶点读入完成 初始化数组
			baseTopo.resize(mshVert.size()); //基准面高程
			forwardTopo.resize(mshVert.size()); //正演高程面
			initTopo.resize(mshVert.size()); //初始面高程
			upperTopo.resize(mshVert.size()); //上界面高程
			bottomTopo.resize(mshVert.size()); //下界面高程
			referTopo.resize(mshVert.size()); //参考面高程
			rhoValue.resize(mshVert.size()); //界面密度值
			//初始化数组值
			for (int i = 0; i < mshVert.size(); i++)
			{
				baseTopo[i] = forwardTopo[i] = initTopo[i] = referTopo[i] = 0.0;
				upperTopo[i] = BDL_MAX; bottomTopo[i] = BDL_MIN;
				rhoValue[i] = 0.0; //初始化密度值为0.0
			}
		}
		else if (temp_str == "$Elements")
		{
			getline(mshin,temp_str);
			temp_ss.str("");
			temp_ss.clear();
			temp_ss << temp_str;
			temp_ss >> temp_int;
			ele_count = 0;
			for (int i = 0; i < temp_int; i++)
			{
				getline(mshin,temp_str);
				temp_ss.str("");
				temp_ss.clear();
				temp_ss << temp_str;
				temp_ss >> temp_pyra.id >> ele_type; //这里temp_int是元素的id
				if (ele_type == 2) //只读入三角形
				{
					pyraIdMap[temp_pyra.id] = ele_count; //添加idmap值
					temp_ss >> attri_num;
					//第一个属性为物理组
					temp_ss >> temp_pyra.phys;
					for (int a = 0; a < attri_num-1; a++) //忽略后面的属性值
						temp_ss >> temp_attri;
					//读入三棱锥顶面三角形顶点索引
					temp_ss >> temp_pyra.vec[0][0]
							>> temp_pyra.vec[0][1]
							>> temp_pyra.vec[0][2];
					ele_count++;
					modelPyramid.push_back(temp_pyra);
				}
			}
		}
		else if (temp_str == "$NodeData")
		{
			for (int i = 0; i < 2; i++) //先读入元素块的名称 按照关键字解析不同类型的元素值
				getline(mshin,temp_str);

			int valIndex = -1;
			if (!strcmp(temp_str.c_str(),baseTopoName)) valIndex = 0;
			else if (!strcmp(temp_str.c_str(),initTopoName)) valIndex = 1;
			else if (!strcmp(temp_str.c_str(),referTopoName)) valIndex = 2;
			else if (!strcmp(temp_str.c_str(),upperTopoName)) valIndex = 3;
			else if (!strcmp(temp_str.c_str(),bottomTopoName)) valIndex = 4;
			else if (!strcmp(temp_str.c_str(),rhoValueName)) valIndex = 5;
			else if (!strcmp(temp_str.c_str(),forwardTopoName)) valIndex = 6;

			if (valIndex >= 0 && valIndex <= 6)
			{
				for (int i = 0; i < 6; i++) //跳过元素属性前面的值 最后一次为当前元素块的个数
				getline(mshin,temp_str);
				temp_ss.str("");
				temp_ss.clear();
				temp_ss << temp_str;
				temp_ss >> temp_int;
				for (int i = 0; i < temp_int; i++)
				{
					getline(mshin,temp_str);
					temp_ss.str("");
					temp_ss.clear();
					temp_ss << temp_str;
					temp_ss >> temp_id >> temp_val;
					iim = VertIdMap.find(temp_id);
					if (iim != VertIdMap.end()) //在idmap中查找 如果有则当前元素属性是一个三棱柱的属性
					{
						if (valIndex == 0) baseTopo[VertIdMap[temp_id]] = temp_val;
						if (valIndex == 1) initTopo[VertIdMap[temp_id]] = temp_val;
						if (valIndex == 2) referTopo[VertIdMap[temp_id]] = temp_val;
						if (valIndex == 3) upperTopo[VertIdMap[temp_id]] = temp_val;
						if (valIndex == 4) bottomTopo[VertIdMap[temp_id]] = temp_val;
						if (valIndex == 5) rhoValue[VertIdMap[temp_id]] = temp_val;
						if (valIndex == 6) forwardTopo[VertIdMap[temp_id]] = temp_val;
					}
				}

				if (valIndex == 0) cout << "base topography initialized" << endl;
				else if (valIndex == 1) cout << "initial topography initialized" << endl;
				else if (valIndex == 2) cout << "reference topography initialized" << endl;
				else if (valIndex == 3) cout << "upper boundary initialized" << endl;
				else if (valIndex == 4) cout << "bottom boundary initialized" << endl;
				else if (valIndex == 5) cout << "density distribution initialized" << endl;
				else if (valIndex == 6) cout << "forward topography initialized" << endl;
			}
		}
		else continue;
	}
	mshin.close();
	return 0;
}

int cgdy::initRef(char* para)
{
	//首先匹配预定义类型
	if (!strcmp(para,"WGS84"))
	{
		refr = WGS84_PoleRadius;
		refR = WGS84_EquatorRadius;
	}
	else if (!strcmp(para,"EarthRadius"))
	{
		refr = EarthRadius;
		refR = EarthRadius;
	}
	//匹配格式化参数
	else if (2 == sscanf(para,"%lf/%lf",&refr,&refR))
	{
		if (refr <= 0 || refR <= 0)
		{
			cout << BOLDRED << "Error ==> " << RESET << "fail to initial reference system" << endl;
			return -1;
		}
	}
	else
	{
		cout << BOLDRED << "Error ==> " << RESET << "fail to initial reference system" << endl;
		return -1;
	}
	return 0;
}

int cgdy::initBaseGravity(char* filename)
{
	double temp_obs;
	vertex temp_vert;
	string temp_str;
	stringstream temp_ss;

	if (!strcmp(filename,"NULL")) return -1;

	ifstream infile;
	if (open_infile(infile,filename)) return -1;

	int icount = 0;
	while(getline(infile,temp_str))
	{
		if (*(temp_str.begin()) == '#') continue;
		else
		{
			//按每行4个数据解析 初始化基准面重力值
			if (4 == sscanf(temp_str.c_str(),"%lf %lf %lf %lf",
				&temp_vert.posis.lon,&temp_vert.posis.lat,&temp_vert.posis.rad,&temp_obs))
			{
				temp_vert.posis.rad += ellipseRadius(temp_vert.posis.lat,refr,refR);
				//与观测点数据做比较若不同 返回错误
				if (temp_vert.posis != obsVert[icount].posis)
				{
					cout << BOLDRED << "Error ==> " << RESET 
					<< "enterence: " << temp_str << " does not match with the observation point " << obsVert[icount].id << endl;
					infile.close();
					return -1;
				}
				else baseGravity[icount] = temp_obs;
				icount++;
			}
			else
			{
				cout << BOLDRED << "Error ==> " << RESET << "wrong enterence: " << temp_str << " ,in the file: " << filename << endl;
				return -1;
			}
		}
	}
	cout << "basement gravity initialized from file" << endl;
	infile.close();
	return 0;
}

/*
我们对于观测点的操作主要有以下几个类型
1. 建立规则分布的观测点 用于正演模型异常使用
2. 从文件读入不规则分布的观测点 用于正演模型异常使用
3. 从文件读入观测点与值 用于反演计算使用 对点位分布无要求
对应的解决方案为
1. 解析格式化参数 建立规则分布的观测点
2. 从文件读入按每行的数据个数识别文件类型
*/
int cgdy::initObs(char* para)
{
	double temp_obs,temp_dev;
	double lon,lat;
	double lonmin,lonmax,dlon,latmin,latmax,dlat,eleva;
	vertex temp_vert;
	string temp_str;
	stringstream temp_ss;
	char obsType[1024];
	int vertIds[3];
	//按格式解析参数 初始化观测位置 用于正演计算
	if (7 == sscanf(para,"%lf/%lf/%lf/%lf/%lf/%lf/%lf",&lonmin,&dlon,&lonmax,&latmin,&dlat,&latmax,&eleva))
	{
		lat = latmin;
		while(lat <= latmax)
		{
			lon = lonmin;
			while(lon <= lonmax)
			{
				temp_vert.id = obsVert.size();
				temp_vert.posis.lon = lon;
				temp_vert.posis.lat = lat;
				temp_vert.posis.rad = ellipseRadius(lat,refr,refR) + eleva;
				temp_vert.set(temp_vert.posis);
				obsVert.push_back(temp_vert);
				lon += dlon;
			}
			lat += dlat;
		}
		cout << "observation points initialized on grid nodes" << endl;
	}
	else if (2 == sscanf(para,"%[^/]/%lf",obsType,&eleva))
	{
		if (!strcmp(obsType,"cell"))
		{
			for (int i = 0; i < modelPyramid.size(); i++)
			{
				vertIds[0] = VertIdMap[modelPyramid[i].vec[0][0]];
				vertIds[1] = VertIdMap[modelPyramid[i].vec[0][1]];
				vertIds[2] = VertIdMap[modelPyramid[i].vec[0][2]];

				temp_vert.posic = 1.0/3.0 * (mshVert[vertIds[0]].posic + mshVert[vertIds[1]].posic + mshVert[vertIds[2]].posic);

				temp_vert.set(temp_vert.posic);
				temp_vert.posis.rad = ellipseRadius(temp_vert.posis.lat,refr,refR) + eleva;
				temp_vert.set(temp_vert.posis);
				temp_vert.id = obsVert.size();
				obsVert.push_back(temp_vert);
			}
			cout << "observation points initialized on cell centers" << endl;
		}
		else if (!strcmp(obsType,"vert"))
		{
			for (int i = 0; i < mshVert.size(); i++)
			{
				temp_vert = mshVert[i];
				temp_vert.id = obsVert.size();
				temp_vert.posis.rad = ellipseRadius(temp_vert.posis.lat,refr,refR) + eleva;
				temp_vert.set(temp_vert.posis);
				obsVert.push_back(temp_vert);
			}
			cout << "observation points initialized on vertex positions" << endl;
		}
		else
		{
			cout << BOLDRED << "Error ==> " << RESET << "wrong observation types: " << para << endl;
			return -1;
		}
	}
	//解析失败 按文件读入 用于反演使用或者正演计算
	else
	{
		ifstream infile;
		if (open_infile(infile,para)) return -1;

		while(getline(infile,temp_str))
		{
			if (*(temp_str.begin()) == '#') continue;
			else
			{
				//按每行5个数据解析 初始化为含观测值与不确定度的观测点
				if (5 == sscanf(temp_str.c_str(),"%lf %lf %lf %lf %lf",
					&temp_vert.posis.lon,&temp_vert.posis.lat,&temp_vert.posis.rad,&temp_obs,&temp_dev))
				{
					temp_vert.posis.rad += ellipseRadius(temp_vert.posis.lat,refr,refR);
					temp_vert.id = obsVert.size();
					temp_vert.set(temp_vert.posis);
					temp_vert.val.push_back(temp_obs);
					temp_vert.val.push_back(temp_dev);
					obsVert.push_back(temp_vert);
					temp_vert.clear();
				}
				//按每行3个数据解析 初始化为用于正演的观测点
				else if (3 == sscanf(temp_str.c_str(),"%lf %lf %lf",
					&temp_vert.posis.lon,&temp_vert.posis.lat,&temp_vert.posis.rad))
				{
					temp_vert.posis.rad += ellipseRadius(temp_vert.posis.lat,refr,refR);
					temp_vert.id = obsVert.size();
					temp_vert.set(temp_vert.posis);
					obsVert.push_back(temp_vert);
				}
				else
				{
					cout << BOLDRED << "Error ==> " << RESET << "wrong enterence: " << temp_str << " ,in the file: " << para << endl;
					return -1;
				}
			}
		}
		infile.close();
		cout << "observation points initialized from file" << endl;
	}
	//观测值初始化完成 初始化数组
	baseGravity.resize(obsVert.size());
	varyGravity.resize(obsVert.size());
	return 0;
}

void cgdy::calObs(double noise_mean,double noise_dev)
{
	//添加高斯噪声值
	default_random_engine generator;
	normal_distribution<double> dist(noise_mean, noise_dev);

	for (int i = 0; i < obsVert.size(); i++)
	{
		obsVert[i].val[0] += dist(generator);
	}

	for (int i = 0; i < obsVert.size(); i++)
	{
		obsVert[i].val.push_back(noise_dev);
	}
}

int cgdy::outObs(char* filename)
{
	if (!strcmp(filename,"NULL")) return -1;

	ofstream outfile;
	if (open_outfile(outfile,filename)) return -1;
	for (int i = 0; i < obsVert.size(); i++)
	{
		outfile << obsVert[i].posis.lon << " "
		 << obsVert[i].posis.lat << " "
		 << obsVert[i].posis.rad-ellipseRadius(obsVert[i].posis.lat,refr,refR) << " "
		 << setprecision(16) << obsVert[i].val[0] << " " 
		 << obsVert[i].val[1] << endl;
	}
	outfile.close();
	return 0;
}

int cgdy::outVertData(char* filename,_1dArray inputData)
{
	if (!strcmp(filename,"NULL")) return -1;

	ofstream outfile;
	if (open_outfile(outfile,filename)) return -1;
	for (int i = 0; i < obsVert.size(); i++)
	{
		outfile << obsVert[i].posis.lon << " "
		 << obsVert[i].posis.lat << " "
		 << obsVert[i].posis.rad-ellipseRadius(obsVert[i].posis.lat,refr,refR) << " "
		 << setprecision(16) << inputData[i] << endl;
	}
	outfile.close();
	return 0;
}

int cgdy::outNodeData(char* filename,char* DataName,_1dArray Data)
{
	ofstream outfile;
	if (open_outfile(outfile,filename)) return -1;

	outfile<<"$MeshFormat"<<endl<<"2.2 0 8"<<endl<<"$EndMeshFormat"<<endl<<"$Nodes"<<endl<<mshVert.size()<<endl;
	//输出顶点 最后一个不输出
	for (int i = 0; i < mshVert.size(); i++)
	{
		outfile << mshVert[i].id+1 << " " << setprecision(16) << modelVert[i].posic.x << " " << modelVert[i].posic.y << " " << modelVert[i].posic.z << endl;
	}
	outfile<<"$EndNodes"<<endl;

	outfile<<"$Elements"<<endl<<modelPyramid.size()<<endl;
	for (int i = 0; i < modelPyramid.size(); i++)
	{
		outfile << modelPyramid[i].id <<" 2 1 " << modelPyramid[i].phys << " " 
		<< modelPyramid[i].vec[0][0] + 1 << " " 
		<< modelPyramid[i].vec[0][1] + 1 << " " 
		<< modelPyramid[i].vec[0][2] + 1 << endl;
	}
	outfile << "$EndElements"<< endl;

	outfile<<"$NodeData"<<endl;
	outfile<<1<<endl<<"\"" << DataName << "\"" <<endl<<1<<endl<<0.0<<endl<<3<<endl<<0<<endl<<1<<endl<<mshVert.size()<<endl;
	for (int i = 0; i < mshVert.size(); i++)
	{
		outfile << mshVert[i].id + 1 << " " << setprecision(16) << Data[i] << endl;
	}
	outfile<<"$EndNodeData"<< endl;
	outfile.close();
	return 0;
}

int cgdy::outNodeData(char* filename, _1dArray inputData)
{
	ofstream outfile;
	if (open_outfile(outfile,filename)) return -1;

	outfile<<"# x y z lon lat rad input-data"<<endl;
	//输出顶点 最后一个不输出
	for (int i = 0; i < mshVert.size(); i++)
	{
		outfile << setprecision(16) << mshVert[i].posic.x << " "
		 << mshVert[i].posic.y << " "
		 << mshVert[i].posic.z << " "
		 << mshVert[i].posis.lon << " "
		 << mshVert[i].posis.lat << " "
		 << mshVert[i].posis.rad << " "
		 << inputData[i] << endl;
	}
	outfile.close();
	return 0;
}

int cgdy::outEleData(char* filename,char* DataName,_1dArray Data)
{
	ofstream outfile;
	if (open_outfile(outfile,filename)) return -1;

	outfile<<"$MeshFormat"<<endl<<"2.2 0 8"<<endl<<"$EndMeshFormat"<<endl<<"$Nodes"<<endl<<mshVert.size()<<endl;
	//输出顶点 最后一个不输出
	for (int i = 0; i < mshVert.size(); i++)
	{
		outfile << mshVert[i].id+1 << " " << setprecision(16) << mshVert[i].posic.x << " " << mshVert[i].posic.y << " " << mshVert[i].posic.z << endl;
	}
	outfile<<"$EndNodes"<<endl;

	outfile<<"$Elements"<<endl<<modelPyramid.size()<<endl;
	for (int i = 0; i < modelPyramid.size(); i++)
	{
		outfile << modelPyramid[i].id <<" 2 1 " << modelPyramid[i].phys << " " 
		<< modelPyramid[i].vec[0][0] + 1 << " " 
		<< modelPyramid[i].vec[0][1] + 1 << " " 
		<< modelPyramid[i].vec[0][2] + 1 << endl;
	}
	outfile << "$EndElements"<< endl;

	outfile<<"$ElementData"<<endl;
	outfile<<1<<endl<<"\"" << DataName << "\"" <<endl<<1<<endl<<0.0<<endl<<3<<endl<<0<<endl<<1<<endl<<modelPyramid.size()<<endl;
	for (int i = 0; i < modelPyramid.size(); i++)
	{
		outfile << modelPyramid[i].id + 1 << " " << setprecision(16) << Data[i] << endl;
	}
	outfile<<"$EndElementData"<< endl;
	outfile.close();
	return 0;
}

void cgdy::initModelSetup()
{
	int vertId,triId[2];
	int vertIds[3];
	int local_id,local_id_cen;
	//初始化modelVert并添加一个位于原点的顶点
	modelVert.resize(mshVert.size()+1);
	for (int i = 0; i < mshVert.size(); i++)
	{
		modelVert[i] = mshVert[i];
	}
	//增加最后一个点 我们只添加到modelVert中
	modelVert.back().id = -1; //这是一个虚拟点 所以我们令它的id为-1 以后可以使情况更改
	modelVert.back().posic.x = 0.0;
	modelVert.back().posic.y = 0.0;
	modelVert.back().posic.z = 0.0;
	modelVert.back().set(modelVert.back().posic);
	//idmap的目的是联系顶点索引到顶点在数组中的索引
	VertIdMap[modelVert.back().id] = mshVert.size(); //添加idmap值
	//初始化三棱锥侧面顶点索引
	for (int i = 0; i < modelPyramid.size(); i++)
	{
		modelPyramid[i].vec[1][0] = modelPyramid[i].vec[0][0]; modelPyramid[i].vec[1][1] = modelVert.back().id; modelPyramid[i].vec[1][2] = modelPyramid[i].vec[0][1];
		modelPyramid[i].vec[2][0] = modelPyramid[i].vec[0][1]; modelPyramid[i].vec[2][1] = modelVert.back().id; modelPyramid[i].vec[2][2] = modelPyramid[i].vec[0][2];
		modelPyramid[i].vec[3][0] = modelPyramid[i].vec[0][2]; modelPyramid[i].vec[3][1] = modelVert.back().id; modelPyramid[i].vec[3][2] = modelPyramid[i].vec[0][0];
	}
	//初始化模型三棱锥密度值
	for (int i = 0; i < modelPyramid.size(); i++)
	{
		for (int j = 0; j < 3; j++)
			vertIds[j] = VertIdMap[modelPyramid[i].vec[0][j]];
		modelPyramid[i].rho = 
		1.0/3.0*(rhoValue[vertIds[0]]+rhoValue[vertIds[1]]+rhoValue[vertIds[2]]);
	}
	//初始化顶点相邻关系
	vertNeigh.resize(mshVert.size());
	for (int i = 0; i < mshVert.size(); i++)
	{
		vertNeigh[i].id = mshVert[i].id;
	}
	//首先确定与顶点相连的三角面
	for (int i = 0; i < modelPyramid.size(); i++)
	{
		for (int j = 0; j < 3; j++)
		{
			vertId = VertIdMap[modelPyramid[i].vec[0][j]];
			vertNeigh[vertId].tri.push_back(modelPyramid[i].id);
		}
	}
	/*
	//确定与顶点与相连的顶点
	for (int i = 0; i < modelPyramid.size(); i++)
	{
		for (int j = 0; j < 3; j++)
		{
			vertId = VertIdMap[modelPyramid[i].vec[0][j]];
			vertNeigh[vertId].vec.push_back(modelPyramid[i].vec[0][(j+1)%3]);
			vertNeigh[vertId].vec.push_back(modelPyramid[i].vec[0][(j+2)%3]);
		}
	}
	//删除重复点
	vector<int>::iterator pos;
	for (int i = 0; i < vertNeigh.size(); i++)
	{
		sort(vertNeigh[i].vec.begin(),vertNeigh[i].vec.end()); //对顶点序列由小到大排序
		pos = unique(vertNeigh[i].vec.begin(),vertNeigh[i].vec.end()); //获取重复序列开始的位置
		vertNeigh[i].vec.erase(pos,vertNeigh[i].vec.end()); //删除重复点
	}
	//确定vertNeigh.vec中每个面逆时针相连的面
	for (int i = 0; i < vertNeigh.size(); i++)
	{
		for (int j = 0; j < vertNeigh[i].tri.size(); j++)
		{
			triId[0] = pyraIdMap[vertNeigh[i].tri[j]];
			local_id_cen = local_index(vertNeigh[i].id,modelPyramid[triId[0]]);
			for (int l = 1; l < vertNeigh[i].tri.size(); l++)
			{
				triId[1] = pyraIdMap[vertNeigh[i].tri[(l+j)%vertNeigh[i].tri.size()]];
				local_id = local_index(vertNeigh[i].id,modelPyramid[triId[1]]);
				if (modelPyramid[triId[0]].vec[0][(local_id_cen+1)%3] == 
					modelPyramid[triId[1]].vec[0][(local_id+2)%3])
				{
					vertNeigh[i].right_tri.push_back(vertNeigh[i].tri[(l+j)%vertNeigh[i].tri.size()]);
				}
			}
		}
	}
	*/
	return;
}

void cgdy::changeModelSetupTo(_1dArray inputTopo)
{
	int vertIds[3];
	cpoint faceNor,faceVec,faceVec_rj;
	cpoint edgeNor,edgeVec;
	cpoint edgeVec_j2, edgeVec_j2_rj;
	cpoint edgeVec_3j, edgeVec_3j_rj;
	cpoint edgeVec_2j, edgeVec_2j_rj;
	cpoint edgeVec_j3, edgeVec_j3_rj;
	cpoint edgeVec_23, edgeVec_23_rj;
	cpoint rjNor;
	dataFunc df;
	//重新设置modelVert的半径值
	for (int i = 0; i < mshVert.size(); i++)
	{
		modelVert[i].posis.rad = mshVert[i].posis.rad + inputTopo[i];
		modelVert[i].set(modelVert[i].posis);
	}
	//计算三棱锥的几何参数
	for (int i = 0; i < modelPyramid.size(); i++)
	{
		for (int f = 0; f < 4; f++)
		{
			vertIds[0] = VertIdMap[modelPyramid[i].vec[f][0]];
			vertIds[1] = VertIdMap[modelPyramid[i].vec[f][1]];
			vertIds[2] = VertIdMap[modelPyramid[i].vec[f][2]];
			faceVec = df.cross(modelVert[vertIds[1]].posic-modelVert[vertIds[0]].posic
				,modelVert[vertIds[2]].posic-modelVert[vertIds[0]].posic);
			modelPyramid[i].F[f] = df.normal(faceVec);
			for (int j = 0; j < 3; j++)
			{
				vertIds[0] = VertIdMap[modelPyramid[i].vec[f][j]];
				vertIds[1] = VertIdMap[modelPyramid[i].vec[f][(j+1)%3]];
				edgeVec = df.cross(modelVert[vertIds[1]].posic-modelVert[vertIds[0]].posic,faceVec);
				modelPyramid[i].edgelen[f*3+j] = df.disCpoint(modelVert[vertIds[1]].posic,modelVert[vertIds[0]].posic);
				modelPyramid[i].E[f*3+j] = df.normal(edgeVec);
			}
		}
	}
	//计算偏导数
	for (int i = 0; i < modelPyramid.size(); i++)
	{
		vertIds[0] = VertIdMap[modelPyramid[i].vec[0][0]];
		vertIds[1] = VertIdMap[modelPyramid[i].vec[0][1]];
		vertIds[2] = VertIdMap[modelPyramid[i].vec[0][2]];

		for (int e = 0; e < 3; e++)
		{
			//计算顶面的情况
			rjNor = df.normal(modelVert[vertIds[e]].posic);

			faceVec = df.cross(modelVert[vertIds[(e+1)%3]].posic-modelVert[vertIds[e]].posic,
				modelVert[vertIds[(e+2)%3]].posic-modelVert[vertIds[e]].posic);
			faceVec_rj = df.cross(rjNor, modelVert[vertIds[(e+1)%3]].posic - modelVert[vertIds[(e+2)%3]].posic);
			modelPyramid[i].nf_rj[e] = nr_dr(faceVec, faceVec_rj);

			edgeVec_j2 = df.cross(modelVert[vertIds[(e+1)%3]].posic - modelVert[vertIds[e]].posic,faceVec);
			edgeVec_j2_rj = df.cross(modelVert[vertIds[(e+1)%3]].posic - modelVert[vertIds[e]].posic,faceVec_rj) - df.cross(rjNor,faceVec);
			modelPyramid[i].nj2_rj[e] = nr_dr(edgeVec_j2,edgeVec_j2_rj);

			edgeVec_23 = df.cross(modelVert[vertIds[(e+2)%3]].posic - modelVert[vertIds[(e+1)%3]].posic,faceVec);
			edgeVec_23_rj = df.cross(modelVert[vertIds[(e+2)%3]].posic - modelVert[vertIds[(e+1)%3]].posic,faceVec_rj);
			modelPyramid[i].n23_rj[e] = nr_dr(edgeVec_23,edgeVec_23_rj);

			edgeVec_3j = df.cross(modelVert[vertIds[e]].posic - modelVert[vertIds[(e+2)%3]].posic,faceVec);
			edgeVec_3j_rj = df.cross(modelVert[vertIds[e]].posic - modelVert[vertIds[(e+2)%3]].posic,faceVec_rj) + df.cross(rjNor,faceVec);
			modelPyramid[i].n3j_rj[e] = nr_dr(edgeVec_3j,edgeVec_3j_rj);

			//计算右侧面的情况
			faceVec = df.cross(modelVert[vertIds[(e+1)%3]].posic,modelVert[vertIds[e]].posic);
			faceVec_rj = df.cross(modelVert[vertIds[(e+1)%3]].posic,rjNor);

			edgeVec_2j = df.cross(modelVert[vertIds[e]].posic - modelVert[vertIds[(e+1)%3]].posic,faceVec);
			edgeVec_2j_rj = df.cross(rjNor,faceVec);
			modelPyramid[i].n2j_rj[e] = nr_dr(edgeVec_2j,edgeVec_2j_rj);
			//计算左侧面的情况
			faceVec = df.cross(modelVert[vertIds[e]].posic,modelVert[vertIds[(e+2)%3]].posic);
			faceVec_rj = df.cross(rjNor,modelVert[vertIds[(e+2)%3]].posic);

			edgeVec_j3 = df.cross(modelVert[vertIds[(e+2)%3]].posic - modelVert[vertIds[e]].posic,faceVec);
			edgeVec_j3_rj = df.cross(faceVec,rjNor);
			modelPyramid[i].nj3_rj[e] = nr_dr(edgeVec_j3,edgeVec_j3_rj);
		}
	}
}

//正演计算当面模型的重力响应
_1dArray cgdy::forwardGravity()
{
	int vertIds[3];
	double Le,wf;
	double dv1,dv2;
	double face_sum,edge_sum;
	cpoint face_temp,edge_temp; //它们并不是直角坐标系下的点 我们只是借用向量操作而已
	tensor face_tensor,edge_tensor;
	cpoint re;
	cpoint R; //注意这里R并不是一个直角坐标系下的点 我们只是借用向量操作而已
	cpoint r_ijk[3];
	double L_ijk[3];
	dataFunc df;

	_1dArray tempGravity;
	tempGravity.resize(obsVert.size());
	_1dArray face_add,edge_add;
	face_add.resize(modelPyramid.size());
	edge_add.resize(modelPyramid.size());

	int j,f,e;
	ProgressBar *bar = new ProgressBar(obsVert.size(),"Calculating gravity");
	for (int i = 0; i < obsVert.size(); i++)
	{
		bar->Progressed(i);
		R.x = sin((0.5-obsVert[i].posis.lat/180)*Pi)*cos((2+obsVert[i].posis.lon/180)*Pi);
		R.y = sin((0.5-obsVert[i].posis.lat/180)*Pi)*sin((2+obsVert[i].posis.lon/180)*Pi);
		R.z = cos((0.5-obsVert[i].posis.lat/180)*Pi);
#pragma omp parallel for private(j,f,e,face_sum,edge_sum,r_ijk,L_ijk,wf,Le,face_temp,edge_temp,dv1,dv2,re,vertIds) shared(obsVert,VertIdMap,modelPyramid,modelVert,R) schedule(guided)
		for (j = 0; j < modelPyramid.size(); j++)
		{
			face_sum = edge_sum = 0.0;
			for (f = 0; f < 4; f++)
			{
				vertIds[0] = VertIdMap[modelPyramid[j].vec[f][0]];
				vertIds[1] = VertIdMap[modelPyramid[j].vec[f][1]];
				vertIds[2] = VertIdMap[modelPyramid[j].vec[f][2]];

				r_ijk[0] = modelVert[vertIds[0]].posic - obsVert[i].posic;
				r_ijk[1] = modelVert[vertIds[1]].posic - obsVert[i].posic;
				r_ijk[2] = modelVert[vertIds[2]].posic - obsVert[i].posic;

				L_ijk[0] = df.lenCpoint(r_ijk[0]);
				L_ijk[1] = df.lenCpoint(r_ijk[1]);
				L_ijk[2] = df.lenCpoint(r_ijk[2]);

				wf =2*atan2(df.dot(r_ijk[0],df.cross(r_ijk[1],r_ijk[2])),
							L_ijk[0]*L_ijk[1]*L_ijk[2]+L_ijk[0]*df.dot(r_ijk[1],r_ijk[2])+L_ijk[1]*df.dot(r_ijk[2],r_ijk[0])+L_ijk[2]*df.dot(r_ijk[0],r_ijk[1]));

				face_temp = df.dot(modelPyramid[j].F[f],r_ijk[0])*modelPyramid[j].F[f];
				face_sum += df.dot(R,face_temp)*wf;

				for (e = 0; e < 3; e++)
				{
					vertIds[0] = VertIdMap[modelPyramid[j].vec[f][e]];
					vertIds[1] = VertIdMap[modelPyramid[j].vec[f][(e+1)%3]];

					dv1 = df.disCpoint(modelVert[vertIds[0]].posic,obsVert[i].posic);
					dv2 = df.disCpoint(modelVert[vertIds[1]].posic,obsVert[i].posic);

					re = df.midCpoint(modelVert[vertIds[0]].posic,modelVert[vertIds[1]].posic) - obsVert[i].posic;
					Le = log((dv1+dv2+modelPyramid[j].edgelen[f*3+e])/(dv1+dv2-modelPyramid[j].edgelen[f*3+e]));

					edge_temp = df.dot(modelPyramid[j].E[f*3+e],re)*modelPyramid[j].F[f];
					edge_sum += df.dot(R,edge_temp)*Le;
				}
			}
			face_add[j] = face_sum;
			edge_add[j] = edge_sum;
		}

		tempGravity[i] = 0.0;
		for (j = 0; j < modelPyramid.size(); j++)
		{
			tempGravity[i] += G0*modelPyramid[j].rho*(edge_add[j] - face_add[j]);
		}
	}

	return tempGravity;
}

_1dArray cgdy::forwardGradient()
{
	int vertIds[3];
	double Le,wf;
	double dv1,dv2;
	double face_sum,edge_sum;
	cpoint face_temp,edge_temp; //它们并不是直角坐标系下的点 我们只是借用向量操作而已
	tensor face_tensor,edge_tensor;
	cpoint re;
	cpoint R; //注意这里R并不是一个直角坐标系下的点 我们只是借用向量操作而已
	cpoint r_ijk[3];
	double L_ijk[3];
	dataFunc df;

	_1dArray tempGravity;
	tempGravity.resize(obsVert.size());
	_1dArray face_add,edge_add;
	face_add.resize(modelPyramid.size());
	edge_add.resize(modelPyramid.size());

	int j,f,e;
	ProgressBar *bar = new ProgressBar(obsVert.size(),"Calculating gradient");
	for (int i = 0; i < obsVert.size(); i++)
	{
		bar->Progressed(i);
		R.x = sin((0.5-obsVert[i].posis.lat/180)*Pi)*cos((2+obsVert[i].posis.lon/180)*Pi);
		R.y = sin((0.5-obsVert[i].posis.lat/180)*Pi)*sin((2+obsVert[i].posis.lon/180)*Pi);
		R.z = cos((0.5-obsVert[i].posis.lat/180)*Pi);
#pragma omp parallel for private(j,f,e,face_sum,edge_sum,r_ijk,L_ijk,wf,Le,face_temp,edge_temp,dv1,dv2,re,vertIds) shared(obsVert,VertIdMap,modelPyramid,modelVert,R) schedule(guided)
		for (j = 0; j < modelPyramid.size(); j++)
		{
			face_sum = edge_sum = 0.0;
			for (f = 0; f < 4; f++)
			{
				vertIds[0] = VertIdMap[modelPyramid[j].vec[f][0]];
				vertIds[1] = VertIdMap[modelPyramid[j].vec[f][1]];
				vertIds[2] = VertIdMap[modelPyramid[j].vec[f][2]];

				r_ijk[0] = modelVert[vertIds[0]].posic - obsVert[i].posic;
				r_ijk[1] = modelVert[vertIds[1]].posic - obsVert[i].posic;
				r_ijk[2] = modelVert[vertIds[2]].posic - obsVert[i].posic;

				L_ijk[0] = df.lenCpoint(r_ijk[0]);
				L_ijk[1] = df.lenCpoint(r_ijk[1]);
				L_ijk[2] = df.lenCpoint(r_ijk[2]);

				wf =2*atan2(df.dot(r_ijk[0],df.cross(r_ijk[1],r_ijk[2])),
							L_ijk[0]*L_ijk[1]*L_ijk[2]+L_ijk[0]*df.dot(r_ijk[1],r_ijk[2])+L_ijk[1]*df.dot(r_ijk[2],r_ijk[0])+L_ijk[2]*df.dot(r_ijk[0],r_ijk[1]));

				face_temp = df.dot(modelPyramid[j].F[f],R)*modelPyramid[j].F[f];
				face_sum += df.dot(R,face_temp)*wf;

				for (e = 0; e < 3; e++)
				{
					vertIds[0] = VertIdMap[modelPyramid[j].vec[f][e]];
					vertIds[1] = VertIdMap[modelPyramid[j].vec[f][(e+1)%3]];

					dv1 = df.disCpoint(modelVert[vertIds[0]].posic,obsVert[i].posic);
					dv2 = df.disCpoint(modelVert[vertIds[1]].posic,obsVert[i].posic);

					re = df.midCpoint(modelVert[vertIds[0]].posic,modelVert[vertIds[1]].posic) - obsVert[i].posic;
					Le = log((dv1+dv2+modelPyramid[j].edgelen[f*3+e])/(dv1+dv2-modelPyramid[j].edgelen[f*3+e]));

					edge_temp = df.dot(modelPyramid[j].E[f*3+e],R)*modelPyramid[j].F[f];
					edge_sum += df.dot(R,edge_temp)*Le;
				}
			}
			face_add[j] = face_sum;
			edge_add[j] = edge_sum;
		}

		tempGravity[i] = 0.0;
		for (j = 0; j < modelPyramid.size(); j++)
		{
			tempGravity[i] += 1e+4*G0*modelPyramid[j].rho*(edge_add[j] - face_add[j]);
		}
	}

	return tempGravity;
}

_1dArray cgdy::forwardDiff(double& meanSquare,char* calType)
{
	double temp_sum;
	_1dArray tempDiff;
	tempDiff.resize(obsVert.size(),0.0);

	if (!strcmp(calType,"gravity"))
		tempDiff = forwardGravity(); //默认使用重力值
	else if (!strcmp(calType,"gradient"))
		tempDiff = forwardGradient();

	temp_sum = 0.0;
	for (int i = 0; i < obsVert.size(); i++)
	{
		tempDiff[i] = (tempDiff[i] - baseGravity[i] - obsVert[i].val[0])/obsVert[i].val[1];
		temp_sum += tempDiff[i] * tempDiff[i];
	}
	meanSquare = temp_sum/obsVert.size();
	return tempDiff;
}

_1dArray cgdy::forwardRadGrad(_1dArray inputDiff,char* calType)
{
	int p;
	int triId,vertIds[3],local_id;
	double beta,beta_rj,alpha,alpha_rj;
	double a_rj,e_rj,Le_rj,Le;
	double wf,wf_rj;
	double point_sum;
	double L_j23; //当前半径的长度
	double L_ijk[3];
	cpoint r_j23[3];
	cpoint r_j23_side[3];
	cpoint r_ijk[3];
	cpoint R;
	cpoint face_temp,edge_temp;
	tensor face_tensor,edge_tensor;
	cpoint re; //从观测点到边中点的矢量
	dataFunc df;

	_1dArray tempGrad;
	tempGrad.resize(mshVert.size());
	_1dArray face_add,edge_add;
	face_add.resize(obsVert.size());
	edge_add.resize(obsVert.size());

	ProgressBar *bar = new ProgressBar(mshVert.size(),"Calculating radial-gradient");
	if (!strcmp(calType,"gravity"))
	{
		for (int i = 0; i < mshVert.size(); i++)
		{
			bar->Progressed(i);
			point_sum = 0.0;
			for (int j = 0; j < vertNeigh[i].tri.size(); j++) //顺序处理所有以当前顶点为中心的三棱锥
			{
				triId = pyraIdMap[vertNeigh[i].tri[j]]; //定位三棱锥在数组中的位置
				local_id = local_index(mshVert[i].id,modelPyramid[triId]); //定位当前顶点在三棱柱顶面顶点中的位置
				vertIds[0] = VertIdMap[modelPyramid[triId].vec[0][local_id]]; //依次定位三个顶点在数组中的位置
				vertIds[1] = VertIdMap[modelPyramid[triId].vec[0][(local_id+1)%3]];
				vertIds[2] = VertIdMap[modelPyramid[triId].vec[0][(local_id+2)%3]];
#pragma omp parallel for private(p,R,r_j23,r_ijk,L_j23,L_ijk,beta,beta_rj,alpha,alpha_rj,wf,wf_rj,face_temp, \
			re,a_rj,e_rj,Le,Le_rj,edge_temp,r_j23_side,face_tensor,edge_tensor) schedule(guided)
				for (p = 0; p < obsVert.size(); p++) //循环所有观测点
				{
					//确定旋转矩阵
					R.x = sin((0.5-obsVert[p].posis.lat/180)*Pi)*cos((2+obsVert[p].posis.lon/180)*Pi);
					R.y = sin((0.5-obsVert[p].posis.lat/180)*Pi)*sin((2+obsVert[p].posis.lon/180)*Pi);
					R.z = cos((0.5-obsVert[p].posis.lat/180)*Pi);

					//先处理顶面*******************************************
					r_j23[0] = modelVert[vertIds[0]].posic;
					r_j23[1] = modelVert[vertIds[1]].posic;
					r_j23[2] = modelVert[vertIds[2]].posic;
					//确定观测点到三个顶点的矢量
					r_ijk[0] = r_j23[0] - obsVert[p].posic; //直接取r_ijk[0]
					r_ijk[1] = r_j23[1] - obsVert[p].posic;
					r_ijk[2] = r_j23[2] - obsVert[p].posic;
					//计算对应的矢量长度
					L_j23 = df.lenCpoint(r_j23[0]);
					L_ijk[0] = df.lenCpoint(r_ijk[0]);
					L_ijk[1] = df.lenCpoint(r_ijk[1]);
					L_ijk[2] = df.lenCpoint(r_ijk[2]);
					//计算wf和wf相对于rj的偏导数
					beta = df.dot(r_ijk[0],df.cross(r_ijk[1],r_ijk[2]));
					beta_rj = df.dot(r_j23[0],df.cross(r_ijk[1], r_ijk[2]))/L_j23;

					alpha = L_ijk[0]*L_ijk[1]*L_ijk[2] + L_ijk[0]*df.dot(r_ijk[1],r_ijk[2]) + 
							L_ijk[1]*df.dot(r_ijk[2],r_ijk[0]) + L_ijk[2]*df.dot(r_ijk[0],r_ijk[1]);

					alpha_rj = (L_ijk[1]*L_ijk[2] + df.dot(r_ijk[1],r_ijk[2]))*df.dot(r_j23[0],r_ijk[0])/(L_j23*L_ijk[0]) + 
							df.dot(L_ijk[1]*r_ijk[2] + L_ijk[2]*r_ijk[1],r_j23[0])/L_j23;

					wf = 2*atan2(beta, alpha);
					wf_rj = 2*alpha*(beta_rj-beta*alpha_rj/alpha)/(alpha*alpha+beta*beta);

					//计算面与边的乘积
					face_temp = (wf*df.dot(modelPyramid[triId].F[0],r_ijk[0])*modelPyramid[triId].nf_rj[local_id] + 
						wf*df.dot(modelPyramid[triId].nf_rj[local_id],r_ijk[0])*modelPyramid[triId].F[0] + 
						wf*df.dot(modelPyramid[triId].F[0],df.normal(r_j23[0]))*modelPyramid[triId].F[0] + 
						wf_rj*df.dot(modelPyramid[triId].F[0],r_ijk[0])*modelPyramid[triId].F[0]);

					//观测点到右侧边
					re = r_j23[0] - obsVert[p].posic;
					//计算a_rj,e_rj
					a_rj = df.dot(r_j23[0],r_ijk[0])/(L_ijk[0]*L_j23);
					e_rj = df.dot(r_j23[0],r_j23[0] - r_j23[1])/(modelPyramid[triId].edgelen[local_id]*L_j23);

					Le = log((L_ijk[0]+L_ijk[1]+modelPyramid[triId].edgelen[local_id])/
						(L_ijk[0]+L_ijk[1]-modelPyramid[triId].edgelen[local_id]));

					Le_rj = (a_rj + e_rj)/(L_ijk[0]+L_ijk[1]+modelPyramid[triId].edgelen[local_id]) - 
							(a_rj - e_rj)/(L_ijk[0]+L_ijk[1]-modelPyramid[triId].edgelen[local_id]);

					edge_temp = 
						(Le*df.dot(modelPyramid[triId].E[local_id],re)*modelPyramid[triId].nf_rj[local_id] + 
						Le*df.dot(modelPyramid[triId].nj2_rj[local_id],re)*modelPyramid[triId].F[0] + 
						Le*df.dot(modelPyramid[triId].E[local_id],df.normal(r_j23[0]))*modelPyramid[triId].F[0] + 
						Le_rj*df.dot(modelPyramid[triId].E[local_id],re)*modelPyramid[triId].F[0] + 
						Le*df.dot(modelPyramid[triId].n2j_rj[local_id],re)*modelPyramid[triId].F[local_id+1] + 
						Le*df.dot(modelPyramid[triId].E[local_id*3+5],df.normal(r_j23[0]))*modelPyramid[triId].F[local_id+1] + 
						Le_rj*df.dot(modelPyramid[triId].E[local_id*3+5],re)*modelPyramid[triId].F[local_id+1]);

					//观测点到左侧边
					//计算a_rj,e_rj
					a_rj = df.dot(r_j23[0],r_ijk[0])/(L_ijk[0]*L_j23);
					e_rj = df.dot(r_j23[0],r_j23[0] - r_j23[2])/(modelPyramid[triId].edgelen[(local_id+2)%3]*L_j23);

					Le = log((L_ijk[0]+L_ijk[2]+modelPyramid[triId].edgelen[(local_id+2)%3])/
						(L_ijk[0]+L_ijk[2]-modelPyramid[triId].edgelen[(local_id+2)%3]));

					Le_rj = (a_rj + e_rj)/(L_ijk[0]+L_ijk[2]+modelPyramid[triId].edgelen[(local_id+2)%3]) - 
							(a_rj - e_rj)/(L_ijk[0]+L_ijk[2]-modelPyramid[triId].edgelen[(local_id+2)%3]);
					
					edge_temp = edge_temp + 
						(Le*df.dot(modelPyramid[triId].E[(local_id+2)%3],re)*modelPyramid[triId].nf_rj[local_id] + 
						Le*df.dot(modelPyramid[triId].n3j_rj[local_id],re)*modelPyramid[triId].F[0] + 
						Le*df.dot(modelPyramid[triId].E[(local_id+2)%3],df.normal(r_j23[0]))*modelPyramid[triId].F[0] + 
						Le_rj*df.dot(modelPyramid[triId].E[(local_id+2)%3],re)*modelPyramid[triId].F[0] + 
						Le*df.dot(modelPyramid[triId].nj3_rj[local_id],re)*modelPyramid[triId].F[(local_id+2)%3+1] + 
						Le*df.dot(modelPyramid[triId].E[((local_id+2)%3)*3+5],df.normal(r_j23[0]))*modelPyramid[triId].F[(local_id+2)%3+1] + 
						Le_rj*df.dot(modelPyramid[triId].E[((local_id+2)%3)*3+5],re)*modelPyramid[triId].F[(local_id+2)%3+1]);

					//观测点到后侧
					re = df.midCpoint(r_j23[1],r_j23[2]) - obsVert[p].posic;

					Le = log((L_ijk[1]+L_ijk[2]+modelPyramid[triId].edgelen[(local_id+1)%3])/
						(L_ijk[1]+L_ijk[2]-modelPyramid[triId].edgelen[(local_id+1)%3]));

					edge_temp = edge_temp +
						(Le*df.dot(modelPyramid[triId].E[(local_id+1)%3],re)*modelPyramid[triId].nf_rj[local_id] + 
						Le*df.dot(modelPyramid[triId].n23_rj[local_id],re)*modelPyramid[triId].F[0]);
					
					//转换r_j23为顶点右边侧面的情况*******************************************
					r_j23_side[0] = r_j23[0];
					r_j23_side[1].x = 0.0; r_j23_side[1].y = 0.0; r_j23_side[1].z = 0.0;
					r_j23_side[2] = r_j23[1];
					//确定观测点到三个顶点的矢量
					r_ijk[0] = r_j23_side[0] - obsVert[p].posic; //直接取r_ijk[0]为rf
					r_ijk[1] = r_j23_side[1] - obsVert[p].posic;
					r_ijk[2] = r_j23_side[2] - obsVert[p].posic;
					//计算对应的矢量长度
					L_j23 = df.lenCpoint(r_j23_side[0]);
					L_ijk[0] = df.lenCpoint(r_ijk[0]);
					L_ijk[1] = df.lenCpoint(r_ijk[1]);
					L_ijk[2] = df.lenCpoint(r_ijk[2]);
					//计算wf和wf相对于rj的偏导数
					beta = df.dot(r_ijk[0],df.cross(r_ijk[1],r_ijk[2]));
					beta_rj = df.dot(r_j23_side[0],df.cross(r_ijk[1], r_ijk[2]))/L_j23;

					alpha = L_ijk[0]*L_ijk[1]*L_ijk[2] + L_ijk[0]*df.dot(r_ijk[1],r_ijk[2]) + 
					L_ijk[1]*df.dot(r_ijk[2],r_ijk[0]) + L_ijk[2]*df.dot(r_ijk[0],r_ijk[1]);

					alpha_rj = (L_ijk[1]*L_ijk[2] + df.dot(r_ijk[1],r_ijk[2]))*df.dot(r_j23_side[0],r_ijk[0])/(L_j23*L_ijk[0]) + 
					df.dot(L_ijk[1]*r_ijk[2] + L_ijk[2]*r_ijk[1],r_j23_side[0])/L_j23;

					wf = 2*atan2(beta, alpha);
					wf_rj = 2*alpha*(beta_rj-beta*alpha_rj/alpha)/(alpha*alpha+beta*beta);
					//累加face_temp
					face_temp = face_temp + 
						(wf*df.dot(modelPyramid[triId].F[local_id+1],r_j23_side[0])*modelPyramid[triId].F[local_id+1] + 
						wf_rj*df.dot(modelPyramid[triId].F[local_id+1],r_ijk[0])*modelPyramid[triId].F[local_id+1]);

					//侧棱
					re = r_j23_side[0] - obsVert[p].posic;
					//计算a_rj,e_rj
					a_rj = df.dot(r_j23_side[0],r_ijk[0])/(L_ijk[0]*L_j23);
					e_rj = df.dot(r_j23_side[0],r_j23_side[0] - r_j23_side[1])/(modelPyramid[triId].edgelen[(local_id+1)*3]*L_j23);;

					Le = log((L_ijk[0]+L_ijk[1]+modelPyramid[triId].edgelen[(local_id+1)*3])/
						(L_ijk[0]+L_ijk[1]-modelPyramid[triId].edgelen[(local_id+1)*3]));

					Le_rj = (a_rj + e_rj)/(L_ijk[0]+L_ijk[1]+modelPyramid[triId].edgelen[(local_id+1)*3]) 
							- (a_rj - e_rj)/(L_ijk[0]+L_ijk[1]-modelPyramid[triId].edgelen[(local_id+1)*3]);

					edge_temp = edge_temp + 
						(Le*df.dot(modelPyramid[triId].E[(local_id+1)*3],r_j23_side[0])*modelPyramid[triId].F[local_id+1] + 
						Le_rj*df.dot(modelPyramid[triId].E[(local_id+1)*3],re)*modelPyramid[triId].F[local_id+1] + 
						Le*df.dot(modelPyramid[triId].E[3*((local_id+2)%3+1)+1],r_j23_side[0])*modelPyramid[triId].F[(local_id+2)%3+1] + 
						Le_rj*df.dot(modelPyramid[triId].E[3*((local_id+2)%3+1)+1],re)*modelPyramid[triId].F[(local_id+2)%3+1]);

					//转换r_j23为顶点左边侧面的情况*******************************************
					r_j23_side[0] = r_j23[0];
					r_j23_side[1] = r_j23[2];
					r_j23_side[2].x = 0.0; r_j23_side[2].y = 0.0; r_j23_side[2].z = 0.0;
					//确定观测点到三个顶点的矢量
					r_ijk[0] = r_j23_side[0] - obsVert[p].posic; //直接取r_ijk[0]为rf
					r_ijk[1] = r_j23_side[1] - obsVert[p].posic;
					r_ijk[2] = r_j23_side[2] - obsVert[p].posic;
					//计算对应的矢量长度
					L_j23 = df.lenCpoint(r_j23_side[0]);
					L_ijk[0] = df.lenCpoint(r_ijk[0]);
					L_ijk[1] = df.lenCpoint(r_ijk[1]);
					L_ijk[2] = df.lenCpoint(r_ijk[2]);
					//计算wf和wf相对于rj的偏导数
					beta = df.dot(r_ijk[0],df.cross(r_ijk[1],r_ijk[2]));
					beta_rj = df.dot(r_j23_side[0],df.cross(r_ijk[1], r_ijk[2]))/L_j23;

					alpha = L_ijk[0]*L_ijk[1]*L_ijk[2] + L_ijk[0]*df.dot(r_ijk[1],r_ijk[2]) + 
					L_ijk[1]*df.dot(r_ijk[2],r_ijk[0]) + L_ijk[2]*df.dot(r_ijk[0],r_ijk[1]);

					alpha_rj = (L_ijk[1]*L_ijk[2] + df.dot(r_ijk[1],r_ijk[2]))*df.dot(r_j23_side[0],r_ijk[0])/(L_j23*L_ijk[0]) + 
					df.dot(L_ijk[1]*r_ijk[2] + L_ijk[2]*r_ijk[1],r_j23_side[0])/L_j23;

					wf = 2*atan2(beta, alpha);
					wf_rj = 2*alpha*(beta_rj-beta*alpha_rj/alpha)/(alpha*alpha+beta*beta);
					//累加face_temp
					face_temp = face_temp + 
					(wf*df.dot(modelPyramid[triId].F[(local_id+2)%3+1],r_j23_side[0])*modelPyramid[triId].F[(local_id+2)%3+1] + 
					wf_rj*df.dot(modelPyramid[triId].F[(local_id+2)%3+1],r_ijk[0])*modelPyramid[triId].F[(local_id+2)%3+1]);

					face_add[p] = modelPyramid[triId].rho*df.dot(face_temp,R);
					edge_add[p] = modelPyramid[triId].rho*df.dot(edge_temp,R);
				}

				for (int h = 0; h < obsVert.size(); h++)
				{
					point_sum += 2.0*G0*(edge_add[h]-face_add[h])*inputDiff[h]/obsVert.size();
				}
			}
			tempGrad[i] = point_sum;
		}
	}
	else if (!strcmp(calType,"gradient"))
	{
		for (int i = 0; i < mshVert.size(); i++)
		{
			bar->Progressed(i);
			point_sum = 0.0;
			for (int j = 0; j < vertNeigh[i].tri.size(); j++) //顺序处理所有以当前顶点为中心的三棱锥
			{
				triId = pyraIdMap[vertNeigh[i].tri[j]]; //定位三棱锥在数组中的位置
				local_id = local_index(mshVert[i].id,modelPyramid[triId]); //定位当前顶点在三棱柱顶面顶点中的位置
				vertIds[0] = VertIdMap[modelPyramid[triId].vec[0][local_id]]; //依次定位三个顶点在数组中的位置
				vertIds[1] = VertIdMap[modelPyramid[triId].vec[0][(local_id+1)%3]];
				vertIds[2] = VertIdMap[modelPyramid[triId].vec[0][(local_id+2)%3]];
#pragma omp parallel for private(p,R,r_j23,r_ijk,L_j23,L_ijk,beta,beta_rj,alpha,alpha_rj,wf,wf_rj,face_temp, \
			a_rj,e_rj,Le,Le_rj,edge_temp,r_j23_side) schedule(guided)
				for (p = 0; p < obsVert.size(); p++) //循环所有观测点
				{
					//确定旋转矩阵
					R.x = sin((0.5-obsVert[p].posis.lat/180)*Pi)*cos((2+obsVert[p].posis.lon/180)*Pi);
					R.y = sin((0.5-obsVert[p].posis.lat/180)*Pi)*sin((2+obsVert[p].posis.lon/180)*Pi);
					R.z = cos((0.5-obsVert[p].posis.lat/180)*Pi);

					//先处理顶面*******************************************
					r_j23[0] = modelVert[vertIds[0]].posic;
					r_j23[1] = modelVert[vertIds[1]].posic;
					r_j23[2] = modelVert[vertIds[2]].posic;
					//确定观测点到三个顶点的矢量
					r_ijk[0] = r_j23[0] - obsVert[p].posic; //直接取r_ijk[0]
					r_ijk[1] = r_j23[1] - obsVert[p].posic;
					r_ijk[2] = r_j23[2] - obsVert[p].posic;
					//计算对应的矢量长度
					L_j23 = df.lenCpoint(r_j23[0]);
					L_ijk[0] = df.lenCpoint(r_ijk[0]);
					L_ijk[1] = df.lenCpoint(r_ijk[1]);
					L_ijk[2] = df.lenCpoint(r_ijk[2]);
					//计算wf和wf相对于rj的偏导数
					beta = df.dot(r_ijk[0],df.cross(r_ijk[1],r_ijk[2]));
					beta_rj = df.dot(r_j23[0],df.cross(r_ijk[1], r_ijk[2]))/L_j23;

					alpha = L_ijk[0]*L_ijk[1]*L_ijk[2] + L_ijk[0]*df.dot(r_ijk[1],r_ijk[2]) + 
							L_ijk[1]*df.dot(r_ijk[2],r_ijk[0]) + L_ijk[2]*df.dot(r_ijk[0],r_ijk[1]);

					alpha_rj = (L_ijk[1]*L_ijk[2] + df.dot(r_ijk[1],r_ijk[2]))*df.dot(r_j23[0],r_ijk[0])/(L_j23*L_ijk[0]) + 
							df.dot(L_ijk[1]*r_ijk[2] + L_ijk[2]*r_ijk[1],r_j23[0])/L_j23;

					wf = 2*atan2(beta, alpha);
					wf_rj = 2*alpha*(beta_rj-beta*alpha_rj/alpha)/(alpha*alpha+beta*beta);

					//计算面与边的乘积
					face_temp = (wf*df.dot(modelPyramid[triId].F[0],R)*modelPyramid[triId].nf_rj[local_id] + 
						wf*df.dot(modelPyramid[triId].nf_rj[local_id],R)*modelPyramid[triId].F[0] + 
						wf_rj*df.dot(modelPyramid[triId].F[0],R)*modelPyramid[triId].F[0]);

					//观测点到右侧边
					//计算a_rj,e_rj
					a_rj = df.dot(r_j23[0],r_ijk[0])/(L_ijk[0]*L_j23);
					e_rj = df.dot(r_j23[0],r_j23[0] - r_j23[1])/(modelPyramid[triId].edgelen[local_id]*L_j23);

					Le = log((L_ijk[0]+L_ijk[1]+modelPyramid[triId].edgelen[local_id])/
						(L_ijk[0]+L_ijk[1]-modelPyramid[triId].edgelen[local_id]));

					Le_rj = (a_rj + e_rj)/(L_ijk[0]+L_ijk[1]+modelPyramid[triId].edgelen[local_id]) - 
							(a_rj - e_rj)/(L_ijk[0]+L_ijk[1]-modelPyramid[triId].edgelen[local_id]);

					edge_temp = 
						(Le*df.dot(modelPyramid[triId].E[local_id],R)*modelPyramid[triId].nf_rj[local_id] + 
						Le*df.dot(modelPyramid[triId].nj2_rj[local_id],R)*modelPyramid[triId].F[0] + 
						Le_rj*df.dot(modelPyramid[triId].E[local_id],R)*modelPyramid[triId].F[0] + 
						Le*df.dot(modelPyramid[triId].n2j_rj[local_id],R)*modelPyramid[triId].F[local_id+1] + 
						Le_rj*df.dot(modelPyramid[triId].E[local_id*3+5],R)*modelPyramid[triId].F[local_id+1]);

					//观测点到左侧边
					//计算a_rj,e_rj
					a_rj = df.dot(r_j23[0],r_ijk[0])/(L_ijk[0]*L_j23);
					e_rj = df.dot(r_j23[0],r_j23[0] - r_j23[2])/(modelPyramid[triId].edgelen[(local_id+2)%3]*L_j23);

					Le = log((L_ijk[0]+L_ijk[2]+modelPyramid[triId].edgelen[(local_id+2)%3])/
						(L_ijk[0]+L_ijk[2]-modelPyramid[triId].edgelen[(local_id+2)%3]));

					Le_rj = (a_rj + e_rj)/(L_ijk[0]+L_ijk[2]+modelPyramid[triId].edgelen[(local_id+2)%3]) - 
							(a_rj - e_rj)/(L_ijk[0]+L_ijk[2]-modelPyramid[triId].edgelen[(local_id+2)%3]);

					edge_temp = edge_temp + 
						(Le*df.dot(modelPyramid[triId].E[(local_id+2)%3],R)*modelPyramid[triId].nf_rj[local_id] + 
						Le*df.dot(modelPyramid[triId].n3j_rj[local_id],R)*modelPyramid[triId].F[0] + 
						Le_rj*df.dot(modelPyramid[triId].E[(local_id+2)%3],R)*modelPyramid[triId].F[0] + 
						Le*df.dot(modelPyramid[triId].nj3_rj[local_id],R)*modelPyramid[triId].F[(local_id+2)%3+1] + 
						Le_rj*df.dot(modelPyramid[triId].E[((local_id+2)%3)*3+5],R)*modelPyramid[triId].F[(local_id+2)%3+1]);

					//观测点到背侧
					Le = log((L_ijk[1]+L_ijk[2]+modelPyramid[triId].edgelen[(local_id+1)%3])/
						(L_ijk[1]+L_ijk[2]-modelPyramid[triId].edgelen[(local_id+1)%3]));

					edge_temp = edge_temp +
						(Le*df.dot(modelPyramid[triId].E[(local_id+1)%3],R)*modelPyramid[triId].nf_rj[local_id] + 
						Le*df.dot(modelPyramid[triId].n23_rj[local_id],R)*modelPyramid[triId].F[0]);
					
					//转换r_j23为顶点右边侧面的情况*******************************************
					r_j23_side[0] = r_j23[0];
					r_j23_side[1].x = 0.0; r_j23_side[1].y = 0.0; r_j23_side[1].z = 0.0;
					r_j23_side[2] = r_j23[1];
					//确定观测点到三个顶点的矢量
					r_ijk[0] = r_j23_side[0] - obsVert[p].posic; //直接取r_ijk[0]为rf
					r_ijk[1] = r_j23_side[1] - obsVert[p].posic;
					r_ijk[2] = r_j23_side[2] - obsVert[p].posic;
					//计算对应的矢量长度
					L_j23 = df.lenCpoint(r_j23_side[0]);
					L_ijk[0] = df.lenCpoint(r_ijk[0]);
					L_ijk[1] = df.lenCpoint(r_ijk[1]);
					L_ijk[2] = df.lenCpoint(r_ijk[2]);
					//计算wf和wf相对于rj的偏导数
					beta = df.dot(r_ijk[0],df.cross(r_ijk[1],r_ijk[2]));
					beta_rj = df.dot(r_j23_side[0],df.cross(r_ijk[1], r_ijk[2]))/L_j23;

					alpha = L_ijk[0]*L_ijk[1]*L_ijk[2] + L_ijk[0]*df.dot(r_ijk[1],r_ijk[2]) + 
					L_ijk[1]*df.dot(r_ijk[2],r_ijk[0]) + L_ijk[2]*df.dot(r_ijk[0],r_ijk[1]);

					alpha_rj = (L_ijk[1]*L_ijk[2] + df.dot(r_ijk[1],r_ijk[2]))*df.dot(r_j23_side[0],r_ijk[0])/(L_j23*L_ijk[0]) + 
					df.dot(L_ijk[1]*r_ijk[2] + L_ijk[2]*r_ijk[1],r_j23_side[0])/L_j23;

					wf_rj = 2*alpha*(beta_rj-beta*alpha_rj/alpha)/(alpha*alpha+beta*beta);
					//累加face_temp
					face_temp = face_temp + 
						wf_rj*df.dot(modelPyramid[triId].F[local_id+1],R)*modelPyramid[triId].F[local_id+1];

					//侧棱
					//计算a_rj,e_rj
					a_rj = df.dot(r_j23_side[0],r_ijk[0])/(L_ijk[0]*L_j23);
					e_rj = df.dot(r_j23_side[0],r_j23_side[0] - r_j23_side[1])/(modelPyramid[triId].edgelen[(local_id+1)*3]*L_j23);

					Le_rj = (a_rj + e_rj)/(L_ijk[0]+L_ijk[1]+modelPyramid[triId].edgelen[(local_id+1)*3]) - 
							(a_rj - e_rj)/(L_ijk[0]+L_ijk[1]-modelPyramid[triId].edgelen[(local_id+1)*3]);

					edge_temp = edge_temp + 
						(Le_rj*df.dot(modelPyramid[triId].E[(local_id+1)*3],R)*modelPyramid[triId].F[local_id+1] + 
						Le_rj*df.dot(modelPyramid[triId].E[3*((local_id+2)%3+1)+1],R)*modelPyramid[triId].F[(local_id+2)%3+1]);

					//转换r_j23为顶点左边侧面的情况*******************************************
					r_j23_side[0] = r_j23[0];
					r_j23_side[1] = r_j23[2];
					r_j23_side[2].x = 0.0; r_j23_side[2].y = 0.0; r_j23_side[2].z = 0.0;
					//确定观测点到三个顶点的矢量
					r_ijk[0] = r_j23_side[0] - obsVert[p].posic; //直接取r_ijk[0]为rf
					r_ijk[1] = r_j23_side[1] - obsVert[p].posic;
					r_ijk[2] = r_j23_side[2] - obsVert[p].posic;
					//计算对应的矢量长度
					L_j23 = df.lenCpoint(r_j23_side[0]);
					L_ijk[0] = df.lenCpoint(r_ijk[0]);
					L_ijk[1] = df.lenCpoint(r_ijk[1]);
					L_ijk[2] = df.lenCpoint(r_ijk[2]);
					//计算wf和wf相对于rj的偏导数
					beta = df.dot(r_ijk[0],df.cross(r_ijk[1],r_ijk[2]));
					beta_rj = df.dot(r_j23_side[0],df.cross(r_ijk[1], r_ijk[2]))/L_j23;

					alpha = L_ijk[0]*L_ijk[1]*L_ijk[2] + L_ijk[0]*df.dot(r_ijk[1],r_ijk[2]) + 
					L_ijk[1]*df.dot(r_ijk[2],r_ijk[0]) + L_ijk[2]*df.dot(r_ijk[0],r_ijk[1]);

					alpha_rj = (L_ijk[1]*L_ijk[2] + df.dot(r_ijk[1],r_ijk[2]))*df.dot(r_j23_side[0],r_ijk[0])/(L_j23*L_ijk[0]) + 
					df.dot(L_ijk[1]*r_ijk[2] + L_ijk[2]*r_ijk[1],r_j23_side[0])/L_j23;

					wf_rj = 2*alpha*(beta_rj-beta*alpha_rj/alpha)/(alpha*alpha+beta*beta);
					//累加face_temp
					face_temp = face_temp + 
						wf_rj*df.dot(modelPyramid[triId].F[(local_id+2)%3+1],R)*modelPyramid[triId].F[(local_id+2)%3+1];

					face_add[p] = modelPyramid[triId].rho*df.dot(face_temp,R);
					edge_add[p] = modelPyramid[triId].rho*df.dot(edge_temp,R);
				}

				for (int h = 0; h < obsVert.size(); h++)
				{
					point_sum += 2.0e+4*G0*(edge_add[h]-face_add[h])*inputDiff[h]/obsVert.size();
				}
			}
			tempGrad[i] = point_sum;
		}
	}
	return tempGrad;
}

//以下是反演函数
int cgdy::initInversionSetup()
{
	//初始化数组
	invertTopo.resize(mshVert.size()); //反演面高程
	invertTopo_temp.resize(mshVert.size());
	gradRad.resize(mshVert.size());
	gradRad_temp.resize(mshVert.size());
	dk.resize(mshVert.size());
	objectDiff.resize(obsVert.size());
	objectDiff_temp.resize(obsVert.size());
	//初始化invertTopo为initTopo
	for (int i = 0; i < mshVert.size(); i++)
	{
		invertTopo[i] = initTopo[i];
	}
	return 0;
}

void cgdy::boundToBox(_1dArray& inputVal) //对范围进行约束
{
	// box constrains
	for (int i = 0; i < mshVert.size(); i++)
	{
		if (inputVal[i] > upperTopo[i])
		{
			inputVal[i] = upperTopo[i];
		}
		else if (inputVal[i] < bottomTopo[i])
		{
			inputVal[i] = bottomTopo[i];
		}
	}
}

//反演函数
int cgdy::linearSearch_Wolfe()
{
	double gTdk,g2Tdk,df;
	double local_ak;
	//计算gTdk
	gTdk = 0.0;
	for (int i = 0; i < mshVert.size(); i++)
		gTdk += gradRad[i]*dk[i];

	for (int i = 0; i < mshVert.size(); i++)
		invertTopo_temp[i] = invertTopo[i] + ak*dk[i];
	boundToBox(invertTopo_temp);
	changeModelSetupTo(invertTopo_temp);
	objectDiff_temp = forwardDiff(objectF_temp,datatype);
	gradRad_temp = forwardRadGrad(objectDiff_temp,datatype);

	gradModule = 0.0;
	for (int i = 0; i < mshVert.size(); i++)
		gradModule += gradRad_temp[i]*gradRad_temp[i];

	if (gradModule < 1e-30)
	{
		cout << "A local/global minimum is reached "<< endl;
		return -1;
	}

	g2Tdk = 0.0;
	for (int i = 0; i < mshVert.size(); i++)
		g2Tdk += gradRad_temp[i]*dk[i];

	df = objectF_temp - objectF;

	while(df > c1*ak*gTdk || g2Tdk <= c2*gTdk)
	{
		cout<< ak << " " << df << ">" << c1*ak*gTdk << " " << g2Tdk << "<=" << c2*gTdk << endl;
		if(debugModel)
		{
			cout << "reset ak (input to 0 to perform inexcat line search) = ";
			cin >> local_ak;
			if (local_ak != 0)
			{
				ak = local_ak;
			}
			else
			{
				if(df < c1*ak*gTdk && fabs(g2Tdk) > fabs(c2*gTdk) && g2Tdk < 0) ak = 2*ak;
				else if(df < c1*ak*gTdk && fabs(g2Tdk) > fabs(c2*gTdk) && g2Tdk > 0) ak = 0.5*ak;
				else ak = 0.5*ak/(1-df/(ak*gTdk)); //单纯切线下降搜索
				cout << "ak = " << ak << endl;
			}
		}
		else
		{
			if(df < c1*ak*gTdk && fabs(g2Tdk) > fabs(c2*gTdk) && g2Tdk < 0) ak = 2*ak;
			else if(df < c1*ak*gTdk && fabs(g2Tdk) > fabs(c2*gTdk) && g2Tdk > 0) ak = 0.5*ak;
			else ak = 0.5*ak/(1-df/(ak*gTdk));
			cout << "ak = " << ak << endl;
		}

		for (int i = 0; i < mshVert.size(); i++)
			invertTopo_temp[i] = invertTopo[i] + ak*dk[i];
		boundToBox(invertTopo_temp);
		changeModelSetupTo(invertTopo_temp);
		objectDiff_temp = forwardDiff(objectF_temp,datatype);
		gradRad_temp = forwardRadGrad(objectDiff_temp,datatype);

		gradModule = 0.0;
		for (int i = 0; i < mshVert.size(); i++)
			gradModule += gradRad_temp[i]*gradRad_temp[i];

		if (gradModule < 1e-30)
		{
			cout << "A local/global minimum is reached "<< endl;
			return -1;
		}

		g2Tdk = 0.0;
		for (int i = 0; i < mshVert.size(); i++)
			g2Tdk += gradRad_temp[i]*dk[i];

		df = objectF_temp - objectF;

		if (fabs(ak)<=1e-30)
		{
			cout << "current ak is too small, break out"<<endl;
			return -1;
		}
	}
	return 0;
}

int cgdy::linearSearch_strongWolfe()
{
	double gTdk,g2Tdk,df;
	double local_ak;
	//计算gTdk
	gTdk = 0.0;
	for (int i = 0; i < mshVert.size(); i++)
		gTdk += gradRad[i]*dk[i];

	//将反演值赋值给临时值
	for (int i = 0; i < mshVert.size(); i++)
		invertTopo_temp[i] = invertTopo[i] + ak*dk[i];
	boundToBox(invertTopo_temp);
	changeModelSetupTo(invertTopo_temp);
	objectDiff_temp = forwardDiff(objectF_temp,datatype);
	gradRad_temp = forwardRadGrad(objectDiff_temp,datatype);

	gradModule = 0.0;
	for (int i = 0; i < mshVert.size(); i++)
		gradModule += gradRad_temp[i]*gradRad_temp[i];

	if (gradModule < 1e-30)
	{
		cout << "A local/global minimum is reached "<< endl;
		return -1;
	}

	g2Tdk = 0.0;
	for (int i = 0; i < mshVert.size(); i++)
		g2Tdk += gradRad_temp[i]*dk[i];

	df = objectF_temp - objectF;

	while(df > c1*ak*gTdk || fabs(g2Tdk) > fabs(c2*gTdk))
	{
		cout<< ak << " " << df << ">" << c1*ak*gTdk << " " << fabs(g2Tdk) << ">" << fabs(c2*gTdk) << endl;
		if(debugModel)
		{
			cout << "reset ak (input to 0 to perform inexcat line search) = ";
			cin >> local_ak;
			if (local_ak != 0)
			{
				ak = local_ak;
			}
			else
			{
				if(df < c1*ak*gTdk && fabs(g2Tdk) > fabs(c2*gTdk) && g2Tdk < 0) ak = 2*ak;
				else if(df < c1*ak*gTdk && fabs(g2Tdk) > fabs(c2*gTdk) && g2Tdk > 0) ak = 0.5*ak;
				else ak = 0.5*ak/(1-df/(ak*gTdk)); //单纯切线下降搜索
				cout << "ak = " << ak << endl;
			}
		}
		else
		{
			if(df < c1*ak*gTdk && fabs(g2Tdk) > fabs(c2*gTdk) && g2Tdk < 0) ak = 2*ak;
			else if(df < c1*ak*gTdk && fabs(g2Tdk) > fabs(c2*gTdk) && g2Tdk > 0) ak = 0.5*ak;
			else ak = 0.5*ak/(1-df/(ak*gTdk));
			cout << "ak = " << ak << endl;
		}

		for (int i = 0; i < mshVert.size(); i++)
			invertTopo_temp[i] = invertTopo[i] + ak*dk[i];
		boundToBox(invertTopo_temp);
		changeModelSetupTo(invertTopo_temp);
		objectDiff_temp = forwardDiff(objectF_temp,datatype);
		gradRad_temp = forwardRadGrad(objectDiff_temp,datatype);

		gradModule = 0.0;
		for (int i = 0; i < mshVert.size(); i++)
			gradModule += gradRad_temp[i]*gradRad_temp[i];

		if (gradModule < 1e-30)
		{
			cout << "A local/global minimum is reached "<< endl;
			return -1;
		}

		g2Tdk = 0.0;
		for (int i = 0; i < mshVert.size(); i++)
			g2Tdk += gradRad_temp[i]*dk[i];

		df = objectF_temp - objectF;

		if (fabs(ak)<=1e-30)
		{
			cout << "current ak is too small, break out"<<endl;
			return -1;
		}
	}
	return 0;
}

int cgdy::invertRuntine()
{
	boundToBox(invertTopo);
	changeModelSetupTo(invertTopo);
	objectDiff = forwardDiff(objectF,datatype);
	gradRad = forwardRadGrad(objectDiff,datatype);

	gradModule = 0.0;
	for (int i = 0; i < mshVert.size(); i++)
		gradModule += gradRad[i]*gradRad[i];

	if (gradModule < 1e-30)
	{
		cout << "A local/global minimum is reached "<< endl;
		return 0;
	}

	for (int i = 0; i < mshVert.size(); i++) //初始化下降方向
		dk[i] = -1.0*gradRad[i];

	//测试输出梯度文件
	outNodeData(dkFile,dkFile,dk);
	if(debugModel)
	{
		int confirmInversion;
		cout << "continue to inversion process (1 for yes & 0 for no) ==> ";
		cin >> confirmInversion;
		if(!confirmInversion)
		{
			cout << "program terminated!" << endl;
			return 0;
		}
	}

	int times = 0;
	while(objectF > 1)
	{
		cout << "iterator: " << times << " , ObjectFunc = " << objectF << " , gradientModule = " << gradModule << endl;

		if (use_strong)
		{
			if(linearSearch_strongWolfe())
			{
				for (int i = 0; i < mshVert.size(); i++)
				{
					invertTopo[i] = invertTopo_temp[i];
				}
				break;
			}
			else
			{
				for (int i = 0; i < mshVert.size(); i++)
				{
					invertTopo[i] = invertTopo_temp[i];
				}
			}
		}
		else
		{
			if(linearSearch_Wolfe())
			{
				for (int i = 0; i < mshVert.size(); i++)
				{
					invertTopo[i] = invertTopo_temp[i];
				}
				break;
			}
			else
			{
				for (int i = 0; i < mshVert.size(); i++)
				{
					invertTopo[i] = invertTopo_temp[i];
				}
			}
		}

		objectF = objectF_temp;
		dkyk = 0;
		for (int i = 0; i < mshVert.size(); i++)
			dkyk += dk[i]*(gradRad_temp[i]-gradRad[i]);
		betak_DY = gradModule/dkyk;
		gkyk = 0;
		for (int i = 0; i < mshVert.size(); i++)
			gkyk += gradRad_temp[i]*(gradRad_temp[i]-gradRad[i]);
		betak_HS = gkyk/dkyk;
		betak = 0>(betak_HS<betak_DY?betak_HS:betak_DY)?0:(betak_HS<betak_DY?betak_HS:betak_DY);

		for (int i = 0; i < mshVert.size(); i++)
		{
			dk[i] = -1.0*gradRad_temp[i] + betak*dk[i];
			gradRad[i] = gradRad_temp[i];
		}
		times++;
		//测试输出梯度文件
		outNodeData(dkFile,dkFile,dk);
		//输出一个中间反演结果
		outNodeData(tempFile,tempFile,invertTopo);
	}

	if (objectF <= 1)
	{
		cout << "Inversion Done!"<<endl<<"iterator: " << times << " , ObjectFunc = " << objectF << endl;
	}
	return 0;
}
#endif
#include "dii_stt.h"

/**
 * @brief      初始化参考椭球半径
 *
 * @param[in]  refsys_para  初始化参数
 */
void DII_STT::InitRefSystem(std::string refsys_para)
{
	// 赋初值（无效值）
	refr_ = refR_ = -1.0;

	//首先匹配预定义类型
	if (refsys_para == "WGS84")
	{
		refr_ = GCTL_WGS84_PoleRadius;
		refR_ = GCTL_WGS84_EquatorRadius;
	}
	else if (refsys_para == "EarthRadius")
	{
		refr_ = GCTL_Earth_Radius;
		refR_ = GCTL_Earth_Radius;
	}
	else if (refsys_para == MoonRadius)
	{
		refr_ = GCTL_Moon_Radius;
		refR_ = GCTL_Moon_Radius;
	}
	//匹配格式化参数
	else sscanf(refsys_para.c_str(),"%lf/%lf",&refr_, &refR_);

	if (refr_ <= 0 || refR_ <= 0)
	{
		throw "Invalid parameters for the reference system."
	}
	return;
}
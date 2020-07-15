/**
 * @defgroup   基于球面三角剖分的密度界面反演
 *
 * @brief      此类型定义了基于球面三角剖分的密度界面反演算法，适用于均匀或
 * 变密度的界面起伏反演。
 *
 * @author     Dr. Yi Zhang (yizhang-geo@zju.edu.cn)
 * @date       2020-07-15
 */

#ifndef _DII_STT_H
#define _DII_STT_H

#include "gctl/gctl_core.h"

#include "lbfgs.h"

#include "map"
#include "iterator"

#define RUN_MODULE    "run-type"
#define REF_SYSTEM    "ref-system"
#define MSH_FILE      "mesh-file"
#define OBS_FILE      "grav-file"

/**
 * @brief      顶点的邻居（包含某个顶点的三角形的索引向量）
 */
struct vert_neigh
{
	int id;
	std::vector<int> tri;
};

/**
 * @brief      三角锥（有一个顶点为原点的四面体）
 */
struct tri_cone
{
	int id; //索引值
	int phys; //物理组
	int vec[4][3]; //顶点索引
	double rho; //属性值 这里我们只需要一个密度值
	gctl::point3d_c E[12]; //边的外法线矢量 这里有12个是因为每条边链接两个面
	gctl::point3d_c F[4]; //面外法线单位矢量
	double edgelen[12]; //边的长度 这里有12个是因为每条边链接两个面
	//注意三棱锥的侧面与侧面的外法线矢量相对于半径的偏导数为0 所以nf_rj为顶面三角形相对于三个顶点半径的偏导数
	//三棱锥顶面的偏导数矢量
	gctl::point3d_c nf_rj[3]; //顶面的单位矢量相对于一个标量（顶点半径）的偏导数
	gctl::point3d_c nj2_rj[3]; //顶面三角形中与顶点相连的逆时针边及其侧面相对于半径的偏导数
	gctl::point3d_c n2j_rj[3]; //侧面上的偏导数 侧面外法线矢量的偏导数为0
	gctl::point3d_c n3j_rj[3];
	gctl::point3d_c nj3_rj[3];
	gctl::point3d_c n23_rj[3];
};

/**
 * @brief      给出一个顶点的索引和其所在三角形，循环确定该顶点在三角形顶点中排序的位置
 *
 * @param[in]  id    顶点的索引值
 * @param[in]  t     顶点所在的三角锥
 *
 * @return     索引在三角锥中的排序（索引）
 */
int local_index(int id, const tri_cone &t)
{
	for (int i = 0; i < 3; i++)
	{
		if (id == t.vec[0][i])
		{
			return i;
		}
	}
	return -1;
}

/**
 * @brief      计算一个矢量的单位矢量相对一个标量（顶点半径）的偏导数矢量，
 * 输入值分别为该矢量和其相对于这个标量的偏导数矢量
 *
 * @param[in]  n     适量
 * @param[in]  n_rj  偏导数矢量
 *
 * @return     单位矢量的偏导数矢量
 */
gctl::point3d_c nr_dr(const gctl::point3d_c &n, const gctl::point3d_c &n_rj)
{
	double moduel_n = n.module();
	double temp = gctl::geometry3d::dot(n, n_rj)/moduel_n;
	gctl::point3d_c nn = n.normal();

	gctl::point3d_c out;
	out.x = (n_rj.x - nn.x*temp)/moduel_n;
	out.y = (n_rj.y - nn.y*temp)/moduel_n;
	out.z = (n_rj.z - nn.z*temp)/moduel_n;

	return out;
}

class DII_STT
{
protected:
	gctl::array<gctl::vertex3d_c> msh_vert_; ///< msh文件顶点数组
	gctl::array<gctl::point3d_s> msh_vert_sph_; ///< msh文件顶点相应的球坐标值
	gctl::array<gctl::vertex3d_c> model_vert_; ///< 模型顶点数组 注意我们会在最后新增一个顶点（在坐标原点）
	std::vector<tri_cone> model_cone_; ///< 由模版得到的三棱锥数组
	gctl::array<vert_neigh> modVert_neigh_; ///< 顶点相邻关系

	int msh_vert_num_, model_vert_num_;
	int cone_num_;
	double refr_, refR_; ///< 参考椭球的短、长半径
public:
	DII_STT();
	~DII_STT();
	///< 初始化参考椭球半径
	void InitRefSystem(std::string refsys_para);
	///< 读入msh文件
	void ReadMshFile(std::string filename);
};

#endif //_DII_STT_H
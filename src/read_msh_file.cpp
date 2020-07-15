#include "dii_stt.h"

void DII_STT::ReadMshFile(std::string filename)
{
	int temp_int, ele_count, ele_type;
	int attri_num, temp_attri, temp_id;
	double temp_val;
	gctl::vertex3d_c temp_vert;
	tri_cone tmp_cone;
	string tmp_str;
	stringstream tmp_ss;

	std::map<int, int> i2i_map;
	std::map<int, int>::iterator iim;

	ifstream mshin = gctl::open_infile(filename, ".msh");

	while(getline(mshin,tmp_str))
	{
		if (tmp_str == "$Nodes")
		{
			getline(mshin, tmp_str);
			tmp_ss = gctl::str2ss(tmp_str);
			tmp_ss >> msh_vert_num_;
			// 初始化数组
			msh_vert_.resize(msh_vert_num_);
			msh_vert_sph_.resize(msh_vert_num_);

			for (int i = 0; i < msh_vert_num_; i++)
			{
				getline(mshin,tmp_str);
				tmp_ss = gctl::str2ss(tmp_str);
				tmp_ss >> msh_vert_[i].id >> msh_vert_[i].x 
					>> msh_vert_[i].y >> msh_vert_[i].z;
				// 计算球坐标值
				msh_vert_sph_[i] = msh_vert_[i].c2s();
				// 记录顶点索引与其在数组中的排序
				i2i_map[msh_vert_[i].id] = i;
			}
		}
		else if (tmp_str == "$Elements")
		{
			getline(mshin,tmp_str);
			tmp_ss = gctl::str2ss(tmp_str);
			tmp_ss >> temp_int;

			ele_count = 0;
			for (int i = 0; i < temp_int; i++)
			{
				getline(mshin,tmp_str);
				tmp_ss = gctl::str2ss(tmp_str);
				tmp_ss >> temp_id >> ele_type;
				if (ele_type == 2) //只读入三角形
				{
					tmp_cone.id = ele_count;
					// 读入属性值数量 至少要有一个属性值 （以后可在此处检查）
					tmp_ss >> attri_num;
					//第一个属性为物理组
					tmp_ss >> tmp_cone.phys;
					for (int a = 0; a < attri_num-1; a++) //忽略后面的属性值
						tmp_ss >> temp_attri;
					//读入三棱锥顶面三角形顶点索引
					tmp_ss >> tmp_cone.vec[0][0] >> tmp_cone.vec[0][1]
						>> tmp_cone.vec[0][2];
					// 映射顶点索引到数组内排序
					for (int j = 0; j < 3; j++)
					{
						tmp_cone.vec[0][j] = i2i_map[tmp_cone.vec[0][j]];
					}

					ele_count++;
					model_cone_.push_back(tmp_cone);
				}
			}
		}
		else continue;
	}
	mshin.close();
	return;
}
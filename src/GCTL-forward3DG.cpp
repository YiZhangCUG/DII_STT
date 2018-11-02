#include "GCTL-forward3DG.h"

double forward3DG::cube_G(cube cu,int valId,vertex ob,char* type)
{
	double R222,R122,R212,R112,R221,R121,R211,R111;
	double G222,G122,G212,G112,G221,G121,G211,G111;

	if (!strcmp(type,"G"))
	{
		R222=sqrt((cu.x2-ob.posic.x)*(cu.x2-ob.posic.x)+(cu.y2-ob.posic.y)*(cu.y2-ob.posic.y)+(cu.z2-ob.posic.z)*(cu.z2-ob.posic.z));
		R122=sqrt((cu.x1-ob.posic.x)*(cu.x1-ob.posic.x)+(cu.y2-ob.posic.y)*(cu.y2-ob.posic.y)+(cu.z2-ob.posic.z)*(cu.z2-ob.posic.z));
		R212=sqrt((cu.x2-ob.posic.x)*(cu.x2-ob.posic.x)+(cu.y1-ob.posic.y)*(cu.y1-ob.posic.y)+(cu.z2-ob.posic.z)*(cu.z2-ob.posic.z));
		R112=sqrt((cu.x1-ob.posic.x)*(cu.x1-ob.posic.x)+(cu.y1-ob.posic.y)*(cu.y1-ob.posic.y)+(cu.z2-ob.posic.z)*(cu.z2-ob.posic.z));
		R221=sqrt((cu.x2-ob.posic.x)*(cu.x2-ob.posic.x)+(cu.y2-ob.posic.y)*(cu.y2-ob.posic.y)+(cu.z1-ob.posic.z)*(cu.z1-ob.posic.z));
		R121=sqrt((cu.x1-ob.posic.x)*(cu.x1-ob.posic.x)+(cu.y2-ob.posic.y)*(cu.y2-ob.posic.y)+(cu.z1-ob.posic.z)*(cu.z1-ob.posic.z));
		R211=sqrt((cu.x2-ob.posic.x)*(cu.x2-ob.posic.x)+(cu.y1-ob.posic.y)*(cu.y1-ob.posic.y)+(cu.z1-ob.posic.z)*(cu.z1-ob.posic.z));
		R111=sqrt((cu.x1-ob.posic.x)*(cu.x1-ob.posic.x)+(cu.y1-ob.posic.y)*(cu.y1-ob.posic.y)+(cu.z1-ob.posic.z)*(cu.z1-ob.posic.z));

		G222=(cu.x2-ob.posic.x)*log((cu.y2-ob.posic.y)+R222)+(cu.y2-ob.posic.y)*log((cu.x2-ob.posic.x)+R222)+(cu.z2-ob.posic.z)*arctg((cu.z2-ob.posic.z)*R222/(cu.x2-ob.posic.x)/(cu.y2-ob.posic.y));
		G122=(cu.x1-ob.posic.x)*log((cu.y2-ob.posic.y)+R122)+(cu.y2-ob.posic.y)*log((cu.x1-ob.posic.x)+R122)+(cu.z2-ob.posic.z)*arctg((cu.z2-ob.posic.z)*R122/(cu.x1-ob.posic.x)/(cu.y2-ob.posic.y));
		G212=(cu.x2-ob.posic.x)*log((cu.y1-ob.posic.y)+R212)+(cu.y1-ob.posic.y)*log((cu.x2-ob.posic.x)+R212)+(cu.z2-ob.posic.z)*arctg((cu.z2-ob.posic.z)*R212/(cu.x2-ob.posic.x)/(cu.y1-ob.posic.y));
		G112=(cu.x1-ob.posic.x)*log((cu.y1-ob.posic.y)+R112)+(cu.y1-ob.posic.y)*log((cu.x1-ob.posic.x)+R112)+(cu.z2-ob.posic.z)*arctg((cu.z2-ob.posic.z)*R112/(cu.x1-ob.posic.x)/(cu.y1-ob.posic.y));
		G221=(cu.x2-ob.posic.x)*log((cu.y2-ob.posic.y)+R221)+(cu.y2-ob.posic.y)*log((cu.x2-ob.posic.x)+R221)+(cu.z1-ob.posic.z)*arctg((cu.z1-ob.posic.z)*R221/(cu.x2-ob.posic.x)/(cu.y2-ob.posic.y));
		G121=(cu.x1-ob.posic.x)*log((cu.y2-ob.posic.y)+R121)+(cu.y2-ob.posic.y)*log((cu.x1-ob.posic.x)+R121)+(cu.z1-ob.posic.z)*arctg((cu.z1-ob.posic.z)*R121/(cu.x1-ob.posic.x)/(cu.y2-ob.posic.y));
		G211=(cu.x2-ob.posic.x)*log((cu.y1-ob.posic.y)+R211)+(cu.y1-ob.posic.y)*log((cu.x2-ob.posic.x)+R211)+(cu.z1-ob.posic.z)*arctg((cu.z1-ob.posic.z)*R211/(cu.x2-ob.posic.x)/(cu.y1-ob.posic.y));
		G111=(cu.x1-ob.posic.x)*log((cu.y1-ob.posic.y)+R111)+(cu.y1-ob.posic.y)*log((cu.x1-ob.posic.x)+R111)+(cu.z1-ob.posic.z)*arctg((cu.z1-ob.posic.z)*R111/(cu.x1-ob.posic.x)/(cu.y1-ob.posic.y));

		return -1.0*G0*(G222-G122-G212+G112-G221+G121+G211-G111)*cu.val.at(valId);
	}
	else if (!strcmp(type,"Gx"))
	{
		R222=sqrt((cu.x2-ob.posic.x)*(cu.x2-ob.posic.x)+(cu.y2-ob.posic.y)*(cu.y2-ob.posic.y)+(cu.z2-ob.posic.z)*(cu.z2-ob.posic.z));
		R122=sqrt((cu.x1-ob.posic.x)*(cu.x1-ob.posic.x)+(cu.y2-ob.posic.y)*(cu.y2-ob.posic.y)+(cu.z2-ob.posic.z)*(cu.z2-ob.posic.z));
		R212=sqrt((cu.x2-ob.posic.x)*(cu.x2-ob.posic.x)+(cu.y1-ob.posic.y)*(cu.y1-ob.posic.y)+(cu.z2-ob.posic.z)*(cu.z2-ob.posic.z));
		R112=sqrt((cu.x1-ob.posic.x)*(cu.x1-ob.posic.x)+(cu.y1-ob.posic.y)*(cu.y1-ob.posic.y)+(cu.z2-ob.posic.z)*(cu.z2-ob.posic.z));
		R221=sqrt((cu.x2-ob.posic.x)*(cu.x2-ob.posic.x)+(cu.y2-ob.posic.y)*(cu.y2-ob.posic.y)+(cu.z1-ob.posic.z)*(cu.z1-ob.posic.z));
		R121=sqrt((cu.x1-ob.posic.x)*(cu.x1-ob.posic.x)+(cu.y2-ob.posic.y)*(cu.y2-ob.posic.y)+(cu.z1-ob.posic.z)*(cu.z1-ob.posic.z));
		R211=sqrt((cu.x2-ob.posic.x)*(cu.x2-ob.posic.x)+(cu.y1-ob.posic.y)*(cu.y1-ob.posic.y)+(cu.z1-ob.posic.z)*(cu.z1-ob.posic.z));
		R111=sqrt((cu.x1-ob.posic.x)*(cu.x1-ob.posic.x)+(cu.y1-ob.posic.y)*(cu.y1-ob.posic.y)+(cu.z1-ob.posic.z)*(cu.z1-ob.posic.z));

		G222=log((cu.y2-ob.posic.y)+R222);
		G122=log((cu.y2-ob.posic.y)+R122);
		G212=log((cu.y1-ob.posic.y)+R212);
		G112=log((cu.y1-ob.posic.y)+R112);
		G221=log((cu.y2-ob.posic.y)+R221);
		G121=log((cu.y2-ob.posic.y)+R121);
		G211=log((cu.y1-ob.posic.y)+R211);
		G111=log((cu.y1-ob.posic.y)+R111);

		return G0*(G222-G122-G212+G112-G221+G121+G211-G111)*cu.val.at(valId)*1.0e+4;
	}
	else if (!strcmp(type,"Gy"))
	{
		R222=sqrt((cu.x2-ob.posic.x)*(cu.x2-ob.posic.x)+(cu.y2-ob.posic.y)*(cu.y2-ob.posic.y)+(cu.z2-ob.posic.z)*(cu.z2-ob.posic.z));
		R122=sqrt((cu.x1-ob.posic.x)*(cu.x1-ob.posic.x)+(cu.y2-ob.posic.y)*(cu.y2-ob.posic.y)+(cu.z2-ob.posic.z)*(cu.z2-ob.posic.z));
		R212=sqrt((cu.x2-ob.posic.x)*(cu.x2-ob.posic.x)+(cu.y1-ob.posic.y)*(cu.y1-ob.posic.y)+(cu.z2-ob.posic.z)*(cu.z2-ob.posic.z));
		R112=sqrt((cu.x1-ob.posic.x)*(cu.x1-ob.posic.x)+(cu.y1-ob.posic.y)*(cu.y1-ob.posic.y)+(cu.z2-ob.posic.z)*(cu.z2-ob.posic.z));
		R221=sqrt((cu.x2-ob.posic.x)*(cu.x2-ob.posic.x)+(cu.y2-ob.posic.y)*(cu.y2-ob.posic.y)+(cu.z1-ob.posic.z)*(cu.z1-ob.posic.z));
		R121=sqrt((cu.x1-ob.posic.x)*(cu.x1-ob.posic.x)+(cu.y2-ob.posic.y)*(cu.y2-ob.posic.y)+(cu.z1-ob.posic.z)*(cu.z1-ob.posic.z));
		R211=sqrt((cu.x2-ob.posic.x)*(cu.x2-ob.posic.x)+(cu.y1-ob.posic.y)*(cu.y1-ob.posic.y)+(cu.z1-ob.posic.z)*(cu.z1-ob.posic.z));
		R111=sqrt((cu.x1-ob.posic.x)*(cu.x1-ob.posic.x)+(cu.y1-ob.posic.y)*(cu.y1-ob.posic.y)+(cu.z1-ob.posic.z)*(cu.z1-ob.posic.z));

		G222=log((cu.x2-ob.posic.x)+R222);
		G122=log((cu.x1-ob.posic.x)+R122);
		G212=log((cu.x2-ob.posic.x)+R212);
		G112=log((cu.x1-ob.posic.x)+R112);
		G221=log((cu.x2-ob.posic.x)+R221);
		G121=log((cu.x1-ob.posic.x)+R121);
		G211=log((cu.x2-ob.posic.x)+R211);
		G111=log((cu.x1-ob.posic.x)+R111);

		return G0*(G222-G122-G212+G112-G221+G121+G211-G111)*cu.val.at(valId)*1.0e+4;
	}
	else if (!strcmp(type,"Gz"))
	{
		R222=sqrt((cu.x2-ob.posic.x)*(cu.x2-ob.posic.x)+(cu.y2-ob.posic.y)*(cu.y2-ob.posic.y)+(cu.z2-ob.posic.z)*(cu.z2-ob.posic.z));
		R122=sqrt((cu.x1-ob.posic.x)*(cu.x1-ob.posic.x)+(cu.y2-ob.posic.y)*(cu.y2-ob.posic.y)+(cu.z2-ob.posic.z)*(cu.z2-ob.posic.z));
		R212=sqrt((cu.x2-ob.posic.x)*(cu.x2-ob.posic.x)+(cu.y1-ob.posic.y)*(cu.y1-ob.posic.y)+(cu.z2-ob.posic.z)*(cu.z2-ob.posic.z));
		R112=sqrt((cu.x1-ob.posic.x)*(cu.x1-ob.posic.x)+(cu.y1-ob.posic.y)*(cu.y1-ob.posic.y)+(cu.z2-ob.posic.z)*(cu.z2-ob.posic.z));
		R221=sqrt((cu.x2-ob.posic.x)*(cu.x2-ob.posic.x)+(cu.y2-ob.posic.y)*(cu.y2-ob.posic.y)+(cu.z1-ob.posic.z)*(cu.z1-ob.posic.z));
		R121=sqrt((cu.x1-ob.posic.x)*(cu.x1-ob.posic.x)+(cu.y2-ob.posic.y)*(cu.y2-ob.posic.y)+(cu.z1-ob.posic.z)*(cu.z1-ob.posic.z));
		R211=sqrt((cu.x2-ob.posic.x)*(cu.x2-ob.posic.x)+(cu.y1-ob.posic.y)*(cu.y1-ob.posic.y)+(cu.z1-ob.posic.z)*(cu.z1-ob.posic.z));
		R111=sqrt((cu.x1-ob.posic.x)*(cu.x1-ob.posic.x)+(cu.y1-ob.posic.y)*(cu.y1-ob.posic.y)+(cu.z1-ob.posic.z)*(cu.z1-ob.posic.z));

		G222=atan((cu.x2-ob.posic.x)*(cu.y2-ob.posic.y)/(R222*(cu.z2-ob.posic.z)));
		G122=atan((cu.x1-ob.posic.x)*(cu.y2-ob.posic.y)/(R122*(cu.z2-ob.posic.z)));
		G212=atan((cu.x2-ob.posic.x)*(cu.y1-ob.posic.y)/(R212*(cu.z2-ob.posic.z)));
		G112=atan((cu.x1-ob.posic.x)*(cu.y1-ob.posic.y)/(R112*(cu.z2-ob.posic.z)));
		G221=atan((cu.x2-ob.posic.x)*(cu.y2-ob.posic.y)/(R221*(cu.z1-ob.posic.z)));
		G121=atan((cu.x1-ob.posic.x)*(cu.y2-ob.posic.y)/(R121*(cu.z1-ob.posic.z)));
		G211=atan((cu.x2-ob.posic.x)*(cu.y1-ob.posic.y)/(R211*(cu.z1-ob.posic.z)));
		G111=atan((cu.x1-ob.posic.x)*(cu.y1-ob.posic.y)/(R111*(cu.z1-ob.posic.z)));

		return -1.0*G0*(G222-G122-G212+G112-G221+G121+G211-G111)*cu.val.at(valId)*1.0e+4;
	}
	else return BDL_MAX;
}

double forward3DG::sphere_G(sphere sp,int valId,vertex ob,char* type)
{
	if (!strcmp(type,"G"))
	{
		return G0*(4.0*sp.val.at(valId)*pow(sp.rad,3)*Pi/3.0)*(ob.posic.z-sp.z)/pow(pow(ob.posic.x-sp.x,2)+pow(ob.posic.y-sp.y,2)+pow(ob.posic.z-sp.z,2),1.5);
	}
	else if (!strcmp(type,"Gx"))
	{
		return -3e+4*G0*(4.0*sp.val.at(valId)*pow(sp.rad,3)*Pi/3.0)*(ob.posic.z-sp.z)*(ob.posic.x-sp.x)/pow(pow(ob.posic.x-sp.x,2)+pow(ob.posic.y-sp.y,2)+pow(ob.posic.z-sp.z,2),2.5);
	}
	else if (!strcmp(type,"Gy"))
	{
		return -3e+4*G0*(4.0*sp.val.at(valId)*pow(sp.rad,3)*Pi/3.0)*(ob.posic.z-sp.z)*(ob.posic.y-sp.y)/pow(pow(ob.posic.x-sp.x,2)+pow(ob.posic.y-sp.y,2)+pow(ob.posic.z-sp.z,2),2.5);
	}
	else if (!strcmp(type,"Gz"))
	{
		return 1e+4*G0*(4.0*sp.val.at(valId)*pow(sp.rad,3)*Pi/3.0)*(2.0*pow(ob.posic.z-sp.z,2)+pow(ob.posic.x-sp.x,2)+pow(ob.posic.y-sp.y,2))/pow(pow(ob.posic.x-sp.x,2)+pow(ob.posic.y-sp.y,2)+pow(ob.posic.z-sp.z,2),2.5);
	}
	else return BDL_MAX;
}
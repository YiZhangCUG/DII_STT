#include "cgdy.h"

int main(int argc, char* argv[])
{
	cgdy icg;
	if (argc == 1)
	{
		cout << "usage: executable.ex <para.conf>" << endl;
	}
	else
	{
		for (int i = 1; i < argc; i++)
		{
			icg.runtine(argv[i]);
		}
	}
	//icg.calResolution("d6.msh");
	//icg.calResolution2("d6.msh");
}
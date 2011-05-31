#include "Material.h"


Material::Material(void)
{
	name = "";
	Ns = 1;
	Ni = d = 0;

	Ka.x = 0;
	Ka.y = 0;
	Ka.z = 0;

	Kd.x = 0;
	Kd.y = 0;
	Kd.z = 0;
	
	Ks.x = 0;
	Ks.y = 0;
	Ks.z = 0;
	
	illum = 0;
	map_Kd = "";
	map_Ka = "";
	map_Ks = "";
	map_Kn = "";

}


Material::~Material(void)
{
}

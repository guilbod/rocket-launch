#include "main.h"

int main(int argc, char *argv[])
{
	DV dv;
	LV lv;
	readParams("param.txt", &dv, &lv);
	printDesignVariables(&dv);
	printLaunchVariables(&lv);
	eulerMethod(&dv, &lv, 0.001);

	return 0;
}
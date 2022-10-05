#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define g -9.81 // gravity
#define Peau 1000 // P water
#define Pair 1.2 // P air
#define Y 1.4 // adiabatic coefficient
#define Pext 101325 // exterior pressure
#define Pi 3.1415

typedef struct DesignVariable_s {
	float Cx;
	float S;
	float r;
	float Mvide;
	float Vr;
} DV;

typedef struct LaunchVariable_s {
	float Veau;
	float P0;
} LV;


void readParams(char *Path, DV *dv, LV *lv) {
	FILE *f;
	f = fopen(Path,"r");
	if (!f) { 
		printf("No file opened\n"); 
		return;
	}

	if (!fscanf(f,"%f %f %f %f %f %f %f",&dv->Cx,&dv->S,&dv->r,&dv->Mvide,&dv->Vr,&lv->Veau,&lv->P0)) { 
		printf("No file read\n");
		return;
	}

	lv->P0 *= 100000;
	dv->r *= 0.001;
	dv->Mvide /=1000.0;
	dv->S/=10000.0;
	fclose(f);

	printf("Values read\n");
}

void printDesignVariables(DV *dv) {
	printf("DesignValues: \n %lg %lg %lg %lg %lg\n", dv->Cx,dv->S,dv->r,dv->Mvide,dv->Vr);
}

void printLaunchVariables(LV *lv) {
	printf("LaunchValues: \n %lg %lg\n", lv->Veau,lv->P0);
}

void eulerMethod(DV *dv, LV *lv, float dt) {
	double p = lv->P0;
	printf("%d\n", Pext);
	double m = lv->Veau + dv->Mvide;
	double v = 0;
	double y = 0;
	double t = 0;

	double aire = -3.1415*dv->r*dv->r;
	double p0 = lv->P0;

	while (m > dv->Mvide){
		t += dt;	
		m += aire*dt*pow(	2*Peau*(	p0*pow(	Peau*(	(dv->Vr-lv->Veau)/( (Peau*dv->Vr) + (dv->Mvide-m)	)	),Y)-Pext	)	,0.5);
		v += (dt/m)*(-m*g-(0.5*dv->S*dv->Cx*Pair*sqrt(pow(v,2))*v)+(2*3.1415*dv->r*dv->r)*(p0*pow(	Peau*(	(dv->Vr-lv->Veau)/( (Peau*dv->Vr) + (dv->Mvide-m)	)	),Y)-Pext	));
		y += v * dt;	
	}
	printf("1rst phase complete:\n t: %lg y: %lg\n", t, y);

	FILE *f;
	f = fopen("altitude.csv","w");

	double ymax = 0;

	while (y > 0){
		t += dt;
		v += g * dt;
		y += v * dt;
		if (y > ymax)
			ymax = y;

		fprintf(f, "%lg\t%lg\n", y, t);
	}

	fclose(f);
	printf("2nd phase complete:\n t: %lg ymax: %lg\n", t, ymax);
}
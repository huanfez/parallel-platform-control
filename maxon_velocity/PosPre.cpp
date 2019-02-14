#include "PosPre.h"
#define  PI 3.14159265358979323846

PosPre *PosPre::clcPos=0;
PosPre::PosPre(void)
{
	PosPre::clcPos=this;
	for (int numn=0;numn<6;numn++)
	{
		Vel[numn]=0;
		AngPredict[numn]=0;
	}
}

PosPre::~PosPre(void)
{
}

void PosPre::VelPredict(double Forcex, double Forcey, double Forcez, double Torquex, double Torquey, double Torquez, double actAngZ)
{
	double admit_B[6]={30, 30, 30, 30, 30, 20};
	double admit_K[6]={0};
	double VelPfx,VelPfy,VelPfz,VelRfx,VelRfy,VelRfz;
	VelPfx=Forcex/admit_B[0];
	VelPfy=Forcex/admit_B[1];
	VelPfz=Forcex/admit_B[2];
	VelRfx=Forcex/admit_B[3];
	VelRfy=Forcex/admit_B[4];
	VelRfz=Forcex/admit_B[5];

	Vel[0]=VelPfx*cos(ang(actAngZ))-VelPfy*sin(ang(actAngZ));
	Vel[1]=VelPfx*sin(ang(actAngZ))+VelPfy*cos(ang(actAngZ));
	Vel[2]=VelPfz;
	Vel[3]=VelRfx*cos(ang(actAngZ))-VelRfy*sin(ang(actAngZ));
	Vel[4]=VelRfx*sin(ang(actAngZ))+VelRfy*cos(ang(actAngZ));
	Vel[5]=VelRfz;
}

void PosPre::MotorVel(double actAngDelt1,  double actAngDelt2, double actAngDelt3, double actAngX, double actAngY, double actAngZ)
{
	double *Pos=forward_kenematics(0.1,0.2,actAngDelt1, actAngDelt2, actAngDelt3, actAngX, actAngY, actAngZ);//½Ç¶ÈÖÆ
	double **inverseMatrix=inverce_kinematics(0.1,0.2,Pos[0],Pos[1],Pos[2],Pos[3],Pos[4],Pos[5] );
	for (int numj=0;numj<3;numj++)
	{
		AngPredict[numj]=0;
		for (int numk=0;numk<3;numk++)
		{
			AngPredict[numj]+=inverseMatrix[numj][numk]*Vel[numk];
		}
		AngPredict[numj]=AngPredict[numj]/PI*180;
	}
	for (int numj_=3;numj_<6;numj_++)
	{
		AngPredict[numj_]=Vel[numj_]/PI*180;
	}
}

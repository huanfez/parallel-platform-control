#pragma once
#include"vector3.h"

class PosPre
{
public:
	PosPre(void);
	~PosPre(void);
	static PosPre *clcPos;
    void VelPredict(double Forcex, double Forcey, double Forcez, double Torquex, double Torquey, double Torquez,  double actAngZ);
	void MotorVel(double actAngDelt1,  double actAngDelt2, double actAngDelt3, double actAngX, double actAngY, double actAngZ);

	double Vel[6];
	double AngPredict[6];
};

#ifndef MAXON_VELOCITY_H
#define MAXON_VELOCITY_H

#include <QtGui/QMainWindow>
#include "ui_maxon_velocity.h"
#include "Sensor.h"
#include "motor_read.h"
#include "PosPre.h"

class maxon_velocity : public QMainWindow
{
	Q_OBJECT

public:
	maxon_velocity(QWidget *parent = 0, Qt::WFlags flags = 0);
	~maxon_velocity();
	static maxon_velocity *motionVel;
    MotorPort 	*maxon_X;
	MotorPort *maxon_Z;
	Sensor *sensorf;
	PosPre *clcPos;

	int timerId;
	long relativePos[6];
	long relativeVel[6];
	BOOL xFlag, zFlag;

	void timerEvent(QTimerEvent *e);
	void MutiMotion(double GetAng1, double GetAng2, double GetAng3, double GetAngX, double GetAngY, double GetAngZ);

//private:
	Ui::maxon_velocityClass ui;

public slots:
	void makeEnable();
	void makeDisable();
	void makeHalt();
	void makeMove();
	void makeConnect();
	void makeDisconnect();
	void SensorOpen();
	void SensorStop();
};

#endif // MAXON_VELOCITY_H

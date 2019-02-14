#ifndef SENSOR_H
#define SENSOR_H

#pragma once
#include "windows.h"
#include "CfsUsb.h"//力传感器的库文件
#include <vector>
#include <QtGui/QWidget>
#include<QtGui/QMessageBox>
#include<QtGui/QPushButton>
#include<QtGui/QLabel>
#include<QtGui/QLineEdit>

typedef void (CALLBACK* FUNC_Initialize)();
typedef void (CALLBACK* FUNC_Finalize)();
typedef bool (CALLBACK* FUNC_PortOpen)(int);
typedef void (CALLBACK* FUNC_PortClose)( int);
typedef bool (CALLBACK* FUNC_SetSerialMode)( int ,bool);
typedef bool (CALLBACK* FUNC_GetSerialData)( int ,double *,char *);
typedef bool (CALLBACK* FUNC_GetLatestData)( int ,double *,char *);
typedef bool (CALLBACK* FUNC_GetSensorLimit)( int ,double *);
typedef bool (CALLBACK* FUNC_GetSensorInfo)( int, char *);

class Sensor:public QWidget
{
	Q_OBJECT
public:
	Sensor(QWidget *parent);
	Sensor(void);
	~Sensor(void);
	static Sensor *sensorf;

	HINSTANCE hDll;

	FUNC_Initialize Initialize;
	FUNC_Finalize Finalize;
	FUNC_PortOpen PortOpen;
	FUNC_PortClose PortClose;
	FUNC_SetSerialMode SetSerialMode;
	FUNC_GetSerialData GetSerialData;
	FUNC_GetLatestData GetLatestData;
	FUNC_GetSensorLimit GetSensorLimit;
	FUNC_GetSensorInfo GetSensorInfo;

	void SensorInitialize();
	void SensorFinalize();
	bool SensorPortOpen(int portNo);
	void SensorPortClose(int portNo);
	bool SensorGetSensorLimit(int portNo,double *Data);
	bool SensorGetSerialData(int portNo,double *Data,char *Status);
	bool SensorGetLatestData(int portNo,double *Data,char *Status);
	bool SensorGetSensorInfo(int portNo,char *SerialNo);
	/*void ForceGet(double *pData,double *Limit);*/
	QString tochar(double num);

	void openSensor();
	void unloadLibrary();
	void getForceData();
	void StopSensor();

public:
	int portNo;
	char Status;
	char SerialNo[9];
	double Data[6],Limit[6];
	double Fx, Fy, Fz, Mx, My, Mz;
	double Fxt, Fyt, Fzt, Mxt, Myt, Mzt;
};

#endif
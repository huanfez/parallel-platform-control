#include "sensor.h"
#include "maxon_velocity.h"
#include<QtGui/QMessageBox>

Sensor *Sensor::sensorf=0;
Sensor::Sensor(QWidget *parent): QWidget(parent)
{  
	Sensor::sensorf=this;
	portNo=4;
	hDll = LoadLibrary(L"CfsUsb.dll");
	if (hDll != NULL)
	{
		// 関数アドレスの取得
		Initialize = (FUNC_Initialize )GetProcAddress(hDll,"Initialize"); // ＤＬＬの初期化処理 
		Finalize = (FUNC_Finalize )GetProcAddress(hDll,"Finalize"); // ＤＬＬの終了処理
		PortOpen = (FUNC_PortOpen )GetProcAddress(hDll,"PortOpen"); // ポートオープン
		PortClose = (FUNC_PortClose )GetProcAddress(hDll,"PortClose"); // ポートクローズ
		SetSerialMode = (FUNC_SetSerialMode )GetProcAddress(hDll,"SetSerialMode"); // データの連続読込の開始/停止
		GetSerialData = (FUNC_GetSerialData )GetProcAddress(hDll,"GetSerialData"); // 連続データ読込み
		GetLatestData = (FUNC_GetLatestData )GetProcAddress(hDll,"GetLatestData"); // 最新データ読込
		GetSensorLimit = (FUNC_GetSensorLimit)GetProcAddress(hDll,"GetSensorLimit"); // センサ定格確認
		GetSensorInfo = (FUNC_GetSensorInfo)GetProcAddress(hDll,"GetSensorInfo"); //シリアルNo取得
	}
	Initialize();
}

Sensor::~Sensor(void)
{
	//
	SensorFinalize();
	// ＤＬＬの解放
	FreeLibrary(hDll);
}

//void Sensor::ForceGet(double *pData,double *Limit)
//{
//	for (int i=0;i<6;i++){
//		double result=Limit[i] / 10000 *pData[i];
//		forceArray.push_back(result);
//	}
//}

void Sensor::SensorInitialize()
{
	Initialize();
}

void Sensor::SensorFinalize()
{
	Finalize();
}

bool Sensor::SensorPortOpen(int portNo)
{
	return PortOpen(portNo);
}

void Sensor::SensorPortClose(int portNo)
{
	PortClose(portNo);
}

bool Sensor::SensorGetSerialData(int portNo,double *Data,char *Status)
{
	return GetSerialData(portNo,Data,Status);
}

bool Sensor::SensorGetLatestData(int portNo,double *Data,char *Status)
{
	return GetLatestData(portNo,Data,Status);
}

bool Sensor::SensorGetSensorLimit(int portNo,double *Limit)
{
	return GetSensorLimit( portNo,Limit);
}

bool Sensor::SensorGetSensorInfo(int portNo,char *SerialNo)
{
	return GetSensorInfo(portNo,SerialNo);
}

QString Sensor::tochar(double num)
{
	QString str;
	str.setNum(num);
	return str;
}

void Sensor::unloadLibrary()
{
	FreeLibrary(hDll);
}

void Sensor::openSensor()
{
	if(SensorPortOpen(portNo)==true)
	{
		if(SensorGetSensorLimit(portNo, Limit)==false)
		{
			QMessageBox::information(this, "Wrong","GetSensorLimit Failed!");
		}
		if(SensorGetSensorInfo(portNo,SerialNo)==false)
		{
			QMessageBox::information(this, "Wrong","GetSensorInfo Failed!");
		}
		if(SensorGetLatestData(portNo,Data,&Status)==true)
		{
			Fxt = Limit[0] / 10000 * Data[0];
			Fyt = Limit[1] / 10000 * Data[1];
			Fzt = Limit[2] / 10000 * Data[2];
			Mxt = Limit[3] / 10000 * Data[3];
			Myt = Limit[4] / 10000 * Data[4];
			Mzt = Limit[5] / 10000 * Data[5];
		}
		else
		{
			QMessageBox::information(this, "Wrong","GetLatestData Failed!");
		}
		if(SetSerialMode(portNo, true)==true)
		{
			maxon_velocity::motionVel->ui.OpenButton->setEnabled(TRUE);
		}
	}
	else
	{
		QMessageBox::information(this, "Wrong","SensorPortOpen Failed!");
	}
}

void Sensor::getForceData()
{
	int midnum=0;
	while (midnum<36)
	{
		if(SensorGetSerialData(portNo,Data,&Status)==true)
		{
			Fx = Limit[0] / 10000 * Data[0]-Fxt;//减去初始值，清零
			Fy = Limit[1] / 10000 * Data[1]-Fyt;
			Fz = Limit[2] / 10000 * Data[2]-Fzt;
			Mx = Limit[3] / 10000 * Data[3]-Mxt;
			My = Limit[4] / 10000 * Data[4]-Myt;
			Mz = Limit[5] / 10000 * Data[5]-Mzt;
			//cout<<sensorf.Fx;
			//qDebug()<<sensorf.Fx;
			midnum++;
		}
	}
}

void Sensor::StopSensor()
{
    SensorPortClose(portNo);
	SensorFinalize();
	//ui.StartButton->setEnabled(TRUE);
	//sensorf.unloadLibrary();
}
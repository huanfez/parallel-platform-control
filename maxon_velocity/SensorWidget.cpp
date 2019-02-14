#include "SensorWidget.h"
#include<QtGui/QMessageBox>
#include<QtGui/QPushButton>
#include<QtGui/QLabel>
#include<QtGui/QLineEdit>
//#include<QtCore/QDebug>
//#include<iostream>
//using namespace std;
SensorWidget *SensorWidget::ForceGet=0;

SensorWidget::SensorWidget(QWidget *parent)
	: QWidget(parent)
{
	SensorWidget::ForceGet=this;

}

SensorWidget::~SensorWidget()
{
	//sensorf.SensorFinalize();
	//FreeLibrary(sensorf.hDll);
}

void SensorWidget::Open()
{
	if(sensorf.SensorPortOpen(sensorf.portNo)==true)
	{
		if(sensorf.SensorGetSensorLimit(sensorf.portNo,sensorf.Limit)==false)
		{
			QMessageBox::information(this, "Wrong","GetSensorLimit Failed!");
		}
		if(sensorf.SensorGetSensorInfo(sensorf.portNo,sensorf.SerialNo)==false)
		{
			QMessageBox::information(this, "Wrong","GetSensorInfo Failed!");
		}
		if(sensorf.SensorGetLatestData(sensorf.portNo,sensorf.Data,&sensorf.Status)==true)
		{
			sensorf.Fxt = sensorf.Limit[0] / 10000 * sensorf.Data[0];
			sensorf.Fyt = sensorf.Limit[1] / 10000 * sensorf.Data[1];
			sensorf.Fzt = sensorf.Limit[2] / 10000 * sensorf.Data[2];
			sensorf.Mxt = sensorf.Limit[3] / 10000 * sensorf.Data[3];
			sensorf.Myt = sensorf.Limit[4] / 10000 * sensorf.Data[4];
			sensorf.Mzt = sensorf.Limit[5] / 10000 * sensorf.Data[5];
		}
		else
		{
			QMessageBox::information(this, "Wrong","GetLatestData Failed!");
		}
		if(sensorf.SetSerialMode(sensorf.portNo, true)==true)
		{
			//SensorWidget::sensorWid->ui.StartButton->setEnabled(TRUE);
			ui.StartButton->setEnabled(TRUE);
		}
	}
	else
	{
		QMessageBox::information(this, "Wrong","SensorPortOpen Failed!");
	}
}

void SensorWidget::StartSensor()
{
	   timerId=QObject::startTimer(40);
	//if(sensorf.SensorGetSerialData(sensorf.portNo,sensorf.Data,&sensorf.Status)==true)
	//{
	//	sensorf.Fx = sensorf.Limit[0] / 10000 * sensorf.Data[0]-sensorf.Fxt;//减去初始值，清零
	//	sensorf.Fy = sensorf.Limit[1] / 10000 * sensorf.Data[1]-sensorf.Fyt;
	//	sensorf.Fz = sensorf.Limit[2] / 10000 * sensorf.Data[2]-sensorf.Fzt;
	//	sensorf.Mx = sensorf.Limit[3] / 10000 * sensorf.Data[3]-sensorf.Mxt;
	//	sensorf.My = sensorf.Limit[4] / 10000 * sensorf.Data[4]-sensorf.Myt;
	//	sensorf.Mz = sensorf.Limit[5] / 10000 * sensorf.Data[5]-sensorf.Mzt;
	//	//cout<<sensorf.Fx;
	//	//qDebug()<<sensorf.Fx;
	//	ui.fx->setText(sensorf.tochar(sensorf.Fx));
	//	ui.fy->setText(sensorf.tochar(sensorf.Fy));
	//	ui.fz->setText(sensorf.tochar(sensorf.Fz));
	//	ui.mx->setText(sensorf.tochar(sensorf.Mx));
	//	ui.my->setText(sensorf.tochar(sensorf.My));
	//	ui.mz->setText(sensorf.tochar(sensorf.Mz));
	//}
}

void SensorWidget::mystart()
{
	//mt.start();
}

void SensorWidget::StopSensor()
{
	mystop();
	killTimer(timerId);
	sensorf.SensorPortClose(sensorf.portNo);
	sensorf.SensorFinalize();
	//ui.StartButton->setEnabled(TRUE);
    //sensorf.unloadLibrary();
}

void SensorWidget::mystop()
{
	//mt.quit();
}

void SensorWidget::getForceData()
{
	int midnum=0;
	while (midnum<48)
	{
		if(sensorf.SensorGetSerialData(sensorf.portNo,sensorf.Data,&sensorf.Status)==true)
		{
			sensorf.Fx = sensorf.Limit[0] / 10000 * sensorf.Data[0]-sensorf.Fxt;//减去初始值，清零
			sensorf.Fy = sensorf.Limit[1] / 10000 * sensorf.Data[1]-sensorf.Fyt;
			sensorf.Fz = sensorf.Limit[2] / 10000 * sensorf.Data[2]-sensorf.Fzt;
			sensorf.Mx = sensorf.Limit[3] / 10000 * sensorf.Data[3]-sensorf.Mxt;
			sensorf.My = sensorf.Limit[4] / 10000 * sensorf.Data[4]-sensorf.Myt;
			sensorf.Mz = sensorf.Limit[5] / 10000 * sensorf.Data[5]-sensorf.Mzt;
			//cout<<sensorf.Fx;
			//qDebug()<<sensorf.Fx;
			midnum++;
		}
	}
	ui.fx->setText(sensorf.tochar(sensorf.Fx));
	ui.fy->setText(sensorf.tochar(sensorf.Fy));
	ui.fz->setText(sensorf.tochar(sensorf.Fz));
	ui.mx->setText(sensorf.tochar(sensorf.Mx));
	ui.my->setText(sensorf.tochar(sensorf.My));
	ui.mz->setText(sensorf.tochar(sensorf.Mz));
}

void SensorWidget::timerEvent(QTimerEvent *e)
{
	//int midnum=0;
 //   while (midnum<48)
 //   {
	//	if(sensorf.SensorGetSerialData(sensorf.portNo,sensorf.Data,&sensorf.Status)==true)
	//	{
	//		sensorf.Fx = sensorf.Limit[0] / 10000 * sensorf.Data[0]-sensorf.Fxt;//减去初始值，清零
	//		sensorf.Fy = sensorf.Limit[1] / 10000 * sensorf.Data[1]-sensorf.Fyt;
	//		sensorf.Fz = sensorf.Limit[2] / 10000 * sensorf.Data[2]-sensorf.Fzt;
	//		sensorf.Mx = sensorf.Limit[3] / 10000 * sensorf.Data[3]-sensorf.Mxt;
	//		sensorf.My = sensorf.Limit[4] / 10000 * sensorf.Data[4]-sensorf.Myt;
	//		sensorf.Mz = sensorf.Limit[5] / 10000 * sensorf.Data[5]-sensorf.Mzt;
	//		//cout<<sensorf.Fx;
	//		//qDebug()<<sensorf.Fx;
	//		midnum++;
	//	}
 //   }
	//ui.fx->setText(sensorf.tochar(sensorf.Fx));
	//ui.fy->setText(sensorf.tochar(sensorf.Fy));
	//ui.fz->setText(sensorf.tochar(sensorf.Fz));
	//ui.mx->setText(sensorf.tochar(sensorf.Mx));
	//ui.my->setText(sensorf.tochar(sensorf.My));
	//ui.mz->setText(sensorf.tochar(sensorf.Mz));
    
	ui.Joint1->setText(QString("%1").arg(joint1));
	ui.Joint2->setText(QString("%1").arg(joint2));
	ui.Joint3->setText(QString("%1").arg(joint3));
	ui.Joint4->setText(QString("%1").arg(joint4));
	ui.Joint5->setText(QString("%1").arg(joint5));
	ui.Joint6->setText(QString("%1").arg(joint6));
}
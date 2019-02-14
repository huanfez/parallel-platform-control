#include "maxon_velocity.h"
#include <math.h>

#define PI 3.1415926;
maxon_velocity *maxon_velocity::motionVel=0;

maxon_velocity::maxon_velocity(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);
	maxon_velocity::motionVel=this ;

	maxon_X=new MotorPort(this, "EPOS2", "MAXON_RS232", "RS232",  "COM2", 1);
	//maxon_Z=new MotorPort(this, "EPOS2", "MAXON_RS232", "RS232",  "COM3", 1);

	sensorf=new Sensor(this);
	clcPos=new PosPre();

	connect (ui.enableButton,SIGNAL(clicked()),this,SLOT(makeEnable()));
	connect (ui.disableButton,SIGNAL(clicked()),this,SLOT(makeDisable()));
	connect (ui.haltButton,SIGNAL(clicked()),this,SLOT(makeHalt()));
	connect (ui.moveButton,SIGNAL(clicked()),this,SLOT(makeMove()));
	connect (ui.connectButton,SIGNAL(clicked()),this,SLOT(makeConnect()));
	connect (ui.disconnectButton,SIGNAL(clicked()),this,SLOT(makeDisconnect()));

	connect(ui.OpenButton,SIGNAL(clicked()),this,SLOT(SensorOpen()));
	connect(ui.StopButton,SIGNAL(clicked()),this,SLOT(SensorStop()));

	relativePos[0]=0; relativePos[1]=0; relativePos[2]=0; relativePos[3]=0; relativePos[4]=0; relativePos[5]=0;
	relativeVel[0]=0; relativeVel[1]=0; relativeVel[2]=0; relativeVel[3]=0; relativeVel[4]=0; relativeVel[5]=0;
	xFlag=FALSE; zFlag=FALSE;
}

maxon_velocity::~maxon_velocity()
{
    maxon_X->RestoreEPOS();
    //maxon_Z->RestoreEPOS();
}

void maxon_velocity::makeMove()
{
	timerId=QObject::startTimer(30);//创建定时器 ms
	maxon_X->m_oUpdateActive = TRUE;
	//maxon_Z->m_oUpdateActive = TRUE;
}

void maxon_velocity::makeHalt()
{
	maxon_X->OnButtonHalt();
	//maxon_Z->OnButtonHalt();
}

void maxon_velocity::makeEnable()
{
	maxon_X->OnButtonEnable();
	//maxon_Z->OnButtonEnable();
}

void maxon_velocity::makeDisable()
{
	maxon_X->OnButtonDisable();
	//maxon_Z->OnButtonDisable();
}

void maxon_velocity::makeConnect()
{
    //传递端口的所有参数
	if(maxon_X->OpenDevice())
	{
		maxon_X->m_oInitialisation = TRUE;
	}
	else
	{
		maxon_X->m_oInitialisation = FALSE;
	}

	//if (maxon_Z->OpenDevice())
	//{
	//	maxon_Z->m_oInitialisation = TRUE;
	//}
	//else
	//{
	//	maxon_Z->m_oInitialisation = FALSE;
	//}
}

void maxon_velocity::makeDisconnect()
{
	maxon_X->RestoreEPOS();
    //maxon_Z->RestoreEPOS();
}

void maxon_velocity::SensorOpen()
{
	sensorf->openSensor();
}

void maxon_velocity::SensorStop()
{
	sensorf->StopSensor();
}

void maxon_velocity::MutiMotion(double GetAng1, double GetAng2, double GetAng3, double GetAngX, double GetAngY, double GetAngZ)
{
	//maxon_X->chrate+=0.04*PI;  maxon_Z->chrate+=0.04*PI;
   double gearRatio[6]={411.0, 411.0, 411.0, 411.0, 411.0, 410.0};
	clcPos->MotorVel(GetAng1,  GetAng2, GetAng3,  GetAngX, GetAngY,  GetAngZ);//计算各轴的速度
	maxon_X->m_lTargetVelocity=clcPos->AngPredict[3]*gearRatio[3]/360*2000*0.03;  
	//maxon_Z->m_lTargetVelocity=clcPos->AngPredict[5]*gearRatio[5]/360*2000*0.03;

	//驱动X轴
	if (abs(maxon_X->m_lTargetVelocity)<100||abs(maxon_X->m_lTargetVelocity)>2000)
	{
		if (abs(maxon_X->m_lTargetVelocity)<100)
		{
			maxon_X->m_lTargetVelocity=0;
		}

		if (abs(maxon_X->m_lTargetVelocity)>2000)
		{
			maxon_X->m_lTargetVelocity=2000;
		}
	}

	if(!VCS_MoveWithVelocity(maxon_X->m_KeyHandle, maxon_X->m_usNodeId, maxon_X->m_lTargetVelocity, &maxon_X->m_ulErrorCode))
	{
		maxon_X->ShowErrorInformation(maxon_X->m_ulErrorCode);
	}

	////驱动Z轴
	//if (abs(maxon_Z->m_lTargetVelocity)<100||abs(maxon_Z->m_lTargetVelocity)>2000)
	//{
	//	if (abs(maxon_Z->m_lTargetVelocity)<100)
	//	{
	//		maxon_Z->m_lTargetVelocity=0;
	//	}

	//	if (abs(maxon_Z->m_lTargetVelocity)>2000)
	//	{
	//		maxon_Z->m_lTargetVelocity=2000;
	//	}
	//}

	//if(!VCS_MoveWithVelocity(maxon_Z->m_KeyHandle, maxon_Z->m_usNodeId, maxon_Z->m_lTargetVelocity, &maxon_Z->m_ulErrorCode))
	//{
	//	maxon_Z->ShowErrorInformation(maxon_Z->m_ulErrorCode);
	//}

	//UpdateStatus();
	if(maxon_X->m_oUpdateActive && !maxon_X->UpdateStatus())
	{
		maxon_X->StopTimer();
	}

	//if(maxon_Z->m_oUpdateActive && !maxon_Z->UpdateStatus())
	//{
	//	maxon_Z->StopTimer();
	//}
}

void maxon_velocity::timerEvent(QTimerEvent *e)
{
	//获取x轴实际速度和位置
	if (VCS_GetVelocityIs(maxon_X->m_KeyHandle, maxon_X->m_usNodeId, &maxon_X->m_lActualVel, &maxon_X->m_ulErrorCode))
	{
		relativeVel[3]=maxon_X->m_lActualVel/411.0*360/2000/0.03;
		ui.xVelocity->setText(QString("%1").arg(relativeVel[3]));

		if (VCS_GetPositionIs(maxon_X->m_KeyHandle, maxon_X->m_usNodeId, &maxon_X->m_lActualValue, &maxon_X->m_ulErrorCode))
		{
			relativePos[3]=(maxon_X->m_lActualValue-maxon_X->m_lStartPosition)/411.0*360/2000;
			ui.xPosition->setText(QString("%1").arg(relativePos[3]));
			xFlag=TRUE;
		}
	}
	
	////获取z轴实际速度和位置
	//if (VCS_GetVelocityIs(maxon_Z->m_KeyHandle, maxon_Z->m_usNodeId, &maxon_Z->m_lActualVel, &maxon_Z->m_ulErrorCode))
	//{
	//	relativeVel[5]=maxon_Z->m_lActualVel/411.0*360/2000/0.03;
	//	ui.zVelocity->setText(QString("%1").arg(relativeVel[5]));

	//	if (VCS_GetPositionIs(maxon_Z->m_KeyHandle, maxon_Z->m_usNodeId, &maxon_Z->m_lActualValue, &maxon_Z->m_ulErrorCode))
	//	{
	//		relativePos[5]=(maxon_Z->m_lActualValue-maxon_Z->m_lStartPosition)/410.0*360/2000;
	//		ui.zPosition->setText(QString("%1").arg(relativePos[5]));
	//		zFlag=TRUE;
	//	}
	//}
	
	//获取传感器力数据
	sensorf->getForceData();
	ui.fx->setText(sensorf->tochar(sensorf->Fx));
	ui.fy->setText(sensorf->tochar(sensorf->Fy));
	ui.fz->setText(sensorf->tochar(sensorf->Fz));
	ui.mx->setText(sensorf->tochar(sensorf->Mx));
	ui.my->setText(sensorf->tochar(sensorf->My));
	ui.mz->setText(sensorf->tochar(sensorf->Mz));

	//预测末端速度
	clcPos->VelPredict(sensorf->Fx,sensorf->Fy,sensorf->Fz,sensorf->Mx,sensorf->My,sensorf->Mz, relativePos[5]);

	//同时驱动多轴
	if (xFlag/*&&zFlag*/)
	{
		MutiMotion(relativePos[0],relativePos[1],relativePos[2],relativePos[3],relativePos[4],relativePos[5]);
		xFlag=FALSE; /*zFlag=FALSE;*/
		ui.yVelocity->setText(QString("%1").arg(maxon_X->m_lTargetVelocity));
	}	
}
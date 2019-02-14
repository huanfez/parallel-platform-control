#ifndef MOTOR_READ_H
#define MOTOR_READ_H

#include <QtCore/QThread>
#include<QtGui/QMessageBox>
#include <QtGui/QLabel>

#include "Definitions.h"

class MotorPort: public QWidget
{
	Q_OBJECT

public:
	MotorPort(QWidget *parent, char *DeviceName, char *ProtocolStackName, char *InterfaceName,  char *m_oPortName, WORD NodeId);
	MotorPort();
    static MotorPort *maxon_X;

	void OnButtonHalt();
	void OnButtonEnable();
	void OnButtonDisable();
	virtual BOOL DeviceInitilize();
	BOOL OpenDevice();
	BOOL RestoreEPOS();
	BOOL UpdateStatus();
	BOOL ShowErrorInformation(DWORD p_ulErrorCode);
	void StopTimer();

	BOOL m_oImmediately;
	BOOL m_oInitialisation;
	BOOL m_oUpdateActive;
	BOOL m_ulEnabled;
	BOOL m_ulDisabled;
	DWORD m_ulErrorCode;
	DWORD m_ulProfileAcceleration;
	DWORD m_ulProfileDeceleration;
	DWORD m_ulProfileVelocity;
	WORD m_usNodeId;
	BOOL oQuickStopped;

//private:
	QString m_strActiveMode;
	long    m_lActualValue;
	long    m_lStartPosition;
	long   m_lTargetVelocity;
	long   m_lActualVel;
	__int8 m_bMode;
	HANDLE m_KeyHandle;

	char* m_oDeviceName;
	char* m_oProtocolStackName;
    char* m_oInterfaceName;
    char* m_oPortName;
    DWORD m_oBaudrate;
	DWORD m_oTimeout;

	BOOL m_oInitialized;
	double chrate;
};

#endif
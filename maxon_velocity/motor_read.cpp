#include "motor_read.h"
#include "maxon_velocity.h"

MotorPort *MotorPort::maxon_X=0;

/////////////////////////////////////////////////////////////////////////////

MotorPort::MotorPort(QWidget *parent, char *DeviceName, char *ProtocolStackName, char *InterfaceName,  char *m_oPortName, WORD NodeId)
: QWidget(parent), m_oDeviceName(DeviceName), m_oProtocolStackName(ProtocolStackName), m_oInterfaceName(InterfaceName),  m_oPortName(m_oPortName), m_usNodeId(NodeId),
   m_lActualValue(0), m_lStartPosition(0), m_lTargetVelocity(10), m_bMode(0), m_KeyHandle(0),m_lActualVel(0)
{
	MotorPort::maxon_X=this;
	 chrate=0;
}


/////////////////////////////////////////////////////////////////////////////

BOOL MotorPort::DeviceInitilize()//¿ØÖÆÆ÷³õÊ¼»¯
{
	//Settings
	m_oImmediately = TRUE;
	m_oUpdateActive = FALSE;
	//m_usNodeId = 1;

	//Close Previous Device
	if(m_KeyHandle)
	{
		if(m_KeyHandle) VCS_CloseDevice(m_KeyHandle, &m_ulErrorCode);
		m_KeyHandle = 0;
	}

	     m_KeyHandle = VCS_OpenDeviceDlg(&m_ulErrorCode);
	     //m_KeyHandle = VCS_OpenDevice( m_oDeviceName, m_oProtocolStackName, m_oInterfaceName,  m_oPortName, &m_ulErrorCode);
		if(m_KeyHandle)
		{	
				//Clear Error History
				if(VCS_ClearFault(m_KeyHandle, m_usNodeId, &m_ulErrorCode))
				{

					QMessageBox::information(this, "Inform","Deletes existing errors Successful!");

					maxon_velocity::motionVel->ui.enableButton->setEnabled(TRUE);
					maxon_velocity::motionVel->ui.disableButton->setDisabled(TRUE);
					maxon_velocity::motionVel->ui.moveButton->setDisabled(TRUE);
					maxon_velocity::motionVel->ui.haltButton->setDisabled(TRUE);

					return TRUE;
				}
				else
				{
					QMessageBox::information(this, "Warning","Deletes existing errors Failed!");
					return FALSE;
				}

		}
		else
		{
			QMessageBox::information(this, "Warning","Setting Failed!");
			return FALSE;
		}
	
}


/************************************************************************
Sets device to disable state
************************************************************************/
void MotorPort::OnButtonDisable()
{
	//UpdateNodeIdString();

	if(!VCS_SetDisableState(m_KeyHandle, m_usNodeId, &m_ulErrorCode))
	{
		ShowErrorInformation(m_ulErrorCode);
	}
	else
	{
		maxon_velocity::motionVel->ui.enableButton->setEnabled(TRUE);
		maxon_velocity::motionVel->ui.disableButton->setDisabled(TRUE);
		maxon_velocity::motionVel->ui.moveButton->setDisabled(TRUE);
		maxon_velocity::motionVel->ui.haltButton->setDisabled(TRUE);
	}
}

/************************************************************************
Sets device to enable state
************************************************************************/
void MotorPort::OnButtonEnable()
{
	BOOL oFault = FALSE;

	//UpdateNodeIdString();

	if(!VCS_GetFaultState(m_KeyHandle, m_usNodeId, &oFault, &m_ulErrorCode))
	{
		ShowErrorInformation(m_ulErrorCode);
		return;
	}

	if(oFault)
	{
		if(!VCS_ClearFault(m_KeyHandle, m_usNodeId, &m_ulErrorCode))
		{
			ShowErrorInformation(m_ulErrorCode);
			return;
		}
	}

	if(!VCS_SetEnableState(m_KeyHandle, m_usNodeId, &m_ulErrorCode))
	{
		ShowErrorInformation(m_ulErrorCode);
	}
	else
	{
		maxon_velocity::motionVel->ui.enableButton->setDisabled(TRUE);
		maxon_velocity::motionVel->ui.disableButton->setEnabled(TRUE);
		maxon_velocity::motionVel->ui.moveButton->setEnabled(TRUE);
		maxon_velocity::motionVel->ui.haltButton->setEnabled(TRUE);
		
	}
}


/************************************************************************
Stops the movement
************************************************************************/
void MotorPort::OnButtonHalt()
{
	//UpdateNodeIdString();

	if(!VCS_HaltPositionMovement(m_KeyHandle, m_usNodeId, &m_ulErrorCode))
	{
		ShowErrorInformation(m_ulErrorCode);
	}
	else
	{
		maxon_velocity::motionVel->ui.moveButton->setEnabled(TRUE);
		maxon_velocity::motionVel->ui.haltButton->setDisabled(TRUE);
	}
}

/************************************************************************
Updates the display
************************************************************************/
BOOL MotorPort::UpdateStatus()
{
	BOOL oEnable = TRUE;
	BOOL oResult = m_oUpdateActive;

	if(oResult)
	{
		oResult = VCS_GetOperationMode(m_KeyHandle, m_usNodeId, &m_bMode, &m_ulErrorCode);
		if(oResult)
		{
			switch(m_bMode)
			{
			case -6: m_strActiveMode = "Step/Direction Mode"; break;
			case -5: m_strActiveMode = "Master Encoder Mode"; break;
			case -3: m_strActiveMode = "Current Mode"; break;
			case -2: m_strActiveMode = "Velocity Mode"; break;
			case -1: m_strActiveMode = "Position Mode"; break;
			case 1: m_strActiveMode = "Profile Position Mode"; break;
			case 3: m_strActiveMode = "Profile Velocity Mode"; break;
			case 6: m_strActiveMode = "Homing Mode"; break;
			case 7: m_strActiveMode = "Interpolated Position Mode"; break;
			default: m_strActiveMode = "Unknown Mode";
			}
		}
		else
		{
			StopTimer();
			ShowErrorInformation(m_ulErrorCode);
			m_strActiveMode = "Unknown Mode";
		}
	}
	else
	{
		m_strActiveMode = "Unknown Mode";
	}

	if(oResult)
	{
		oResult = VCS_GetEnableState(m_KeyHandle, m_usNodeId, &oEnable, &m_ulErrorCode);

		if(oResult)
		{
			//maxon_velocity::motionVel->ui.enableButton->setEnabled(!oEnable);
			//maxon_velocity::motionVel->ui.disableButton->setEnabled(oEnable);
			//maxon_velocity::motionVel->ui.moveButton->setEnabled(oEnable);
			//maxon_velocity::motionVel->ui.haltButton->setEnabled(oEnable);
		}
		else
		{
			StopTimer();
			ShowErrorInformation(m_ulErrorCode);
			/*maxon_velocity::motionVel->ui.enableButton->setEnabled(oEnable);
			maxon_velocity::motionVel->ui.disableButton->setEnabled(!oEnable);
			maxon_velocity::motionVel->ui.moveButton->setEnabled(!oEnable);
			maxon_velocity::motionVel->ui.haltButton->setEnabled(!oEnable);*/
		}
	}
	else
	{
		//maxon_velocity::motionVel->ui.enableButton->setEnabled(!oEnable);
		//maxon_velocity::motionVel->ui.disableButton->setEnabled(!oEnable);
		//maxon_velocity::motionVel->ui.moveButton->setEnabled(!oEnable);
		//maxon_velocity::motionVel->ui.haltButton->setEnabled(!oEnable);
	}

	if(oResult)
	{
		oResult = VCS_GetPositionIs(m_KeyHandle, m_usNodeId, &m_lActualValue, &m_ulErrorCode);
		if(!oResult)
		{
			StopTimer();
			ShowErrorInformation(m_ulErrorCode);

			m_lActualValue = 0;
			m_lStartPosition = 0;
		}
	}
	else
	{
		m_lActualValue = 0;
		m_lStartPosition = 0;
	}

	return oResult;
}

/************************************************************************
Restores the old settings, stops the timer and delete the key handle
************************************************************************/
BOOL MotorPort::RestoreEPOS()
{
	//Restore Settings
	if(m_oInitialisation)
	{
		VCS_SetDisableState(m_KeyHandle, m_usNodeId, &m_ulErrorCode);
		VCS_SetOperationMode(m_KeyHandle, m_usNodeId, m_bMode, &m_ulErrorCode);
		VCS_SetPositionProfile(m_KeyHandle, m_usNodeId, m_ulProfileVelocity, m_ulProfileAcceleration, m_ulProfileDeceleration, &m_ulErrorCode);
	}

	//Stop Updating
	StopTimer();

	//Close Device
	if(m_KeyHandle) VCS_CloseDevice(m_KeyHandle, &m_ulErrorCode);

	return TRUE;
}


/************************************************************************
Opens communication interface via the OpenDeviceDlg
************************************************************************/
BOOL MotorPort::OpenDevice()
{
	if(DeviceInitilize())
	{
				//Read Operation Mode
				if(VCS_GetOperationMode(m_KeyHandle, m_usNodeId, &m_bMode, &m_ulErrorCode))
				{
					//Read Position Profile Objects
					if(VCS_GetPositionProfile(m_KeyHandle, m_usNodeId, &m_ulProfileVelocity, &m_ulProfileAcceleration, &m_ulProfileDeceleration, &m_ulErrorCode))
					{
						//Write Profile Position Mode
						if(VCS_SetOperationMode(m_KeyHandle, m_usNodeId,/* OMD_PROFILE_POSITION_MODE*/ OMD_PROFILE_VELOCITY_MODE, &m_ulErrorCode))
						{
							//Write Profile Position Objects
							//if(VCS_SetPositionProfile(m_KeyHandle, m_usNodeId, 100, 1000, 1000, &m_ulErrorCode))
							if(VCS_SetVelocityProfile(m_KeyHandle, m_usNodeId, 1000, 1000, &m_ulErrorCode))
							{
								//Read Actual Position
								if(VCS_GetPositionIs(m_KeyHandle, m_usNodeId, &m_lStartPosition, &m_ulErrorCode))
								{
									return TRUE;
								}
							}
						}
					}
		         }
	}
	else
	{
		QMessageBox::information(this, "Warning","Can't open device!");
	}
	return FALSE;
}


/************************************************************************
Stops timer. Status will be displayed as disconnected
************************************************************************/
void MotorPort::StopTimer()
{
	killTimer(maxon_velocity::motionVel->timerId);
	m_oUpdateActive = FALSE;

	UpdateStatus();
}

/************************************************************************
Shows a message box with error description of the input error code
************************************************************************/
BOOL MotorPort::ShowErrorInformation(DWORD p_ulErrorCode)
{
	char* pStrErrorInfo;
	QString strDescription;

	if((pStrErrorInfo = (char*)malloc(100)) == NULL)
	{
		QMessageBox::information(this,"error","Not enough memory to allocate buffer for error information string");

		return FALSE;
	}

	if(VCS_GetErrorInfo(p_ulErrorCode, pStrErrorInfo, 100))
	{
		strDescription = pStrErrorInfo;
		QMessageBox::information(this,"error",strDescription);
		free(pStrErrorInfo);
		return TRUE;
	}
	else
	{
		free(pStrErrorInfo);
		QMessageBox::information(this,"error","Error information can't be read!");

		return FALSE;
	}
}

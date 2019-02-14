#ifndef SENSORWIDGET_H
#define SENSORWIDGET_H

#include <QtGui/QWidget>
#include"Sensor.h"
#include"ui_SensorWidget.h"
#include<QtCore/QTimer>
//#include"mythread.h"

class SensorWidget : public QWidget
{
	Q_OBJECT

public:
	SensorWidget(QWidget *parent );
	~SensorWidget();

	static SensorWidget *ForceGet;
	Sensor sensorf;
	//mythread mt;

	void getForceData();

	//int timerId;
	//void timerEvent(QTimerEvent *e);

private:
	//Ui::SensorWidgetClass ui;

public slots:
	void Open();
	void StartSensor();
	void mystart();
	void StopSensor();
	void mystop();
};

#endif // SENSORWIDGET_H

#pragma once

#include <QWidget>
#include <QSettings>
#include <QVector>
#include "ui_sensorinfowidget.h"

// the type of the sensor
enum sensorType
{
	Temperature, Pressure, Humidity
};

// to save the information of sensors
struct  sensorInfo
{
	QString ssName;		//sensor name
	quint8 chl;			//channel number
	sensorType type;	//sensor type
	quint32 wavRange[2];//rough range of the wavelength
	double k;			//calibration parameter, k
	double b;			//calibration parameter, b
};

class sensorInfoWidget : public QWidget
{
	Q_OBJECT

public:
	sensorInfoWidget(QWidget *parent = Q_NULLPTR);
	~sensorInfoWidget();

	QVector<sensorInfo> *ssInfo = nullptr;

private:
	Ui::sensorInfoWidget ui;
};

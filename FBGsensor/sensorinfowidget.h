#pragma once

#include <QWidget>
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
	sensorType type;	//sensor type
	quint8 chl;			//channel number
	quint32 wavRangeStart;//rough start range of the wavelength
	quint32 wavRangeEnd;	//rough end range of the wavelength
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

public slots:
	void on_sensorAddBtn_clicked();

signals:
	void sensorInfoChanged(QVector<sensorInfo> *info);
private:
	Ui::sensorInfoWidget ui;
};

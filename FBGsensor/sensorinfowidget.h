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
	quint32 wavRangeStart;//rough start range of the wavelength
	quint32 wavRangeEnd;	//rough end range of the wavelength
	double k;			//calibration parameter, k
	double b;			//calibration parameter, b
	quint8 chl;			//channel number
	bool isconnected;	//connecting status
};

class sensorInfoModel :public QAbstractTableModel
{
	Q_OBJECT

public:
	sensorInfoModel( const QVector<sensorInfo> &ssInfo, QObject *parent = nullptr);
	~sensorInfoModel();
	void setInfo(const QVector<sensorInfo> &ssInfo);

	int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
	int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;

	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;

private:
	QVector<sensorInfo> infoTable;
	QStringList Horiheader = QStringList() << QString::fromLocal8Bit("序号") << QString::fromLocal8Bit("类型")
		<< QString::fromLocal8Bit("特征波段") << QString::fromLocal8Bit("标定参数k")
		<< QString::fromLocal8Bit("标定参数b") << QString::fromLocal8Bit("所在通道") << QString::fromLocal8Bit("已连接");

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

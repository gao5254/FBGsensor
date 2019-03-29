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
	double mu[2];			//centering parameter, mu[0] is mean, mu[1] is std
	double a[4];			//calibration parameter, a[0-4] is the 0-4 polynomial coefficients
	int chl;			//channel number
	bool isconnected;	//connecting status
};

class sensorInfoModel :public QAbstractTableModel
{
	Q_OBJECT

public:
	sensorInfoModel(const QVector<sensorInfo> &ssInfo, QObject *parent = nullptr);
	~sensorInfoModel();

	int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
	int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;

	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;

	const QStringList typeList = QStringList() << QString::fromLocal8Bit("温度") << QString::fromLocal8Bit("压力") << QString::fromLocal8Bit("湿度");

	public slots:
	void setInfo(const QVector<sensorInfo> *ssInfo);


private:
	QVector<sensorInfo> infoTable;
	QStringList Horiheader = QStringList() << QString::fromLocal8Bit("类型") << QString::fromLocal8Bit("起始特征波长") << QString::fromLocal8Bit("终止特征波长") 
		<< QString::fromLocal8Bit("中心化参数组") << QString::fromLocal8Bit("标定参数组") 
		<< QString::fromLocal8Bit("所在通道") << QString::fromLocal8Bit("是否连接");

};

class sensorInfoWidget : public QWidget
{
	Q_OBJECT

public:
	sensorInfoWidget(QWidget *parent = Q_NULLPTR);
	~sensorInfoWidget();
	bool getDetectStatus() const;
	void setChannelNum(const QVector<int> &chnl);

	QVector<sensorInfo> *ssInfo = nullptr;
	const QStringList unitList = QStringList() << QString::fromLocal8Bit("℃") << "kPa" << "%RH";


public slots:
	void on_sensorDetectBtn_clicked();

signals:
	void sensorInfoChanged(QVector<sensorInfo> *info) const;
private:
	Ui::sensorInfoWidget ui;
	sensorInfoModel *ssmodel = nullptr;

	bool isdetected = false;
};

#pragma once

#include <QtWidgets/QMainWindow>
#include <QTime>
#include "ui_fbgsensor.h"

//pre-declaration
class SerialPortManager;
class QFile;

class FBGsensor : public QMainWindow
{
	Q_OBJECT

public:
	FBGsensor(QWidget *parent = Q_NULLPTR);
	~FBGsensor();

public slots:
	void on_openDeviceBtn_toggled(bool chk);		//打开设备按钮槽函数，自动连接
	void on_setParaBtn_clicked();			//设置设备参数槽函数，自动连接
	void on_scanBtn_toggled(bool chk);		//start scan 
// 	void on_channelCBBox_currentIndexChanged(int index);		//pass the index to label, and update it
	void on_chnl1Color_clicked();
	void on_chnl2Color_clicked();
	void on_chnl1ChkBox_toggled(bool checked);
	void on_chnl2ChkBox_toggled(bool checked);


//已在designer中连接 	void on_openDeviceAct_triggered();		//打开设备动作槽函数，自动连接
private:
	Ui::FBGsensorClass ui;
	QString portNumber = "COM3";		//存储端口号
	quint32 waveStart = 1527000, waveEnd = 1568000, waveStep = 20, channelNum = 2;		//存储扫描波长起始值，终止值，间隔，单位为pm，通道数
	quint8 currentChannel = 0;		//the currently sampled channel, should be reset on every scan
	SerialPortManager *serialPManager;		//串口管理及通信
	QLabel *statusLabel = nullptr;		//label showing msg in the statusbar
	QTimer *sendMsgTimer = nullptr;		//timer to send msg repeatly when there's no reply
	QVector<quint16> *spectrumData = nullptr;	//a pointer to the data
	QFile *csvfile = nullptr;
	QTime t;

	bool firstSetInfo=true;			//if first set info, change the status of edit and btn
// 	bool scanContinuously = false;		//if the scanning is continuously, resend msg after receiving msg
	bool scanStarted = false;		//to stop the scan procedure

	void msgProcess(QByteArray msg);
	void showDeviceInfo(QByteArray msg);
	void spectrumSample();
	void loadSpectrumData(QByteArray msg);
};

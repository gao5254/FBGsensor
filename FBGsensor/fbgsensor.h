#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_fbgsensor.h"
//pre-declaration
class SerialPortManager;

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

//已在designer中连接 	void on_openDeviceAct_triggered();		//打开设备动作槽函数，自动连接
private:
	Ui::FBGsensorClass ui;
	QString portNumber = "COM3";		//存储端口号
	quint32 waveStart = 1527000, waveEnd = 1568000, waveStep = 20, channelNum = 2;		//存储扫描波长起始值，终止值，间隔，单位为pm，通道数
	quint8 currentChannel = 1;		//the currently sampled channel, should be reset on every scan
	SerialPortManager *serialPManager;		//串口管理及通信
	QLabel *statusLabel;		//label showing msg in the statusbar
	QTimer *sendMsgTimer;		//timer to send msg repeatly when there's no reply
	bool firstSetInfo=true;			//if first set info, change the status of edit and btn
// 	bool scanContinuously = false;		//if the scanning is continuously, resend msg after receiving msg
	bool scanStarted = false;		//to stop the scan procedure

	void msgProcess(QByteArray msg);
	void showDeviceInfo(QByteArray msg);
	void spectrumSample();
};

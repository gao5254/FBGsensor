#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_fbgsensor.h"

class FBGsensor : public QMainWindow
{
	Q_OBJECT

public:
	FBGsensor(QWidget *parent = Q_NULLPTR);
	~FBGsensor();

	public slots:
	void on_openDeviceBtn_clicked();		//打开设备按钮槽函数，自动连接
//已在designer中连接 	void on_openDeviceAct_triggered();		//打开设备动作槽函数，自动连接
	void on_setParaBtn_clicked();			//设置设备参数槽函数，自动连接
private:
	Ui::FBGsensorClass ui;
	QString portnumber = "COM3";		//存储端口号
	quint32 waveStart = 1527000, waveEnd = 1568000, waveStep = 20;		//存储扫描波长起始值，终止值，间隔，单位为pm

};

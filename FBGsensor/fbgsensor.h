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
	void on_openDeviceBtn_toggled(bool chk);		//���豸��ť�ۺ������Զ�����
	void on_setParaBtn_clicked();			//�����豸�����ۺ������Զ�����

//����designer������ 	void on_openDeviceAct_triggered();		//���豸�����ۺ������Զ�����
private:
	Ui::FBGsensorClass ui;
	QString portNumber = "COM3";		//�洢�˿ں�
	quint32 waveStart = 1527000, waveEnd = 1568000, waveStep = 20, channelNum = 2;		//�洢ɨ�貨����ʼֵ����ֵֹ���������λΪpm��ͨ����
	SerialPortManager *serialPManager;		//���ڹ���ͨ��
	QLabel *statusLabel;		//label showing msg in the statusbar
	QTimer *getInfoTimer;		//timer to send "getinfo" msg after "setinfo"
	bool firstSetInfo=true;			//if first set info, change the status of edit and btn
	

	void msgProcess(QByteArray msg);
	void showDeviceInfo(QByteArray msg);
};

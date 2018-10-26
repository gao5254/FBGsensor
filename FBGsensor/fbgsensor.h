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
	void on_openDeviceBtn_toggled(bool chk);		//���豸��ť�ۺ������Զ�����
	void on_setParaBtn_clicked();			//�����豸�����ۺ������Զ�����
	void on_scanBtn_toggled(bool chk);		//start scan 
// 	void on_channelCBBox_currentIndexChanged(int index);		//pass the index to label, and update it
	void on_chnl1Color_clicked();
	void on_chnl2Color_clicked();
	void on_chnl1ChkBox_toggled(bool checked);
	void on_chnl2ChkBox_toggled(bool checked);


//����designer������ 	void on_openDeviceAct_triggered();		//���豸�����ۺ������Զ�����
private:
	Ui::FBGsensorClass ui;
	QString portNumber = "COM3";		//�洢�˿ں�
	quint32 waveStart = 1527000, waveEnd = 1568000, waveStep = 20, channelNum = 2;		//�洢ɨ�貨����ʼֵ����ֵֹ���������λΪpm��ͨ����
	quint8 currentChannel = 0;		//the currently sampled channel, should be reset on every scan
	SerialPortManager *serialPManager;		//���ڹ���ͨ��
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

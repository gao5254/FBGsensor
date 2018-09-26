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
	void on_openDeviceBtn_clicked();		//���豸��ť�ۺ������Զ�����
//����designer������ 	void on_openDeviceAct_triggered();		//���豸�����ۺ������Զ�����
	void on_setParaBtn_clicked();			//�����豸�����ۺ������Զ�����
private:
	Ui::FBGsensorClass ui;
	QString portnumber = "COM3";		//�洢�˿ں�
	quint32 waveStart = 1527000, waveEnd = 1568000, waveStep = 20;		//�洢ɨ�貨����ʼֵ����ֵֹ���������λΪpm

};

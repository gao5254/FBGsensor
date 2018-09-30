#include "fbgsensor.h"
#include <QMessageBox>
#include "serialportmanager.h"


FBGsensor::FBGsensor(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	//initialize member
	serialPManager = new SerialPortManager((QObject*)this);

	//init statusbar
	statusLabel = new QLabel();
	ui.statusBar->addWidget(statusLabel);

	//��ʼ״̬��ĳЩ����򲻿���
	ui.setParaBtn->setEnabled(false);
	ui.wavEndEdit->setEnabled(false);
	ui.wavStartEdit->setEnabled(false);
	ui.wavStepEdit->setEnabled(false);

	//connnect
	connect(serialPManager, &SerialPortManager::msgReceived, this, &FBGsensor::msgProcess);
}

FBGsensor::~FBGsensor()
{

}

/*�����Ҫ�򿪣���
1.��ȡ�˿ں���Ϣ
2.�򿪶˿�
3.���ͻ�ȡ�豸��Ϣ����
4.�ɹ��������������򼰸ı䰴ť״̬
�����Ҫ�رգ���
1.�رմ���
2.�ɹ�����ס��������򼰸ı䰴ť״̬*/
void FBGsensor::on_openDeviceBtn_toggled(bool chk)
{
	if (chk)
	{//��״̬����ȡ�˿ں���Ϣ
		portNumber = ui.serialSpinBox->text();
		if (!serialPManager->openDevice(portNumber))
		{
			//if failed,show warningbox, change the btn checked to false
			statusLabel->setText(QString::fromLocal8Bit("���ڴ�ʧ��"));
			ui.openDeviceBtn->setChecked(false);
			QMessageBox msgBox;
			msgBox.setText(QString::fromLocal8Bit("���ڴ�ʧ�ܣ�"));
			msgBox.exec();
			return;
		}
		//get device info
		serialPManager->getDeviceInfo();
		
	} 
	else
	{
		//close the serialport
		serialPManager->closeDevice();
		//disable other btn and edit
		ui.setParaBtn->setEnabled(false);
		ui.wavEndEdit->setEnabled(false);
		ui.wavStartEdit->setEnabled(false);
		ui.wavStepEdit->setEnabled(false);
		//change text on this btn
		ui.openDeviceBtn->setText(QString::fromLocal8Bit("���豸"));
	}
}

//process the received msg, depending on the 5th bit 
void FBGsensor::msgProcess(QByteArray msg)
{
	switch (msg.at(5))
	{
	case 0x87:
		setDeviceInfo(msg);
		break;
	default:
		break;
	}
}

//set the number, enable the edit, change the text on btn, show success info
void FBGsensor::setDeviceInfo(QByteArray msg)
{
	//set the parameter from msg
	quint16 dStart = SerialPortManager::getNum(msg, 8), dEnd = SerialPortManager::getNum(msg, 10);
	channelNum = (quint32)(msg.at(14));
	waveStart = (quint32)dStart + 1527000;
	waveEnd = (quint32)dEnd + 1527000;
	waveStep = (quint32)SerialPortManager::getNum(msg, 12);
	//show in UI
	ui.wavStartEdit->setText(QString::number((double)waveStart / 1000, 'f', 3));
	ui.wavStartEdit->setText(QString::number((double)waveEnd / 1000, 'f', 3));
	ui.wavStartEdit->setText(QString::number((double)waveStep / 1000, 'f', 3));
	//UNDONE: continue here
}

#include "fbgsensor.h"
#include <QMessageBox>

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

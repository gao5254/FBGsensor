#include "fbgsensor.h"
#include <QMessageBox>
#include <QTimer>
#include "serialportmanager.h"
#include "doublevalidator.h"

FBGsensor::FBGsensor(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	//initialize member
	serialPManager = new SerialPortManager((QObject*)this);
	sendMsgTimer = new QTimer(this);

	//init statusbar
	statusLabel = new QLabel();
	ui.statusBar->addWidget(statusLabel);

	//init combobox
	ui.channelCBBox->addItem(QString::fromLocal8Bit("ȫ��"));
	ui.channelCBBox->addItem(QString::fromLocal8Bit("ͨ��1"));
	ui.channelCBBox->addItem(QString::fromLocal8Bit("ͨ��2"));
	ui.channelCBBox->setCurrentIndex(0);

	//init tabwidget
	ui.tabWidget->setCurrentIndex(0);

	//disable btn and edit and tabwidget, set validator
	ui.setParaBtn->setEnabled(false);

	ui.wavEndEdit->setEnabled(false);
	ui.wavEndEdit->setValidator(new DoubleValidator(1527.000, 1568.000, 3, 1568.000, ui.wavEndEdit));

	ui.wavStartEdit->setEnabled(false);
	ui.wavStartEdit->setValidator(new DoubleValidator(1527.000, 1568.000, 3, 1527.000, ui.wavStartEdit));

	ui.wavStepEdit->setEnabled(false);
	ui.wavStepEdit->setValidator(new DoubleValidator(0.010, 0.040, 3, 0.020, ui.wavStepEdit));

	ui.scanTab->setEnabled(false);

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
// 			statusLabel->setText(QString::fromLocal8Bit("���ڴ�ʧ��"));
			ui.openDeviceBtn->setChecked(false);
			ui.statusBar->showMessage(QString::fromLocal8Bit("���ڴ�ʧ��"), 5000);
			QMessageBox msgBox;
			msgBox.setText(QString::fromLocal8Bit("���ڴ�ʧ�ܣ�"));
			msgBox.exec();
			return;
		}
		//get device info
		serialPManager->getDeviceInfo();
		firstSetInfo = true;
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

//show a asking dialog, set the info, then get the info
void FBGsensor::on_setParaBtn_clicked()
{
	if (QMessageBox::question(this, "FBGsensor", QString::fromLocal8Bit("ȷ��Ҫ�ı��豸������"), QMessageBox::Ok | QMessageBox::Cancel, QMessageBox::Ok)==QMessageBox::Cancel)
	{
		return;
	}
	//get UI text
// 	quint16 dStart = (quint16)((quint64)(ui.wavStartEdit->text().toDouble()*1000.0) - 1527000);
	quint16 dStart = (quint16)(ui.wavStartEdit->text().remove('.').toUInt() - 1527000);
// 	quint16 dEnd = (quint16)((quint64)(ui.wavEndEdit->text().toDouble()*1000.0) - 1527000);
	quint16 dEnd = (quint16)(ui.wavEndEdit->text().remove('.').toUInt() - 1527000);
// 	quint16 dStep = (quint16)(ui.wavStepEdit->text().toFloat()*1000.0);
	quint16 dStep = (quint16)(ui.wavStepEdit->text().remove('.').toUInt());

	//set info
	serialPManager->setDeviceInfo(dStart, dEnd, dStep, channelNum);
	//get device info, in a appropriate time
	sendMsgTimer->disconnect();
	connect(sendMsgTimer, &QTimer::timeout, serialPManager, &SerialPortManager::getDeviceInfo);
	sendMsgTimer->start(100);

}

//disable continuousCheck, change the state depending on continuousCheck, set currentChannel to 1, send msg
void FBGsensor::on_scanBtn_toggled(bool chk)
{
	if (chk)
	{
		ui.continuousCheck->setEnabled(false);
		if (ui.continuousCheck->isChecked())
		{
			//continuously scan
			ui.scanBtn->setText(QString::fromLocal8Bit("ֹͣɨ��"));
		} 
		else
		{
			//discontinuously scan
			ui.scanBtn->setText(QString::fromLocal8Bit("ɨ����"));
		}
		scanStarted = true;
		currentChannel = 1;
		sendMsgTimer->disconnect();
		connect(sendMsgTimer, &QTimer::timeout, serialPManager, &SerialPortManager::scanOnce);
		serialPManager->scanOnce();
		sendMsgTimer->start(2000);
	} 
	else
	{
		scanStarted = false;
		currentChannel = 1;
		ui.scanBtn->setText(QString::fromLocal8Bit("��ʼɨ��"));
		ui.continuousCheck->setEnabled(true);
	}
}

//process the received msg, depending on the 5th bit 
void FBGsensor::msgProcess(QByteArray msg)
{
	switch ((uchar)msg.at(5))
	{
	case 0x87:
		showDeviceInfo(msg);
		break;
	case 0x59:
		spectrumSample();
	default:
		break;
	}
}

//set the number, enable the edit, change the text on btn, show success info
void FBGsensor::showDeviceInfo(QByteArray msg)
{
	//stop the timer first
	if (sendMsgTimer->isActive())
	{
		sendMsgTimer->stop();
	}
	//set the parameter from msg
	quint16 dStart = SerialPortManager::getNum(msg, 8), dEnd = SerialPortManager::getNum(msg, 10);
	channelNum = (quint32)(uchar)(msg.at(14));
	waveStart = (quint32)dStart + 1527000;
	waveEnd = (quint32)dEnd + 1527000;
	waveStep = (quint32)SerialPortManager::getNum(msg, 12);
	//show in UI
	ui.wavStartEdit->setText(QString::number(waveStart).insert(4, '.'));
	ui.wavEndEdit->setText(QString::number(waveEnd).insert(4, '.'));
	ui.wavStepEdit->setText(QString::number(waveStep).rightJustified(4, '0', false));
	if (firstSetInfo)
	{
		//enable the edit
		ui.setParaBtn->setEnabled(true);
		ui.wavEndEdit->setEnabled(true);
		ui.wavStartEdit->setEnabled(true);
		ui.wavStepEdit->setEnabled(true);
		//change the text
		ui.openDeviceBtn->setText(QString::fromLocal8Bit("�ر��豸"));
		firstSetInfo = false;
	}
	//show success info
// 	statusLabel->setText(QString::fromLocal8Bit("�豸�������³ɹ�"));
	ui.statusBar->showMessage(QString::fromLocal8Bit("�豸�������³ɹ�"), 5000);

}

//scan finish, get sample data
void FBGsensor::spectrumSample()
{
	//stop the timer first
	if (sendMsgTimer->isActive())
	{
		sendMsgTimer->stop();
	}
	if (!scanStarted)
	{
		return;
	}
	//TODO: fix the logic
	if (currentChannel > channelNum)
	{
		currentChannel = 1;
		//single scan finish
		if (ui.continuousCheck->isChecked())
		{
			sendMsgTimer->disconnect();
			connect(sendMsgTimer, &QTimer::timeout, serialPManager, &SerialPortManager::scanOnce);
			serialPManager->scanOnce();
			sendMsgTimer->start(2000);

		} 
		else
		{
			ui.scanBtn->setChecked(false);
		}
	}
	else if (ui.channelCBBox->currentIndex() == currentChannel || ui.channelCBBox->currentIndex() == 0)
	{
		currentChannel++;

	}
	else
	{
	}
}

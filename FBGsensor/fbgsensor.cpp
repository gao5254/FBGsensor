#include "fbgsensor.h"
#include <QMessageBox>
#include <QTimer>
#include "serialportmanager.h"


FBGsensor::FBGsensor(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	//initialize member
	serialPManager = new SerialPortManager((QObject*)this);
	getInfoTimer = new QTimer(this);

	//init statusbar
	statusLabel = new QLabel();
	ui.statusBar->addWidget(statusLabel);

	//disable btn and edit, set validator
	ui.setParaBtn->setEnabled(false);

// 	ui.wavEndEdit->setEnabled(false);
	ui.wavEndEdit->setValidator(new QDoubleValidator(1527.000, 1568.000, 3, ui.wavEndEdit));

// 	ui.wavStartEdit->setEnabled(false);
	ui.wavStartEdit->setValidator(new QDoubleValidator(1527.000, 1568.000, 3, ui.wavStartEdit));

// 	ui.wavStepEdit->setEnabled(false);
	ui.wavStepEdit->setValidator(new QDoubleValidator(0.010, 0.040, 3, ui.wavStepEdit));


	//connnect
	connect(serialPManager, &SerialPortManager::msgReceived, this, &FBGsensor::msgProcess);
	connect(getInfoTimer, &QTimer::timeout, serialPManager, &SerialPortManager::getDeviceInfo);
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
	quint16 dStart = (quint16)((quint64)(ui.wavStartEdit->text().toDouble()*1000.0) - 1527000);
	quint16 dStart = (quint16)(ui.wavStartEdit->text().remove('.').toUInt() - 1527000);
	quint16 dEnd = (quint16)((quint64)(ui.wavEndEdit->text().toDouble()*1000.0) - 1527000);
	quint16 dEnd = (quint16)(ui.wavEndEdit->text().remove('.').toUInt() - 1527000);
	quint16 dStep = (quint16)(ui.wavStepEdit->text().toFloat()*1000.0);
	quint16 dStep = (quint16)(ui.wavStepEdit->text().remove('.').toUInt());

	//set info
	serialPManager->setDeviceInfo(dStart, dEnd, dStep, channelNum);
	//get device info, in a appropriate time
	getInfoTimer->start(50);

}

//process the received msg, depending on the 5th bit 
void FBGsensor::msgProcess(QByteArray msg)
{
	switch ((uchar)msg.at(5))
	{
	case 0x87:
		showDeviceInfo(msg);
		break;
	default:
		break;
	}
}

//set the number, enable the edit, change the text on btn, show success info
void FBGsensor::showDeviceInfo(QByteArray msg)
{
	//stop the timer first
	if (getInfoTimer->isActive())
	{
		getInfoTimer->stop();
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

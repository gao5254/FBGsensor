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
	sendMsgTimer = new QTimer(this) ;
	spectrumData = new QVector<quint16> [channelNum];
	for (int i = 0; i < channelNum; i++)
	{
		spectrumData[i].resize((waveEnd - waveStart) / waveStep + 1);
	}

	//init statusbar
	statusLabel = new QLabel();
	ui.statusBar->addWidget(statusLabel);

	//init combobox
	ui.channelCBBox->addItem(QString::fromLocal8Bit("全部"));
	ui.channelCBBox->addItem(QString::fromLocal8Bit("通道1"));
	ui.channelCBBox->addItem(QString::fromLocal8Bit("通道2"));
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
	delete[] spectrumData;
}

/*如果需要打开，则
1.获取端口号信息
2.打开端口
3.发送获取设备信息命令
4.成功后解锁其他输入框及改变按钮状态
如果需要关闭，则
1.关闭串口
2.成功后锁住其他输入框及改变按钮状态*/
void FBGsensor::on_openDeviceBtn_toggled(bool chk)
{
	if (chk)
	{//打开状态，获取端口号信息
		portNumber = ui.serialSpinBox->text();
		if (!serialPManager->openDevice(portNumber))
		{
			//if failed,show warningbox, change the btn checked to false
// 			statusLabel->setText(QString::fromLocal8Bit("串口打开失败"));
			ui.openDeviceBtn->setChecked(false);
			ui.statusBar->showMessage(QString::fromLocal8Bit("串口打开失败"), 5000);
			QMessageBox msgBox;
			msgBox.setText(QString::fromLocal8Bit("串口打开失败！"));
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
		ui.scanTab->setEnabled(false);
		//change text on this btn
		ui.openDeviceBtn->setText(QString::fromLocal8Bit("打开设备"));
	}
}

//show a asking dialog, set the info, then get the info
void FBGsensor::on_setParaBtn_clicked()
{
	if (QMessageBox::question(this, "FBGsensor", QString::fromLocal8Bit("确定要改变设备参数吗？"), QMessageBox::Ok | QMessageBox::Cancel, QMessageBox::Ok)==QMessageBox::Cancel)
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
			ui.scanBtn->setText(QString::fromLocal8Bit("停止扫描"));
		} 
		else
		{
			//discontinuously scan
			ui.scanBtn->setText(QString::fromLocal8Bit("扫描中"));
		}
		scanStarted = true;
		currentChannel = 0;
		sendMsgTimer->disconnect();
		connect(sendMsgTimer, &QTimer::timeout, serialPManager, &SerialPortManager::scanOnce);
		serialPManager->scanOnce();
		sendMsgTimer->start(2000);
	} 
	else
	{
		scanStarted = false;
		currentChannel = 0;
		ui.scanBtn->setText(QString::fromLocal8Bit("开始扫描"));
		ui.continuousCheck->setEnabled(true);
	}
}

//pass the index to label, and update it
void FBGsensor::on_channelCBBox_currentIndexChanged(int index)
{
	ui.showLabel->setIndex(index);
	ui.showLabel->update();
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
		break;
	case 0x58:
		loadSpectrumData(msg);
		break;
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
	quint16 temp = (quint32)(uchar)(msg.at(14));
	if (temp != channelNum)
	{
		//reallocate the spectrumData
		channelNum = temp;
		if (spectrumData != nullptr)
		{
			delete[] spectrumData;
			spectrumData = new QVector<quint16>[channelNum];
		}
		//change the item in channelCBBox, not implement now

	}
	waveStart = (quint32)dStart + 1527000;
	waveEnd = (quint32)dEnd + 1527000;
	waveStep = (quint32)SerialPortManager::getNum(msg, 12);
	for (int i = 0; i < channelNum; i++)
	{
		spectrumData[i].resize((waveEnd - waveStart) / waveStep + 1);
	}
	//set label para
	ui.showLabel->setPara(waveStart, waveEnd, waveStep, channelNum, spectrumData);
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
		ui.scanTab->setEnabled(true);
		//change the text
		ui.openDeviceBtn->setText(QString::fromLocal8Bit("关闭设备"));
		firstSetInfo = false;
	}
	//show success info
// 	statusLabel->setText(QString::fromLocal8Bit("设备参数更新成功"));
	ui.statusBar->showMessage(QString::fromLocal8Bit("设备参数更新成功"), 5000);

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
	if (currentChannel >= (quint8)channelNum)
	{
		//update tht label
		ui.showLabel->update();
		currentChannel = 0;
		//TODO: analyze the data

		//check if continuously
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
	else
	{
		//continue get data in other channels
		serialPManager->getSpectrumData(currentChannel);
	}
}

//extract the AD value from the msg 
void FBGsensor::loadSpectrumData(QByteArray msg)
{
	//if scan is not started, return
	if (!scanStarted)
	{
		return;
	}
	quint8 chnl = msg.at(8);
	int p = 9;
	for (int i = 0; i < spectrumData[chnl].size(); i++)
	{
		spectrumData[chnl][i] = SerialPortManager::getNum(msg, p);
		p += 2;
	}
	//goto spectrumSample to get next channel
	currentChannel++;
	spectrumSample();
}

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

	//初始状态下某些输入框不可用
	ui.setParaBtn->setEnabled(false);
	ui.wavEndEdit->setEnabled(false);
	ui.wavStartEdit->setEnabled(false);
	ui.wavStepEdit->setEnabled(false);
}

FBGsensor::~FBGsensor()
{

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
			statusLabel->setText(QString::fromLocal8Bit("串口打开失败"));
			ui.openDeviceBtn->setChecked(false);
			QMessageBox msgBox;
			msgBox.setText(QString::fromLocal8Bit("串口打开失败！"));
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
		ui.openDeviceBtn->setText(QString::fromLocal8Bit("打开设备"));
	}
}

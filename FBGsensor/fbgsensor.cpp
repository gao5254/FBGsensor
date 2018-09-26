#include "fbgsensor.h"

FBGsensor::FBGsensor(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	//初始状态下某些输入框不可用
	ui.setParaBtn->setEnabled(false);
	ui.wavEndEdit->setEnabled(false);
	ui.wavStartEdit->setEnabled(false);
	ui.wavStepEdit->setEnabled(false);
}

FBGsensor::~FBGsensor()
{
//TODO:
// 如果串口未关闭，关闭串口

}

void FBGsensor::on_openDeviceBtn_clicked()
{
//TODO:
// 按钮和工具栏动作均调用此槽函数
// 1.获取端口号信息
// 2.打开端口
// 3.获取设备信息
// 4.成功后解锁其他输入框及按钮

}

#include "fbgsensor.h"

FBGsensor::FBGsensor(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	//��ʼ״̬��ĳЩ����򲻿���
	ui.setParaBtn->setEnabled(false);
	ui.wavEndEdit->setEnabled(false);
	ui.wavStartEdit->setEnabled(false);
	ui.wavStepEdit->setEnabled(false);
}

FBGsensor::~FBGsensor()
{
//TODO:
// �������δ�رգ��رմ���

}

void FBGsensor::on_openDeviceBtn_clicked()
{
//TODO:
// ��ť�͹��������������ô˲ۺ���
// 1.��ȡ�˿ں���Ϣ
// 2.�򿪶˿�
// 3.��ȡ�豸��Ϣ
// 4.�ɹ��������������򼰰�ť

}

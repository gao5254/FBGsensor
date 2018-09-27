#pragma once

#include <QObject>
//ǰ������
class QSerialPort;
class QByteArray;

/*���������ڣ��������ݵĹ������������յ����ݷ�����һ������*/
class SerialPortManager : public QObject
{
	Q_OBJECT

public:
	SerialPortManager(QObject *parent);
	~SerialPortManager();
	bool openDevice(QString str);
	bool getDeviceInfo();

private:
	QSerialPort *sPort;		//֧�ִ��ڲ���
	QByteArray *arrBuffer,*msgHeader;		//a buffer to receive msg from device, a header to be edited before sending to device
};

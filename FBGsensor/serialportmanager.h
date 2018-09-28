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
	quint16 CRC16(QByteArray arr, int n);		
	bool openDevice(QString str);
	void closeDevice();
	void getDeviceInfo();
public slots:
	void receiveMsg();

private:
	QSerialPort *sPort;		//֧�ִ��ڲ���
	QByteArray *arrBuffer;		//a buffer to receive msg from device, //a header to be edited before sending to device
};

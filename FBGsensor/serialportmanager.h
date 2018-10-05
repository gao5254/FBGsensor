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
	void setDeviceInfo(quint16 dStart, quint16 dEnd, quint16 dStep, quint16 channleNum);
	void scanOnce();
	static quint16 getNum(QByteArray arr, int idx);
	static void setNum(QByteArray &arr, int idx, quint16 num);

public slots:
	void receiveMsg();

signals:
	void msgReceived(QByteArray msg);

private:
	QSerialPort *sPort;		//֧�ִ��ڲ���
	QByteArray *arrBuffer;		//a buffer to receive msg from device, //a header to be edited before sending to device
};

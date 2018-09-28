#pragma once

#include <QObject>
//前置声明
class QSerialPort;
class QByteArray;

/*负责开启串口，传输数据的工作，并将接收的数据返回上一级处理*/
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
	QSerialPort *sPort;		//支持串口操作
	QByteArray *arrBuffer;		//a buffer to receive msg from device, //a header to be edited before sending to device
};

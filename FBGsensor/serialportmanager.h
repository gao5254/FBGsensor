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
	bool openDevice(QString str);
	bool getDeviceInfo();

private:
	QSerialPort *sPort;		//支持串口操作
	QByteArray *arrBuffer,*msgHeader;		//a buffer to receive msg from device, a header to be edited before sending to device
};

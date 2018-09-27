#include "serialportmanager.h"
#include <QtSerialPort/QSerialPort>
#include <QByteArray>


SerialPortManager::SerialPortManager(QObject *parent)
	: QObject(parent)
{
	//初始化成员变量
	sPort = new QSerialPort(this);
	arrBuffer = new QByteArray();
	msgHeader = new QByteArray();
	(*msgHeader)[0] = 0x55;
	(*msgHeader)[1] = 0xAA;
	(*msgHeader)[2] = 0xFF;
	(*msgHeader)[3] = 0xFF;
	(*msgHeader)[4] = 0xFF;

}

SerialPortManager::~SerialPortManager()
{
	//串口未关闭时关闭串口
	if (sPort->isOpen())
	{
		sPort->close();
	}
}

bool SerialPortManager::openDevice(QString str)
{
	//open the "str" serail port
	sPort->setPortName(str);
	return sPort->open(QIODevice::ReadWrite);
}

bool SerialPortManager::getDeviceInfo()
{
	//send "get info "msg to device
	if (!sPort->isOpen())
	{
		return false;
	}

}

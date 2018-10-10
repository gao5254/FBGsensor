#include "serialportmanager.h"
#include <QtSerialPort/QSerialPort>
#include <QByteArray>
#include <QDebug>


SerialPortManager::SerialPortManager(QObject *parent)
	: QObject(parent)
{
	//��ʼ����Ա����
	sPort = new QSerialPort(this);
	arrBuffer = new QByteArray();
// 	msgHeader = &QByteArray::fromHex("55AAFFFFFF");

	//connect
	connect(sPort, &QSerialPort::readyRead, this, &SerialPortManager::receiveMsg);
}

SerialPortManager::~SerialPortManager()
{
	//����δ�ر�ʱ�رմ���
	if (sPort->isOpen())
	{
		sPort->close();
	}
}


/*check the first n+1 bytes of the arr and give the check code (n is the last index of the byte to be checked)
the first two bytes art skiped, here are the algorithm
'1�� ��16λCRC�Ĵ���Ϊʮ������FFFF��
'2�� ��һ��8λ������CRC�Ĵ����ĵ�8λ����򣬰ѽ������CRC�Ĵ�����
'3�� ��CRC�Ĵ�������������һλ����0����λ������Ƴ�λ��
'4�� ������λΪ0���ظ���3�����ٴ���λ����
'    ������λΪ1: CRC�Ĵ�����ʮ��������A001�������?
'5�� �ظ�����3��4��ֱ������8�Σ���������8λ����ȫ�������˴���
'6�� �ظ�����2��5��������һ�����ݴ���
'7�� ���õ���CRC�Ĵ���ֵ��ΪCRC�룬����ʱ����8λ�ȷ��ͣ���8λ����͡�
*/
quint16 SerialPortManager::CRC16(QByteArray arr, int n)
{	

	qint32 crc = 0xFFFFFFFF;
	for (int i = 2; i <= n; ++i)
	{
		qint32 temp = arr.at(i);
		temp &= 0xFF;
		crc ^= temp;
		for (int j = 0; j < 8; ++j)
		{
			int flag = crc & 0x00000001;
			crc = crc >> 1;
			crc &= 0x7FFF;
			if (flag == 1)
			{
				crc ^= 0xA001;
			} 
		}
	}
	if (crc < 0)
	{
		crc -= 0xFFFF0000;
	}
	quint16 A = (quint16)crc;
	return (quint16)crc;
}

//open the "str" serail port
bool SerialPortManager::openDevice(QString str)
{
	sPort->setPortName(str);
	sPort->setBaudRate(115200);
	sPort->setParity(QSerialPort::NoParity);
	sPort->setDataBits(QSerialPort::Data8);
	sPort->setStopBits(QSerialPort::OneStop);
	return sPort->open(QIODevice::ReadWrite);
// 	qDebug() << sPort->baudRate() << sPort->parity() << sPort->dataBits() << sPort->stopBits();
}

//close the serial port
void SerialPortManager::closeDevice()
{
	if (sPort->isOpen())
	{
		sPort->close();
	}
}

//send "get info "msg to device
void SerialPortManager::getDeviceInfo()
{
	if (!sPort->isOpen())
	{
		return ;
	}
	//struct the msg to be sent
// 	static int i= 0;
// 	qDebug() << ++i;
	QByteArray msg = QByteArray::fromHex("55AAFFFFFF");
	msg.resize(10);
	msg[5] = 0x87;
	setNum(msg, 6, 2);
	quint16 crc = CRC16(msg, 7);
	msg[8] = (char)crc;
	msg[9] = (char)(crc >> 8);
	//send the msg
	sPort->write(msg);
}

void SerialPortManager::setDeviceInfo(quint16 dStart, quint16 dEnd, quint16 dStep, quint16 channleNum)
{
	if (!sPort->isOpen())
	{
		return;
	}
	//struct the msg to be sent
	QByteArray msg = QByteArray::fromHex("55AAFFFFFF");
	msg.resize(channleNum * 4 + 7 + 2 + 8);
	msg[5] = 0x63;
	setNum(msg, 6, channleNum * 4 + 7 + 2);
	setNum(msg, 8, dStart);
	setNum(msg, 10, dEnd);
	setNum(msg, 12, dStep);
	msg[14] = (char)channleNum;
	for (int i = 0; i < channleNum;++i)
	{
		setNum(msg, 15 + i * 4, 1000);
		setNum(msg, 17 + i * 4, 400);
	}
	quint16 crc = CRC16(msg, 8 + 7 + channleNum * 4 - 1);
	msg[8 + 7 + channleNum * 4] = (char)crc;
	msg[8 + 7 + channleNum * 4 + 1] = (char)(crc >> 8);
	sPort->write(msg);
}

//send "scan" msg to device
void SerialPortManager::scanOnce()
{
	if (!sPort->isOpen())
	{
		return;
	}
	//struct the msg to be sent
	QByteArray msg = QByteArray::fromHex("55AAFFFFFF");
	msg.resize(10);
	msg[5] = 0x59;
	setNum(msg, 6, 2);
	quint16 crc = CRC16(msg, 7);
	msg[8] = (char)crc;
	msg[9] = (char)(crc >> 8);
	//send the msg
	sPort->write(msg);

}

void SerialPortManager::getSpectrumData(quint8 currentChannel)
{
	if (!sPort->isOpen())
	{
		return;
	}
	//struct the msg to be sent
	QByteArray msg = QByteArray::fromHex("55AAFFFFFF");
	msg.resize(11);
	msg[5] = 0x58;
	setNum(msg, 6, 3);
	msg[8] = currentChannel;
	quint16 crc = CRC16(msg, 8);
	msg[9] = (char)crc;
	msg[10] = (char)(crc >> 8);
	//send the msg
	sPort->write(msg);

}

quint16 SerialPortManager::getNum(QByteArray arr, int idx)
{
		quint16 temp = (quint16)(uchar)(arr.at(idx));
		temp = temp << 8;
		temp += (quint16)(uchar)(arr.at(idx + 1));
		return temp;
}

void SerialPortManager::setNum(QByteArray &arr, int idx, quint16 num)
{
	arr[idx + 1] = (char)num;
	arr[idx] = (char)(num >> 8);
}

//receive msg from device to buffer and decide whether it is complete, if so, emit completed signal and send it in parameter
void SerialPortManager::receiveMsg()
{
	//receive
	arrBuffer->append(sPort->readAll());
	//check for the header
	QByteArray header = QByteArray::fromHex("AA55FFFFFF");
	int idx=0;
	//check continuously
	while ((idx = arrBuffer->indexOf(header,idx)) != -1)
	{
		//if header is found, check if its length is enough
		quint16 l = getNum(*arrBuffer, idx + 6);
		if (idx + 8 + l <= arrBuffer->size())
		{
			//if length is enough, extract the msg array, check CRC16
			QByteArray msg = arrBuffer->mid(idx, l + 8);
			arrBuffer->remove(0, idx + 8 + l);
			quint16 crc = CRC16(msg, 5 + l);
			if (msg.at(6 + l) == (char)(crc) && msg.at(7 + l) == (char)(crc >> 8));
			{
				//send the complete msg
				emit msgReceived(msg);
			}
			idx = 0;
		}
		else
		{
			idx++;
		}
	}
}

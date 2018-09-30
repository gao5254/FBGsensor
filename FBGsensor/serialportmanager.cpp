#include "serialportmanager.h"
#include <QtSerialPort/QSerialPort>
#include <QByteArray>


SerialPortManager::SerialPortManager(QObject *parent)
	: QObject(parent)
{
	//��ʼ����Ա����
	sPort = new QSerialPort(this);
	arrBuffer = new QByteArray();
// 	msgHeader = &QByteArray::fromHex("55AAFFFFFF");

	//connect
	connect(sPort, &QIODevice::readyRead, this, &SerialPortManager::receiveMsg);
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

	quint16 crc = 0xFFFF;
	for (int i = 2; i <= n; ++i)
	{
		crc ^= (quint16)arr.at(i);
		for (int j = 0; j < 8; ++j)
		{
			int flag = crc & 0x0001;
			crc = crc >> 1;
			if (flag == 1)
			{
				crc ^= 0xA001;
			} 
		}
	}
	return crc;
}

//open the "str" serail port
bool SerialPortManager::openDevice(QString str)
{
	sPort->setPortName(str);
	return sPort->open(QIODevice::ReadWrite);
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
	QByteArray msg = QByteArray::fromHex("55AAFFFFFF");
	msg.resize(10);
	msg[5] = 0x87;
	msg[6] = 0;
	msg[7] = 2;
	quint16 crc = CRC16(msg, 7);
	msg[8] = (char)crc;
	msg[9] = (char)(crc >> 8);
	//send the msg
	sPort->write(msg);
}

quint16 SerialPortManager::getNum(QByteArray arr, int idx, int n /*= 2*/)
{
	if (n == 2)
	{
		quint16 temp = (quint16)(arr.at(idx));
		temp = temp << 8;
		temp += (quint16)(arr.at(idx + 1));
		return temp;
	}
	return 0;
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

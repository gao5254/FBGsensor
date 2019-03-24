#include <QVector>
#include "dataprocess.h"

DataProcess::DataProcess(QObject *parent)
	: QObject(parent)
{
}

DataProcess::~DataProcess()
{
}

//set the basic parameter
void DataProcess::setPara(quint32 Sta, quint32 En, quint32 Ste, quint32 chnln, const QVector<quint16> *p)
{
	wStart = Sta;
	wEnd = En;
	wStep = Ste;
	chNum = chnln;
	pData = p;

}

//get the wave peak in the range (rangeStart, rangeEnd), and return the wave
double DataProcess::getPeakWav(quint32 rangeStart, quint32 rangeEnd, quint32 chl) const
{
	Q_ASSERT(rangeStart < rangeEnd);
	Q_ASSERT(rangeStart >= wStart);
	Q_ASSERT(rangeEnd <= wEnd);

	quint32 begeinPos = (rangeStart - wStart) / wStep,
		endPos = (rangeEnd - wStart) / wStep;
	PeakInfo peakInfo = getMainPart(begeinPos, endPos, chl);
	if (peakInfo.length == 0 && peakInfo.pos == 0)
	{
		//no peak, return -1
		return -1;
	}
	double peakPos = findPeak_Centroid(peakInfo, chl);
	return (peakPos + peakInfo.pos) * wStep + wStart;
}

//get the main part from the range (beginPos, endPos) in the chl-th spectrum
//the main part is the part containing the peak and the slope around it 
//find the maximum and then use 30% of the maximum as threshold to extract the  point above the threshold
//the base value is calculated by averaging all the point in the range except 11 points around the peak
DataProcess::PeakInfo DataProcess::getMainPart(quint32 beginPos, quint32 endPos, quint32 chl) const
{
	Q_ASSERT(chl < chNum);
	quint64 total = 0;
	quint32 maxpos = beginPos;
	quint16 peak = pData[chl].at(beginPos);
	//find the max position and sum all the points
	for (int i = beginPos; i <= endPos; i++)
	{
		total += pData[chl].at(i);
		if (peak < pData[chl].at(i))
		{
			peak = pData[chl].at(i);
			maxpos = i;
		}
	}
	quint32 num = endPos - beginPos + 1;
	//subtract some large points from the total sum
	for (int i = qMax(maxpos - 10, beginPos); i <= qMin(maxpos + 30, endPos); ++i)
	{
		total -= pData[chl].at(i);
		num--;
	}
	double ratio = 0.4;
	quint16 base = total / num, thr = (quint16)((peak - base) * ratio) + base;
	//if peak is too close to the base, then there is no peak
	if ((peak - base) < 400)
	{
		return PeakInfo{ 0, 0, thr };
	}

	quint32 pos = maxpos;
	for (pos = maxpos; pData[chl].at(pos) >= thr; pos--)
	{
	}
	quint32 length = maxpos - pos + 1;
	for (length; pData[chl].at(pos + length - 1) >= thr; length ++)
	{
	}
	return PeakInfo{ pos, length, thr};
}

// using center of gravity (subtract threshold)
double DataProcess::findPeak_Centroid(PeakInfo info, quint32 chl) const
{
	QVector<quint16> mainPart = pData[chl].mid(info.pos, info.length);
	quint64 sumxy = 0, sumy = 0;
	for (int i = 0; i < mainPart.size(); i++)
	{
		sumxy += i * (mainPart.at(i) - info.threshold);
		sumy += (mainPart.at(i) - info.threshold);
	}
	return (double)sumxy / sumy;
}

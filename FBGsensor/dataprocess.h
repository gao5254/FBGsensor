#pragma once

#include <QObject>

//to process the spectrum data, get the peak wavelength or the offset of peak
class DataProcess : public QObject
{
	Q_OBJECT

	struct PeakInfo
	{
		quint32 pos;
		quint32 length;
	};


public:
	DataProcess(QObject *parent);
	~DataProcess();
	void setPara(quint32 Sta, quint32 En, quint32 Ste, quint32 chnln, const QVector<quint16> *p);
	double getPeakWav(quint32 rangeStart, quint32 rangeEnd, quint32 chl) const;

private:
	quint32 wStart = 1527000, wEnd = 1568000, wStep = 20, chNum = 2;
	const QVector<quint16> *pData = nullptr;

	struct PeakInfo getMainPart(quint32 beginPos, quint32 endPos, quint32 chl) const ;
	double findPeak_Centroid(PeakInfo info, quint32 chl) const;

};

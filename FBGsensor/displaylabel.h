#pragma once

#include <QLabel>
#include "ui_displaylabel.h"

class displayLabel : public QLabel
{
	Q_OBJECT

public:
	displayLabel(QWidget *parent = Q_NULLPTR);
	~displayLabel();
	void setPara(quint32 Sta, quint32 En, quint32 Ste, quint32 n, QVector<quint16> *p);
	void setIndex(int index);

private:
	Ui::displayLabel ui;
	quint32 wStart = 1527000, wEnd = 1568000, wStep = 20, chNum = 2, chToShow = 0;
	quint32 xBegin = 1527000, xEnd = 1568000, yBegin = 0, yEnd = 4096;
	QVector<quint16> *pData = nullptr;

	void paintEvent(QPaintEvent *);
	void drawCoordinateSys(QPainter *p);
};

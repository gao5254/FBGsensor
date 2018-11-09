#pragma once

#include <QLabel>
#include <QImage>
#include "ui_displaylabel.h"

class displayLabel : public QLabel
{
	Q_OBJECT

public:
	displayLabel(QWidget *parent = Q_NULLPTR);
	~displayLabel();
	void setPara(quint32 Sta, quint32 En, quint32 Ste, quint32 n, QVector<quint16> *p);
	void setChnnelInfo(int index, bool b, QColor c = QColor::Invalid);
	void rePaintImage();

private:
	Ui::displayLabel ui;
	quint32 wStart = 1527000, wEnd = 1568000, wStep = 20, chNum = 2;
	quint32 xBegin = 1527000, xEnd = 1568000, yBegin = 0, yEnd = 4096;
	quint32 cursorPos = 0;
	int axisGap = 50;		//the gap between the axis and the border line
	bool iszooming;			//the flag when the mouse pressed down
	QVector<quint16> *pData = nullptr;
	QImage img;
	QPoint startPoint, endPoint;
	QTransform imgTransform;
	struct  
	{
		bool checked = true;
		QColor lineColor;
	}channelInfo[2];

	void drawCoordinateSys(QPainter *p);
	void transformCoordinateSys(QPainter *p);
	void drawDataLines(QPainter *p);
	int getXInterval();
	int getYInterval();


protected:
	void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
	void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
	void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
// 	void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
	void keyReleaseEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
	void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;
	void paintEvent(QPaintEvent *) Q_DECL_OVERRIDE;

};

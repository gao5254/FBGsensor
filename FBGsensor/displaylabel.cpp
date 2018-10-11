#include "displaylabel.h"
#include <QPainter>

displayLabel::displayLabel(QWidget *parent)
	: QLabel(parent)
{
	ui.setupUi(this);
}

displayLabel::~displayLabel()
{
}

// set parameter into this class, keep same with main window
void displayLabel::setPara(quint32 Sta, quint32 En, quint32 Ste, quint32 n, QVector<quint16> *p)
{
	wStart = Sta;
	wEnd = En;
	wStep = Ste;
	n = chNum;
	pData = p;
}

void displayLabel::setIndex(int index)
{
	chToShow = index;
}

//to paint the coordinate system and curve line on the label 
void displayLabel::paintEvent(QPaintEvent *e)
{
	QPainter *painter = new QPainter(this);

	//paint the coordinate system

}

//draw the coordinate system, including the x-axis and y-axis, and dashline in the area
void displayLabel::drawCoordinateSys(QPainter *p)
{
	QPen oriPen = p->pen();
	QPen axisPen;

}

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
	painter->setRenderHint(QPainter::Antialiasing);

	//paint the coordinate system
	drawCoordinateSys(painter);
}

//draw the coordinate system, including the x-axis and y-axis, and dashline in the area
void displayLabel::drawCoordinateSys(QPainter *p)
{
	p->resetTransform();		//reset all transforms
	QPen oriPen = p->pen();
	//draw the main axis
	QPen axisPen;
	axisPen.setWidth(3);		//bold line to draw axis
	p->setPen(axisPen);
	p->drawLine(axisGap >> 1, this->height() - axisGap, this->width() - axisGap >> 1, this->height() - axisGap);	//x axis
	p->drawLine(axisGap, this->height() - axisGap >> 1, axisGap, axisGap >> 1);		//y axis
	int xinter = getXInterval(), yinter = getYInterval();
	//draw the dashline 
	QPen dashPen;
	dashPen.setStyle(Qt::DashLine);
	dashPen.setBrush(Qt::darkGray);
	p->setPen(dashPen);
	//vertical line
	int offsetx, offsety;
	offsetx = xinter == 500 ? (xBegin / 500 + 1) * 500 : (xBegin / 1000 + 1) * 1000;
	for (offsetx; offsetx < xEnd; offsetx += xinter)
	{
		//TODO: continue
		p->drawLine(axisGap + (offsetx - xBegin, this->height() - axisGap, axisGap, axisGap >> 1);		

	}
}

//transform the coordinate system according to the x_begin, x_end, y_begin, y_end
void displayLabel::transformCoordinateSys(QPainter *p)
{

}

//get the x axis interval(display scale) for dash-line drawing, depending on x_begin and x_end
int displayLabel::getXInterval()
{
	int dis = xBegin - xEnd;
	//if distance less than 500, internal is 500
	if (dis <5000)
	{
		return 500;
	}
	//the default step is 6 or 7, test which interval is more close to the nearest 1000*n number
	int inter6 = dis / 6, inter7 = dis / 7;
	if (qMin(inter6 % 1000, 1000 - inter6 % 1000) <= qMin(inter7 % 1000, 1000 - inter7 % 1000))
	{
		return (inter6 + 500) / 1000 * 1000;
	} 
	else
	{
		return (inter7 + 500) / 1000 * 1000;
	}
}

//get the y axis interval(display) for dash-line drawing, depending on y_begin and y_end
int displayLabel::getYInterval()
{
	int dis = yBegin - yEnd;
	//if distance less than 500, internal is 500
	if (dis < 500)
	{
		return 50;
	}
	//the default step is 6 or 7, test which interval is more close to the nearest 1000*n number
	int inter6 = dis / 6, inter7 = dis / 7;
	if (qMin(inter6 % 100, 100 - inter6 % 100) <= qMin(inter7 % 100, 100 - inter7 % 100))
	{
		return (inter6 + 50) / 100 * 100;
	}
	else
	{
		return (inter7 + 50) / 100 * 100;
	}
}

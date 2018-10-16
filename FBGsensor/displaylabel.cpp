#include "displaylabel.h"
#include <QPainter>
#include <QDebug>

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

	xBegin = wStart, xEnd = wEnd, yBegin = 0, yEnd = 4096;
}

void displayLabel::setIndex(int index)
{
	chToShow = index;
}

//to paint the coordinate system and curve line on the label 
void displayLabel::paintEvent(QPaintEvent *e)
{
	setFrameStyle(QFrame::Box);

	QPainter *painter = new QPainter(this);
	painter->setRenderHint(QPainter::Antialiasing);

	//transform the painter coordinate system
	transformCoordinateSys(painter);

	//paint the coordinate system
	drawCoordinateSys(painter);


}

//draw the coordinate system, including the x-axis and y-axis, and dashline in the area
void displayLabel::drawCoordinateSys(QPainter *p)
{
	//TODO:fix this
	p->save();		//save the transform
	p->resetTransform();	//reset the transform
	qDebug() << p->transform();

	QPen oriPen = p->pen();		//store the original pen
	
	//draw the main axis
	QPen axisPen;
	axisPen.setWidth(3);		//bold line to draw axis
	p->setPen(axisPen);
	p->drawLine(axisGap >> 1, this->height() - axisGap, this->width() - (axisGap >> 1), this->height() - axisGap);	//x axis
	p->drawLine(axisGap, this->height() - (axisGap >> 1), axisGap, axisGap >> 1);		//y axis

	//draw the dashline 
	p->restore();			//restore the transform
	int xinter = getXInterval(), yinter = getYInterval();
// 	int xlen = this->width() - axisGap - (axisGap >> 1), ylen = this->height() - axisGap - (axisGap >> 1);		//the display length in the widget
// 	int xdis = xBegin - xEnd, ydis = yBegin - yEnd;		//the actual distance of the data
	QPen dashPen;
	dashPen.setStyle(Qt::DashLine);
	dashPen.setBrush(Qt::darkGray);
	p->setPen(dashPen);
	//vertical line
	for (int offsetx = xinter; offsetx < xEnd - xBegin; offsetx += xinter)
	{
		
		p->drawLine(offsetx, 0, offsetx, yEnd - yBegin);		

	}
	p->drawLine(xEnd - xBegin, 0, xEnd - xBegin, yEnd - yBegin);

	p->setPen(oriPen);		//reset the original pen

}

//transform the coordinate system according to the x_begin, x_end, y_begin, y_end
void displayLabel::transformCoordinateSys(QPainter *p)
{

	p->translate(axisGap, this->height() - axisGap);
// 	p->translate(0 - (qint32)xBegin, 0 - (qint32)yBegin);
	p->scale((qreal)(this->width() - axisGap - (axisGap >> 1)) / (qreal)(xEnd - xBegin), (qreal)(axisGap + (axisGap >> 1) - this->height()) / (yEnd - yBegin));
}

//get the x axis interval(display scale) for dash-line drawing, depending on x_begin and x_end
int displayLabel::getXInterval()
{
	int dis = xEnd - xBegin;
// 	//if distance less than 500, internal is 500
// 	if (dis <5000)
// 	{
// 		return 500;
// 	}
// 	//the default step is 6 or 7, test which interval is more close to the nearest 1000*n number
// 	int inter6 = dis / 6, inter7 = dis / 7;
// 	if (qMin(inter6 % 1000, 1000 - inter6 % 1000) <= qMin(inter7 % 1000, 1000 - inter7 % 1000))
// 	{
// 		return (inter6 + 500) / 1000 * 1000;
// 	} 
// 	else
// 	{
// 		return (inter7 + 500) / 1000 * 1000;
// 	}
// 	if (dis < 10)
// 	{
// 		return 1;
// 	} 
// 	else if (dis < 20)
// 	{
// 		return 2;
// 	}
// 	else if (dis < 50)
// 	{
// 		return 5;
// 	} 
// 	else if (dis < 100)
// 	{
// 		return 10;
// 	}
// 	else if (dis < 200)
// 	{
// 		return 20;
// 	}
// 	else if (dis < 500)
// 	{
// 		return 50;
// 	}
// 	else if (dis < 1000)
// 	{
// 		return 100;
// 	}
// 	else if (dis < 2000)
// 	{
// 		return 200;
// 	}
// 	else if (dis < 5000)
// 	{
// 		return 500;
// 	}
// 	else if (dis < 10000)
// 	{
// 		return 1000;
// 	}
// 	else if (dis < 20000)
// 	{
// 		return 2000;
// 	}
// 	else 
// 	{
// 		return 5000;
// 	}

	//the interval depends on the total distance, if dis is less than a number, then the interval is number/10
	int arr[] = { 10, 20, 50, 100, 200, 500, 1000, 2000, 5000, 10000, 20000, 50000 };
	int i = 0;
	for (i = 0; dis >= arr[i]; i++);
	return arr[i] / 10;

}

//get the y axis interval(display) for dash-line drawing, depending on y_begin and y_end
int displayLabel::getYInterval()
{
	int dis = yEnd - yBegin;
// 	//if distance less than 500, internal is 500
// 	if (dis < 500)
// 	{
// 		return 50;
// 	}
// 	//the default step is 6 or 7, test which interval is more close to the nearest 1000*n number
// 	int inter6 = dis / 6, inter7 = dis / 7;
// 	if (qMin(inter6 % 100, 100 - inter6 % 100) <= qMin(inter7 % 100, 100 - inter7 % 100))
// 	{
// 		return (inter6 + 50) / 100 * 100;
// 	}
// 	else
// 	{
// 		return (inter7 + 50) / 100 * 100;
// 	}
// 	if (dis < 10)
// 	{
// 		return 1;
// 	}
// 	else if (dis < 20)
// 	{
// 		return 2;
// 	}
// 	else if (dis < 50)
// 	{
// 		return 5;
// 	}
// 	else if (dis < 100)
// 	{
// 		return 10;
// 	}
// 	else if (dis < 200)
// 	{
// 		return 20;
// 	}
// 	else if (dis < 500)
// 	{
// 		return 50;
// 	}
// 	else if (dis < 1000)
// 	{
// 		return 100;
// 	}
// 	else if (dis < 2000)
// 	{
// 		return 200;
// 	}
// 	else 
// 	{
// 		return 500;
// 	}

	//the interval depends on the total distance
	int arr[] = { 10, 20, 50, 100, 200, 500, 1000, 2000, 5000 };
	int i = 0;
	for (i = 0; dis >= arr[i]; i++);
	return arr[i] / 10;

}

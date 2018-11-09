#include "displaylabel.h"
#include <QPainter>
#include <QRect>
#include <QPaintEvent>
#include <QDebug>

displayLabel::displayLabel(QWidget *parent)
	: QLabel(parent)
{
	ui.setupUi(this);
	//init the img
	img = QImage(this->size(), QImage::Format_RGB32);
	img.fill(Qt::white);
	rePaintImage();

	//init the channelInfo
	channelInfo[0].lineColor = QColor(Qt::blue);
	channelInfo[1].lineColor = QColor(Qt::red);


	setAttribute(Qt::WA_OpaquePaintEvent);		//all the area will be painted opaquely
	setFocusPolicy(Qt::StrongFocus);			//receive keyboard press
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
	rePaintImage();
	update();
}

//change the channel to show
void displayLabel::setChnnelInfo(int index, bool b, QColor c /*= QColor::Invalid*/)
{
// 	chToShow = index;
	channelInfo[index].checked = b;
	if (c.isValid())
	{
		channelInfo[index].lineColor = c;
	}
	rePaintImage();
	update();
}

//if some parameter change, repaint the img
void displayLabel::rePaintImage()
{
	//check if size changes
	if (this->size() != img.size())
	{
		img = img.scaled(this->size(), Qt::IgnoreAspectRatio);
	}
	img.fill(Qt::white);

	QPainter painter(&img);

	//transform the painter coordinate system
	transformCoordinateSys(&painter);

	//paint the coordinate system
	drawCoordinateSys(&painter);

	//paint lines on img
	drawDataLines(&painter);
}

//when the mouse pressed, record the position
void displayLabel::mousePressEvent(QMouseEvent *event) 
{
	if (event->button() == Qt::LeftButton)
	{
		startPoint = event->pos();
		endPoint = event->pos();
		iszooming = true;
	}
}

//when the mouse move, check if the distance is greater than 10,then draw a rectangle
void displayLabel::mouseMoveEvent(QMouseEvent *event) 
{
	if ((event->buttons() & Qt::LeftButton) && iszooming)
	{
		endPoint = event->pos();
		QPoint distance = startPoint - endPoint;
		if (distance.manhattanLength() > 10)
		{
// 			update(QRect(startPoint, endPoint).normalized().adjusted(-5, -5, 5, 5));
			update();
		}
	}
}

//when the mouse release, if the distance is small, draw a cursor, else zoom the image
void displayLabel::mouseReleaseEvent(QMouseEvent *event) 
{
	if (event->button() == Qt::LeftButton && iszooming)
	{
		endPoint = event->pos();
		iszooming = false;
		if ((startPoint - endPoint).manhattanLength() > 10)
		{
			//transform the rect
			QRect zoomRect = imgTransform.inverted().mapRect(QRect(startPoint, endPoint)).normalized();
			//constrain the rect
			zoomRect.setX(qMax(0, zoomRect.x()));
			zoomRect.setY(qMax(0, zoomRect.y()));
// 			zoomRect.setWidth(qMin((this->width() - axisGap - (axisGap >> 1) - zoomRect.x()), zoomRect.width()));
// 			zoomRect.setHeight(qMin((this->height() - axisGap - (axisGap >> 1) - zoomRect.y()), zoomRect.height()));
			//zoom the image
			double xFactor = (double)(xEnd - xBegin) / (this->width() - axisGap - (axisGap >> 1)),
				yFactor = (double)(yEnd - yBegin) / (this->height() - axisGap - (axisGap >> 1));
			xEnd = qMin((quint32)((zoomRect.x() + zoomRect.width()) * xFactor + xBegin), xEnd);
			xBegin = zoomRect.x() * xFactor + xBegin;
			yEnd = qMin((quint32)((zoomRect.y() + zoomRect.height()) * yFactor + yBegin), yEnd);
			yBegin = zoomRect.y() * yFactor + yBegin;
			// repaint the img
			rePaintImage();
			update();
		} 
		else
		{
			//set the cursor position
// 			qint32 preX = (qint32)(cursorPos - xBegin) * (this->width() - axisGap - (axisGap >> 1)) / (xEnd - xBegin);
// 			imgTransform.map(preX, 0, &preX, nullptr);
			endPoint = imgTransform.inverted().map(endPoint);
			quint32 temp= endPoint.x() * (xEnd - xBegin) / (this->width() - axisGap - (axisGap >> 1)) ;
			cursorPos = ((temp + xBegin -wStart) / wStep) * wStep + wStart;
// 			update(preX - 5, 0, 10, this->height());
			update();
		}
	}
}

//process keyboard press event
// void displayLabel::keyPressEvent(QKeyEvent *event) 
// {
// 	switch (event->key())
// 	{
// 	default:
// 		QLabel::keyReleaseEvent(event);
// 		break;
// 	}
// 
// }

//process space and enter to reset, and left/right/PageUp/PageDown to move cursor
void displayLabel::keyReleaseEvent(QKeyEvent *event) 
{
	switch (event->key())
	{
	case Qt::Key_Space:
	case Qt::Key_Enter:
	case Qt::Key_Return:
		xBegin = wStart;
		xEnd = wEnd;
		yBegin = 0;
		yEnd = 4096;
		rePaintImage();
		update();
		break;
	case Qt::Key_Left:
		if (cursorPos >= xBegin && cursorPos <= xEnd)
		{
			cursorPos -= wStep;
		}
		update();
		break;
	case Qt::Key_Right:
		if (cursorPos >= xBegin && cursorPos <= xEnd)
		{
			cursorPos += wStep;
		}
		update();
		break;
	case Qt::Key_PageDown:
		if (cursorPos >= xBegin && cursorPos <= xEnd)
		{
			cursorPos += wStep << 4;
		}
		update();
		break;
	case Qt::Key_PageUp:
		if (cursorPos >= xBegin && cursorPos <= xEnd)
		{
			cursorPos -= wStep << 4;
		}
		update();
	default:
		QLabel::keyReleaseEvent(event);
		break;
	}
}

void displayLabel::resizeEvent(QResizeEvent *event) 
{
	rePaintImage();

	QWidget::resizeEvent(event);
}

//to paint the coordinate system and curve line on the label 
void displayLabel::paintEvent(QPaintEvent *event)
{

	QPainter painter(this);
	QRect dRect = event->rect();
	QPen oriPen = painter.pen();

// 	qDebug() << dRect;
	painter.setRenderHint(QPainter::Antialiasing);
	painter.drawImage(dRect, img, dRect);

	//draw the zooming rectangle
	if (iszooming && ((startPoint - endPoint).manhattanLength() >11))
	{
		QPen recPen;
		recPen.setStyle(Qt::DashLine);
		painter.setPen(recPen);
		painter.drawRect(QRect(startPoint, endPoint).normalized());
		painter.setPen(oriPen);

	}
	
	//draw the cursor 
	if (cursorPos > xBegin && cursorPos < xEnd)
	{
		QPen cursorPen;
		cursorPen.setWidth(1);
		painter.setPen(cursorPen);
		QFont oriFont = painter.font();
		QFont numFont(oriFont);
		numFont.setPointSize(9);
		numFont.setFamily(QString::fromLocal8Bit("Î¢ÈíÑÅºÚ,ºÚÌå"));
		painter.setFont(numFont);

		int xPos = (cursorPos - xBegin) * (this->width() - axisGap - (axisGap >> 1)) / (xEnd - xBegin);
		for (int i = 0; i < chNum;i++)
		{
			if (channelInfo[i].checked)
			{
				quint16 yData = pData[i].at((cursorPos - wStart) / wStep);
				if (yData > yBegin && yData < yEnd)
				{
					quint32 yPos = (yData - yBegin) * (this->height() - axisGap - (axisGap >> 1)) / (yEnd - yBegin);
					QPoint pnt(xPos, yPos);
					pnt = imgTransform.map(pnt);
					painter.drawText(pnt + QPoint(5, -5), QString("CH%1:(%2,%3)").arg(i + 1).arg(cursorPos).arg(yData));
				}
			}
		}
		painter.setTransform(imgTransform);
		painter.drawLine(xPos, 0, xPos, (this->height() - axisGap - (axisGap >> 1)));
		painter.resetTransform();

		painter.setPen(oriPen);
		painter.setFont(oriFont);
	}

}

//draw the coordinate system, including the x-axis and y-axis, and dashline in the area
void displayLabel::drawCoordinateSys(QPainter *p)
{

	QPen oriPen = p->pen();		//store the original pen
	QFont oriFont = p->font();

	//draw the main axis
	QPen axisPen;
	axisPen.setWidth(3);		//bold line to draw axis
	p->setPen(axisPen);
	p->drawLine(0 - axisGap >> 1, 0, img.width() - axisGap - (axisGap >> 1),0);	//x axis
	p->drawLine(0, img.height() - axisGap - (axisGap >> 1), 0, 0);		//y axis

	//draw the dashline 
	int xinter = getXInterval(), yinter = getYInterval();
// 	int xlen = this->width() - axisGap - (axisGap >> 1), ylen = this->height() - axisGap - (axisGap >> 1);		//the display length in the widget
// 	int xdis = xBegin - xEnd, ydis = yBegin - yEnd;		//the actual distance of the data
	double xFactor = (img.width() - axisGap - (axisGap >> 1)) / (double)(xEnd - xBegin), 
		yFactor = (img.height() - axisGap - (axisGap >> 1)) / (double)(yEnd - yBegin);
	QPen dashPen;
	dashPen.setStyle(Qt::DashLine);
	dashPen.setBrush(Qt::darkGray);
	p->setPen(dashPen);
	//vertical line
	for (int offsetx = xinter; offsetx < (xEnd - xBegin); offsetx += xinter)
	{
		//draw vertical line 
		p->drawLine(offsetx * xFactor, 0, offsetx * xFactor, img.height() - axisGap - (axisGap >> 1));		
// 		p->drawText(offsetx * xFactor - 5, -5, QString::number(xBegin + offsetx).insert(4, '.'));
	}
	//horizontal line
	for (int offsety = yinter; offsety < (yEnd - yBegin); offsety += yinter)
	{
		//draw horizontal line 
		p->drawLine(0, offsety * yFactor, img.width() - axisGap - (axisGap >> 1), offsety * yFactor);
	}


	//draw numbers
	p->setPen(oriPen);		//reset the original pen
	QFont numFont(oriFont);
	numFont.setPointSize(10);
	numFont.setFamily(QString::fromLocal8Bit("Î¢ÈíÑÅºÚ,ºÚÌå"));
	p->setFont(numFont);

	p->save();		//save the transform
	p->scale(1, -1);
	for (int offsetx = 0; offsetx < (xEnd - xBegin); offsetx += xinter)
	{
		//draw x axis number 
		p->drawText(offsetx * xFactor - 30, 15, QString::number(xBegin + offsetx));
	}
	for (int offsety = 0; offsety < (yEnd - yBegin); offsety += yinter)
	{
		//draw y axis number 
		p->drawText(-35, -offsety * yFactor - 2, QString::number(yBegin +offsety));
	}
// 	p->drawText(-40, -((int)yEnd - (int)yBegin) * yFactor + 5, QString::number(yEnd));
// 	p->drawText((xEnd - xBegin) * xFactor - 20, 10, QString::number(xEnd));

	//draw titles
	numFont.setPointSize(12);
// 	numFont.setBold(true);
	p->setFont(numFont);
	p->drawText((this->width() - axisGap * 2) >> 1, 35, QString::fromLocal8Bit("²¨³¤/pm"));
	p->drawText(-15, -(this->height() - axisGap - 20), QString::fromLocal8Bit("ADÖµ"));
	
	p->restore();
	p->setFont(oriFont);	//reset the original font

}

//transform the coordinate system according to the x_begin, x_end, y_begin, y_end
void displayLabel::transformCoordinateSys(QPainter *p)
{

	p->translate(axisGap, img.height() - axisGap);
	p->scale(1, -1);
	imgTransform = p->transform();
// 	p->translate(0 - (qint32)xBegin, 0 - (qint32)yBegin);
// 	p->scale((qreal)(img.width() - axisGap - (axisGap >> 1)) / (qreal)(xEnd - xBegin), (qreal)(axisGap + (axisGap >> 1) - img.height()) / (yEnd - yBegin));
}

//draw the lines from the data
void displayLabel::drawDataLines(QPainter *p)
{
	if (pData == nullptr)
	{
		return;
	}
	QPen oriPen = p->pen();
	//decide which channel to draw 
	for (int i = 0; i < chNum;i++)
	{
		if (!channelInfo[i].checked)
		{
			continue;
		}
		QPen linePen(channelInfo[i].lineColor, 2);
		p->setPen(linePen);
		QPolygonF polygon;
		double xFactor = (img.width() - axisGap - (axisGap >> 1)) / (double)(xEnd - xBegin), 
			yFactor = (img.height() - axisGap - (axisGap >> 1)) / (double)(yEnd - yBegin);
		int index = (xBegin - wStart) / wStep;
		for (int offsetx = index * wStep + wStart - xBegin; offsetx <= (int)(xEnd - xBegin); offsetx += wStep)
		{
			polygon << QPointF(offsetx * xFactor, (pData[i].at(index) - yBegin) * yFactor);
			index++;
		}
		p->drawPolyline(polygon);
	}
	p->setPen(oriPen);
}

//get the x axis interval(display scale) for dash-line drawing, depending on x_begin and x_end
int displayLabel::getXInterval()
{
	int dis = xEnd - xBegin;

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

	//the interval depends on the total distance
	int arr[] = { 10, 20, 50, 100, 200, 500, 1000, 2000, 5000 };
	int i = 0;
	for (i = 0; dis >= arr[i]; i++);
	return arr[i] / 10;

}

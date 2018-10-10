#include "displaylabel.h"

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

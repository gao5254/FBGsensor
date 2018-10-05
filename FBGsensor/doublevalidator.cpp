#include "doublevalidator.h"

DoubleValidator::DoubleValidator(QObject *parent)
	: QDoubleValidator(parent)
{
}

DoubleValidator::DoubleValidator(double bottom, double top, int decimals, double ori, QObject *parent /*= Q_NULLPTR*/)
: QDoubleValidator(bottom, top, decimals, parent)
{
	origin = ori;
}

DoubleValidator::~DoubleValidator()
{
}

void DoubleValidator::fixup(QString &input) const
{
	input = QString::number(origin, 'f', 3);
}

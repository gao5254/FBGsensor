#pragma once

#include <QDoubleValidator>

class DoubleValidator : public QDoubleValidator
{
	Q_OBJECT

public:
	DoubleValidator(QObject *parent);
	DoubleValidator(double bottom, double top, int decimals, double ori, QObject *parent = Q_NULLPTR);
	~DoubleValidator();
	void fixup(QString &input) const;
private:
	double origin;
};

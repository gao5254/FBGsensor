#include "fbgsensor.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	FBGsensor w;
	w.show();
	return a.exec();
}

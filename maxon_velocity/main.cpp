#include "maxon_velocity.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	maxon_velocity w;
	w.show();
	return a.exec();
}

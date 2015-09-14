#include <QtGui>
#include <qapplication.h>
#include "orbit_wdg.h"
#include "ui_untitled.h"
// Include header files for application components.
// ...

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	OrbitCalcForm  form;// = new OrbitCalcForm();
	form.show();
	//form->show();
	// Set up and show widgets.
	// ...
	return app.exec();
}

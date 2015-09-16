#include <QtGui>
#include <qapplication.h>
#include "orbit_wdg.h"
// Include header files for application components.
// ...

int amain(int argc, char *argv[])
{
	QApplication app(argc, argv);

	OrbitCalcForm  form;// = new OrbitCalcForm();
	form.show();
	//form->show();
	// Set up and show widgets.
	// ...
	return app.exec();
}

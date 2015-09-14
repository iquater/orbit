#include "ui_orbit_wdg.h"
#include "maneuver_scheme.h"
#include "memory.h"

class OrbitCalcForm : public QWidget
{
	Q_OBJECT
	Ui::orbit * orbit_wdg;
	CEllepticalOrbit * init_earth_orbit;
	CEllepticalOrbit * target_earth_orbit;
public:
	OrbitCalcForm() :orbit_wdg(new Ui::orbit), init_earth_orbit(0x0), target_earth_orbit(0x0)
	{ 
		orbit_wdg->setupUi(this);
		/*orbit_wdg->edt_init_peri->setEnabled(true);
		orbit_wdg->edt_init_peri->setText("6600");
		orbit_wdg->edt_init_apo->setEnabled(true);
		orbit_wdg->edt_init_apo->setText("6600");

		orbit_wdg->edt_init_ascnode->setEnabled(true);
		orbit_wdg->edt_init_ascnode->setText("0");
		orbit_wdg->edt_init_periarg->setEnabled(true);
		orbit_wdg->edt_init_periarg->setText("0");
		orbit_wdg->edt_init_inclination->setEnabled(true);
		orbit_wdg->edt_init_inclination->setText("0");

		orbit_wdg->edt_target_peri->setEnabled(true);
		orbit_wdg->edt_target_peri->setText("6600");
		orbit_wdg->edt_target_apo->setEnabled(true);
		orbit_wdg->edt_target_apo->setText("6600");

		orbit_wdg->edt_target_ascnode->setEnabled(true);
		orbit_wdg->edt_target_ascnode->setText("0");
		orbit_wdg->edt_target_periarg->setEnabled(true);
		orbit_wdg->edt_target_periarg->setText("0");
		orbit_wdg->edt_target_inclination->setEnabled(true);
		orbit_wdg->edt_target_inclination->setText("0");*/

		connect(orbit_wdg->edt_init_peri, SIGNAL(textChanged(const QString&)), this, SLOT(OnEdit(const QString&)));
		connect(orbit_wdg->edt_init_apo, SIGNAL(textChanged(const QString&)), this, SLOT(OnEdit(const QString&)));
		connect(orbit_wdg->edt_init_ascnode, SIGNAL(textChanged(const QString&)), this, SLOT(OnEdit(const QString&)));
		connect(orbit_wdg->edt_init_periarg, SIGNAL(textChanged(const QString&)), this, SLOT(OnEdit(const QString&)));
		connect(orbit_wdg->edt_init_inclination, SIGNAL(textChanged(const QString&)), this, SLOT(OnEdit(const QString&)));
		connect(orbit_wdg->pushButton, SIGNAL(clicked()), this, SLOT(calc()));

	}
private slots:

	void OnEdit(const QString& msg)
	{
		double min_radius = 150;
	}

	void calc()
	{
		if (init_earth_orbit) delete init_earth_orbit;
		if (target_earth_orbit) delete target_earth_orbit;
		double init_peri = orbit_wdg->edt_init_peri->text().toDouble();
		double init_apo = orbit_wdg->edt_init_apo->text().toDouble();
		double init_asc = orbit_wdg->edt_init_ascnode->text().toDouble();
		double init_periarg = orbit_wdg->edt_init_periarg->text().toDouble();
		double init_inc = orbit_wdg->edt_init_inclination->text().toDouble();
		init_earth_orbit = new CEllepticalOrbit(EarthMass, 6400, init_peri, init_apo, init_inc, init_asc, init_periarg);

		double target_peri = orbit_wdg->edt_target_peri->text().toDouble();
		double target_apo = orbit_wdg->edt_target_apo->text().toDouble();
		double target_asc = orbit_wdg->edt_target_ascnode->text().toDouble();
		double target_periarg = orbit_wdg->edt_target_periarg->text().toDouble();
		double target_inc = orbit_wdg->edt_target_inclination->text().toDouble();

		target_earth_orbit = new CEllepticalOrbit(EarthMass, 6400, target_peri, target_apo,
			target_inc, target_asc, target_periarg);

		utilites::maneuver_scheme * scheme = new utilites::maneuver_scheme(init_earth_orbit->GetKeplerOrbitFormat(), target_earth_orbit->GetKeplerOrbitFormat());

		orbit_wdg->textEdit->insertPlainText(tr("маневров : %1").arg(scheme->transfer_orbits.size()));
		for (size_t i = 0; i < scheme->transfer_orbits.size(); ++i)
		{ 
			orbit_wdg->textEdit->insertPlainText(tr("приращение скорости маневра номер %1: %2").arg(i + 1).arg(scheme->transfer_orbits.at(i).impulse) );
		}
		
	}
};
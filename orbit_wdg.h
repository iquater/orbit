#include "ui_orbit_wdg.h"
#include "maneuver_scheme.h"
#include "memory.h"

class OrbitCalcForm : public QWidget
{
	Q_OBJECT
	Ui::orbit * orbit_wdg;
	CEllepticalOrbit * init_earth_orbit;
	CEllepticalOrbit * target_earth_orbit;
	utilites::maneuver_scheme * scheme;

	bool validate(QStringList & msg_list)
	{
		bool result = true;
		double init_peri = orbit_wdg->edt_init_peri->text().toDouble();
		double init_apo = orbit_wdg->edt_init_apo->text().toDouble();
		double init_asc = orbit_wdg->edt_init_ascnode->text().toDouble();
		double init_periarg = orbit_wdg->edt_init_periarg->text().toDouble();
		double init_inc = orbit_wdg->edt_init_inclination->text().toDouble();

		if (init_peri < 6550) {msg_list.append(QString::fromUtf8("Неправильно задан радиус перицентра.\nЗначение радиуса перицентра ниже ограничения (6550 км)")); result = false;}
		if (init_apo < 6550) {msg_list.append(QString::fromUtf8("Значение радиуса апоцентра ниже ограничения (6550 км)"));result = false;}
		if (init_peri > init_apo) {msg_list.append(QString::fromUtf8("Неправильно задан радиус перицентра.\nЗначение радиуса перицентра превышает значение радиуса апоцентра"));result = false;}
		if (init_asc < 0 || init_periarg  < 0|| init_inc < 0 )  {msg_list.append(QString::fromUtf8("угловые параметры орбиты меньше нуля"));result = false;}

		double target_peri = orbit_wdg->edt_target_peri->text().toDouble();
		double target_apo = orbit_wdg->edt_target_apo->text().toDouble();
		double target_asc = orbit_wdg->edt_target_ascnode->text().toDouble();
		double target_periarg = orbit_wdg->edt_target_periarg->text().toDouble();
		double target_inc = orbit_wdg->edt_target_inclination->text().toDouble();

		if (target_peri < 6550) {msg_list.append(QString::fromUtf8("Неправильно задан радиус перицентра.\nЗначение радиуса перицентра ниже ограничения (6550 км)")); result = false;}
		if (target_apo < 6550) {msg_list.append(QString::fromUtf8("Значение радиуса апоцентра ниже ограничения (6550 км)"));result = false;}
		if (target_peri > target_apo) {msg_list.append(QString::fromUtf8("Неправильно задан радиус перицентра.\nЗначение радиуса перицентра превышает значение радиуса апоцентра"));result = false;}
		if (target_asc < 0 || target_periarg < 0 || target_inc < 0)  {msg_list.append(QString::fromUtf8("угловые параметры орбиты меньше нуля"));result = false;}

		return result;
	}

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
		if (scheme)
		{
			scheme->transfer_orbits.clear();
			delete scheme;
		}

		double init_peri = orbit_wdg->edt_init_peri->text().toDouble();
		double init_apo = orbit_wdg->edt_init_apo->text().toDouble();
		double init_asc = orbit_wdg->edt_init_ascnode->text().toDouble() * PiConst / 180;
		double init_periarg = orbit_wdg->edt_init_periarg->text().toDouble() * PiConst / 180;
		double init_inc = orbit_wdg->edt_init_inclination->text().toDouble() * PiConst / 180;
		

		double target_peri = orbit_wdg->edt_target_peri->text().toDouble();
		double target_apo = orbit_wdg->edt_target_apo->text().toDouble();
		double target_asc = orbit_wdg->edt_target_ascnode->text().toDouble() * PiConst / 180;
		double target_periarg = orbit_wdg->edt_target_periarg->text().toDouble() * PiConst / 180;
		double target_inc = orbit_wdg->edt_target_inclination->text().toDouble() * PiConst / 180;

		QStringList msgs;
		if(!validate(msgs))
		{
			orbit_wdg->textEdit->setTextColor(Qt::red);
			for (int i = 0 ; i < msgs.size(); ++i)
			{
				
				orbit_wdg->textEdit->insertPlainText(msgs.at(i));
				return;
			}

		}

		init_earth_orbit = new CEllepticalOrbit(EarthMass, 6400, init_peri, init_apo, init_inc, init_asc, init_periarg);
		target_earth_orbit = new CEllepticalOrbit(EarthMass, 6400, target_peri, target_apo,
			target_inc, target_asc, target_periarg);

		scheme = new utilites::maneuver_scheme(init_earth_orbit->GetKeplerOrbitFormat(), target_earth_orbit->GetKeplerOrbitFormat());
		orbit_wdg->textEdit->clear();
		orbit_wdg->textEdit->insertPlainText(QString::fromUtf8("маневров : %1 \n").arg(scheme->transfer_orbits.size()));
		orbit_wdg->textEdit->setTextColor(Qt::black);
		for (size_t i = 0; i < scheme->transfer_orbits.size(); ++i)
		{ 
			const virtual_transfer_orbit & vto = scheme->transfer_orbits.at(i);
			if(vto.transversal_impulse)
				orbit_wdg->textEdit->insertPlainText(QString::fromUtf8("%1. приращение трансверсальной состовляющей скорости: %2 м/с\n").arg(i + 1).arg(vto.transversal_impulse) );
			if(vto.radial_impulse)
				orbit_wdg->textEdit->insertPlainText(QString::fromUtf8("%1. приращение радиальной состовляющей скорости: %2 м/с\n").arg(i + 1).arg(vto.radial_impulse) );
			orbit_wdg->textEdit->insertPlainText(QString::fromUtf8("%1. точка приложения импульса (истинная аномалия): %2°\n").arg(i + 1).arg(vto.imp_true_anomaly * 180/PiConst ) );
			orbit_wdg->textEdit->insertPlainText(QString::fromUtf8("Параметры орбиты после приложения импульса:\n"));
			orbit_wdg->textEdit->insertPlainText(QString::fromUtf8("\t Перицентр %1 км\n").arg(vto.finish_orbit.pericenter / 1000));
			orbit_wdg->textEdit->insertPlainText(QString::fromUtf8("\t Апоцентр %1 км\n").arg(vto.finish_orbit.apocenter / 1000 ));
			orbit_wdg->textEdit->insertPlainText(QString::fromUtf8("\t Эксцентриситет %1\n").arg(vto.finish_orbit.eccentricity));
			orbit_wdg->textEdit->insertPlainText(QString::fromUtf8("\t Аргумент перицентра,  %1°\n").arg(vto.finish_orbit.pericenter_angle * 180/PiConst));
		}
		/*scheme->transfer_orbits.clear();
		delete scheme;*/
	}
};
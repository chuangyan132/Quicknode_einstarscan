#include "e10device.h"
#include "sn3dfiledialog.h"
#include "qjsonobject.h"
#include "qjsonarray.h"

using namespace sn3dsdkdemo;

E10Device::E10Device(QObject* parent, ScanServiceImp* pService)
	:Device(parent, pService)
{
}

int E10Device::saveCloudPoint(const QString& projectPath)
{
	QString saveFileName;
	QString checkedState;
	QString protocol = "000111000000000111000";
	sn3d_file_dialog dlg(protocol);
	if (QDialog::Accepted == dlg.exec()) {
		checkedState = dlg.getCheckedsState();
		saveFileName = dlg.selectedFile();
		QJsonObject joparams;
		joparams.insert("mesh_resize_edit", 100);
		joparams.insert("p3", false);
		joparams.insert("fm_asc", false);
		joparams.insert("fm_txt", false);
		joparams.insert("asc", true);
		QString currentPath = projectPath;
		QStringList projList;
		projList << currentPath;
		QJsonArray projJa;
		for (size_t i = 0; i < projList.size(); i++)
		{
			projJa.append(projList.at(i));
		}
		QJsonObject jo;
		jo.insert("file", saveFileName);
		jo.insert("params", joparams);
		jo.insert("projList", projJa);
		Device::saveCloudPoint(jo);
	}
	else
		return USER_CANCEL;
	return OK;
}

int E10Device::calibrateTypeMap(QMap<QString, int>& map)
{
	map.clear();
	map["CT_STEREO"] = 1;
	map["CT_RAPID"] = 5;
	map["CT_WHITE_BALANCE"] = 3;
	return OK;
}

int E10Device::saveMesh(const QString& projectPath)
{
	int ret = USER_CANCEL;
	QString saveFileName = QFileDialog::getSaveFileName(nullptr, "SaveScanData", "", tr("File (*.stl *.obj)"));
	if (saveFileName.isEmpty())
	{
		return ret;
	}
	QJsonObject joparams;
	joparams.insert("mesh_resize_edit", 100); //zoom ratio
	joparams.insert("p3", false);
	joparams.insert("fm_asc", false);
	joparams.insert("fm_txt", false);
	joparams.insert("asc", false);
	joparams.insert("stl", true);
	joparams.insert("saveMesh", true);
	joparams.insert("obj", false);
	// 	mapParam["sasc"].toBool();
	// 	mapParam["obj"].toBool();
	// 	mapParam["ply"].toBool();
	// 	mapParam["mf3"].toBool();
	// 	mapParam["saveMesh"].toInt();
	// 	mapParam["proj"].toString();
	QStringList projList;
	projList << projectPath;
	QJsonArray projJa;
	for (size_t i = 0; i < projList.size(); i++)
	{
		projJa.append(projList.at(i));
	}

	QJsonObject jo;
	jo.insert("file", saveFileName);
	jo.insert("params", joparams);
	jo.insert("projList", projJa);

	return Device::saveMesh(jo);
}
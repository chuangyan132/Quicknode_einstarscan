//#include "ScannerDemo.h"
#include "sn3dmainwindow.h"
#include <QtWidgets/QApplication>
#include <string>
#include <iostream>
#include "sdklogger.h"
//#define OLDUI ""
#include "sn3dstyle.h"
using namespace std;
int main(int argc, char *argv[])
{
	//QThread::sleep(10);

	QApplication a(argc, argv);
	qRegisterMetaType<QVector<int>>();
	qRegisterMetaTypeStreamOperators<QVector<int>>();
	//Sn3DStyle sn3DStyle;
 	//sn3DStyle.setStyleSheet();
#ifdef OLDUI
	ScannerDemo w;
	w.initCameraVideo();
#else
    //ScannerDemo w;
    Sn3DMainWindow w;
#endif
    // SDKLogger::createInstanc(); // for debug only
	w.show();
 	w.initWidget();
 	w.initMember();
	return a.exec();
}

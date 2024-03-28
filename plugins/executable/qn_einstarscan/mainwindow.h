#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <Sn3DPublic.h>

enum LogType
{
	SEND_MESSAGE,				//send to service
	RECEIVE_MESSAGE,			//receive message from service,only correct result
	ERROR_MESSAGE,				//receive message from service,only incorrect result
	INTERAL_LOG					//interal log
};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

	void closeEvent(QCloseEvent *event) {
		emit dialogClosed();
	}

	void setProperties(bool isVideo, bool isSaveWholeCloud, QString saveWholeCloudPath);

signals:
	void dialogClosed();

private slots:
    void on_pushButton_Start_clicked();

    void on_pushButton_End_clicked();

	void slot_updateVideo(int camId, const unsigned char* data, int width, int height, int channel);

	void slot_pointIncreUpdate(LPSn3dIncreasePointCloud increasePointCloud);

	void slot_cameraPositionUpdate(LPSn3dCameraPosition cameraPosition);

	void slot_printInfo(QString info, LogType type = LogType::INTERAL_LOG);


private:
	bool m_isVideo = false;
	bool m_isSaveWholeCloud = false;
	QString m_saveWholeCloudPath;

	void getTrackLostStatus(bool status);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H

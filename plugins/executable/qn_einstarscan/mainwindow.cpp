#include <fstream>
#include <ctime>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Sn3DE10Sdk.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

std::string g_callBackPath;
void MainWindow::setProperties(bool isVideo, bool isSaveWholeCloud, QString saveWholeCloudPath)
{
	m_isVideo = isVideo;
	m_isSaveWholeCloud = isSaveWholeCloud;
	m_saveWholeCloudPath = saveWholeCloudPath;
	g_callBackPath = m_saveWholeCloudPath.toStdString();
}

void MainWindow::on_pushButton_Start_clicked()
{
	auto getImageCallback = [](int camId, const unsigned char* data, int  width, int height, int channel, void* owner) {
		((MainWindow*)owner)->slot_updateVideo(camId, data, width, height, channel);

	};
	if (m_isVideo)
		Sn3DIntallGetImagasCallBack(getImageCallback, this);
	else
		Sn3DIntallGetImagasCallBack(nullptr, nullptr);

	// 经确认SDK有bug，必须调用此函数，否则Sn3DSetIncreasePointCloudCallBack中的owner在回调中会被置为nullptr
	Sn3DSetCurrentPointCloudCallBack(nullptr, this);

	auto getIncreasePointCloudCallBack = [](LPSn3dIncreasePointCloud increasePointCloud, void* owner) {
		//先解析数据，再调用槽函数
		((MainWindow*)owner)->slot_pointIncreUpdate(increasePointCloud);

	};
	Sn3DSetIncreasePointCloudCallBack(getIncreasePointCloudCallBack, this);

	auto getCameraPositionUpdateCallBack = [](LPSn3dCameraPosition cameraPosition, void* owner) {
		//先解析数据，再调用槽函数
		((MainWindow*)owner)->slot_cameraPositionUpdate(cameraPosition);

	};
	Sn3DSetCameraPositionCallBack(getCameraPositionUpdateCallBack, this);

	auto getTrackLostStatusCallback = [](bool status, void* owner) {
		((MainWindow*)owner)->getTrackLostStatus(status);
	};
	Sn3DSetTrackLostStatusCallBack(getTrackLostStatusCallback, this); // 注册接受跟踪丢失回调
																// 如果想终止直接Sn3DSetTrackLostStatusCallBack(nullptr, nullptr);


	int rc = Sn3DPreviewScan();

	rc = Sn3DStartScan();
}

void getWholeCloudCallBack (LPSn3dPointCloud wholePointCloud)
{
	std::ofstream ofs;
	//std::string callBackPath("./demo/Whole/");
	std::string callBackPath = g_callBackPath + "/";

	callBackPath.append(std::to_string(std::time(0)));
	callBackPath.append(".asc");
	ofs.open(callBackPath);

	for (int index = 0; index < wholePointCloud->norma_count; index++)
	{
		const auto& vertex = wholePointCloud->vertex_data;
		const auto& normal = wholePointCloud->norma_data;

		ofs << vertex[index].x << " " \
			<< vertex[index].y << " " \
			<< vertex[index].z << " " \
			<< normal[index].x << " " \
			<< normal[index].y << " " \
			<< normal[index].z << std::endl;
	}
	ofs.close();
	return;
}

void MainWindow::on_pushButton_End_clicked()
{
	int rc = Sn3DPauseScan();
	Sn3DIntallGetImagasCallBack(nullptr, nullptr);

	//SaveCurrentPointCloud();
	//SaveIncreasePointCloud();
	auto temp = m_isSaveWholeCloud ? getWholeCloudCallBack : nullptr;
	bool t_globalOpt = true;
	float t_point_dis = 1.f;
	if (m_isSaveWholeCloud)
		rc = Sn3DEndScan(t_globalOpt, t_point_dis, getWholeCloudCallBack);
	else
		rc = Sn3DEndScan(t_globalOpt, t_point_dis, nullptr);


	//rc = Sn3DExitScan();

}

void MainWindow::slot_updateVideo(int camId, const unsigned char * data, int width, int height, int channel)
{
	QImage vidoeImage = QImage(data, width, height, QImage::Format_RGB888);

	QTransform m;
	m.rotate(-90);
	vidoeImage = vidoeImage.transformed(m);


	QRect rectArea = vidoeImage.rect();

	QPixmap pixmap = QPixmap::fromImage(vidoeImage);
	QSize size = ui->camera_left->size();
	pixmap = pixmap.scaled(size, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	if (camId == 0)
	{
		ui->camera_left->updateImage(pixmap);
	}
	else if (camId == 1)
	{
		ui->camera_right->updateImage(pixmap);
	}
	else
	{
		ui->camera_color->updateImage(pixmap);
	}
}


void MainWindow::slot_pointIncreUpdate(LPSn3dIncreasePointCloud increasePointCloud)
{
	//调用ui->hub->realtimePointIncreUpdate
	ui->hub->realtimePointIncreUpdate(increasePointCloud);
}

void MainWindow::slot_cameraPositionUpdate(LPSn3dCameraPosition cameraPosition)
{
	//调用ui->hub->realtimePointIncreUpdate
	ui->hub->realtimeCameraPositionUpdate(cameraPosition);
}

void MainWindow::slot_printInfo(QString info, LogType type)
{
	QColor color = Qt::white;
	std::string sColor;
	switch (type)
	{
	case LogType::SEND_MESSAGE:
	{
		color = QColor(62, 162, 255);//蓝色
		sColor = "color=\"blue\">";
	}
	break;
	case LogType::RECEIVE_MESSAGE:
	{
		color = QColor(73, 170, 25);//绿色
		sColor = "color=\"green\">";
	}
	break;
	case LogType::ERROR_MESSAGE:
	{
		color = QColor(166, 29, 36);//红色
		sColor = "color=\"red\">";
	}
	break;
	case LogType::INTERAL_LOG:
	{
		color = Qt::white;
		sColor = "color=\"white\">";
	}
	break;
	default:
		break;
	}
	ui->textBrowser->setTextColor(color);
	std::string printinfo = "<font size=\"9\" " + sColor + info.toStdString() + "<\font>";
	ui->textBrowser->append(QString::fromStdString(printinfo));
}

void MainWindow::getTrackLostStatus(bool status)
{
	if (status)
		slot_printInfo(QString("===== track lost! ====="), LogType::ERROR_MESSAGE);
}

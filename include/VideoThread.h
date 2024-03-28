#pragma once
#include <QThread>
#include <QTimer>
#include "SharedMemoryParser.h"
#include "PublicClass.h"
#include "transmem.h"
#include <QMutex>

class VideoThread:public QThread
{
	Q_OBJECT
public:
	VideoThread();
	~VideoThread();
	void setStop(bool isSttop) { mIsStop = isSttop; }
	void setSharedMemoryParser(SharedMemoryParser*parser) { m_sharedMemoryParser = parser; }
	void setGetVideoCallBack(std::function<void(int camId, uint8_t* data, int  width, int height, int channel)> callback);
signals:
	void videoImageChanged(QVariantMap imageMap);
protected:
	void run();
private:
	bool setVideo(int cameraId, unsigned char* data, int len, int width, int height, int channel);
private:
	bool mIsStop = false;
	SharedMemoryParser*m_sharedMemoryParser = nullptr;
	unsigned char* m_pataGray = nullptr;
	unsigned char* m_pataColor = nullptr;
	std::function<void(int camId, unsigned char* data, int  width, int height, int channel)> m_videoGetCallBack = nullptr;
};


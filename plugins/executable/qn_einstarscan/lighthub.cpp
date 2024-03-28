#include "lighthub.h"
#include "osgmgr.h"
#include "increase_update.h"

#define TEST_INCRE_UPDATE_OSG_OPENGL_SWITCH

USING_RENDERKIT_NAMESPACE

Canvas3DItem* g_canvasItem = nullptr;

void LightHub::setCanvas(Canvas3DItem * canvas)
{
	g_canvasItem = canvas;
	m_canvas = canvas->getCanvasBase();
	m_renderKit = Sn3D::RenderKit::RenderKitSingleton::instance();
	OsgMgr::init(m_canvas);

	m_pIncreaseUpdate = new IncreaseUpdate(m_canvas);
	m_pIncreaseUpdate->initRealtimeIncre();	//Mark: ��ʼ���������Ƹ���
}

void LightHub::startPointIncreUpdate()
{
	if (m_pIncreaseUpdate == nullptr) {
		return;
	}

#ifndef TEST_INCRE_UPDATE_OSG_OPENGL_SWITCH
	static bool bIncrease = false;
	bIncrease = !bIncrease;
	// ��ʼ��������
	if (bIncrease) {
		m_pIncreaseUpdate->startUpdatePointCloud();
	}
	// �Ƴ��ڵ�
	else {
		m_pIncreaseUpdate->removeCloudNode();
	}
#else
	static bool bStart = false;
	static bool bPause = false;

	if (!bStart) {
		m_pIncreaseUpdate->startUpdatePointCloud();
		bStart = true;
	}
	else {
		if (m_pIncreaseUpdate->isUpdateFinished()) {
			m_pIncreaseUpdate->removeCloudNode();
			bStart = false;
			bPause = false;
		}
		else {
			bPause = !bPause;
			qDebug() << "m_pIncreaseUpdate->pauseIncreUpdate(" << bPause << ")";
			m_pIncreaseUpdate->pauseIncreUpdate(bPause);
		}
	}
#endif
}

void LightHub::realtimePointIncreUpdate(LPSn3dIncreasePointCloud increasePointCloud)
{
	m_pIncreaseUpdate->realtimePointIncreUpdate(increasePointCloud);
}

void LightHub::realtimeCameraPositionUpdate(LPSn3dCameraPosition cameraPosition)
{
	m_pIncreaseUpdate->realtimeCameraPositionUpdate(cameraPosition);
}
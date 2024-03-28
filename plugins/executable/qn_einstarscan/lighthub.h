#ifndef LIGHTHUB_H
#define LIGHTHUB_H

#include <QObject>
#include <QQmlEngine>
#include <QQmlContext>
#include <renderkit_singleton.h>
#include <canvas3d_renderinfo.h>
#include <canvas3d_item.h>
#include <cutplanedragger.h>

#include <Sn3DPublic.h>

USING_RENDERKIT_NAMESPACE

class IncreaseUpdate;


class LightHub : public QObject
{
	Q_OBJECT
public:
	explicit LightHub() {}
signals:

public slots:
	//��ʼ��canvas
	void setCanvas(Canvas3DItem * canvas);

	// ��ʼ������������
	void startPointIncreUpdate();

	//ʵʱ�������ݵ���������
	void realtimePointIncreUpdate(LPSn3dIncreasePointCloud increasePointCloud);

	void realtimeCameraPositionUpdate(LPSn3dCameraPosition cameraPosition);

private:
	osg::ref_ptr<CanvasBase> m_canvas;
	Sn3D::RenderKit::RenderKitSingleton* m_renderKit;

	IncreaseUpdate* m_pIncreaseUpdate;
};

#endif // LIGHTHUB_H
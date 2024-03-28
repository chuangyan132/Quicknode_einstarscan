#ifndef INCREASE_UPDATE_H
#define INCREASE_UPDATE_H

#include <vector>
#include <array>
#include <memory>
#include <QObject>
#include <QQmlEngine>
#include <QQmlContext>
#include <renderkit_singleton.h>
#include "canvas3d_item.h"
#include <osgText/Text>

#include "incremental_update/incre_point_geomtry.h"
#include "incremental_update/incre_cur_frame_geomtry.h"

#include <Sn3DPublic.h>

USING_RENDERKIT_NAMESPACE

/*!
@addtogroup Demo
@details \ref IncreaseUpdate 3D数字化，增量更新，用于模拟测试增量更新功能
@{
*/

/*!
@brief 点云增量的参数
*/
struct IncrePointUpdateParam
{
    // 整体点云及增量更新相关数据
    osg::ref_ptr<osg::Vec3Array> vertex;
    osg::ref_ptr<osg::Vec3Array> normal;
    osg::ref_ptr<osg::Vec3Array> color;
    osg::ref_ptr<osg::ByteArray> state;
    osg::ref_ptr<osg::IntArray> vIdxArray;
    osg::ref_ptr<osg::IntArray> vFrameIdxArray;
    std::vector<std::array<float, 16>> frameRtArray;
    int pointTotalNum;

    // 当前帧数据
    osg::ref_ptr<osg::Vec3Array> curFrameVertex;
    osg::ref_ptr<osg::Vec3Array> curFrameNormal;
};
typedef std::shared_ptr<IncrePointUpdateParam> PtrIncreUpdateParam;

/*!
@brief 点云增量更新的节点
*/
class IncrePointNode : public osg::Switch
{
public:
    IncrePointNode();
    ~IncrePointNode();

    // 传递数据到geometry
    void update(std::shared_ptr<IncrePointUpdateParam> pUpdateParam, bool isStart);
    void pauseIncreUpdate(bool isPause);

    IncrePointGeometry* getPointGeometry();
    IncreCurFrameGeometry* getCurFrameGeometry();

private:
    osg::ref_ptr<osg::Geode> createPointGeode(bool hasColor, bool hasState, bool bRigidScan);
    osg::Geometry* getGeometry(const char* geomName);

    // 更新增量数据
    void updateIncreasePointData(std::shared_ptr<IncrePointUpdateParam> pUpdateParam, bool isStart);

    // 更新当前帧数据
    void updateCurrentFramePointData(std::shared_ptr<IncrePointUpdateParam> pUpdateParam);

    // 更新节点的包围盒
    void updateNodeBoundingBox(const bool bIncrease, const bool bRigidScan,
                               osg::ref_ptr<osg::Vec3Array> pos,
                               osg::ref_ptr<osg::IntArray> vFrameIdxArray,
                               const std::vector<std::array<float, 16>>& frameRtList,
                               osg::Vec3& min,
                               osg::Vec3& max);

    // 计算刚体模型的包围盒
    void computeBoundingBox(osg::ref_ptr<osg::Vec3Array>& pos, osg::Vec3& min, osg::Vec3& max);

    // 计算非刚体模型的包围盒
    void computeBoundBoxNonRigid(osg::ref_ptr<osg::Vec3Array>& pos,
                                 osg::ref_ptr<osg::IntArray> vFrameIdxArray,
                                 const std::vector<std::array<float, 16>>& frameRtList,
                                 osg::Vec3& min, osg::Vec3& max);

private:
    osg::Vec3 m_min;
    osg::Vec3 m_max;
};

/*!
@brief 点云增量更新的osg::Operation操作类

于用于增量更新点云数据，线程安全

*/
class IncreaseUpdatePointOpe : public osg::Operation
{
public:
    enum OperRunType {
        ORT_Start = 0,
        ORT_Pause,
        ORT_Resume,
        ORT_Stop
    };
    explicit IncreaseUpdatePointOpe(
        const std::string& nodeName,
        PtrIncreUpdateParam pUpdateParam,
        osg::ref_ptr<osg::Group> pGroup,
        OperRunType runType = OperRunType::ORT_Start, 
        QObject *msgRecver = nullptr);

    void operator()(osg::Object* caller) override;

private:
    static osg::Node* findChildByName(const std::string& name, osg::Group* group);

private:
    PtrIncreUpdateParam      m_pUpdateParam;
    osg::ref_ptr<osg::Group> m_pGroup;
    bool m_isStart;
    OperRunType m_runType;
    QObject *m_msgRecver;
};

/*!
@brief 增量更新的类，仅用于测试
*/
class IncreaseUpdate : public QObject
{
    Q_OBJECT
public:

    // 点云排列类型
    enum PointCloudArrangeType
    {
        ACAT_2D, ACAT_3D
    };

    explicit IncreaseUpdate(CanvasBase* canvas);
    virtual ~IncreaseUpdate();

public:
    // 开始增量更新
    void startUpdatePointCloud();

    // 移除增量更新节点
    void removeCloudNode();

    void pauseIncreUpdate(bool needPause);

    bool isUpdateFinished();

	void initRealtimeIncre();

	void realtimePointIncreUpdate(LPSn3dIncreasePointCloud increasePointCloud);

	void realtimeCameraPositionUpdate(LPSn3dCameraPosition cameraPosition);

protected slots:
    void increaseUpdatePointCloud();
    void onIncreUpdateOperFinished(int operRunType);

private:

    // 创建点云，2D平面方式排列
    void createPointCloud2D();

    // 创建点云，立方体方式排列
    void createPointCloud3D();

private:
    CanvasBase* m_canvas;
    QTimer* m_timer;

    bool m_bIncrease;                                   // 更新方式，true增量更新，false全量更新
    bool m_bRigidScan;                                  // 是否刚体扫描，true刚体扫描，false非刚体扫描；另外，全量更新肯定是刚体

    PointCloudArrangeType m_pointCloudArrange;          // 点云的排列类型

    // ===================================== 点云数据 =====================================
    //
    osg::ref_ptr<osg::Vec3Array> m_positions;           // 坐标
    osg::ref_ptr<osg::Vec3Array> m_normals;             // 法向量
    osg::ref_ptr<osg::Vec3Array> m_colors;              // 颜色
    osg::ref_ptr<osg::ByteArray> m_states;              // 属性

    std::vector<int> m_indices;                         // 顶点id
    std::vector<int> m_frameIndices;                    // 顶点frameId，用于获取帧矩阵，非刚体扫描使用
    std::vector<std::array<float, 16>> m_frameRtArray;  // 所有帧的矩阵，非刚体扫描使用
    //
    // ===================================== 点云数据 =====================================

    // ===================================== 更新过程的点数 =====================================
    //
    uint32_t m_currentIndex;                            // 当前更新的起始顶点id
    uint32_t m_totalPointSize;                          // 总点数
    uint32_t m_patchVertexCount;                        // 每帧更新的顶点数量
    //
    // ===================================== 更新过程的点数 =====================================
	uint32_t m_totalIncrePointSize;                     // 实时增量更新的总点数(需初始化为0)

    osg::ref_ptr<osg::Group> m_pointGroup;
    const int m_sendInterval;
};

/*! @} */

#endif // CALIBRATE_NAVIGATION_H

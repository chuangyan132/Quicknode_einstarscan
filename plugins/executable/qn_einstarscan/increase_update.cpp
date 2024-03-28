#include "increase_update.h"
#include "canvas3d_drawable.h"
#include "canvas3d_tool.h"
#include "renderkit_operations.h"
//#include "incremental_update/Incre_point_node.h"
#include <random>
#include "canvas3d_view.h"
#include "camera_ctrl/customize_manipulator.h"

#include <osg/Matrixd>
#include <osg/Vec3d>

//#define TEST_FRAME_MATRIX //测试当前帧使用自定义帧矩阵

USING_RENDERKIT_NAMESPACE

osg::Matrixd makeLookAt(const osg::Vec3d& eye, const osg::Vec3d& center, const osg::Vec3d& up) {
	osg::Vec3d forward, side, upward;

	// 计算前向向量（从中心点到眼睛，注意这里的方向与右手坐标系相反）
	forward = eye - center;
	forward.normalize();

	// 计算侧向向量（前向向量和上向量的叉积）
	side = forward ^ up;
	side.normalize();

	// 重新计算上向量（侧向向量和前向向量的叉积）
	upward = side ^ forward;
	upward.normalize();

	// 使用set方法创建视图矩阵
	osg::Matrixd matrix;
	matrix.set(
		side.x(), side.y(), side.z(), -eye.x(),
		upward.x(), upward.y(), upward.z(), -eye.y(),
		forward.x(), forward.y(), forward.z(), -eye.z(),
		0.0, 0.0, 0.0, 1.0
	);
	//matrix.set(
	//	side.x(), upward.x(), forward.x(), 0.0,
	//	side.y(), upward.y(), forward.y(), 0.0,
	//	side.z(), upward.z(), forward.z(), 0.0,
	//	-eye.x(), -eye.y(), -eye.z(), 1.0
	//);

	return matrix;
}

IncrePointNode::IncrePointNode()
{
    auto mt = new osg::MatrixTransform;
    addChild(mt);
}

IncrePointNode::~IncrePointNode()
{

}

osg::ref_ptr<osg::Geode> IncrePointNode::createPointGeode(bool hasColor, bool hasState, bool bRigidScan)
{
    osg::ref_ptr<osg::Geode> geode = new osg::Geode;
    const osg::ref_ptr<IncrePointGeometry> geometry = new IncrePointGeometry(hasColor, true);
    //geometry->setName("point_cloud_increase_geometry");

    // 设置每个顶点的float数量，在分配缓存时需要计算
    geometry->setHasColor(hasColor);
    geometry->setHasState(hasState);
    geometry->setIsRigidScan(bRigidScan);

    geometry->initDrawCall();
    geometry->getOrCreateStateSet()->setMode(GL_VERTEX_PROGRAM_POINT_SIZE, osg::StateAttribute::ON);

    bool enableTextureColor = false;
    //this->getUserValue(std::string("enableTextureColor"), false);

    //当前帧
    osg::ref_ptr<IncreCurFrameGeometry> curFrameGeom = new IncreCurFrameGeometry;
    //curFrameGeom->setName("point_cloud_cur_frame_geometry");
    bool isEnableCurFrameAlpha = false;
    if (isEnableCurFrameAlpha) {
        curFrameGeom->setAlpha(0.15);
        curFrameGeom->getOrCreateStateSet()->setMode(GL_BLEND, osg::StateAttribute::ON);
        curFrameGeom->getOrCreateStateSet()->setRenderBinDetails(RBR_OSG_TransparentBin, "DepthSortedBin");
    }

    //配置当前帧距离颜色
    osg::ref_ptr<osg::FloatArray> curFrameDist = new osg::FloatArray;
    osg::ref_ptr<osg::Vec3Array> curFrameDistClr = new osg::Vec3Array;
    curFrameDist->resize(6);
    curFrameDistClr->resize(3);
    auto &curFrameDistVec = curFrameDist->asVector();
    auto &curFrameDistClrVec = curFrameDistClr->asVector();
    curFrameDistVec[0] = -1E6;
    curFrameDistVec[1] = 0.333 - 0.5;//160;
    curFrameDistVec[2] = 0.333 - 0.5;//160;
    curFrameDistVec[3] = 0.666 - 0.5;//230;
    curFrameDistVec[4] = 0.666 - 0.5;//230;
    curFrameDistVec[5] = 1E6;
    curFrameDistClrVec[0] = osg::Vec3(0, 1, 0);
    curFrameDistClrVec[1] = osg::Vec3(0, 0, 1);
    curFrameDistClrVec[2] = osg::Vec3(1, 0, 0);
    //curFrameDistClrVec[2] = osg::Vec3(1, 1, 0);
    curFrameGeom->cfgCurFrameDistClr(curFrameDist, curFrameDistClr);
    curFrameGeom->enableCurFrameDistClr(true);////开启当前帧距离颜色
    //curFrameGeom->initDrawCall();
    curFrameGeom->setLightMode(0);

    geode->addDrawable(geometry);
    geode->addDrawable(curFrameGeom);
    return geode;
}

void IncrePointNode::update(std::shared_ptr<IncrePointUpdateParam> pUpdateParam, bool isStart)
{
    // 更新增量数据
    updateIncreasePointData(pUpdateParam, isStart);

    // 更新当前帧数据
    updateCurrentFramePointData(pUpdateParam);
}

void IncrePointNode::updateIncreasePointData(std::shared_ptr<IncrePointUpdateParam> pUpdateParam, bool isStart)
{
    if (pUpdateParam == nullptr) {
        return;
    }
    auto& vertex = pUpdateParam->vertex;
    auto& normal = pUpdateParam->normal;
    auto& color = pUpdateParam->color;
    auto& state = pUpdateParam->state;
    auto& vIdxArray = pUpdateParam->vIdxArray;
    auto& vFrameIdxArray = pUpdateParam->vFrameIdxArray;
    //state = nullptr;
    const bool bHasColor = (color.valid() && !color->empty());
    const bool bHasState = (state.valid() && !state->empty());

    // 判断是增量数据还是全量数据
    bool bIncrease = false;
    std::vector<int> vecVid;
    if (vIdxArray != nullptr) {
        vecVid = vIdxArray->asVector();
        bIncrease = !vecVid.empty();
    }

    // 是否为刚体扫描，非刚体顶点有帧id，另外，全量更新肯定是刚体
    bool bRigidScan = true;
    if (vFrameIdxArray != nullptr && bIncrease) {
        bRigidScan = vFrameIdxArray->empty();
    }

    auto mt = dynamic_cast<osg::MatrixTransform*>(this->getChild(0));
    if (mt->getNumChildren() == 0)
    {
        const osg::ref_ptr<osg::Geode> geode = createPointGeode(bHasColor, bHasState, bRigidScan);
        mt->addChild(geode);
        auto pGeom = getPointGeometry();
        qDebug() << "IncrePointNode::updateIncreasePointData - beginOfIncreUpdate";
        pGeom->beginOfIncreUpdate();
    }
    auto pGeom = getPointGeometry();
    if (pGeom == nullptr) {
        qDebug() << "geom is nullptr";
        return;
    }
    if (!isStart) {
        qDebug() << "IncrePointNode::updateIncreasePointData - endOfIncreUpdate";
        pGeom->endOfIncreUpdate();
        return;
    }
    if (pUpdateParam->vertex == nullptr || pUpdateParam->normal == nullptr) {
        return;
    }

    // todo, 选择相关需要验证，以下代码暂时不使用
    //if (!bIncrease)
    //{
    //    //设置法向数据
    //    pGeom->setNormalArray(normal);

    //    //设置顶点数据，用于选择
    //    pGeom->setVertexArray(vertex);
    //    //设置PrimitiveSet，用于射线求交
    //    osg::ref_ptr<osg::DrawElementsUInt> pointIndex = new osg::DrawElementsUInt(osg::PrimitiveSet::POINTS);
    //    pointIndex->resize(vertex->size(), 0);
    //    auto& pIndex = pointIndex->asVector();
    //    for (auto i = 0; i < vertex->size(); ++i) {
    //        pIndex[i] = i;
    //    }
    //    pGeom->addPrimitiveSet(pointIndex);
    //}

    const auto vSize = vertex->size();
    //printf("%s\n", typeid(decltype(vSize)).name());
    if (vSize <= 0) {
        return;
    }

    std::shared_ptr<IncrePatchData> patch = std::make_shared<IncrePatchData>();
    patch->vertArr = vertex;
    patch->normArr = normal;
    patch->clrArr = bHasColor ? color : nullptr;
    patch->stateArr = bHasState ? state : nullptr;

    if (bIncrease) {
        patch->patchIdData.resize(vSize);
    }

    // 非刚体扫描
    if (!bRigidScan)
    {
        patch->patchFrameIdData.resize(vSize);

        // 直接拷贝顶点frameId
        auto& frameIndices = vFrameIdxArray->asVector();
        memcpy(patch->patchFrameIdData.data(), frameIndices.data(), frameIndices.size() * sizeof(int));
    }

    // 直接拷贝顶点id
    if (bIncrease) {
        memcpy(patch->patchIdData.data(), vecVid.data(), vecVid.size() * sizeof(int));
    }

    // ====>注意：添加帧数据时，必须保证不是多帧数据保存到一个数组中，如果有多帧数据，必须拆开到多个IncrePatchData中，否则可能会出现杂点 
    // 添加帧数据
    pGeom->addIncreasePatchData(patch);

    // 设置所有帧的矩阵数据
    pGeom->setPatchVertexSize(vecVid.size());

    // 设置所有帧的矩阵数据
    auto& frameRtArray = pUpdateParam->frameRtArray;
    pGeom->setFrameMatrix(frameRtArray);

    //设置点云数量，用于绘制
    int pointTotalNum = pUpdateParam->pointTotalNum;
    if (pointTotalNum <= 0) {
        pointTotalNum = vSize;
    }

    if (bIncrease) {
        auto curSize = pGeom->getWholeSize();
        pointTotalNum = std::max(curSize, pointTotalNum);
    }
    pGeom->setIsIncreaseUpdate(bIncrease);
    pGeom->setIsRigidScan(bRigidScan);
    pGeom->setWholeSize(pointTotalNum);

    // 设置点显示的颗粒大小，正常情况下读取属性后再设置
    //pGeom->setGLPointSize(3);//使用默认值，或者使用setGLPointSize设置

    // 更新包围盒
    osg::Vec3 min, max;
    updateNodeBoundingBox(bIncrease, bRigidScan, vertex, vFrameIdxArray, frameRtArray, min, max);
    pGeom->setBoxValue(min, max);
}

void IncrePointNode::updateCurrentFramePointData(std::shared_ptr<IncrePointUpdateParam> pUpdateParam)
{
    //auto pGeom = getPointGeometry();
    auto pGeom = getCurFrameGeometry();
    if (pGeom == nullptr) {
        qDebug() << "geom is nullptr";
        return;
    }
    if (pUpdateParam->curFrameVertex == nullptr || pUpdateParam->curFrameNormal == nullptr) {
        // 设置当前帧点云的顶点数量
        pGeom->setCurFramePointCount(0);
        return;
    }
    const auto& vecCoord = pUpdateParam->curFrameVertex->asVector();
    const auto& vecNormal = pUpdateParam->curFrameNormal->asVector();
    auto testIdxArray = new osg::IntArray;
    testIdxArray->asVector().resize(vecCoord.size(), 1);
    std::vector<int> *vecFrameIdx = pUpdateParam->vFrameIdxArray ? &(pUpdateParam->vFrameIdxArray->asVector()) : nullptr;
    if (vecCoord.empty() || vecNormal.empty()) {
        // 设置当前帧点云的顶点数量
        pGeom->setCurFramePointCount(0);
        return;
    }
    const auto vCount = vecCoord.size();

    const uint32_t V_FLOAT_COUNT = 7;
    std::shared_ptr<CurrentFramePointData> pCurFramePoints = std::make_shared<CurrentFramePointData>();
    auto& pointData = pCurFramePoints->pointData;
    pointData.resize(vCount * V_FLOAT_COUNT);
    for (auto i = 0; i < vCount; ++i)
    {
        const auto idx = i * V_FLOAT_COUNT;

        memcpy(&pointData[idx], &vecCoord[i], sizeof(osg::Vec3));
        memcpy(&pointData[idx + 3], &vecNormal[i], sizeof(osg::Vec3));
#ifdef TEST_FRAME_MATRIX
        pointData[idx + 6] = i % 2 == 0 ? 0 : 1;
#else
        pointData[idx + 6] = vecFrameIdx ? (*vecFrameIdx)[i] : -1;
#endif

        //pointData[idx]     = vecCoord[i].x();
        //pointData[idx + 1] = vecCoord[i].y();
        //pointData[idx + 2] = vecCoord[i].z();

        //pointData[idx + 3] = vecNormal[i].x();
        //pointData[idx + 4] = vecNormal[i].y();
        //pointData[idx + 5] = vecNormal[i].z();
    }
    // 设置当前帧点云的数据
    pGeom->setCurrentFramePointData(pCurFramePoints);

    // 设置当前帧点云的顶点数量
    pGeom->setCurFramePointCount(vCount);

    // 设置点显示的颗粒大小，正常情况下读取属性后再设置
    pGeom->setCurFrameGLPointSize(5.0f);
#ifdef TEST_FRAME_MATRIX
    FrameMatrixArray frameMatArr;
    std::array<float, 16> mat = {
        1, 0, 0, 0,
        0, 2, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    };
    std::array<float, 16> mat1 = {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 2, 0,
        0, 0, 0, 1
    };
    frameMatArr.push_back(mat);
    frameMatArr.push_back(mat1);
    pGeom->setFrameMatrix(frameMatArr);
#endif
}

void IncrePointNode::pauseIncreUpdate(bool isPause)
{
    auto pGeom = getPointGeometry();
    if (pGeom == nullptr) {
        qDebug() << "geom is nullptr";
        return;
    }
    if (isPause) {
        qDebug() << "IncrePointNode::pauseIncreUpdate - endOfIncreUpdate";
        pGeom->endOfIncreUpdate();
    }
    else {
        qDebug() << "IncrePointNode::pauseIncreUpdate - beginOfIncreUpdate";
        pGeom->beginOfIncreUpdate();
    }
}

IncrePointGeometry* IncrePointNode::getPointGeometry()
{
    return dynamic_cast<IncrePointGeometry*>(getGeometry("point_cloud_increase_geometry"));
}

IncreCurFrameGeometry* IncrePointNode::getCurFrameGeometry()
{
    return dynamic_cast<IncreCurFrameGeometry*>(getGeometry("point_cloud_cur_frame_geometry"));
}

osg::Geometry* IncrePointNode::getGeometry(const char* geomName)
{
    if (geomName == nullptr) {
        qDebug() << "geomName is nullptr";
        return nullptr;
    }

    if (this->getNumChildren() <= 0) {
        qDebug() << "getGeometry error, no child";
    }
    osg::MatrixTransform* mt = dynamic_cast<osg::MatrixTransform*>(this->getChild(0));
    if (mt->getNumChildren() == 0) return nullptr;

    osg::Node* geoNode = mt->getChild(0);
    osg::Geode* geod = dynamic_cast<osg::Geode*>(geoNode);
    if (geod == nullptr) {
        qDebug() << "geod is nullptr";
        return nullptr;
    }

    osg::Geometry* geom = nullptr;
    for (int i = 0; i < geod->getNumChildren(); i++) {
        auto temp = dynamic_cast<osg::Geometry*>(geod->getDrawable(i));
        if (temp != nullptr && temp->getName() == geomName) {
            geom = temp;
            break;
        }
    }
    return geom;
}

void IncrePointNode::updateNodeBoundingBox(const bool bIncrease, const bool bRigidScan,
    osg::ref_ptr<osg::Vec3Array> pos,
    osg::ref_ptr<osg::IntArray> vFrameIdxArray,
    const std::vector<std::array<float, 16>>& frameRtList,
    osg::Vec3& min,
    osg::Vec3& max)
{
    if (bIncrease)
    {
        // 刚体
        if (bRigidScan)
        {
            computeBoundingBox(pos, min, max);
        }
        // 非刚体
        else {
            computeBoundBoxNonRigid(pos, vFrameIdxArray, frameRtList, min, max);
        }
    }
    else
    {
        // 全量点云，需要恢复为初始包围盒
        m_min = osg::Vec3(1E6, 1E6, 1E6);
        m_max = osg::Vec3(-1E6, -1E6, -1E6);
        computeBoundingBox(pos, min, max);
    }
}

void IncrePointNode::computeBoundingBox(
    osg::ref_ptr<osg::Vec3Array>& pos,
    osg::Vec3& min,
    osg::Vec3& max
)
{
    auto minTmp = osg::Vec3(1E6, 1E6, 1E6);
    auto maxTmp = osg::Vec3(-1E6, -1E6, -1E6);

    const auto vSize = pos->size();
    const auto& vecPos = pos->asVector();
    for (auto vIndex = 0; vIndex < vSize; ++vIndex)
    {
        if (vecPos[vIndex].x() < minTmp.x()) {
            minTmp.x() = vecPos[vIndex].x();
        }
        if (vecPos[vIndex].y() < minTmp.y()) {
            minTmp.y() = vecPos[vIndex].y();
        }
        if (vecPos[vIndex].z() < minTmp.z()) {
            minTmp.z() = vecPos[vIndex].z();
        }
        if (vecPos[vIndex].x() > maxTmp.x()) {
            maxTmp.x() = vecPos[vIndex].x();
        }
        if (vecPos[vIndex].y() > maxTmp.y()) {
            maxTmp.y() = vecPos[vIndex].y();
        }
        if (vecPos[vIndex].z() > maxTmp.z()) {
            maxTmp.z() = vecPos[vIndex].z();
        }
    }

    if (minTmp.x() < m_min.x()) {
        m_min.x() = minTmp.x();
    }
    if (minTmp.y() < m_min.y()) {
        m_min.y() = minTmp.y();
    }
    if (minTmp.z() < m_min.z()) {
        m_min.z() = minTmp.z();
    }
    if (maxTmp.x() > m_max.x()) {
        m_max.x() = maxTmp.x();
    }
    if (maxTmp.y() > m_max.y()) {
        m_max.y() = maxTmp.y();
    }
    if (maxTmp.z() > m_max.z()) {
        m_max.z() = maxTmp.z();
    }
    min = m_min;
    max = m_max;
}

void IncrePointNode::computeBoundBoxNonRigid(osg::ref_ptr<osg::Vec3Array>& pos,
                                             osg::ref_ptr<osg::IntArray> vFrameIdxArray,
                                             const std::vector<std::array<float, 16>>& frameRtList,
                                             osg::Vec3& min, osg::Vec3& max)
{
    const auto vSize = pos->size();
    const auto& vecPos = pos->asVector();
    const auto& vertexFrameId = vFrameIdxArray->asVector();

    // 帧矩阵数量较少，先提前转换为osg::Matrix类型
    const auto frameCount = frameRtList.size();
    std::vector<osg::Matrix> vecFrameMatrix;
    vecFrameMatrix.resize(frameCount);
    for (auto i = 0; i < frameCount; ++i)
    {
        const std::array<float, 16>& floatArray = frameRtList[i];
        vecFrameMatrix[i] = osg::Matrix(floatArray.data());
    }

    int index = 0;
    for (auto vIndex = 0; vIndex < vSize; ++vIndex)
    {
        // 获取顶点对应的帧矩阵
        const auto frameId = vertexFrameId[vIndex];
        const osg::Matrix& frameMatrix = vecFrameMatrix[frameId];

        // 顶点坐标乘以帧矩阵，转换到世界坐标，再进行包围盒计算
        auto posFrame = osg::Vec3(vecPos[vIndex][0], vecPos[vIndex][1], vecPos[vIndex][2]);
        posFrame = posFrame * frameMatrix;

        if (posFrame.x() < m_min.x()) {
            m_min.x() = posFrame.x();
        }
        if (posFrame.y() < m_min.y()) {
            m_min.y() = posFrame.y();
        }
        if (posFrame.z() < m_min.z()) {
            m_min.z() = posFrame.z();
        }
        if (posFrame.x() > m_max.x()) {
            m_max.x() = posFrame.x();
        }
        if (posFrame.y() > m_max.y()) {
            m_max.y() = posFrame.y();
        }
        if (posFrame.z() > m_max.z()) {
            m_max.z() = posFrame.z();
        }
    }

    min = m_min;
    max = m_max;
}

IncreaseUpdatePointOpe::IncreaseUpdatePointOpe(
    const std::string& nodeName,
    PtrIncreUpdateParam pUpdateParam,
    osg::ref_ptr<osg::Group> pGroup,
    OperRunType runType,
    QObject *msgRecver)
    : osg::Operation(nodeName, false)
{
    m_pUpdateParam = pUpdateParam;
    m_pGroup = pGroup;
    m_runType = runType;
    m_msgRecver = msgRecver;
}

osg::Node* IncreaseUpdatePointOpe::findChildByName(const std::string& name, osg::Group* group)
{
    unsigned int count = group->getNumChildren();
    for (unsigned int i = 0; i < count; i++)
    {
        osg::Node * child = group->getChild(i);
        if (child->getName() == name) {
            return child;
        }
    }
    return nullptr;
}

void IncreaseUpdatePointOpe::operator()(osg::Object* caller)
{
    std::string name = this->getName();
    if (!m_pGroup.valid())
    {
        return;
    }
    auto osgNode = findChildByName(name, m_pGroup);
    auto* pPointNode = dynamic_cast<IncrePointNode*>(osgNode);
    if (pPointNode == nullptr)
    {
        pPointNode = new IncrePointNode;
        pPointNode->setName(name);
        m_pGroup->addChild(pPointNode);
    }

    if (m_runType == ORT_Start || m_runType == ORT_Stop) {
        pPointNode->update(m_pUpdateParam, (m_runType == ORT_Start));
    }
    else if (m_runType == ORT_Pause || m_runType == ORT_Resume) {
        qDebug() << "operator() - " << ((m_runType == ORT_Pause)? "Pause": "Resume");
        pPointNode->pauseIncreUpdate(m_runType == ORT_Pause);
    }

    if (m_msgRecver) {
        QMetaObject::invokeMethod(m_msgRecver, "onIncreUpdateOperFinished", 
            Qt::QueuedConnection, Q_ARG(int, (int)m_runType));
    }
}

IncreaseUpdate::IncreaseUpdate(CanvasBase* canvas) : m_sendInterval(30)
{
    m_canvas = canvas;
    m_pointGroup = new osg::Group;

    m_timer = new QTimer;
    connect(m_timer, &QTimer::timeout, this, &IncreaseUpdate::increaseUpdatePointCloud);
}

IncreaseUpdate::~IncreaseUpdate()
{
}

void IncreaseUpdate::startUpdatePointCloud()
{
    // 测试更新方式，true增量更新，false全量更新
    m_bIncrease = true;
    // 测量是否刚体扫描，true刚体扫描，false非刚体扫描；另外，全量更新肯定是刚体
    m_bRigidScan = true;

    // 点云排列类型
    m_pointCloudArrange = ACAT_3D;
    if (m_pointCloudArrange == ACAT_2D)
    {
        createPointCloud2D();
        m_patchVertexCount = 98;
    }
    else
    {
        createPointCloud3D();
        m_patchVertexCount = 10000;
    }

    m_canvas->addOperation(RenderKitSingleton::instance()->addGroup(m_pointGroup));
    m_currentIndex = 0;

    // 开启时间函数，更新点云数据
    m_timer->start(m_sendInterval);
}

void IncreaseUpdate::removeCloudNode()
{
    const auto findChildByName = [](const std::string& name, osg::Group* group)->osg::Node*
    {
        unsigned int count = group->getNumChildren();
        for (unsigned int i = 0; i < count; i++)
        {
            osg::Node * child = group->getChild(i);
            if (child->getName() == name) {
                return child;
            }
        }
        return nullptr;
    };

    const auto osgNode = findChildByName("increase_point", m_pointGroup);
    auto* pPointNode = dynamic_cast<IncrePointNode*>(osgNode);
    if (pPointNode == nullptr) {
        return;
    }
    std::shared_ptr<osg::NodeList> pNodeList(new osg::NodeList);
    pNodeList->push_back(pPointNode);
    m_canvas->addOperation(RenderKitSingleton::instance()->removeNode(pNodeList));
}

void IncreaseUpdate::createPointCloud2D()
{
    const int vNum = 100;
    //const uint32_t totalNum = vNum * vNum * 6;
    const float len = 1.0f;
    const float stride = len / (float)vNum;
    const float angleStride = 360.0 / (float)vNum;

    const auto sizeShow = vNum * vNum;
    m_totalPointSize = sizeShow;
    m_positions = new osg::Vec3Array;
    m_positions->resize(sizeShow);
    m_normals = new osg::Vec3Array;
    m_normals->resize(sizeShow);
    m_colors = new osg::Vec3Array;
    m_colors->resize(sizeShow);
    m_states = new osg::ByteArray;
    m_states->resize(sizeShow);

    m_indices.resize(sizeShow);
    m_frameIndices.resize(sizeShow);

    auto& vecPos = m_positions->asVector();
    auto& vecNor = m_normals->asVector();
    auto& vecClr = m_colors->asVector();
    auto& vecState = m_states->asVector();

    m_frameRtArray.resize(vNum);

    const float startPos = -len * 0.5f;
    //const float startPos = 0;

    // 随机数
    const float randomRange = stride * 0.8f;
    std::default_random_engine radomEngine;
    std::uniform_real_distribution<float> uDistrib(-randomRange, randomRange);   // 左闭右闭区间
    radomEngine.seed(time(0));

    for (int x = 0; x < vNum; ++x)
    {
        // 创建矩阵
        const osg::Vec3 rotateCenter(startPos * 2 + stride * float(x), 0, startPos + stride * 0.5f);
        osg::Matrixf frameRt = osg::Matrix::translate(-rotateCenter) * osg::Matrixf::rotate(osg::DegreesToRadians(angleStride) * float(x), osg::Vec3(0, 1, 0));
        const auto rtPtr = frameRt.ptr();
        std::array<float, 16> stdRt;
        memcpy(stdRt.data(), rtPtr, 16 * sizeof(float));
        m_frameRtArray[x] = stdRt;

        for (int y = 0; y < vNum; ++y)
        {
            int id = x * vNum + y;

            // 使用数修改坐标值，避免过于整齐
            //const float radomZ = uDistrib(radomEngine);
            const float radomZ = 0.0f;
            vecPos[id].x() = startPos + stride * float(x) + radomZ;
            vecPos[id].y() = 0 - radomZ;
            vecPos[id].z() = startPos + stride * float(y) + radomZ;

            //vecPos[id] = vecPos[id] * frameRt;

            vecNor[id].x() = 0;
            vecNor[id].y() = 1;
            vecNor[id].z() = 0;

            vecClr[id] = vecPos[id] * frameRt;
            vecClr[id].y() = 0.5f;

            vecState[id] = (id % 2 == 0)? -1 : 0;

            m_indices[id] = id;
            m_frameIndices[id] = x;
        }
    }
}

void IncreaseUpdate::createPointCloud3D()
{
    const int vNum = 100;
    const float len = 1.0f;
    const float stride = len / (float)vNum;
    const float angleStride = 360.0 / (float)vNum;

    const auto sizeShow = vNum * vNum * vNum;
    m_totalPointSize = sizeShow;
    m_positions = new osg::Vec3Array;
    m_positions->resize(sizeShow);
    m_normals = new osg::Vec3Array;
    m_normals->resize(sizeShow);
    m_colors = new osg::Vec3Array;
    m_colors->resize(sizeShow);
    m_states = new osg::ByteArray;
    m_states->resize(sizeShow);

    m_indices.resize(sizeShow);
    m_frameIndices.resize(sizeShow);
    m_frameRtArray.resize(vNum);

    auto& vecPos = m_positions->asVector();
    auto& vecNor = m_normals->asVector();
    auto& vecClr = m_colors->asVector();
    auto& vecState = m_states->asVector();

    const float startPos = -len * 0.5f;
    //const float startPos = 0;

    // 随机数
    const float randomRange = stride * 0.8f;
    std::default_random_engine radomEngine;
    std::uniform_real_distribution<float> uDistrib(-randomRange, randomRange);   // 左闭右闭区间
    radomEngine.seed(time(0));

    // 生成点云数据
    int eachDimNum = vNum;
    for (int x = 0; x < eachDimNum; ++x)
    {
        // 创建矩阵
        const osg::Vec3 rotateCenter(startPos * 2 + stride * float(x), 0, startPos + stride * 0.5f);
        osg::Matrixf frameRt = osg::Matrix::translate(-rotateCenter) * osg::Matrixf::rotate(osg::DegreesToRadians(angleStride) * float(x), osg::Vec3(0, 1, 0));
        const auto rtPtr = frameRt.ptr();
        std::array<float, 16> stdRt;
        memcpy(stdRt.data(), rtPtr, 16 * sizeof(float));
        m_frameRtArray[x] = stdRt;

        for (uint32_t y = 0; y < eachDimNum; ++y)
        {
            for (int z = 0; z < eachDimNum; ++z)
            {
                uint32_t id = (x * eachDimNum * eachDimNum + y * eachDimNum + z);

                // 使用数修改坐标值，避免过于整齐
                const float radomZ = uDistrib(radomEngine);
                //const float radomZ = 0;
                vecPos[id].x() = startPos + stride * float(x) + radomZ;
                vecPos[id].y() = startPos + stride * float(y) - radomZ;
                vecPos[id].z() = startPos + stride * float(z) + radomZ;

                //vecPos[id] = vecPos[id] * frameRt;

                //vecNor[id].x() = 0;
                //vecNor[id].y() = 1;
                //vecNor[id].z() = 0;
                vecNor[id] = vecPos[id];
                vecNor[id].normalize();

                vecClr[id] = vecPos[id] * frameRt;
                vecClr[id] = osg::Vec3(abs(vecClr[id].x()), 0.8, abs(vecClr[id].z()));

                vecState[id] = (id % 10 == 0) ? 0 : -1;

                m_indices[id] = id;
                m_frameIndices[id] = x;
            }
        }
    }
}

// class EndIncreUpdateOpe: public osg::Operation
// {
// public:
//     explicit EndIncreUpdateOpe(
//         const std::string& nodeName,
//         osg::ref_ptr<osg::Group> pGroup) 
//         : osg::Operation(nodeName, false)
//     {
//         m_pGroup = pGroup;
//     }
// 
//     void operator()(osg::Object* caller) override
//     {
// 
//     }
// 
// private:
//     osg::ref_ptr<osg::Group> m_pGroup;
// };

bool IncreaseUpdate::isUpdateFinished()
{
    bool isFinished = (m_currentIndex >= m_totalPointSize);
    return isFinished;
}

void IncreaseUpdate::initRealtimeIncre()
{
	// 测试更新方式，true增量更新，false全量更新
	m_bIncrease = true;
	// 测量是否刚体扫描，true刚体扫描，false非刚体扫描；另外，全量更新肯定是刚体
	m_bRigidScan = true;

	m_canvas->addOperation(RenderKitSingleton::instance()->addGroup(m_pointGroup));
	m_totalIncrePointSize = 0;
}

void IncreaseUpdate::realtimePointIncreUpdate(LPSn3dIncreasePointCloud increasePointCloud)
{
	//数据转换
	osg::ref_ptr<osg::Vec3Array> patchVert = new osg::Vec3Array;
	patchVert->resize(increasePointCloud->vertex_count);
	osg::ref_ptr<osg::Vec3Array> patchNorm = new osg::Vec3Array;
	patchNorm->resize(increasePointCloud->norma_count);
	osg::ref_ptr<osg::Vec3Array> patchColor = new osg::Vec3Array;
	patchColor->resize(increasePointCloud->vertex_color_count);
	osg::ref_ptr<osg::ByteArray> patchState = new osg::ByteArray;
	patchState->resize(increasePointCloud->vertex_count);

	osg::ref_ptr<osg::IntArray> vIdxArray = new osg::IntArray;
	vIdxArray->resize(increasePointCloud->vertex_count);
	auto& vecIdx = vIdxArray->asVector();

	osg::ref_ptr<osg::IntArray> vFrameIdxArray = new osg::IntArray;
	vFrameIdxArray->resize(increasePointCloud->vertex_count);
	auto& vecFrameIdx = vFrameIdxArray->asVector();

	//重要的是如何填充vecCurXXX，表示当前增量更新的数据
	auto& vecCurPos = patchVert->asVector();
	auto& vecCurNor = patchNorm->asVector();
	auto& vecCurClr = patchColor->asVector();
	auto& vecCurState = patchState->asVector();

	for (uint32_t i = 0; i < increasePointCloud->vertex_count; ++i)
	{
		vecCurPos[i] = osg::Vec3(increasePointCloud->vertex_data[i].x, increasePointCloud->vertex_data[i].y, increasePointCloud->vertex_data[i].z);
		vecCurNor[i] = osg::Vec3(increasePointCloud->norma_data[i].x, increasePointCloud->norma_data[i].y, increasePointCloud->norma_data[i].z);
		vecCurClr[i] = osg::Vec3(increasePointCloud->vertex_color_data[i].x, increasePointCloud->vertex_color_data[i].y, increasePointCloud->vertex_color_data[i].z);
		vecCurState[i] = 0;	//Mark: 0:-1
		vecIdx[i] = increasePointCloud->index[i];
		vecFrameIdx[i] = i;
	}

	m_totalIncrePointSize += increasePointCloud->vertex_count;

	auto pUpdateParam = std::make_shared<IncrePointUpdateParam>();
	pUpdateParam->vertex = patchVert;					//增量点坐标
	pUpdateParam->normal = patchNorm;					//增量点法向量
	pUpdateParam->color = patchColor;					//增量点颜色
	pUpdateParam->state = patchState;					//增量点状态（控制显示颜色和显隐？）
	pUpdateParam->vIdxArray = vIdxArray;				//顶点id
	pUpdateParam->vFrameIdxArray = vFrameIdxArray;		//帧id（非刚体使用）
	pUpdateParam->frameRtArray = m_frameRtArray;		//帧矩阵（非刚体使用）
	pUpdateParam->pointTotalNum = m_totalIncrePointSize;//增量点云总数

	pUpdateParam->curFrameVertex = patchVert;			//当前帧的增量点坐标
	pUpdateParam->curFrameNormal = patchNorm;			//当前帧的增量点法向量

	// 是否刚体扫描
	if (m_bRigidScan)
	{
		pUpdateParam->vFrameIdxArray = nullptr;
		pUpdateParam->frameRtArray = std::vector<std::array<float, 16>>();
	}
	else
	{
		pUpdateParam->vFrameIdxArray = vFrameIdxArray;
		pUpdateParam->frameRtArray = m_frameRtArray;
	}

	// 是否增量更新，如果是全量更新，则肯定是刚体扫描
	if (!m_bIncrease)
	{
		pUpdateParam->vIdxArray = nullptr;
		pUpdateParam->vFrameIdxArray = nullptr;
		pUpdateParam->frameRtArray = std::vector<std::array<float, 16>>();
	}

	// 开始更新
	const auto ope = new IncreaseUpdatePointOpe("increase_point", pUpdateParam, m_pointGroup);
	m_canvas->addOperation(ope);

	//// fit显示
	//static bool bFit = true;
	//if (bFit)
	//{
	//	bFit = false;
	//	const auto pView = m_canvas->mainView();
	//	const auto rotate = osg::Matrix::rotate(osg::Vec3(0, 0, 1), osg::Vec3(0, 1, 0)).getRotate();
	//	const auto center = osg::Vec3(0.0, 0, 0.0);
	//	const auto distance = 6.0f;

	//	auto ca_manip = dynamic_cast<CustomizeManipulator*>(pView->getCameraManipulator());
	//	if (ca_manip != nullptr)
	//	{
	//		ca_manip->setRotation(rotate);
	//		ca_manip->setCenter(center);
	//		ca_manip->setRotateCenter(center);
	//		ca_manip->setDistance(distance);
	//	}
	//}

	m_canvas->frame();
}

void IncreaseUpdate::realtimeCameraPositionUpdate(LPSn3dCameraPosition cameraPosition)
{
	//数据转换
	osg::Vec3 eyePosition(cameraPosition->position.x(), cameraPosition->position.y(), cameraPosition->position.z());
	osg::Vec3 center(cameraPosition->center.x(), cameraPosition->center.y(), cameraPosition->center.z());
	osg::Vec3 up(cameraPosition->up.x(), cameraPosition->up.y(), cameraPosition->up.z());

	//const auto viewMatrix = osg::Matrix::lookAt(eyePosition, center, up);
	const auto viewMatrix = makeLookAt(eyePosition, center, up);
	osg::Vec3d translation;
	osg::Quat rotation;
	osg::Vec3d scale;
	osg::Quat so;
	viewMatrix.decompose(translation, rotation, scale, so);

	float distance = (eyePosition - center).length();

	// 从当前片的局部坐标系转到opengl坐标系(当前片的坐标系z轴向屏幕内，而opengl坐标系的z轴负方向向屏幕内), 绕x轴转180度
	const osg::Quat defaultRotation(osg::PI, osg::Vec3(1.0, 0.0, 0.0));
	//const osg::Quat defaultRotation = osg::Matrix::rotate(osg::DegreesToRadians(180.0), osg::Vec3(0, 1, 0)).getRotate();
	const osg::Quat rotationSeted = defaultRotation*rotation;
	const auto ope = new UpdateCameraOperation("updateCamera", false, rotationSeted, center, distance, 0.05, nullptr, true);
	m_canvas->addOperation(ope);

	m_canvas->frame();
}

void IncreaseUpdate::increaseUpdatePointCloud()
{
    if (isUpdateFinished()) {
        m_timer->stop();

        // 当前帧数据设置为空，不显示当前帧
        auto pUpdateParam = std::make_shared<IncrePointUpdateParam>();
        const auto ope = new IncreaseUpdatePointOpe("increase_point", pUpdateParam, m_pointGroup, IncreaseUpdatePointOpe::ORT_Stop);
        m_canvas->addOperation(ope);
        return;
    }

    //// todo, 测试代码，使用数据出现一部分重复
    //if (m_currentIndex > 30) {
    //    m_currentIndex -= 30;
    //}

    // 全量更新，数据包为所有的数据
    if (!m_bIncrease) {
        m_patchVertexCount = m_totalPointSize;
    }

    const uint32_t stride = m_patchVertexCount;
    uint32_t curStride;

    if (m_currentIndex + stride < m_totalPointSize) {
        curStride = stride;
    }
    else {
        curStride = m_totalPointSize - m_currentIndex;
    }

    osg::ref_ptr<osg::Vec3Array> patchVert = new osg::Vec3Array;
    patchVert->resize(curStride);
    osg::ref_ptr<osg::Vec3Array> patchNorm = new osg::Vec3Array;
    patchNorm->resize(curStride);
    osg::ref_ptr<osg::Vec3Array> patchColor = new osg::Vec3Array;
    patchColor->resize(curStride);
    osg::ref_ptr<osg::ByteArray> patchState = new osg::ByteArray;
    patchState->resize(curStride);

    osg::ref_ptr<osg::IntArray> vIdxArray = new osg::IntArray;
    vIdxArray->resize(curStride);
    auto& vecIdx = vIdxArray->asVector();

    osg::ref_ptr<osg::IntArray> vFrameIdxArray = new osg::IntArray;
    vFrameIdxArray->resize(curStride);
    auto& vecFrameIdx = vFrameIdxArray->asVector();

    auto& vecPos = m_positions->asVector();
    auto& vecNor = m_normals->asVector();
    auto& vecClr = m_colors->asVector();
    auto& vecState = m_states->asVector();

    auto& vecCurPos = patchVert->asVector();
    auto& vecCurNor = patchNorm->asVector();
    auto& vecCurClr = patchColor->asVector();
    auto& vecCurState = patchState->asVector();

    for (uint32_t i = 0; i < curStride; ++i)
    {
        uint32_t idx = i + m_currentIndex;
        vecCurPos[i] = vecPos[idx];
        vecCurNor[i] = vecNor[idx];
        vecCurClr[i] = vecClr[idx];
        vecCurState[i] = vecState[idx];
        vecIdx[i] = m_indices[idx];
        vecFrameIdx[i] = m_frameIndices[idx];
    }

    m_currentIndex += curStride;

    uint32_t showPointSize = 0;
    if (m_currentIndex <= m_totalPointSize)
    {
        showPointSize = m_currentIndex;
    }
    else
    {
        showPointSize = m_totalPointSize;
        m_timer->stop();
    }

    auto pUpdateParam = std::make_shared<IncrePointUpdateParam>();
    pUpdateParam->vertex = patchVert;
    pUpdateParam->normal = patchNorm;
    pUpdateParam->color = patchColor;
    pUpdateParam->state = patchState;
    pUpdateParam->vIdxArray = vIdxArray;
    pUpdateParam->vFrameIdxArray = vFrameIdxArray;
    pUpdateParam->frameRtArray = m_frameRtArray;
    pUpdateParam->pointTotalNum = showPointSize;

    pUpdateParam->curFrameVertex = patchVert;
    pUpdateParam->curFrameNormal = patchNorm;

    // 是否刚体扫描
    if (m_bRigidScan)
    {
        pUpdateParam->vFrameIdxArray = nullptr;
        pUpdateParam->frameRtArray = std::vector<std::array<float, 16>>();
    }
    else
    {
        pUpdateParam->vFrameIdxArray = vFrameIdxArray;
        pUpdateParam->frameRtArray = m_frameRtArray;
    }

    // 是否增量更新，如果是全量更新，则肯定是刚体扫描
    if (!m_bIncrease)
    {
        pUpdateParam->vIdxArray = nullptr;
        pUpdateParam->vFrameIdxArray = nullptr;
        pUpdateParam->frameRtArray = std::vector<std::array<float, 16>>();
    }

    // 开始更新
    const auto ope = new IncreaseUpdatePointOpe("increase_point", pUpdateParam, m_pointGroup);
    m_canvas->addOperation(ope);

    // fit显示
    static bool bFit = true;
    if (bFit)
    {
        bFit = false;
        const auto pView = m_canvas->mainView();
        const auto rotate = osg::Matrix::rotate(osg::Vec3(0, 0, 1), osg::Vec3(0, 1, 0)).getRotate();
        const auto center = osg::Vec3(0.0, 0, 0.0);
        const auto distance = 6.0f;

        auto ca_manip = dynamic_cast<CustomizeManipulator*>(pView->getCameraManipulator());
        if (ca_manip != nullptr)
        {
            ca_manip->setRotation(rotate);
            ca_manip->setCenter(center);
            ca_manip->setRotateCenter(center);
            ca_manip->setDistance(distance);
        }
    }

    m_canvas->frame();
}

void IncreaseUpdate::pauseIncreUpdate(bool needPause)
{
    if (isUpdateFinished()) {
        return;
    }

    if (needPause) {
        m_timer->stop();
        qDebug() << "IncreaseUpdatePointOpe - ORT_Pause";
        //调用stop后可能不会立即停止，所以使用singleShot执行暂停
        QTimer::singleShot(m_sendInterval, [this]() {
            const auto ope = new IncreaseUpdatePointOpe("increase_point", nullptr, m_pointGroup, IncreaseUpdatePointOpe::ORT_Pause, this);
            m_canvas->addOperation(ope);
        });
    }
    else {
        qDebug() << "IncreaseUpdatePointOpe - ORT_Resume";
        const auto ope = new IncreaseUpdatePointOpe("increase_point", nullptr, m_pointGroup, IncreaseUpdatePointOpe::ORT_Resume, this);
        m_canvas->addOperation(ope);
    }
}

void IncreaseUpdate::onIncreUpdateOperFinished(int operRunType)
{
    if (IncreaseUpdatePointOpe::ORT_Resume == operRunType) {
        qDebug() << "onIncreUpdateOperFinished - resumeeeeeeeeeeeeeeeeeeeeeeeeeee oper finished";
        m_timer->start(m_sendInterval);
    }
    else if(IncreaseUpdatePointOpe::ORT_Pause == operRunType) {
        qDebug() << "onIncreUpdateOperFinished - pauseeeeeeeeeeeeeeeeeeeeeeeeeee oper finished";
    }
}

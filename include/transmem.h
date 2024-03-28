#ifndef _TRANS_MEM_H_
#define _TRANS_MEM_H_

#include <QObject>
#include <QThread>
#include <QVariantMap>
#include <QSysInfo>
#include <QVersionNumber>
#include <QSharedMemory>
#include <QDataStream>

#define MAX_CAM_NUM 4
#define CAM_MEM_KEY_PREFIX QStringLiteral("cam")
#define CURRENT_PC_MEM_KEY QStringLiteral("currentPointCloud")
#define CURRENT_MARKER_MEM_KEY QStringLiteral("currentMarker")
#define WHOLE_PC_MEM_KEY QStringLiteral("wholePointCloud")
#define FAILED_PC_MEM_KEY QStringLiteral("failedPointCloud")
#define FRAME_MARKER_POINT_MEM_KEY QStringLiteral("frameMarkerPoint")
#define WHOLE_MARKER_POINT_MEM_KEY QStringLiteral("wholeMarkerPoint")
#define MESH_DATA_MEM_KEY QStringLiteral("meshData")
#define EDIT_APPLY_DATA_MEM_KEY QStringLiteral("editApplyData")
#define MEASURE_NODE_KEY QStringLiteral("measureEditLine")
// 多工程数据节点名称
#define MULTIPLE_PORJECT_DATA_MEM_KEY QStringLiteral("multipleProjectData") 
#define BODY_SCAN_MEM_KEY QStringLiteral("bodyScanPointCloud")
//多工程标志点节点
#define MULTI_PROJECT_MARKER_NODE "MarkerGroupNode"
//拼接场景下标志点节点名称
#define MULTI_PROJECT_MARKER_NODE_ALIGN "MarkerGroupNodeAlign"

#define SCENE_DATA_MEMORY_KEY_1 QStringLiteral("sceneData1")
#define SCENE_DATA_MEMORY_KEY_2 QStringLiteral("sceneData2")
#define SCENE_DATA_MEMORY_KEY_3 QStringLiteral("sceneData3")
#define SCENE_BIG_DATA_MEMORY_KEY QStringLiteral("sceneBigData")

//by cjz
#define HOLE_LINE_DATA_MEM_KEY QStringLiteral("holeline")
#define FILL_HOLE_MESH_DATA_MEM_KEY QStringLiteral("fillholemesh")
#define HOLE_GROUP_MEM_KEY QStringLiteral("holeGroup")					//补的洞操作相关的节点
//#define EDIT_APPLY_DATA_REVERSE_LENGTH (1024*1024*3)
#define EDIT_APPLY_DATA_REVERSE_LENGTH (1024*1024*500)

struct CommonHeader{
    int headerLength = 0;
    bool handled = false;
};

template <typename T>
class TransMem
{
public:
    explicit TransMem(QString key)
    {
        m_memory.setKey(key);
		m_commonHeader.headerLength = 0;
		m_commonHeader.handled = false;
    }
    explicit TransMem()
    {

    }
    ~TransMem()
    {
    }

    QString key() const{ return m_memory.key(); }
    void setKey(const QString &newKey){
        if (m_memory.key() == newKey) return;
        m_memory.setKey(newKey);
    }

    int size() const{ return m_memory.size(); }
    void setSize(int newSize){
        if (m_memory.size() >= newSize) return;

		//备份头数据重新设置？
		unsigned char* headCopy = nullptr;
		if (m_memory.size() > m_commonHeader.headerLength && m_commonHeader.headerLength>0){

			auto d = reinterpret_cast<char*>(m_memory.data());
			d += sizeof(CommonHeader) + m_commonHeader.headerLength;
			headCopy = new unsigned char[sizeof(CommonHeader) + m_commonHeader.headerLength];
		}
		

       m_memory.destroyed();
       bool ret= m_memory.create(newSize);
	    
	   qDebug() << "m_memory.create ret=" << ret << "err=" << m_memory.errorString() << "size=" << m_memory.size();
	  
		if (headCopy){
			m_memory.lock();
			memcpy(m_memory.data(), headCopy, sizeof(CommonHeader) + m_commonHeader.headerLength);
			m_memory.unlock();
			delete[] headCopy;
			headCopy = nullptr;
		}
    }

    bool create(int size){
        return m_memory.create(size);
    }

    bool isAttached() const {
        return m_memory.isAttached();
    }

    bool attach(QSharedMemory::AccessMode mode){
        return m_memory.attach(mode);
    }
    bool detach(){
        return m_memory.detach();
    }
    T header(){
        auto dataPtr = static_cast<char*>(m_memory.data());
        Q_ASSERT(dataPtr != nullptr);
        memcpy(&m_commonHeader, m_memory.data(), sizeof(CommonHeader));
        QByteArray array(dataPtr + sizeof(CommonHeader), m_commonHeader.headerLength);
        QDataStream ds(array);
        T h;
        ds >> h;
        return h;
    }
	template <typename UserT>
	void getHeader(UserT& h){
		auto dataPtr = static_cast<char*>(m_memory.data());
		Q_ASSERT(dataPtr != nullptr);
		memcpy(&m_commonHeader, m_memory.data(), sizeof(CommonHeader));
		QByteArray array(dataPtr + sizeof(CommonHeader), m_commonHeader.headerLength);
		QDataStream ds(array);
	 
		ds >> h;
		
	}
    void setHeader(const T* h){
        if (h == nullptr) return;
        QByteArray array;
        QDataStream ds(&array, QIODevice::WriteOnly);
        ds << *h;
        m_commonHeader.headerLength = array.size();
        m_commonHeader.handled = false;
        memcpy((char*)m_memory.data(), &m_commonHeader, sizeof(CommonHeader));
        memcpy((char*)m_memory.data() + sizeof(CommonHeader), array.constData(), m_commonHeader.headerLength);
    }

    int headerSize() const {
        return m_commonHeader.headerLength;
    }

    bool handled() const{
        memcpy((char*)&m_commonHeader, m_memory.data(), sizeof(CommonHeader));
        return m_commonHeader.handled;
    }

    void setHandled(bool h = true){
        m_commonHeader.handled = h;
        memcpy((char*)m_memory.data(), &m_commonHeader, sizeof(CommonHeader));
    }

    char* data(){

        auto d = reinterpret_cast<char*>(m_memory.data());
        d += sizeof(CommonHeader) + m_commonHeader.headerLength;

        return d;
    }
    int maxDataLength() const{
        return m_memory.size() - (sizeof(CommonHeader) + m_commonHeader.headerLength);
    }
    void lock()
    {
        m_memory.lock();
    }
    void unlock()
    {
        m_memory.unlock();
    }
private:
    QSharedMemory m_memory;
    CommonHeader m_commonHeader;
};

#endif // _TRANS_MEM_H_

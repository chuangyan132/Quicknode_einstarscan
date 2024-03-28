#ifndef  _PUBLICCLASS_H_
#define  _PUBLICCLASS_H_


#pragma once
#include <QString>
#include <QObject>
#include <QtRemoteObjects>
#include<QVector3D>
#include "sn3d_smc.h"

//shared memory, point cloud header
class PointCloudMemoryHeader
{
	Q_GADGET

		Q_PROPERTY(bool isOpen READ isOpen WRITE setIsOpen)
		Q_PROPERTY(QString name READ name WRITE setName)
		Q_PROPERTY(int alignIndex READ alignIndex WRITE setAlignIndex)
		Q_PROPERTY(QString groupName READ groupName WRITE setGroupName)
		Q_PROPERTY(int pointCount READ pointCount WRITE setPointCount)
		Q_PROPERTY(bool hasTexture READ hasTexture WRITE setHasTexture)
		Q_PROPERTY(bool hasNormal READ hasNormal WRITE setHasNormal)
		Q_PROPERTY(bool incremental READ incremental WRITE setIncremental)
		Q_PROPERTY(bool hasMarkers READ hasMarkers WRITE setHasMarkers)
		Q_PROPERTY(int markerCount READ markerCount WRITE setMarkerCount)
		Q_PROPERTY(bool haveUsed READ haveUsed WRITE setHaveUsed)
		Q_PROPERTY(bool hasTexurePicture READ hasTexurePicture WRITE setHasTexurePicture)
		Q_PROPERTY(int faceCount READ faceCount WRITE setFaceCount)
		Q_PROPERTY(int textureImgWidth READ textureImgWidth WRITE setTextureImgWidth)
		Q_PROPERTY(int textureImgHeight READ textureImgHeight WRITE setTextureImgHeight)
		Q_PROPERTY(int textureUVCount READ textureUVCount WRITE setTextureUVCount)
		Q_PROPERTY(bool hasFaceNormal READ hasFaceNormal WRITE setHasFaceNormal)
		Q_PROPERTY(int packId READ packId WRITE setPackId)
		Q_PROPERTY(int totalPacks READ totalPacks WRITE setTotalPacks)
		Q_PROPERTY(int dataSize READ dataSize WRITE setDataSize)
public:
	PointCloudMemoryHeader() : _isOpen(), _name(), _alignIndex(), _groupName(), _pointCount(), _hasTexture(), _hasNormal(), _incremental(), _hasMarkers(), _markerCount(), _haveUsed(), _hasTexurePicture(), _faceCount(), _textureImgWidth(), _textureImgHeight(), _textureUVCount(), _hasFaceNormal(), _packId(), _totalPacks(), _dataSize() {}
	explicit PointCloudMemoryHeader(bool isOpen, QString name, int alignIndex, QString groupName, int pointCount, bool hasTexture, bool hasNormal, bool incremental, bool hasMarkers, int markerCount, bool haveUsed, bool hasTexurePicture, int faceCount, int textureImgWidth, int textureImgHeight, int textureUVCount, bool hasFaceNormal, int packId, int totalPacks, int dataSize) : _isOpen(isOpen), _name(name), _alignIndex(alignIndex), _groupName(groupName), _pointCount(pointCount), _hasTexture(hasTexture), _hasNormal(hasNormal), _incremental(incremental), _hasMarkers(hasMarkers), _markerCount(markerCount), _haveUsed(haveUsed), _hasTexurePicture(hasTexurePicture), _faceCount(faceCount), _textureImgWidth(textureImgWidth), _textureImgHeight(textureImgHeight), _textureUVCount(textureUVCount), _hasFaceNormal(hasFaceNormal), _packId(packId), _totalPacks(totalPacks), _dataSize(dataSize) {}
	bool isOpen() const { return _isOpen; }
	void setIsOpen(bool isOpen) { if (isOpen != _isOpen) { _isOpen = isOpen; } }
	QString name() const { return _name; }
	void setName(QString name) { if (name != _name) { _name = name; } }
	int alignIndex() const { return _alignIndex; }
	void setAlignIndex(int alignIndex) { if (alignIndex != _alignIndex) { _alignIndex = alignIndex; } }
	QString groupName() const { return _groupName; }
	void setGroupName(QString groupName) { if (groupName != _groupName) { _groupName = groupName; } }
	int pointCount() const { return _pointCount; }
	void setPointCount(int pointCount) { if (pointCount != _pointCount) { _pointCount = pointCount; } }
	bool hasTexture() const { return _hasTexture; }
	void setHasTexture(bool hasTexture) { if (hasTexture != _hasTexture) { _hasTexture = hasTexture; } }
	bool hasNormal() const { return _hasNormal; }
	void setHasNormal(bool hasNormal) { if (hasNormal != _hasNormal) { _hasNormal = hasNormal; } }
	bool incremental() const { return _incremental; }
	void setIncremental(bool incremental) { if (incremental != _incremental) { _incremental = incremental; } }
	bool hasMarkers() const { return _hasMarkers; }
	void setHasMarkers(bool hasMarkers) { if (hasMarkers != _hasMarkers) { _hasMarkers = hasMarkers; } }
	int markerCount() const { return _markerCount; }
	void setMarkerCount(int markerCount) { if (markerCount != _markerCount) { _markerCount = markerCount; } }
	bool haveUsed() const { return _haveUsed; }
	void setHaveUsed(bool haveUsed) { if (haveUsed != _haveUsed) { _haveUsed = haveUsed; } }
	bool hasTexurePicture() const { return _hasTexurePicture; }
	void setHasTexurePicture(bool hasTexurePicture) { if (hasTexurePicture != _hasTexurePicture) { _hasTexurePicture = hasTexurePicture; } }
	int faceCount() const { return _faceCount; }
	void setFaceCount(int faceCount) { if (faceCount != _faceCount) { _faceCount = faceCount; } }
	int textureImgWidth() const { return _textureImgWidth; }
	void setTextureImgWidth(int textureImgWidth) { if (textureImgWidth != _textureImgWidth) { _textureImgWidth = textureImgWidth; } }
	int textureImgHeight() const { return _textureImgHeight; }
	void setTextureImgHeight(int textureImgHeight) { if (textureImgHeight != _textureImgHeight) { _textureImgHeight = textureImgHeight; } }
	int textureUVCount() const { return _textureUVCount; }
	void setTextureUVCount(int textureUVCount) { if (textureUVCount != _textureUVCount) { _textureUVCount = textureUVCount; } }
	bool hasFaceNormal() const { return _hasFaceNormal; }
	void setHasFaceNormal(bool hasFaceNormal) { if (hasFaceNormal != _hasFaceNormal) { _hasFaceNormal = hasFaceNormal; } }
	int packId() const { return _packId; }
	void setPackId(int packId) { if (packId != _packId) { _packId = packId; } }
	int totalPacks() const { return _totalPacks; }
	void setTotalPacks(int totalPacks) { if (totalPacks != _totalPacks) { _totalPacks = totalPacks; } }
	int dataSize() const { return _dataSize; }
	void setDataSize(int dataSize) { if (dataSize != _dataSize) { _dataSize = dataSize; } }
private:
	bool _isOpen;
	QString _name;
	int _alignIndex;
	QString _groupName;
	int _pointCount;
	bool _hasTexture;
	bool _hasNormal;
	bool _incremental;
	bool _hasMarkers;
	int _markerCount;
	bool _haveUsed;
	bool _hasTexurePicture;
	int _faceCount;
	int _textureImgWidth;
	int _textureImgHeight;
	int _textureUVCount;
	bool _hasFaceNormal;
	int _packId;
	int _totalPacks;
	int _dataSize;
};
inline bool operator==(const PointCloudMemoryHeader &left, const PointCloudMemoryHeader &right) Q_DECL_NOTHROW {
	return left.isOpen() == right.isOpen() && left.name() == right.name() && left.alignIndex() == right.alignIndex() && left.groupName() == right.groupName() && left.pointCount() == right.pointCount() && left.hasTexture() == right.hasTexture() && left.hasNormal() == right.hasNormal() && left.incremental() == right.incremental() && left.hasMarkers() == right.hasMarkers() && left.markerCount() == right.markerCount() && left.haveUsed() == right.haveUsed() && left.hasTexurePicture() == right.hasTexurePicture() && left.faceCount() == right.faceCount() && left.textureImgWidth() == right.textureImgWidth() && left.textureImgHeight() == right.textureImgHeight() && left.textureUVCount() == right.textureUVCount() && left.hasFaceNormal() == right.hasFaceNormal() && left.packId() == right.packId() && left.totalPacks() == right.totalPacks() && left.dataSize() == right.dataSize();
}
inline bool operator!=(const PointCloudMemoryHeader &left, const PointCloudMemoryHeader &right) Q_DECL_NOTHROW {
	return !(left == right);
}

inline QDataStream &operator<<(QDataStream &ds, const PointCloudMemoryHeader &obj) {
	QtRemoteObjects::copyStoredProperties(&obj, ds);
	return ds;
}

inline QDataStream &operator>>(QDataStream &ds, PointCloudMemoryHeader &obj) {
	QtRemoteObjects::copyStoredProperties(ds, &obj);
	return ds;
}

//shared memory, video data header
class VideoDataMemoryHeader
{
	Q_GADGET
		Q_PROPERTY(int camId READ camId WRITE setCamId)
		Q_PROPERTY(int width READ width WRITE setWidth)
		Q_PROPERTY(int height READ height WRITE setHeight)
		Q_PROPERTY(int channel READ channel WRITE setChannel)
		Q_PROPERTY(int dataChanged READ dataChanged WRITE setDataChanged)
public:
	VideoDataMemoryHeader() : _camId(), _width(), _height(), _channel(), _dataChanged() {}
	explicit VideoDataMemoryHeader(int camId, int width, int height, int channel, int dataChanged) : _camId(camId), _width(width), _height(height), _channel(channel), _dataChanged(dataChanged) {}
	int camId() const { return _camId; }
	void setCamId(int camId) { if (camId != _camId) { _camId = camId; } }
	int width() const { return _width; }
	void setWidth(int width) { if (width != _width) { _width = width; } }
	int height() const { return _height; }
	void setHeight(int height) { if (height != _height) { _height = height; } }
	int channel() const { return _channel; }
	void setChannel(int channel) { if (channel != _channel) { _channel = channel; } }
	int dataChanged() const { return _dataChanged; }
	void setDataChanged(int dataChanged) { if (dataChanged != _dataChanged) { _dataChanged = dataChanged; } }
private:
	int _camId;
	int _width;
	int _height;
	int _channel;
	int _dataChanged;
};

inline bool operator==(const VideoDataMemoryHeader &left, const VideoDataMemoryHeader &right) Q_DECL_NOTHROW {
	return left.camId() == right.camId() && left.width() == right.width() && left.height() == right.height() && left.channel() == right.channel() && left.dataChanged() == right.dataChanged();
}
inline bool operator!=(const VideoDataMemoryHeader &left, const VideoDataMemoryHeader &right) Q_DECL_NOTHROW {
	return !(left == right);
}

inline QDataStream &operator<<(QDataStream &ds, const VideoDataMemoryHeader &obj) {
	QtRemoteObjects::copyStoredProperties(&obj, ds);
	return ds;
}

inline QDataStream &operator>>(QDataStream &ds, VideoDataMemoryHeader &obj) {
	QtRemoteObjects::copyStoredProperties(ds, &obj);
	return ds;
}

class CameraPosition
{
	Q_GADGET

		Q_PROPERTY(QVector3D position READ position WRITE setPosition)
		Q_PROPERTY(QVector3D center READ center WRITE setCenter)
		Q_PROPERTY(QVector3D up READ up WRITE setUp)
		Q_PROPERTY(bool boundbox READ boundbox WRITE setBoundbox)
public:
	CameraPosition() : _position(), _center(), _up(), _boundbox() {}
	explicit CameraPosition(QVector3D position, QVector3D center, QVector3D up, bool boundbox) : _position(position), _center(center), _up(up), _boundbox(boundbox) {}
	QVector3D position() const { return _position; }
	void setPosition(QVector3D position) {if (position != _position) { _position = position; } }
	QVector3D center() const { return _center; }
	void setCenter(QVector3D center) { if (center != _center) { _center = center; }}
	QVector3D up() const { return _up; }
	void setUp(QVector3D up) {if (up != _up) { _up = up; } }
	bool boundbox() const { return _boundbox; }
	void setBoundbox(bool boundbox) {if (boundbox != _boundbox) { _boundbox = boundbox; } }
private:
	QVector3D _position;
	QVector3D _center;
	QVector3D _up;
	bool _boundbox;
};

inline bool operator==(const CameraPosition &left, const CameraPosition &right) Q_DECL_NOTHROW {
	return left.position() == right.position() && left.center() == right.center() && left.up() == right.up() && left.boundbox() == right.boundbox();
}
inline bool operator!=(const CameraPosition &left, const CameraPosition &right) Q_DECL_NOTHROW {
	return !(left == right);
}

inline QDataStream &operator<<(QDataStream &ds, const CameraPosition &obj) {
	QtRemoteObjects::copyStoredProperties(&obj, ds);
	return ds;
}

inline QDataStream &operator>>(QDataStream &ds, CameraPosition &obj) {
	QtRemoteObjects::copyStoredProperties(ds, &obj);
	return ds;
}

enum ScanType {
	ST_FIXED = 0,           // fix scan
	ST_HD = 1,				// HD
	ST_RAPID = 2,			// rapid scan         
	ST_E3_HX_NORMAL = 3,	// E3_HX rapid
	ST_E3_HX_LASER = 4,		// E3_HX laser
	ST_E3_H_NORMAL = 5,		// E3_H normal
	ST_E3_H_BODY = 6,		// E3_H body
	ST_E3_H_PORTRAIT = 7,	// E3_H portrait
	ST_E3_CALIB = 8,	     // calib
	ST_E10_CALIB = 9,
	ST_E10_NORMAL = 10,
	ST_UE_LASER = 11,       // UE
};

enum AlignType {
	AT_FEATURES = 0,         // features align
	AT_MARKERS = 1,          // markers algin
	AT_HYBRID = 2,           // features and markers mix align,first use markers ,if failed use features align again. 
	//AT_AUTO = 4,            // auto switch(if start scanning detect markers use markers align ,else use features align)
	//AT_TURTABLE = 5,	     //turtable align
	//AT_CODE_POINT = 6,	      //code point align
	AT_GLOBAL_POINT = 7,        //frame markers align
	AT_TEXTURE = 8        //texture align
};

enum ScanMode {
	AT_PORTRAIT = 0,        
	AT_OBJECT = 1,          
	AT_SMALL_OBJECT = 2     
};

enum MeshType {
	AT_UNWATERTIGHT = 0,    
	AT_WATERTIGHT = 1,      
	AT_HALFWATERTIGHT = 2   
};

enum DeviceSubType{
	DST_PRO = 1,//2X 
	DST_PRO_PLUS = 2,//EP 2X Plus
	DST_S_S10 = 3,
	DST_S20 = 5,
	DST_S2PLUS = 6,
	DST_E3_HX = 8,
	DST_E3_H = 9,
	DST_E10 = 10,
	DST_UE = 11,
};
enum AsyncActionType {
	AAT_CHECK_DEVICE = 0,
	AAT_ENTER_CALI = 1,
	AAT_CHANGE_CALI_TYPE = 2,
	AAT_EXIT_CALI = 3,
	AAT_COMPUTE_CALI = 4,
	AAT_NEW_PROJECT = 5,
	AAT_OPEN_PROJECT = 6,
	AAT_ENTER_SCAN = 7,
	AAT_EXIT_SCAN = 8,
	AAT_CANCEL_SCAN = 9,
	AAT_END_SCAN = 10,
	AAT_SCAN = 11,
	AAT_WHITE_BAL = 12,
	AAT_MANUAL_ALIGN = 13,
	AAT_MESH = 14,
	AAT_SIMPLIFY = 15,
	AAT_EDIT = 16,
	AAT_EDIT_LIST = 17,
	AAT_DEL_POINT_CLOUD = 18,
	AAT_SAVE = 19,
	AAT_FIX_SINGLE_EDIT = 20,
	AAT_AXIS_VERIFY = 21,
	AAT_FIX_SCAN = 22,
	AAT_FIX_REMOVE_DATA = 23,
	AAT_FIX_UPDATE_DATA_RT = 24,
	AAT_EXPORT_SHARE_DATA = 25,
	AAT_LOAD_MODEL = 26,
	AAT_ENTER_POSTDATAPROCESS = 27,
	AAT_EXIT_POSTDATAPROCESS = 28,
	AAT_ENTER_FILL_HOLE_MODE = 29,
	AAT_APPLAY_FILLHOLE = 30,
	AAT_FILLING_HOLE = 31,
	AAT_SMOOTH = 32,
	AAT_SHARP = 33,
	AAT_ZOOM = 34,
	AAT_REBUILDTEXTURE = 35,
	AAT_RESTORE_RAW_MESH = 36,
	AAT_CANCEL_ALIGEN = 37,
	AAT_DELETE_MESH_FACE = 38,
	AAT_MULTI_PROJECT_ALIGING = 39,
	AAT_MULTI_PROJECT_CANCEL_ALIG = 40,
	AAT_MULTI_PROJECT_MESHING = 41,
	AAT_MULTI_PROJECT_REMOVE = 42,
	AAT_MULTI_PROJECT_APPLY_ALIG = 43,
	AAT_MULTI_PROJECT_APPLI_EDIT = 44,
	AAT_LOAING_SOLUTION = 45,
	AAT_MULTI_PROJECT_LOAD_SUB_PROJECT_DATA = 46,
	AAT_MULTI_PROJECT_LOAD_MAIN_PROJECT = 47,
	AAT_MULTI_PROJECT_SAVE = 48,
	AAT_MULTI_GENERATE_DATA = 49,
	AAT_POINT_TOO_LITTLE = 50,
	AAT_RECONNECT_DEVICE = 51,
	AAT_GENERATE_DATA = 52,
	AAT_REMOVE_SMALL_PART = 53,
	AAT_MESH_OPTIMATION = 54,
	ATT_MEMORY_DEFICIENCY = 55,
	ATT_ONLY_LOAD_FRAMER_MARKER = 56,
	ATT_CHANGE_FRAMERMARK_OR_POINTCLOUD_MODE = 57,
	ATT_UNKONW_ERROR = 58,
	ATT_PREDICTED_MEMORY_DEFICIENCY = 59,
	ATT_SCANING_NOT_ENOUGH_MEMORY = 60,
	ATT_FLIP = 61,
	ATT_MIRROR = 62,
	ATT_SQUARE_OPT = 63,
	ATT_SQUARE = 64,
	ATT_INTERSECTION = 65,
	ATT_AUTOEXPOSING = 66,
	ATT_CLOSE_AUTO_CONNECT_DEVICE = 67,
	ATT_DEFAULT = 68,
};
enum MemoryType {
	MT_POINT_CLOUD = 0,
	MT_DELETE_POINTS = 1,
	MT_MARKERS = 2,
	MT_TRI_MESH = 3,
	MT_VIDEO_DATA = 4,
	MT_RANGE_DATA = 5,
	MT_HOLE_LINE = 6,
	MT_FILL_HOLE_MESH = 7,
	MT_MULTI_PROJECT_POINT_CLOUD = 8,
	MT_BODY_SCAN_POINT_CLOUD = 9,
	MT_GENERATE_POINT_CLOUD_EDIT = 10,
	MT_SCENE_DATA = 11
};

enum LogType
{
	SEND_MESSAGE,				//send to service
	RECEIVE_MESSAGE,			//receive message from service,only correct result
	ERROR_MESSAGE,				//receive message from service,only incorrect result
	INTERAL_LOG					//interal log
};
//enum DeviceEvent {
//	DE_NULL = -1,
//	DE_DOUBLECLICK = 0,
//	DE_CLICK = 1,
//	DE_PLUS = 2,						// 2x series device little button
//	DE_SUB = 3,							// 2x series device little button
//	DE_E3_DISTANCE_PLUS = 4,			// E3 series device little button
//	DE_E3_DISTANCE_SUB = 5,				// E3 series device little button
//	DE_E3_TAP_POSITION_PLUS = 6,		// E3 series device little button
//	DE_E3_TAP_POSITION_SUB = 7,			// E3 series device little button
//	DE_LONGPRESS = 8,
//	DE_UE_CENTER_BTN_MEN,
//	DE_UE_LBTN_LONGPRESS,
//	DE_UE_RBTN_LONGPRESS,
//	DE_UE_UBTN_DCLICK,
//	DE_UE_UBTN_LONGPRESS,
//	DE_UE_DBTN_DCLICK,
//	DE_UE_DBTN_LONGPRESS,
//	DE_UE_BTN_DCLICK
//};

enum ERROR_CODE
{
	SN3D_RET_NOERROR = 0,	//no error
	SN3D_RET_PARAM_ERROR = -1,	//param error
	SN3D_RET_ORDER_ERROR = -2,	//call order
	SN3D_RET_TIME_OUT_ERROR = -3,	//time out error
	SN3D_RET_NOT_SUPPORT_ERROR = -4,	//not support error
	SN3D_RET_NO_DEVICE_ERROR = -6,	//not connect device
	SN3D_RET_DEVICE_LICENSE_ERROR = -7,	//device license error
	SN3D_RET_GPU_ERROR = -8,	//graphic card error
	SN3D_RET_INNER_ERROR = -9,	//sdk inner card error
	SN3D_RET_NOT_CALIBRATE_ERROR = -10,	//not calibrate error
	SN3D_RET_LOST_CONFIG_FILE_ERROR = -11,	//not calibrate error
	SN3D_RET_NO_DATA_ERROR = -12,	//no data error
	SN3D_RET_LOST_CALIBRATE_FILE_ERROR = -13,	//not calibrate error
	SN3D_RET_NO_GLOBAL_MARK_POINT_PARAM_ERROR = -14,	//no global mark point
	SN3D_RET_DEVICE_HAS_NO_CALIB_PARAM = -15,		// no calibrate in device
	SN3D_RET_CALIB_SYNC_CALLBALL_FAIL = -16,		// calibrate video callback and state callback failed
	SN3D_RET_DEVICE_EXPIRE_ERROR = -17   //device expire
};

enum ScanStatus {
    SS_PRE_SCAN = 1,
    SS_PRE_SCANNING = 2,
    SS_SCAN = 3,
    SS_PAUSED = 4,
    SS_SCAN_STOPED = 5,
};

// begin used for E7 Plus Device
namespace E7PlusDevice{
    enum ProjectType {
        PT_FIX = 0,
        PT_HANDLE_HD = 1,
        PT_HANDLE_RAPID = 2,
    };

    enum MeshModel {
        MT_NON_WATERTIGHT = 0,
        MT_WATERTIGHT = 1,
    };

    enum DetailType{
        High,
        Medium,
        Low
    };

    enum FixAlignType {
        FIX_AT_MARKERS = 1,          // markers algin
        FIX_AT_FEATURES = 2,         // features align
        FIX_AT_TURTABLE = 3,         // turtable align
    };
}
// end used for E7 Plus Device


const QString C_E3HDevice = "DST_E3_H";
const QString C_E3HXDevice = "DST_E3_HX";
const QString C_E7PlusDevice = "DST_PRO_PLUS";
const QString C_E7ProDevice = "DST_PRO";
const QString C_E10Device = "DST_E10";

#endif

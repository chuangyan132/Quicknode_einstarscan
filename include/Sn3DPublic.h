#ifndef _SN3DPUBLIC_H_
#define _SN3DPUBLIC_H_

#define  SN3D_MAX_FACELIMIT 20000000
#define  SN3D_MAX_COUNT 256

typedef struct
{
	float              x;						//X coordinate value
	float              y;						//Y coordinate value
	float              z;						//Z coordinate value
}Sn3dPointData, *LPSn3dPointData;

typedef struct
{
	int               vertex_count;			//The number of vertex .
	LPSn3dPointData   vertex_data;			//The data of vertex.
	int               norma_count;			//The number of vertex normal.
	LPSn3dPointData   norma_data;			//The data of vertex normal.
	int               vertex_color_count;	//The number of vertex color.
	LPSn3dPointData   vertex_color_data;	//The data of vertex color.
}Sn3DPointCloud,*LPSn3dPointCloud;

typedef struct
{
	int               vertex_count;			//The number of vertex .
	LPSn3dPointData   vertex_data;			//The data of vertex.
	int               norma_count;			//The number of vertex normal.
	LPSn3dPointData   norma_data;			//The data of vertex normal.
	int               vertex_color_count;	//The number of vertex color.
	LPSn3dPointData   vertex_color_data;	//The data of vertex color.
	int				  index_count;			//The number of index
	unsigned int*	  index;				//The data of index
}Sn3DIncreasePointCloud, *LPSn3dIncreasePointCloud;

#include <QVector3D>
typedef struct {
	QVector3D position;				//Camera Position
	QVector3D center;				//Center of view 
	QVector3D up;					//Normal of up
}Sn3DCameraPosition, *LPSn3dCameraPosition;

//LSC 数据结构设计
// x y z : index of face 
typedef struct {
	int x;
	int y;
	int z;
}Sn3DFaceId, *LPSn3dFaceId;

//x, y : coordinate of texture
typedef struct {
	float x;
	float y;
}Sn3DVec2F, *LPSn3dVec2F;

typedef struct {
	int width;				//width of image
	int height;				//height of image
	int channel;			//channel of image 
	uchar* data;			//data of image
}Sn3DImage;


typedef struct {
	//点云数量和数据   
	int				meshpoint_count;	//num of point cloud
	LPSn3dPointData meshpoint;			//data of point cloud
	//法向数量和数据
	int				meshnormal_count;	//num of normal
	LPSn3dPointData	meshnormal;			//data of normal
	//点云索引
	int				meshtrifaceid_count;//num of point index
	LPSn3dFaceId	meshtrifaceid;		//data of point index
	//纹理索引
	int				meshtextureid_count;//num of textureimage
	LPSn3dFaceId	meshtextureid;		//data of texture image
	//纹理UV坐标
	int				textureUV_count;	//num of uv
	LPSn3dVec2F		textureUV;			//data of uv
	//纹理图
	Sn3DImage		image;				//texture image
}Sn3DMeshData, *LPSn3dMeshData;

//typedef struct {
//	int				marker_count;
//	LPSn3dPointData	coord;          //坐标
//	LPSn3dPointData	normal;         //法向
//	double*			diameter; //直径
//	int*            codeId;     //编码点ID(<=0即为普通的标志点)
//}Sn3DMarkerData, *LPSn3DMakerData;


//单个标志点信息 single marker
struct MarkerInfo
{
	Sn3dPointData	coord;          //坐标  data of marker
	Sn3dPointData	normal;         //法向	data of noraml
	double			diameter;		//直径	diameter
	int             codeId;			//编码点ID(<=0即为普通的标志点)  marker id (<=0 means common marker)
};

typedef struct {
	int			marker_count;		//marker count
	MarkerInfo* markerList;			//marker list
}Sn3DMarkerData, *LPSn3DMakerData;


//typedef struct _tagSn3DVariant
//{
//	int type;
//	int nLengeth;
//
//	union 
//	{
//		int _iValue;
//		double _dValue;
//		float _fValue;
//		char _cValue;
//		ushort _usValue;
//		char* _sValue;
//	}Sn3dValue;
//
//}Sn3dVariant;

enum DeviceEvent {
	DE_NULL = -1,				//NULL
	DE_DOUBLECLICK = 0,			//Double Click
	DE_CLICK = 1,				//Click
	DE_PLUS = 2,				//Plus button click
	DE_SUB = 3,					//Sub button click
	DE_LONGPRESS_PLUS = 4,		//Plus button long press
	DE_LONGPRESS_SUB = 5,		//Sub button long press
	DE_LONGPRESS = 8,			// Ok button long press
};

struct Async {};

#endif //_SN3DPUBLIC_H_

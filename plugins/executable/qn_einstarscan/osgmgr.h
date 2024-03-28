#ifndef OSGMGR_H
#define OSGMGR_H
#include <osg/Group>
#include <renderkit_singleton.h>
#include "canvas3d_renderinfo.h"

/*!
@addtogroup Demo
@details \ref OsgMgr 提供了简单的对shader以及group的管理，切换shader等操作
@{
*/

/*!
@brief 对网格/点云的group的管理（设置、切换shader等）
*/
class OsgMgr
{
public:
    enum ModelType {
        Point,
        Mesh
    };
    enum ColorFlag {
        SingleColor = 0x0,
        VertexColor = 0x1,
        TextureColor = 0x2,
        DoubleTextureColor = 0x4
    };

    enum ShaderType {
        Display,
        Edit,
        SphereEdit,
        Transparent,
        CutPlane
    };
    struct OsgModelInfo : public osg::Referenced {
        ModelType m_modelType;
        int m_colorFlag;
    };

    struct OsgGroupInfo : public osg::Referenced {
        ModelType   m_groupType;
        ColorFlag   m_colorType;
        ShaderType  m_shaderType;
    };

    static void init(Sn3D::RenderKit::CanvasBase* canvas) { m_canvas = canvas; }

    static void setGroupShaderType(osg::Group* group, ShaderType shaderType);

    static osg::Operation* setGroupShaderTypeOperation(osg::Group* group, ShaderType shaderType);

    // 获取对应的group，如果没有创建，则创建group，并设置相应的shader，且添加到场景中
    static osg::Group* getOrCreateAppropriateGroup(ModelType modelType, ColorFlag colorFlag);
    static osg::Group* getOrCreateMeshSingleColorGroup();
    static osg::Group* getOrCreateMeshVertexColorGroup();
    static osg::Group* getOrCreateMeshTextureColorGroup();
    static osg::Group* getOrCreateMeshDoubleTextureColorGroup();
    static osg::Group* getOrCreateRttCameraMeshDoubleTextureGroup();

    static osg::Group* getOrCreatePointSingleColorGroup();
    static osg::Group* getOrCreatePointVertexColorGroup();

    // 只创建对应的group,通过operation设置相应的shader
    static osg::Group* createAppropriateGroup(ModelType modelType, ColorFlag colorFlag);
    static osg::Group* createMeshSingleColorGroup();
    static osg::Group* createMeshVertexColorGroup();
    static osg::Group* createMeshTextureColorGroup();
    static osg::Group* createMeshDoubleTextureColorGroup();
    static osg::Group* createRttCameraMeshDoubleTextureGroup();

    static osg::Group* createPointSingleColorGroup();
    static osg::Group* createPointVertexColorGroup();

    //TODO, other groups with shader what we need

    static osg::Vec3Array* getOrCreateGlobalColorMap();
private:

    static osg::ref_ptr<osg::Group> m_meshSingleColorGroup;
    static osg::ref_ptr<osg::Group> m_meshVertexColorGroup;
    static osg::ref_ptr<osg::Group> m_meshTextureColorGroup;
    static osg::ref_ptr<osg::Group> m_meshDoubleTextureColorGroup;
    static osg::ref_ptr<osg::Group> m_meshDoubleTextureColorGroupToRttCamera;

    static osg::ref_ptr<osg::Group> m_pointSingleColorGroup;
    static osg::ref_ptr<osg::Group> m_pointVertexColorGroup;

    static osg::ref_ptr<osg::Vec3Array> m_colorMap;

    static osg::ref_ptr<Sn3D::RenderKit::CanvasBase> m_canvas;
};

/*! @} */

#endif
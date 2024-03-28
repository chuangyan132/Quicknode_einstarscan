#include "osgmgr.h"
#include <canvas3d_shadermanager.h>

USING_RENDERKIT_NAMESPACE

osg::ref_ptr<osg::Group> OsgMgr::m_meshSingleColorGroup;
osg::ref_ptr<osg::Group> OsgMgr::m_meshVertexColorGroup;
osg::ref_ptr<osg::Group> OsgMgr::m_meshTextureColorGroup;
osg::ref_ptr<osg::Group> OsgMgr::m_meshDoubleTextureColorGroup;
osg::ref_ptr<osg::Group> OsgMgr::m_meshDoubleTextureColorGroupToRttCamera;
osg::ref_ptr<osg::Group> OsgMgr::m_pointSingleColorGroup;
osg::ref_ptr<osg::Group> OsgMgr::m_pointVertexColorGroup;
osg::ref_ptr<osg::Vec3Array> OsgMgr::m_colorMap;
osg::ref_ptr<Sn3D::RenderKit::CanvasBase> OsgMgr::m_canvas;

void OsgMgr::setGroupShaderType(osg::Group* group, ShaderType shaderType)
{
    OsgGroupInfo* groupInfo = dynamic_cast<OsgGroupInfo*>(group->getUserData());
    if (groupInfo->m_shaderType == shaderType) {
        return;
    }

    auto op = setGroupShaderTypeOperation(group, shaderType);
    if (nullptr != op) {
        m_canvas->addOperation(op);
        groupInfo->m_shaderType = shaderType;
    }
}

osg::Operation* OsgMgr::setGroupShaderTypeOperation(osg::Group* group, ShaderType shaderType)
{
    OsgGroupInfo* groupInfo = dynamic_cast<OsgGroupInfo*>(group->getUserData());
    if (groupInfo->m_shaderType == shaderType) {
        return nullptr;
    }

    switch (groupInfo->m_groupType)
    {
    case Mesh: {
        switch (groupInfo->m_colorType)
        {
        case SingleColor: {
            switch (shaderType)
            {
            case Display: return RenderKitSingleton::instance()->setGroupShader(group, g_meshProgramStr);
            case Edit: return RenderKitSingleton::instance()->setGroupShader(group, g_editMeshProgramStr);
            case SphereEdit: return RenderKitSingleton::instance()->setGroupShader(group, g_sphereEditMeshProgramStr);
            case Transparent: return RenderKitSingleton::instance()->setGroupShader(group, g_transMeshProgramStr);
            case CutPlane: return RenderKitSingleton::instance()->setGroupShader(group, g_cutMeshProgramStr);
            default: return nullptr;
            }
        }break;
        case VertexColor: {
            switch (shaderType)
            {
            case Display: return RenderKitSingleton::instance()->setGroupShader(group, g_meshColorProgramStr);
            case Edit: return RenderKitSingleton::instance()->setGroupShader(group, g_editMeshColorProgramStr);
            case SphereEdit: return RenderKitSingleton::instance()->setGroupShader(group, g_sphereEditMeshColorProgramStr);
            case Transparent: return RenderKitSingleton::instance()->setGroupShader(group, g_transMeshColorProgramStr);
            case CutPlane: return RenderKitSingleton::instance()->setGroupShader(group, g_cutMeshColorProgramStr);
            default: return nullptr;
            }
        }break;
        case TextureColor: {
            switch (shaderType)
            {
            case Display: return RenderKitSingleton::instance()->setGroupShader(group, g_meshTextureProgramStr);
            case Edit: return RenderKitSingleton::instance()->setGroupShader(group, g_editMeshTextureProgramStr);
            case SphereEdit: return RenderKitSingleton::instance()->setGroupShader(group, g_sphereEditMeshTextureProgramStr);
            case Transparent: return RenderKitSingleton::instance()->setGroupShader(group, g_transMeshTextureProgramStr);
            case CutPlane: return RenderKitSingleton::instance()->setGroupShader(group, g_cutMeshTextureProgramStr);
            default: return nullptr;
            }
        }break;
        case DoubleTextureColor: {
            switch (shaderType)
            {
            // 双纹理，用于贴图软件，暂时只考虑display和edit，都使用相同的shader, fangmingcai 2023.04.06
            case Display: return RenderKitSingleton::instance()->setGroupShader(group, g_editMeshDoubleTextureProgramStr);
            case Edit: return RenderKitSingleton::instance()->setGroupShader(group, g_editMeshDoubleTextureProgramStr);
            case SphereEdit: return RenderKitSingleton::instance()->setGroupShader(group, g_sphereEditMeshTextureProgramStr);
            case Transparent: return RenderKitSingleton::instance()->setGroupShader(group, g_transMeshTextureProgramStr);
            case CutPlane: return RenderKitSingleton::instance()->setGroupShader(group, g_cutMeshTextureProgramStr);
            default: return nullptr;
            }
        }break;
        default: return nullptr;
        }
    }break;
    case Point: {
        switch (groupInfo->m_colorType)
        {
        case SingleColor: {
            switch (shaderType)
            {
            case Display: return RenderKitSingleton::instance()->setGroupShader(group, g_pointProgramStr);
            case Edit: return RenderKitSingleton::instance()->setGroupShader(group, g_editPointProgramStr);
            case CutPlane: return RenderKitSingleton::instance()->setGroupShader(group, g_cutPointProgramStr);
            default: return nullptr;
            }
        }break;
        case VertexColor: {
            switch (shaderType)
            {
            case Display: return RenderKitSingleton::instance()->setGroupShader(group, g_pointColorProgramStr);
            case Edit: return RenderKitSingleton::instance()->setGroupShader(group, g_editPointColorProgramStr);
            case CutPlane: return RenderKitSingleton::instance()->setGroupShader(group, g_cutPointColorProgramStr);
            default: return nullptr;
            }
        }break;
        default: return nullptr;
        }
    }break;
    default: return nullptr;
    }
    return nullptr;
}

osg::Group* OsgMgr::getOrCreateAppropriateGroup(ModelType modelType, ColorFlag colorFlag)
{
    osg::Group* group = nullptr;
    switch (modelType) {
    case ModelType::Mesh: {
        switch (colorFlag)
        {
        case ColorFlag::VertexColor: group = OsgMgr::getOrCreateMeshVertexColorGroup(); break;
        case ColorFlag::TextureColor: group = OsgMgr::getOrCreateMeshTextureColorGroup(); break;
        case ColorFlag::SingleColor: group = OsgMgr::getOrCreateMeshSingleColorGroup(); break;
        default:
            break;
        }
        break;
    };
    case ModelType::Point: {
        switch (colorFlag)
        {
        case ColorFlag::VertexColor: group = OsgMgr::getOrCreatePointVertexColorGroup(); break;
        case ColorFlag::SingleColor: group = OsgMgr::getOrCreatePointSingleColorGroup(); break;
        default:
            break;
        }
        break;
    };
    default: {
        break;
    }
    }
    return group;
}

osg::Group* OsgMgr::getOrCreateMeshSingleColorGroup()
{
    if (!m_meshSingleColorGroup.valid()) {
        m_meshSingleColorGroup = createMeshSingleColorGroup();
        m_canvas->addOperation(RenderKitSingleton::instance()->addGroup(m_meshSingleColorGroup.get()));
    }
    return m_meshSingleColorGroup.get();
}

osg::Group* OsgMgr::getOrCreateMeshVertexColorGroup()
{
    if (!m_meshVertexColorGroup.valid()) {
        m_meshVertexColorGroup = createMeshVertexColorGroup();
        m_canvas->addOperation(RenderKitSingleton::instance()->addGroup(m_meshVertexColorGroup.get()));
    }
    return m_meshVertexColorGroup.get();
}

osg::Group* OsgMgr::getOrCreateMeshTextureColorGroup()
{
    if (!m_meshTextureColorGroup.valid()) {
        m_meshTextureColorGroup = createMeshTextureColorGroup();
        m_canvas->addOperation(RenderKitSingleton::instance()->addGroup(m_meshTextureColorGroup.get()));
    }
    return m_meshTextureColorGroup.get();
}

osg::Group* OsgMgr::getOrCreateMeshDoubleTextureColorGroup()
{
    if (!m_meshDoubleTextureColorGroup.valid()) {
        m_meshDoubleTextureColorGroup = createMeshDoubleTextureColorGroup();
        m_canvas->addOperation(RenderKitSingleton::instance()->addGroup(m_meshDoubleTextureColorGroup.get()));
    }
    return m_meshDoubleTextureColorGroup.get();
}

osg::Group* OsgMgr::getOrCreateRttCameraMeshDoubleTextureGroup()
{
    if (!m_meshDoubleTextureColorGroupToRttCamera.valid()) {
        m_meshDoubleTextureColorGroupToRttCamera = createRttCameraMeshDoubleTextureGroup();
        //m_canvas->addOperation(RenderKitSingleton::instance()->addGroup(m_meshDoubleTextureColorGroupToRttCamera.get()));
    }
    return m_meshDoubleTextureColorGroupToRttCamera.get();
}

osg::Group* OsgMgr::getOrCreatePointSingleColorGroup()
{
    if (!m_pointSingleColorGroup.valid()) {
        m_pointSingleColorGroup = createPointSingleColorGroup();
        m_canvas->addOperation(RenderKitSingleton::instance()->addGroup(m_pointSingleColorGroup.get()));
    }
    return m_pointSingleColorGroup.get();
}

osg::Group* OsgMgr::getOrCreatePointVertexColorGroup()
{
    if (!m_pointVertexColorGroup.valid()) {
        m_pointVertexColorGroup = createPointVertexColorGroup();
        m_canvas->addOperation(RenderKitSingleton::instance()->addGroup(m_pointVertexColorGroup.get()));
    }
    return m_pointVertexColorGroup.get();
}

osg::Group* OsgMgr::createAppropriateGroup(ModelType modelType, ColorFlag colorFlag)
{
    osg::Group* group = nullptr;
    switch (modelType)
    {
    case ModelType::Mesh: {
        switch (colorFlag)
        {
        case ColorFlag::VertexColor: group = OsgMgr::createMeshVertexColorGroup(); break;
        case ColorFlag::TextureColor: group = OsgMgr::createMeshTextureColorGroup(); break;
        case ColorFlag::SingleColor: group = OsgMgr::createMeshSingleColorGroup(); break;
        default:
            break;
        }
        break;
    };
    case ModelType::Point: {
        switch (colorFlag)
        {
        case ColorFlag::VertexColor: group = OsgMgr::createPointVertexColorGroup(); break;
        case ColorFlag::SingleColor: group = OsgMgr::createPointSingleColorGroup(); break;
        default:
            break;
        }
        break;
    };
    default: {
        break;
    }
    }
    return group;
}

osg::Group* OsgMgr::createMeshSingleColorGroup()
{
    osg::Group* group = new osg::Group;
    OsgGroupInfo* groupInfo = new OsgGroupInfo;
    groupInfo->m_groupType = ModelType::Mesh;
    groupInfo->m_colorType = ColorFlag::SingleColor;
    groupInfo->m_shaderType = ShaderType::Edit;
    group->setUserData(groupInfo);
    m_canvas->addOperation(RenderKitSingleton::instance()->setGroupShader(
        group, g_editMeshProgramStr));
    auto colorMap = OsgMgr::getOrCreateGlobalColorMap();
    m_canvas->addOperation(RenderKitSingleton::instance()->setGroupColorIndex(group, colorMap));
    return group;
}

osg::Group* OsgMgr::createMeshVertexColorGroup()
{
    osg::Group* group = new osg::Group;
    OsgGroupInfo* groupInfo = new OsgGroupInfo;
    groupInfo->m_groupType = ModelType::Mesh;
    groupInfo->m_colorType = ColorFlag::VertexColor;
    groupInfo->m_shaderType = ShaderType::Edit;
    group->setUserData(groupInfo);
    m_canvas->addOperation(RenderKitSingleton::instance()->setGroupShader(
        group, g_editMeshColorProgramStr));
    auto colorMap = OsgMgr::getOrCreateGlobalColorMap();
    m_canvas->addOperation(RenderKitSingleton::instance()->setGroupColorIndex(group, colorMap));
    return group;
}

osg::Group* OsgMgr::createMeshTextureColorGroup()
{
    osg::Group* group = new osg::Group;
    OsgGroupInfo* groupInfo = new OsgGroupInfo;
    groupInfo->m_groupType = ModelType::Mesh;
    groupInfo->m_colorType = ColorFlag::TextureColor;
    groupInfo->m_shaderType = ShaderType::Edit;
    group->setUserData(groupInfo);
    m_canvas->addOperation(RenderKitSingleton::instance()->setGroupShader(
        group, g_editMeshTextureProgramStr));
    auto colorMap = OsgMgr::getOrCreateGlobalColorMap();
    m_canvas->addOperation(RenderKitSingleton::instance()->setGroupColorIndex(group, colorMap));
    return group;
}

osg::Group* OsgMgr::createMeshDoubleTextureColorGroup()
{
    osg::Group* group = new osg::Group;
    OsgGroupInfo* groupInfo = new OsgGroupInfo;
    groupInfo->m_groupType = ModelType::Mesh;
    groupInfo->m_colorType = ColorFlag::DoubleTextureColor;
    groupInfo->m_shaderType = ShaderType::Edit;
    group->setUserData(groupInfo);
    m_canvas->addOperation(RenderKitSingleton::instance()->setGroupShader(
        group, g_editMeshDoubleTextureProgramStr));
    auto colorMap = OsgMgr::getOrCreateGlobalColorMap();
    m_canvas->addOperation(RenderKitSingleton::instance()->setGroupColorIndex(group, colorMap));
    return group;
}

osg::Group* OsgMgr::createRttCameraMeshDoubleTextureGroup()
{
    osg::Group* group = new osg::Group;
    OsgGroupInfo* groupInfo = new OsgGroupInfo;
    groupInfo->m_groupType = ModelType::Mesh;
    groupInfo->m_colorType = ColorFlag::DoubleTextureColor;
    groupInfo->m_shaderType = ShaderType::Edit;
    group->setUserData(groupInfo);
    m_canvas->addOperation(RenderKitSingleton::instance()->setGroupShader(
        group, g_editMeshDoubleTextureRedBlueProgramStr));
    auto colorMap = OsgMgr::getOrCreateGlobalColorMap();
    m_canvas->addOperation(RenderKitSingleton::instance()->setGroupColorIndex(group, colorMap));
    return group;
}

osg::Group* OsgMgr::createPointSingleColorGroup()
{
    osg::Group* group = new osg::Group;
    OsgGroupInfo* groupInfo = new OsgGroupInfo;
    groupInfo->m_groupType = ModelType::Point;
    groupInfo->m_colorType = ColorFlag::SingleColor;
    groupInfo->m_shaderType = ShaderType::Display;
    group->setUserData(groupInfo);
    m_canvas->addOperation(RenderKitSingleton::instance()->setGroupShader(
        group, g_pointProgramStr));
    auto colorMap = OsgMgr::getOrCreateGlobalColorMap();
    m_canvas->addOperation(RenderKitSingleton::instance()->setGroupColorIndex(group, colorMap));
    return group;
}

osg::Group* OsgMgr::createPointVertexColorGroup()
{
    osg::Group* group = new osg::Group;
    OsgGroupInfo* groupInfo = new OsgGroupInfo;
    groupInfo->m_groupType = ModelType::Point;
    groupInfo->m_colorType = ColorFlag::VertexColor;
    groupInfo->m_shaderType = ShaderType::Display;
    group->setUserData(groupInfo);
    m_canvas->addOperation(RenderKitSingleton::instance()->setGroupShader(
        group, g_pointColorProgramStr));
    auto colorMap = OsgMgr::getOrCreateGlobalColorMap();
    m_canvas->addOperation(RenderKitSingleton::instance()->setGroupColorIndex(group, colorMap));
    return group;
}

osg::Vec3Array* OsgMgr::getOrCreateGlobalColorMap()
{
    if (!m_colorMap.valid()) {
        m_colorMap = new osg::Vec3Array;
        m_colorMap->push_back(osg::Vec3(0.05f, 0.695f, 1.0f));
        m_colorMap->push_back(osg::Vec3(1.0f, 0.05f, 0.695f));
        m_colorMap->push_back(osg::Vec3(0.695f, 1.0f, 0.05f));
        m_colorMap->push_back(osg::Vec3(1.0f, 0.0f, 0.0f));
        m_colorMap->push_back(osg::Vec3(0.0f, 1.0f, 0.0f));
        m_colorMap->push_back(osg::Vec3(0.0f, 0.0f, 1.0f));
    }
    return m_colorMap.get();
}
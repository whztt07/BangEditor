#include "BangEditor/EditorTextureFactory.h"

#include "Bang/Assets.h"
#include "Bang/Assets.tcc"
#include "Bang/Behaviour.h"
#include "Bang/Component.h"
#include "Bang/Extensions.h"
#include "Bang/GL.h"
#include "Bang/Map.tcc"
#include "Bang/Material.h"
#include "Bang/Mesh.h"
#include "Bang/Model.h"
#include "Bang/Serializable.h"
#include "Bang/Texture2D.h"
#include "Bang/TextureFactory.h"
#include "BangEditor/EditorPaths.h"
#include "BangEditor/MaterialPreviewFactory.h"
#include "BangEditor/MeshPreviewFactory.h"
#include "BangEditor/ModelPreviewFactory.h"

namespace Bang
{
class Behaviour;
}

using namespace Bang;
using namespace BangEditor;

Texture2D *EditorTextureFactory::GetIconForPath(const Path &path)
{
    if (!path.IsDir())
    {
        EditorTextureFactory *etf = EditorTextureFactory::GetInstance();

        AH<Texture2D> icon;
        if (path.HasExtension(Extensions::GetImageExtensions()))
        {
            if (etf->m_iconCacheMap.ContainsKey(path))
            {
                return etf->m_iconCacheMap.Get(path).Get();
            }
            else
            {
                icon = Assets::Load<Texture2D>(path);
            }
        }
        else if (path.HasExtension(Extensions::GetModelExtensions()))
        {
            AH<Model> model = Assets::Load<Model>(path);
            icon = ModelPreviewFactory::GetPreviewTextureFor(model.Get());
        }
        else if (path.HasExtension(Extensions::GetMeshExtension()))
        {
            AH<Model> model = Assets::Load<Model>(path.GetDirectory());
            if (model)
            {
                AH<Mesh> mesh = Assets::Load<Mesh>(path);
                icon = MeshPreviewFactory::GetPreviewTextureFor(mesh.Get());
            }
        }
        else if (path.HasExtension(Extensions::GetMaterialExtension()))
        {
            AH<Material> material = Assets::Load<Material>(path);
            icon = MaterialPreviewFactory::GetPreviewTextureFor(material.Get());
        }
        else
        {
            icon.Set(
                EditorTextureFactory::GetIconForExtension(path.GetExtension()));
        }

        if (icon)
        {
            etf->m_iconCacheMap.Add(path, icon);
        }

        return icon.Get();
    }
    return EditorTextureFactory::GetFolderIcon();
}

bool EditorTextureFactory::IsIconAnImage(const Path &path)
{
    return !path.HasExtension(Extensions::GetModelExtensions()) &&
           !path.HasExtension(Extensions::GetMaterialExtension());
}

Texture2D *EditorTextureFactory::GetAddIcon()
{
    return EditorTextureFactory::GetTexture2D("Add.png");
}

Texture2D *EditorTextureFactory::GetLessIcon()
{
    return EditorTextureFactory::GetTexture2D("Less.png");
}

Texture2D *EditorTextureFactory::GetStarsIcon()
{
    return EditorTextureFactory::GetTexture2D("Stars.png");
}

Texture2D *EditorTextureFactory::GetWorldIcon()
{
    return EditorTextureFactory::GetTexture2D("World.png");
}

Texture2D *EditorTextureFactory::GetCameraIcon()
{
    return EditorTextureFactory::GetTexture2D("Camera.png");
}

Texture2D *EditorTextureFactory::GetRotateIcon()
{
    return EditorTextureFactory::GetTexture2D("Rotate.png");
}

Texture2D *EditorTextureFactory::GetGreenCubeIcon()
{
    return EditorTextureFactory::GetTexture2D("GreenCube.png");
}

Texture2D *EditorTextureFactory::GetAnimatorSMIcon()
{
    return EditorTextureFactory::GetTexture2D("AnimatorSMIcon.png");
}

Texture2D *EditorTextureFactory::GetAnimatorLayerMaskIcon()
{
    return EditorTextureFactory::GetTexture2D("AnimatorLayerMaskIcon.png");
}

Texture2D *EditorTextureFactory::GetGreenSphereIcon()
{
    return EditorTextureFactory::GetTexture2D("GreenSphere.png");
}

Texture2D *EditorTextureFactory::GetGreenCapsuleIcon()
{
    return EditorTextureFactory::GetTexture2D("GreenCapsule.png");
}

Texture2D *EditorTextureFactory::GetCrashDummyIcon()
{
    return EditorTextureFactory::GetTexture2D("CrashDummy.png");
}

Texture2D *EditorTextureFactory::GetPlayWithCircleIcon()
{
    return EditorTextureFactory::GetTexture2D("PlayWithCircle.png");
}

Texture2D *EditorTextureFactory::GetRightArrowAndBarIcon()
{
    return EditorTextureFactory::GetTexture2D("RightArrowAndBar.png");
}

Texture2D *EditorTextureFactory::GetDoubleBarIcon()
{
    return EditorTextureFactory::GetTexture2D("DoubleBar.png");
}

Texture2D *EditorTextureFactory::GetBackArrowIcon()
{
    return EditorTextureFactory::GetTexture2D("BackArrow.png");
}

Texture2D *EditorTextureFactory::GetLensIcon()
{
    return EditorTextureFactory::GetTexture2D("Lens.png");
}

Texture2D *EditorTextureFactory::GetLensLittleIcon()
{
    return EditorTextureFactory::GetTexture2D("LensLittle.png");
}

Texture2D *EditorTextureFactory::GetSquareIcon()
{
    return EditorTextureFactory::GetTexture2D("Square.png");
}

Texture2D *EditorTextureFactory::GetAnchorIcon()
{
    return EditorTextureFactory::GetTexture2D("Anchor.png");
}

Texture2D *EditorTextureFactory::GetCubeMapIcon()
{
    return EditorTextureFactory::GetTexture2D("CubeMap.png");
}

Texture2D *EditorTextureFactory::GetGrid2x2()
{
    Texture2D *tex = EditorTextureFactory::GetTexture2D("Grid2x2.png");
    tex->SetWrapMode(GL::WrapMode::REPEAT);
    return tex;
}

Texture2D *EditorTextureFactory::GetWhiteSphereIcon()
{
    return EditorTextureFactory::GetTexture2D("WhiteSphere.png");
}

Texture2D *EditorTextureFactory::GetFolderIcon()
{
    return EditorTextureFactory::GetTexture2D("Folder.png");
}

Texture2D *EditorTextureFactory::GetLetterIcon()
{
    return EditorTextureFactory::GetTexture2D("Letter.png");
}

Texture2D *EditorTextureFactory::GetImageIcon()
{
    return EditorTextureFactory::GetTexture2D("Image.png");
}

Texture2D *EditorTextureFactory::GetSplashIcon()
{
    return EditorTextureFactory::GetTexture2D("Splash.png");
}

Texture2D *EditorTextureFactory::GetPillIcon()
{
    return EditorTextureFactory::GetTexture2D("Pill.png");
}

Texture2D *EditorTextureFactory::GetCubeIcon()
{
    return EditorTextureFactory::GetTexture2D("Cube.png");
}

Texture2D *EditorTextureFactory::GetBracketsIcon()
{
    return EditorTextureFactory::GetTexture2D("Brackets.png");
}

Texture2D *EditorTextureFactory::GetPhysicsBallIcon()
{
    return EditorTextureFactory::GetTexture2D("PhysicsBall.png");
}

Texture2D *EditorTextureFactory::GetFileIcon()
{
    return EditorTextureFactory::GetTexture2D("File.png");
}

Texture2D *EditorTextureFactory::GetSceneIcon()
{
    return EditorTextureFactory::GetTexture2D("Scene.png");
}

Texture2D *EditorTextureFactory::GetAxesIcon()
{
    return EditorTextureFactory::GetTexture2D("Axes.png");
}

Texture2D *EditorTextureFactory::GetColoredAxesIcon()
{
    return EditorTextureFactory::GetTexture2D("ColoredAxes.png");
}

Texture2D *EditorTextureFactory::GetHairCrossIcon()
{
    return EditorTextureFactory::GetTexture2D("HairCross.png");
}

Texture2D *EditorTextureFactory::GetCircleIcon()
{
    return EditorTextureFactory::GetTexture2D("Circle.png");
}

Texture2D *EditorTextureFactory::GetCircleHardIcon()
{
    return EditorTextureFactory::GetTexture2D("CircleHard.png");
}

Texture2D *EditorTextureFactory::GetEyeIcon()
{
    return EditorTextureFactory::GetTexture2D("Eye.png");
}

Texture2D *EditorTextureFactory::GetVertexShaderIcon()
{
    return EditorTextureFactory::GetTexture2D("VertexShaderIcon.png");
}

Texture2D *EditorTextureFactory::GetGeometryShaderIcon()
{
    return EditorTextureFactory::GetTexture2D("GeometryShaderIcon.png");
}

Texture2D *EditorTextureFactory::GetFragmentShaderIcon()
{
    return EditorTextureFactory::GetTexture2D("FragmentShaderIcon.png");
}

Texture2D *EditorTextureFactory::GetUnifiedShaderIcon()
{
    return EditorTextureFactory::GetTexture2D("UnifiedShaderIcon.png");
}

Texture2D *EditorTextureFactory::GetShaderProgramIcon()
{
    return EditorTextureFactory::GetTexture2D("ShaderProgramIcon.png");
}

Texture2D *EditorTextureFactory::GetAnchoredRectIcon()
{
    return EditorTextureFactory::GetTexture2D("AnchoredRect.png");
}

Texture2D *EditorTextureFactory::GetComponentIcon(const String &componentName)
{
    if (componentName == "Transform")
    {
        return EditorTextureFactory::GetColoredAxesIcon();
    }
    else if (componentName == "Behaviour" ||
             componentName == "BehaviourContainer")
    {
        return EditorTextureFactory::GetBracketsIcon();
    }
    else if (componentName == "RectTransform")
    {
        return EditorTextureFactory::GetAnchoredRectIcon();
    }
    else if (componentName == "BoxCollider")
    {
        return EditorTextureFactory::GetGreenCubeIcon();
    }
    else if (componentName == "SphereCollider")
    {
        return EditorTextureFactory::GetGreenSphereIcon();
    }
    else if (componentName == "CapsuleCollider")
    {
        return EditorTextureFactory::GetGreenCapsuleIcon();
    }
    else if (componentName == "RigidBody")
    {
        return EditorTextureFactory::GetCrashDummyIcon();
    }
    else if (componentName == "PointLight")
    {
        return TextureFactory::GetLightBulbIcon();
    }
    else if (componentName == "DirectionalLight")
    {
        Texture2D *tex = TextureFactory::GetSunIcon();
        return tex;
    }
    else if (componentName == "Behaviour")
    {
        return EditorTextureFactory::GetBracketsIcon();
    }
    else if (componentName == "DecalRenderer")
    {
        return EditorTextureFactory::GetSplashIcon();
    }
    else if (componentName == "UIImageRenderer" ||
             componentName.BeginsWith("PostProcessEffect"))
    {
        return EditorTextureFactory::GetImageIcon();
    }
    else if (componentName == "MeshRenderer")
    {
        return EditorTextureFactory::GetCubeIcon();
    }
    else if (componentName == "Animator")
    {
        return EditorTextureFactory::GetPlayWithCircleIcon();
    }
    else if (componentName == "AudioSource" || componentName == "AudioListener")
    {
        return TextureFactory::GetAudioIcon();
    }
    else if (componentName == "ParticleSystem" || componentName == "Rope")
    {
        return EditorTextureFactory::GetStarsIcon();
    }
    else if (componentName == "Camera")
    {
        return EditorTextureFactory::GetCameraIcon();
    }

    return EditorTextureFactory::GetCubeIcon();
}

Color EditorTextureFactory::GetComponentIconTint(const Component *component)
{
    String className = component->GetClassName();
    if (DCAST<const Behaviour *>(component))
    {
        className = "Behaviour";
    }
    return GetComponentIconTint(className);
}

Color EditorTextureFactory::GetComponentIconTint(const String &componentName)
{
    if (componentName == "MeshRenderer")
    {
        return Color::White().WithValue(0.2f);
    }
    else if (componentName == "RectTransform")
    {
        return Color::White().WithValue(0.2f);
    }
    else if (componentName == "SkinnedMeshRenderer")
    {
        return Color::White().WithValue(0.2f);
    }
    else if (componentName == "DecalRenderer")
    {
        return Color::White().WithValue(0.2f);
    }
    else if (componentName == "UIImageRenderer" ||
             componentName.BeginsWith("PostProcessEffect"))
    {
        return Color::White().WithValue(0.2f);
    }
    else if (componentName == "DirectionalLight")
    {
        return Color::White().WithValue(0.2f);
    }
    else if (componentName == "PointLight")
    {
        return Color::White().WithValue(0.2f);
    }
    else if (componentName == "AudioSource")
    {
        return Color::White().WithValue(0.2f);
    }
    else if (componentName == "AudioListener")
    {
        return Color::White().WithValue(0.2f);
    }
    else if (componentName == "BehaviourContainer")
    {
        return Color::White().WithValue(0.2f);
    }
    else if (componentName == "Behaviour" ||
             componentName == "BehaviourContainer")
    {
        return Color::White().WithValue(0.2f);
    }
    else if (componentName == "ParticleSystem")
    {
        return Color::White().WithValue(0.2f);
    }
    else if (componentName == "MeshCollider")
    {
        return Color::Green().WithValue(0.8f);
    }
    else if (componentName == "Camera")
    {
        return Color::White().WithValue(0.2f);
    }
    return Color::White();
}

Texture2D *EditorTextureFactory::GetIconForExtension(const String &ext)
{
    if (Extensions::Equals(ext, Extensions::GetFontExtensions()))
    {
        return EditorTextureFactory::GetLetterIcon();
    }
    else if (Extensions::Equals(ext, Extensions::GetAudioClipExtensions()))
    {
        return TextureFactory::GetAudioIcon();
    }
    else if (Extensions::Equals(ext, Extensions::GetVertexShaderExtensions()))
    {
        return EditorTextureFactory::GetVertexShaderIcon();
    }
    else if (Extensions::Equals(ext, Extensions::GetGeometryShaderExtensions()))
    {
        return EditorTextureFactory::GetGeometryShaderIcon();
    }
    else if (Extensions::Equals(ext, Extensions::GetFragmentShaderExtensions()))
    {
        return EditorTextureFactory::GetFragmentShaderIcon();
    }
    else if (Extensions::Equals(ext, Extensions::GetUnifiedShaderExtension()))
    {
        return EditorTextureFactory::GetUnifiedShaderIcon();
    }
    else if (Extensions::Equals(ext, Extensions::GetShaderProgramExtension()))
    {
        return EditorTextureFactory::GetShaderProgramIcon();
    }
    else if (Extensions::Equals(ext, Extensions::GetSceneExtension()))
    {
        return EditorTextureFactory::GetSceneIcon();
    }
    else if (Extensions::Equals(ext, Extensions::GetPrefabExtension()))
    {
        return EditorTextureFactory::GetPillIcon();
    }
    else if (Extensions::Equals(ext, Extensions::GetModelExtensions()))
    {
        return EditorTextureFactory::GetCubeIcon();
    }
    else if (Extensions::Equals(ext,
                                Extensions::GetAnimatorLayerMaskExtension()))
    {
        return EditorTextureFactory::GetAnimatorLayerMaskIcon();
    }
    else if (Extensions::Equals(ext, Extensions::GetPhysicsMaterialExtension()))
    {
        return EditorTextureFactory::GetPhysicsBallIcon();
    }
    else if (Extensions::Equals(ext, Extensions::GetBehaviourExtensions()))
    {
        return EditorTextureFactory::GetBracketsIcon();
    }
    else if (Extensions::Equals(ext, Extensions::GetTextureCubeMapExtension()))
    {
        return EditorTextureFactory::GetCubeMapIcon();
    }
    else if (Extensions::Equals(ext,
                                Extensions::GetAnimatorStateMachineExtension()))
    {
        return EditorTextureFactory::GetAnimatorSMIcon();
    }
    else if (Extensions::Equals(ext, Extensions::GetAnimationExtension()))
    {
        return EditorTextureFactory::GetPlayWithCircleIcon();
    }
    else if (Extensions::Equals(ext, Extensions::GetMeshExtension()))
    {
        return EditorTextureFactory::GetCubeIcon();
    }

    return EditorTextureFactory::GetFileIcon();
}

Color EditorTextureFactory::GetPathIconTint(const Path &path)
{
    return EditorTextureFactory::GetExtensionIconTint(path.GetExtension());
}

Color EditorTextureFactory::GetExtensionIconTint(const String &extension)
{
    if (Extensions::Equals(extension, Extensions::GetSceneExtension()))
    {
        return Color::Black();
    }
    else if (Extensions::Equals(extension,
                                Extensions::GetBehaviourExtensions()))
    {
        return Color::Black();
    }
    return Color::White();
}

Texture2D *EditorTextureFactory::GetComponentIcon(const Component *component)
{
    String className = component->GetClassName();
    if (DCAST<const Behaviour *>(component))
    {
        className = "Behaviour";
    }
    return GetComponentIcon(className);
}

EditorTextureFactory *EditorTextureFactory::GetInstance()
{
    return SCAST<EditorTextureFactory *>(TextureFactory::GetInstance());
}

Texture2D *EditorTextureFactory::GetTexture2D(const String &filename)
{
    Texture2D *tex = TextureFactory::GetTexture2D(
        filename, EditorPaths::GetEditorAssetsDir().Append("Textures"));
    return tex;
}

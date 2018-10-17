#ifndef UITEXTURECUBEMAPPREVIEWER_H
#define UITEXTURECUBEMAPPREVIEWER_H

#include <vector>

#include "Bang/Array.tcc"
#include "Bang/Bang.h"
#include "Bang/BangDefines.h"
#include "Bang/EventEmitter.h"
#include "Bang/EventListener.tcc"
#include "Bang/GameObject.h"
#include "Bang/IEventsValueChanged.h"
#include "Bang/ResourceHandle.h"
#include "Bang/String.h"
#include "Bang/Texture2D.h"
#include "Bang/UIFocusable.h"
#include "BangEditor/BangEditor.h"
#include "BangEditor/ResourcePreviewFactory.h"

namespace Bang
{
class IEventsValueChanged;
class Material;
class TextureCubeMap;
class UIFocusable;
class UIAspectRatioFitter;
class UIImageRenderer;
}

using namespace Bang;
namespace BangEditor
{
class UITextureCubeMapPreviewer : public GameObject,
                                  public EventEmitter<IEventsValueChanged>
{
    GAMEOBJECT_EDITOR(UITextureCubeMapPreviewer);

public:
    UITextureCubeMapPreviewer();
    virtual ~UITextureCubeMapPreviewer() override;

    // GameObject
    void Update() override;

    void InvalidateRenderer();
    void SetTextureCubeMap(TextureCubeMap *texCubeMap);
    TextureCubeMap *GetTextureCubeMap() const;

private:
    UIFocusable *p_focusable = nullptr;
    RH<TextureCubeMap> p_textureCubeMap;
    Vector2 m_rotationRads = Vector2::Zero;
    RH<Material> p_previewImageRendererMaterial;
    UIImageRenderer *p_previewImageRenderer = nullptr;
    UIAspectRatioFitter *p_previewImgAspectRatioFitter = nullptr;
};
}

#endif  // UITEXTURECUBEMAPPREVIEWER_H

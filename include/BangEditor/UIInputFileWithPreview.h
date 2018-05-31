#ifndef UIINPUTFILEWITHPREVIEW_H
#define UIINPUTFILEWITHPREVIEW_H

#include "Bang/Bang.h"
#include "Bang/Texture2D.h"
#include "Bang/IFocusListener.h"
#include "Bang/ResourceHandle.h"

#include "BangEditor/BangEditor.h"
#include "BangEditor/UIInputFile.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class Texture2D;
FORWARD class UIImageRenderer;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class UIInputFileWithPreview : public UIInputFile,
                               public IFocusListener
{
public:
	UIInputFileWithPreview();
	virtual ~UIInputFileWithPreview();

    void SetPath(const Path &path) override;
    virtual RH<Texture2D> GetPreviewTextureFromPath(const Path &path) = 0;

private:
    UIImageRenderer *p_previewImg = nullptr;
    UIImageRenderer *p_bigPreviewImg = nullptr;

    // IFocusable
    virtual void OnMouseEnter(IFocusable *focusable) override;
    virtual void OnMouseExit(IFocusable *focusable)  override;
};

NAMESPACE_BANG_EDITOR_END

#endif // UIINPUTFILEWITHPREVIEW_H

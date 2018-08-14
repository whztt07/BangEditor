#ifndef CIWREFLECTIONPROBE_H
#define CIWREFLECTIONPROBE_H

#include "Bang/Bang.h"
#include "Bang/UIInputNumber.h"

#include "BangEditor/UIInputVector.h"
#include "BangEditor/ComponentInspectorWidget.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class ReflectionProbe;
FORWARD class UIComboBox;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class UIInputFile;
FORWARD class UIInputColor;
FORWARD class UITextureCubeMapPreviewer;

class CIWReflectionProbe : public ComponentInspectorWidget
{
    GAMEOBJECT_EDITOR(CIWReflectionProbe);

public:
    // InspectorWidget
    virtual void InitInnerWidgets() override;
    virtual void UpdateFromReference() override;

    void LimitValues();

private:
    UIComboBox *p_reflectionProbSizeInput = nullptr;
    UIInputVector *p_sizeInput = nullptr;
    UICheckBox *p_isBoxedCheckBox = nullptr;
    UIInputNumber *p_restTimeInput = nullptr;
    UICheckBox *p_filterForIBLCheckBox = nullptr;

    UIInputNumber *p_zNearInput = nullptr;
    UIInputNumber *p_zFarInput = nullptr;
    UIComboBox *p_clearModeInput = nullptr;
    UIInputColor *p_clearColorInput = nullptr;
    UIInputFile *p_textureCubeMapInput = nullptr;

    UITextureCubeMapPreviewer *p_previewCMRenderer = nullptr;

    CIWReflectionProbe();
    virtual ~CIWReflectionProbe();

    ReflectionProbe *GetReflectionProbe() const;

    // ComponentInspectorWidget
    void OnValueChangedCIW(EventEmitter<IEventsValueChanged> *object) override;
};

NAMESPACE_BANG_EDITOR_END

#endif // CIWREFLECTIONPROBE_H

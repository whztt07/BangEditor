#ifndef CIWRECTTRANSFORM_H
#define CIWRECTTRANSFORM_H

#include "Bang/BangDefines.h"
#include "Bang/IEventsValueChanged.h"
#include "Bang/String.h"
#include "BangEditor/BangEditor.h"
#include "BangEditor/CIWTransform.h"

namespace Bang
{
class IEventsValueChanged;
class RectTransform;
template <class>
class EventEmitter;
}

using namespace Bang;
namespace BangEditor
{
class UIInputVector;

class CIWRectTransform : public CIWTransform
{
    GAMEOBJECT_EDITOR(CIWRectTransform);

public:
    CIWRectTransform() = default;

    // InspectorWidget
    virtual void InitInnerWidgets() override;
    virtual void UpdateFromReference() override;

protected:
    virtual bool CanBeRemovedFromContextMenu() const override;

    RectTransform *GetRectTransform() const;

    // ComponentInspectorWidget
    void OnValueChangedCIW(EventEmitter<IEventsValueChanged> *object) override;

    // ComponentWidget
    bool MustShowEnabledCheckbox() const override;

private:
    virtual ~CIWRectTransform() override = default;
};
}

#endif  // CIWRECTTRANSFORM_H

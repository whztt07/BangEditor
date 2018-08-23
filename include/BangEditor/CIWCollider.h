#ifndef CIWCOLLIDER_H
#define CIWCOLLIDER_H

#include "Bang/Bang.h"

#include "BangEditor/ComponentInspectorWidget.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class Collider;
FORWARD class UIInputNumber;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class UIInputFile;

FORWARD class UIInputVector;

class CIWCollider : public ComponentInspectorWidget
{
    GAMEOBJECT_EDITOR(CIWCollider);

public:
    // InspectorWidget
    virtual void InitInnerWidgets() override;

    // ComponentInspectorWidget
    virtual void UpdateFromReference() override;

protected:
    CIWCollider();
    virtual ~CIWCollider();

    // ComponentInspectorWidget
    virtual void OnValueChangedCIW(EventEmitter<IEventsValueChanged> *object) override;

    Collider *GetCollider() const;

private:
    UIInputVector *p_centerInput = nullptr;
    UIInputFile *p_physicsMaterialInput = nullptr;

    friend class ComponentInspectorWidgetFactory;
};

NAMESPACE_BANG_EDITOR_END

#endif // CIWCOLLIDER_H

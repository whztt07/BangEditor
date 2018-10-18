#ifndef CIWBEHAVIOURCONTAINER_H
#define CIWBEHAVIOURCONTAINER_H

#include "Bang/BangDefines.h"
#include "Bang/IEventsValueChanged.h"
#include "Bang/ReflectVariable.h"
#include "Bang/String.h"
#include "Bang/Time.h"
#include "BangEditor/BangEditor.h"
#include "BangEditor/ComponentInspectorWidget.h"

namespace Bang
{
class BehaviourContainer;
class ReflectStruct;
class IEventsValueChanged;
class UIInputFileWithPreview;
template <class>
class EventEmitter;
class UIButton;
}

using namespace Bang;
namespace BangEditor
{
class UIInputFileWithPreview;

class CIWBehaviourContainer : public ComponentInspectorWidget
{
    GAMEOBJECT_EDITOR(CIWBehaviourContainer);

public:
    // InspectorWidget
    virtual void InitInnerWidgets() override;
    virtual void UpdateFromReference() override;

    BehaviourContainer *GetBehaviourContainer() const;

private:
    UIInputFileWithPreview *p_sourceInputFile = nullptr;
    UIButton *p_resetValuesButton = nullptr;

    CIWBehaviourContainer();
    virtual ~CIWBehaviourContainer() override;

    void UpdateFromReflection(const ReflectStruct &reflectStruct);
    void UpdateModifiedInitializationMetaFromWidget(GameObject *widget);

    // ComponentInspectorWidget
    void OnComponentSet() override;
    ReflectStruct GetComponentReflectStruct() const override;

    // ComponentInspectorWidget
    void OnValueChangedCIW(EventEmitter<IEventsValueChanged> *object) override;

    friend class ComponentInspectorWidgetFactory;
};
}

#endif  // CIWBEHAVIOURCONTAINER_H

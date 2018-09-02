#include "BangEditor/UndoRedoSerializableChange.h"

#include "Bang/Resource.h"

#include "BangEditor/Explorer.h"
#include "BangEditor/Inspector.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

UndoRedoSerializableChange::UndoRedoSerializableChange(Serializable *serializable,
                                                       const XMLNode &xmlBefore,
                                                       const XMLNode &xmlAfter)
{
    p_serializable = serializable;
    m_xmlBefore = xmlBefore;
    m_xmlAfter = xmlAfter;

    if (auto *destroyable = DCAST<EventEmitter<IEventsDestroy>*>(p_serializable))
    {
        destroyable->EventEmitter<IEventsDestroy>::RegisterListener(this);
    }
}

UndoRedoSerializableChange::~UndoRedoSerializableChange()
{
}

void UndoRedoSerializableChange::SetXMLBefore(const XMLNode &xmlBefore)
{
    m_xmlBefore = xmlBefore;
}

void UndoRedoSerializableChange::SetXMLAfter(const XMLNode &xmlAfter)
{
    m_xmlAfter = xmlAfter;
}

void UndoRedoSerializableChange::Undo()
{
    if (p_serializable)
    {
        p_serializable->ImportXML(m_xmlBefore);
        SelectSerializableOrShowInInspectorIfPossible();
    }
}

void UndoRedoSerializableChange::Redo()
{
    if (p_serializable)
    {
        p_serializable->ImportXML(m_xmlAfter);
        SelectSerializableOrShowInInspectorIfPossible();
    }
}

void UndoRedoSerializableChange::OnDestroyed(EventEmitter<IEventsDestroy> *object)
{
    p_serializable = nullptr;
}

void UndoRedoSerializableChange::SelectSerializableOrShowInInspectorIfPossible() const
{
    GameObject *go = DCAST<GameObject*>(p_serializable);
    if (!go)
    {
        Component *comp = DCAST<Component*>(p_serializable);
        if (comp)
        {
            go = comp->GetGameObject();
        }
    }

    if (go)
    {
        Editor::SelectGameObject(go, false);
    }
    else
    {
        bool keepTrying = true;
        if (Resource *res = DCAST<Resource*>(p_serializable))
        {
            Path path = res->GetResourceFilepath();
            if (path.IsFile())
            {
                Explorer::GetInstance()->SelectPath(path, false);
                keepTrying = false;
            }
        }

        if (keepTrying)
        {
            Inspector::GetActive()->ShowSerializable(p_serializable);
        }
    }
}


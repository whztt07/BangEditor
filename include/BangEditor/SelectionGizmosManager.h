#ifndef SELECTIONGIZMOSMANAGER_H
#define SELECTIONGIZMOSMANAGER_H

#include "Bang/Bang.h"
#include "Bang/IDestroyListener.h"

#include "BangEditor/Editor.h"

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class TransformGizmo;
FORWARD class GameObjectSelectionGizmo;
FORWARD class ComponentsSelectionGizmo;

class SelectionGizmosManager : public IEditorListener,
                               public IDestroyListener
{
public:
    SelectionGizmosManager();
	virtual ~SelectionGizmosManager();

    void Update();

    void OnBeginRender(Scene *scene);
    void OnEndRender(Scene *scene);

    TransformGizmo *GetTransformGizmo() const;
    ComponentsSelectionGizmo *GetComponentsSelectionGizmo() const;
    GameObjectSelectionGizmo *GetGameObjectSelectionGizmo() const;

    static SelectionGizmosManager* GetInstance();

private:
    TransformGizmo *p_transformGizmo = nullptr;
    GameObjectSelectionGizmo *p_gameObjectSelectionGizmo = nullptr;
    ComponentsSelectionGizmo *p_componentsSelectionGizmo = nullptr;

    // IEditorListener
    void OnGameObjectSelected(GameObject *selectedGameObject) override;

    // IDestroyListener
    void OnDestroyed(EventEmitter<IDestroyListener> *object) override;
};

NAMESPACE_BANG_EDITOR_END

#endif // SELECTIONGIZMOSMANAGER_H


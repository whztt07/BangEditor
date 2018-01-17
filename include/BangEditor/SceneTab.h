#ifndef SCENETAB_H
#define SCENETAB_H

#include "Bang/GameObject.h"
#include "Bang/IValueChangedListener.h"

#include "BangEditor/Editor.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class UIButton;
FORWARD class Texture2D;
FORWARD class UICheckBox;
FORWARD class UITextRenderer;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class UISceneContainer;

class SceneTab : public GameObject,
                 public IEditorListener,
                 public IValueChangedListener
{
    GAMEOBJECT_EDITOR(SceneTab);

public:
	SceneTab();
	virtual ~SceneTab();

    void OnPlayScene();
    void OnStopScene();

    void SetSceneImageTexture(Texture2D *sceneTexture);
    Rect GetSceneImageRectNDC() const;

private:
    Scene *p_editingScene = nullptr;
    Scene *p_playingScene = nullptr;

    UIButton *p_playButton = nullptr;
    UIButton *p_stopButton = nullptr;
    UITextRenderer *p_stateText = nullptr;
    UICheckBox *p_showDebugStatsCheckbox = nullptr;
    UISceneContainer *p_sceneContainer = nullptr;

    // IEditorListener;
    void OnPlayStateChanged(EditorPlayState previousPlayState,
                            EditorPlayState newPlayState) override;

    // IValueChangedListener
    void OnValueChanged(Object *object) override;
};

NAMESPACE_BANG_EDITOR_END

#endif // SCENETAB_H


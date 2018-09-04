#ifndef SCENEOPENERSAVER_H
#define SCENEOPENERSAVER_H

#include "Bang/Path.h"
#include "Bang/Dialog.h"
#include "Bang/EventListener.h"

#include "BangEditor/ScenePlayer.h"
#include "BangEditor/UndoRedoManager.h"
#include "BangEditor/IEventsScenePlayer.h"
#include "BangEditor/EditorSceneManager.h"

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class SceneOpenerSaver : public EventListener<IEventsSceneManager>,
                         public EventListener<IEventsScenePlayer>,
                         public EventListener<IEventsUndoRedo>
{
public:
    SceneOpenerSaver();
    virtual ~SceneOpenerSaver();

    bool OnNewScene();
    bool OnOpenScene();
    bool OnSaveScene();
    bool OnSaveSceneAs();
    bool CloseScene();
    bool IsCurrentSceneSaved() const;
    void OpenDefaultScene() const;

    bool OpenSceneInEditor(const Path &scenePath);

    // IEventsSceneManager
    void OnSceneLoaded(Scene *scene, const Path &sceneFilepath) override;

    const Path& GetOpenScenePath() const;
    const Path& GetLoadedScenePath() const;

    static SceneOpenerSaver* GetInstance();

private:
    int m_numActionsDoneSinceLastSave = 0;

    Path m_previousLoadedScenePath = Path::Empty;
    Path m_previousOpenScenePath = Path::Empty;

    Path m_currentLoadedScenePath = Path::Empty;
    Path m_currentOpenScenePath = Path::Empty;

    bool OnSaveScene(bool saveAs);
    Dialog::YesNoCancel Overwrite(const Path &path);

    Path GetDialogStartPath() const;

    // IEventsUndoRedo
    void OnActionPushed(UndoRedoAction *action) override;
    void OnUndo(UndoRedoAction *action) override;
    void OnRedo(UndoRedoAction *action) override;

    // IEventsScenePlayer
    void OnPlayStateChanged(PlayState previousPlayState,
                            PlayState newPlayState) override;
};

NAMESPACE_BANG_EDITOR_END

#endif // SCENEOPENERSAVER_H


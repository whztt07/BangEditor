#ifndef EDITORAPPLICATION_H
#define EDITORAPPLICATION_H

#include "BangEditor/BangEditor.h"

#include "Bang/Application.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class Scene;
FORWARD class Texture2D;
FORWARD class DialogWindow;
FORWARD class UIImageRenderer;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class EditorScene;
FORWARD class EditorPaths;

class EditorApplication : public Application
{
public:
    EditorApplication();
    virtual ~EditorApplication();

    void Init(const Path &engineRootPath) override;

    EditorPaths *GetEditorPaths() const;
    void OpenEditorScene(Window *containingWindow);
    EditorScene *GetEditorScene() const;

private:
    EditorScene *m_editorScene = nullptr;

    Paths* CreatePaths() override;
    Window* _CreateWindow() override;
    virtual void SetupWindow(Window *window) override;

    static EditorApplication* GetInstance();

    friend class EditorPaths;
};

NAMESPACE_BANG_EDITOR_END

#endif // EDITORAPPLICATION_H

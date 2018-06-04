#ifndef UNDOREDOMANAGER_H
#define UNDOREDOMANAGER_H

#include "Bang/Bang.h"

#include "BangEditor/BangEditor.h"
#include "BangEditor/UndoRedoAction.h"
#include "BangEditor/ShortcutManager.h"

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class UndoRedoManager
{
public:
	UndoRedoManager();
	virtual ~UndoRedoManager();

    static void Clear();
    static void PushAction(UndoRedoAction *action);
    static void PushActionsInSameStep(
                        const Array<UndoRedoAction*> &actionsInSameStep);

    static UndoRedoManager* GetInstance();

private:
    static constexpr int UndoListSize = 10000;

    List< Array<UndoRedoAction*> > m_undoActions;
    List< Array<UndoRedoAction*> > m_redoActions;

    // Only for debugging
    bool m_undoingOrRedoing = false;

    void OnUndoRedoPressed(bool undo);
};

NAMESPACE_BANG_EDITOR_END

#endif // UNDOREDOMANAGER_H

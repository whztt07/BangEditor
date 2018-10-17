#ifndef UNDOREDOFILECHANGE_H
#define UNDOREDOFILECHANGE_H

#include <cstddef>

#include "Bang/Bang.h"
#include "Bang/BangDefines.h"
#include "Bang/Path.h"
#include "BangEditor/BangEditor.h"
#include "BangEditor/UndoRedoAction.h"

using namespace Bang;
namespace BangEditor
{
class UndoRedoFileChange : public UndoRedoAction
{
public:
    UndoRedoFileChange(const Path &path);
    virtual ~UndoRedoFileChange() override;

    void ReadBefore();
    void ReadAfter();

    void Undo() override;
    void Redo() override;

private:
    Path m_path;

    Byte *m_previousContents = nullptr;
    std::size_t m_previousContentsSize = -1u;

    Byte *m_newContents = nullptr;
    std::size_t m_newContentsSize = -1u;

    void Read(Byte **bytePointer, std::size_t *sizePointer);
};
}

#endif  // UNDOREDOFILECHANGE_H

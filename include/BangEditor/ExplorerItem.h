#ifndef EXPLORERITEM_H
#define EXPLORERITEM_H

#include "Bang/Path.h"
#include "Bang/UIButton.h"
#include "Bang/GameObject.h"

#include "BangEditor/BangEditor.h"

USING_NAMESPACE_BANG

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class UILabel;
FORWARD class UITintedButton;
FORWARD class UIImageRenderer;
FORWARD NAMESPACE_BANG_END

NAMESPACE_BANG_EDITOR_BEGIN

class ExplorerItem : public GameObject,
                      public IUIButtonListener
{
    GAMEOBJECT_EDITOR(ExplorerItem)

public:
    void Update() override;

    void SetFilepath(const Path &path);
    void SetSelected(bool selected);

    bool IsSelected() const;
    const Path& GetFilepath() const;

protected:
    ExplorerItem();
    virtual ~ExplorerItem();

private:
    bool m_selected = false;
    Path m_filepath = Path::Empty;

    UILabel *p_label = nullptr;
    UIImageRenderer *p_bg = nullptr;
    UIImageRenderer *p_icon = nullptr;
    UITintedButton *p_button = nullptr;

    virtual void OnButton_Clicked(UIButton *btn) override;
    virtual void OnButton_DoubleClicked(UIButton *btn) override;
};

NAMESPACE_BANG_EDITOR_END

#endif // EXPLORERITEM_H

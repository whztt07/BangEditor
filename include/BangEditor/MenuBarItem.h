#ifndef MENUBARITEM_H
#define MENUBARITEM_H

#include "BangEditor/BangEditor.h"

#include "Bang/List.h"
#include "Bang/UIButton.h"
#include "Bang/GameObject.h"
#include "Bang/UITextRenderer.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class UIButton;
FORWARD class UIVerticalLayout;
FORWARD class UITintedButton;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class MenuBarItem : public GameObject,
                    public UIButtonListener
{
public:
    MenuBarItem(bool topItem);
    virtual ~MenuBarItem();

    void AddSeparator();
    void AddChild(MenuBarItem *childItem);
    MenuBarItem* AddChild(const String &text);

    UITextRenderer *GetText() const;
    UIButton *GetButton() const;

private:
    bool m_isTopItem = false;
    UITextRenderer *m_text = nullptr;
    UITintedButton *m_buttonWithTint = nullptr;

    List<MenuBarItem*> m_childrenItems;
    GameObject *m_childrenContainer = nullptr;
    UIVerticalLayout *m_childrenContainerVL = nullptr;

    virtual void OnButton_MouseEnter(UIButton *btn);
    virtual void OnButton_MouseExit(UIButton *btn);
    virtual void OnButton_Clicked(UIButton *btn);
};

NAMESPACE_BANG_EDITOR_END

#endif // MENUBARITEM_H
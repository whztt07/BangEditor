#ifndef MENUITEM_H
#define MENUITEM_H

#include "BangEditor/BangEditor.h"

#include "Bang/List.h"
#include "Bang/GameObject.h"
#include "Bang/UITextRenderer.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class IFocusable;
FORWARD class UIFocusable;
FORWARD class UIVerticalLayout;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class MenuItem : public GameObject
{
    GAMEOBJECT(MenuItem);

public:
    enum class MenuItemType { Root, Top, Normal };

    // GameObject
    void Update() override;

    void AddSeparator();
    void AddItem(MenuItem *childItem);
    MenuItem* AddItem(const String &text);

    UITextRenderer *GetText() const;
    UIFocusable *GetButton() const;

    void CloseRecursiveUp();
    bool MustDisplayChildren() const;
    void SetFontSize(uint fontSize);
    void SetDestroyOnClose(bool destroyOnClose);

protected:
    MenuItem(MenuItemType itemType = MenuItemType::Normal);
    virtual ~MenuItem();

private:
    MenuItemType m_itemType = Undef<MenuItemType>();
    uint m_fontSize = Undef<uint>();

    bool m_destroyOnClose = false;

    UIFocusable *p_button = nullptr;
    UITextRenderer *p_text = nullptr;

    List<MenuItem*> p_childrenItems;
    MenuItem* p_parentItem = nullptr;
    GameObject *p_childrenContainer = nullptr;
    UIVerticalLayout *p_childrenContainerVL = nullptr;

    bool m_canDisplayChildrenThisFrame = true;

    friend class Bang::ObjectManager;
};

NAMESPACE_BANG_EDITOR_END

#endif // MENUITEM_H
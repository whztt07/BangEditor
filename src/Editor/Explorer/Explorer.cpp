#include "BangEditor/Explorer.h"

#include <unordered_map>

#include "Bang/Alignment.h"
#include "Bang/Array.h"
#include "Bang/Assert.h"
#include "BangMath/Color.h"
#include "Bang/Dialog.h"
#include "Bang/EventEmitter.h"
#include "Bang/EventListener.tcc"
#include "Bang/Extensions.h"
#include "Bang/File.h"
#include "Bang/GameObject.tcc"
#include "Bang/GameObjectFactory.h"
#include "Bang/IEventsFileTracker.h"
#include "Bang/IEventsProjectManager.h"
#include "Bang/IEventsValueChanged.h"
#include "Bang/Key.h"
#include "Bang/LayoutSizeType.h"
#include "Bang/List.tcc"
#include "Bang/MetaFilesManager.h"
#include "Bang/Paths.h"
#include "Bang/RectTransform.h"
#include "Bang/UIButton.h"
#include "Bang/UICanvas.h"
#include "Bang/UIContentSizeFitter.h"
#include "Bang/UIFocusable.h"
#include "Bang/UIGridLayout.h"
#include "Bang/UIHorizontalLayout.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/UIInputNumber.h"
#include "Bang/UILabel.h"
#include "Bang/UILayoutElement.h"
#include "Bang/UIScrollArea.h"
#include "Bang/UIScrollPanel.h"
#include "Bang/UISlider.h"
#include "Bang/UITextRenderer.h"
#include "Bang/UIVerticalLayout.h"
#include "Bang/UMap.tcc"
#include "BangEditor/Editor.h"
#include "BangEditor/EditorClipboard.h"
#include "BangEditor/EditorFileTracker.h"
#include "BangEditor/EditorPaths.h"
#include "BangEditor/EditorProjectManager.h"
#include "BangEditor/EditorScene.h"
#include "BangEditor/EditorSceneManager.h"
#include "BangEditor/EditorTextureFactory.h"
#include "BangEditor/ExplorerItem.h"
#include "BangEditor/ExplorerItemFactory.h"
#include "BangEditor/IEventsEditor.h"
#include "BangEditor/IEventsExplorerItem.h"
#include "BangEditor/MenuBar.h"
#include "BangEditor/MenuItem.h"
#include "BangEditor/QtProjectManager.h"
#include "BangEditor/SceneOpenerSaver.h"
#include "BangEditor/UIContextMenu.h"

namespace Bang
{
class Texture2D;
class Project;
}

using namespace Bang;
using namespace BangEditor;

Explorer::Explorer()
{
    SetName("Explorer");

    UILayoutElement *le = AddComponent<UILayoutElement>();
    le->SetMinSize(Vector2i(100));
    le->SetFlexibleSize(Vector2::One());

    GameObjectFactory::CreateUIGameObjectInto(this);

    GameObject *mainVLGo = this;
    mainVLGo->AddComponent<UIVerticalLayout>();

    // Tool Bar
    GameObject *toolBar = GameObjectFactory::CreateUIGameObject();
    UIHorizontalLayout *toolBarHL = toolBar->AddComponent<UIHorizontalLayout>();
    toolBarHL->SetPaddingTop(3);
    toolBarHL->SetPaddingBot(3);
    toolBarHL->SetPaddingLeft(3);
    toolBarHL->SetPaddingRight(3);
    toolBarHL->SetSpacing(3);

    constexpr int ToolBarHeight = 30;
    UILayoutElement *toolBarLE = toolBar->AddComponent<UILayoutElement>();
    toolBarLE->SetMinHeight(ToolBarHeight);
    toolBarLE->SetFlexibleHeight(0);

    // Scroll Panel
    p_scrollPanel = GameObjectFactory::CreateUIScrollPanel();
    p_scrollPanel->GetScrollArea()->GetBackground()->SetTint(
        Color::White().WithValue(0.7f));

    GameObject *scrollPanelGo = p_scrollPanel->GetGameObject();
    UILayoutElement *spLE = scrollPanelGo->AddComponent<UILayoutElement>();
    spLE->SetFlexibleSize(Vector2::One());

    // Back button
    Texture2D *backButtonTex = EditorTextureFactory::GetBackArrowIcon();
    p_backButton = GameObjectFactory::CreateUIButton("", backButtonTex);
    p_backButton->SetIcon(backButtonTex, Vector2i(20, 15), 0);
    p_backButton->GetText()->SetContent("");
    p_backButton->AddClickedCallback([this]() { GoDirectoryUp(); });

    // Direction label
    p_currentPathLabel = GameObjectFactory::CreateUILabel();
    p_currentPathLabel->SetSelectable(true);
    p_currentPathLabel->GetText()->SetTextSize(11);
    p_currentPathLabel->GetText()->SetCullByRectTransform(true);
    p_currentPathLabel->GetText()->SetHorizontalAlign(
        HorizontalAlignment::RIGHT);

    GameObject *dirBar = p_currentPathLabel->GetGameObject();

    // Items Container
    p_itemsContainer = GameObjectFactory::CreateUIGameObject();
    p_itemsContainer->GetRectTransform()->SetPivotPosition(Vector2(-1, 1));

    UIContentSizeFitter *csf =
        p_itemsContainer->AddComponent<UIContentSizeFitter>();
    csf->SetHorizontalSizeType(LayoutSizeType::NONE);
    csf->SetVerticalSizeType(LayoutSizeType::PREFERRED);

    // Grid layout
    p_explorerGridLayout = p_itemsContainer->AddComponent<UIGridLayout>();
    p_explorerGridLayout->SetPaddings(10);
    p_explorerGridLayout->SetSpacing(10);

    // Icon size slider
    p_iconSizeSlider = GameObjectFactory::CreateUISlider();
    p_iconSizeSlider->GetInputNumber()->SetDecimalPlaces(0);
    p_iconSizeSlider->SetMinMaxValues(64, 256);
    p_iconSizeSlider->GetInputNumber()->GetGameObject()->SetEnabled(false);
    p_iconSizeSlider->EventEmitter<IEventsValueChanged>::RegisterListener(this);
    p_iconSizeSlider->SetValue(128);

    UIImageRenderer *eyeImg = GameObjectFactory::CreateUIImage();
    eyeImg->SetImageTexture(EditorTextureFactory::GetEyeIcon());
    eyeImg->SetTint(Color::Black());
    UILayoutElement *eyeImgLE =
        eyeImg->GetGameObject()->AddComponent<UILayoutElement>();
    eyeImgLE->SetPreferredSize(Vector2i(20, ToolBarHeight));
    eyeImgLE->SetFlexibleSize(Vector2(0, 1));

    UILayoutElement *iconsSizeSliderLE =
        p_iconSizeSlider->GetGameObject()->AddComponent<UILayoutElement>();
    iconsSizeSliderLE->SetMinWidth(128);
    iconsSizeSliderLE->SetFlexibleWidth(0.0f);
    iconsSizeSliderLE->SetLayoutPriority(2);

    p_backButton->GetGameObject()->SetParent(toolBar);
    GameObjectFactory::CreateUIVSpacer(LayoutSizeType::MIN, 15)
        ->SetParent(toolBar);
    eyeImg->GetGameObject()->SetParent(toolBar);
    GameObjectFactory::CreateUIVSpacer(LayoutSizeType::MIN, 5)
        ->SetParent(toolBar);
    p_iconSizeSlider->GetGameObject()->SetParent(toolBar);
    dirBar->SetParent(toolBar);
    toolBar->SetParent(mainVLGo);
    GameObjectFactory::CreateUIHSpacer(LayoutSizeType::MIN, 5)
        ->SetParent(mainVLGo);
    p_scrollPanel->GetGameObject()->SetParent(mainVLGo);

    p_scrollPanel->GetScrollArea()->SetContainedGameObject(p_itemsContainer);
    p_scrollPanel->SetVerticalShowScrollMode(ShowScrollMode::WHEN_NEEDED);
    p_scrollPanel->SetVerticalScrollBarSide(HorizontalSide::RIGHT);
    p_scrollPanel->SetHorizontalScrollEnabled(false);

    UIFocusable *focusable = AddComponent<UIFocusable>();
    focusable->EventEmitter<IEventsFocus>::RegisterListener(this);

    p_contextMenu = AddComponent<UIContextMenu>();
    p_contextMenu->SetCreateContextMenuCallback(
        [this](MenuItem *menuRootItem) { OnCreateContextMenu(menuRootItem); });
    p_contextMenu->SetFocusable(focusable);

    SetCurrentPath(Paths::GetEngineAssetsDir());

    Editor::GetInstance()->EventEmitter<IEventsEditor>::RegisterListener(this);
    ProjectManager::GetInstance()
        ->EventEmitter<IEventsProjectManager>::RegisterListener(this);
    EditorFileTracker::GetInstance()
        ->EventEmitter<IEventsFileTracker>::RegisterListener(this);
}

Explorer::~Explorer()
{
}

void Explorer::Update()
{
    GameObject::Update();

#ifdef DEBUG
    if (Input::GetKey(Key::P))
    {
        if (Input::GetKeyDown(Key::NUM1))
        {
            SetRootPath(EditorPaths::GetEngineAssetsDir());
        }
        else if (Input::GetKeyDown(Key::NUM2))
        {
            SetRootPath(EditorPaths::GetEditorAssetsDir());
        }
        else if (Input::GetKeyDown(Key::NUM0))
        {
            SetRootPath(EditorPaths::GetProjectAssetsDir());
        }
    }
#endif
}

void Explorer::ForceCheckFileChanges()
{
    EditorFileTracker::GetInstance()->CheckFiles();
}

void Explorer::SelectPath(const Path &path,
                          bool registerUndo,
                          bool travelToDirectory)
{
    if (path != GetSelectedPath())
    {
        if (registerUndo)
        {
            // UndoRedoManager::PushAction( new UndoRedoExplorerSelect(
            //                                  GetSelectedPath(), path) );
        }

        m_selectedPath = path;

        if (path.GetDirectory().Exists() && travelToDirectory)
        {
            SetCurrentPath(path.GetDirectory());
        }

        ExplorerItem *explorerItem = GetItemFromPath(path);
        if (explorerItem)
        {
            UICanvas::GetActive(this)->SetFocus(explorerItem->GetFocusable());
            Editor::OnPathSelected(explorerItem->GetPath());
        }
    }
}

void Explorer::SetRootPath(const Path &rootPath)
{
    if (rootPath != GetRootPath())
    {
        m_rootPath = rootPath;
        if (!IsInsideRootPath(GetCurrentPath()))
        {
            SetCurrentPath(GetRootPath());
        }
    }
}

void Explorer::SetCurrentPath(const Path &path)
{
    if (GetCurrentPath() != path && IsInsideRootPath(path))
    {
        m_currentPath = path;
        p_currentPathLabel->GetText()->SetContent(
            GetCurrentPath().GetAbsolute());

        bool canGoBack = (GetCurrentPath() != GetRootPath());
        p_backButton->SetBlocked(!canGoBack);

        Clear();

        Array<ExplorerItem *> subExplorerItems =
            ExplorerItemFactory::CreateAndGetSubPathsExplorerItems(path,
                                                                   canGoBack);
        for (ExplorerItem *expItem : subExplorerItems)
        {
            AddItem(expItem);
        }
        p_scrollPanel->SetScrolling(Vector2i::Zero());
    }
}

const Path &Explorer::GetRootPath() const
{
    return m_rootPath;
}

const Path &Explorer::GetCurrentPath() const
{
    return m_currentPath;
}

const Path &Explorer::GetSelectedPath() const
{
    return m_selectedPath;
}

void Explorer::Clear()
{
    while (!p_items.IsEmpty())
    {
        RemoveItem(p_items.Front()->GetPath());
    }
    m_pathsToItem.Clear();
    p_items.Clear();
}

UIEventResult Explorer::OnUIEvent(UIFocusable *, const UIEvent &event)
{
    switch (event.type)
    {
        case UIEvent::Type::MOUSE_CLICK_FULL:
            SelectPath(Path::Empty());
            return UIEventResult::INTERCEPT;
            break;

        case UIEvent::Type::KEY_DOWN:
            if (event.key.key == Key::V &&
                event.key.modifiers == KeyModifier::LCTRL)
            {
                if (EditorClipboard::HasCopiedPath())
                {
                    const Path &copiedPath = EditorClipboard::GetCopiedPath();
                    const Path newDir = GetCurrentPath();
                    DuplicatePathIntoDir(copiedPath, newDir);
                    return UIEventResult::INTERCEPT;
                }
            }
            break;

        default: break;
    }
    return UIEventResult::IGNORE;
}

void Explorer::OnProjectOpen(const Project *project)
{
    IEventsProjectManager::OnProjectOpen(project);
    SetRootPath(Paths::GetProjectAssetsDir());
    SetCurrentPath(Paths::GetProjectAssetsDir());
}

void Explorer::OnProjectClosed(const Project *project)
{
    IEventsProjectManager::OnProjectClosed(project);
    SetCurrentPath(Paths::GetEngineAssetsDir());
}

void Explorer::OnPathAdded(const Path &addedPath)
{
    if (addedPath.GetDirectory() == GetCurrentPath() &&
        !addedPath.IsHiddenFile() && addedPath.Exists())
    {
        AddItem(addedPath);
    }
}

void Explorer::OnPathModified(const Path &)
{
}

void Explorer::OnPathRemoved(const Path &removedPath)
{
    if (!removedPath.Exists())
    {
        if (removedPath == GetSelectedPath())
        {
            SelectPath(Path::Empty(), false, false);
        }
        RemoveItem(removedPath);
    }
}

void Explorer::OnGameObjectSelected(GameObject *selectedGameObject)
{
    if (selectedGameObject)
    {
        SelectPath(Path::Empty(), false);
    }
}

void Explorer::AddItem(const Path &itemPath)
{
    AddItem(ExplorerItemFactory::CreateExplorerItem(itemPath));
}

void Explorer::AddItem(ExplorerItem *explorerItem)
{
    Path itemPath = explorerItem->GetPath();
    if (GetItemFromPath(itemPath))
    {
        return;
    }

    explorerItem->SetParent(p_itemsContainer);

    explorerItem->GetFocusable()->AddEventCallback([this, explorerItem](
        UIFocusable *focusable, const UIEvent &event) {
        if (event.type == UIEvent::Type::MOUSE_CLICK_FULL)
        {
            bool travelToDirectory = (explorerItem->GetPathString() != "..");
            SelectPath(explorerItem->GetPath(), true, travelToDirectory);
            return UIEventResult::INTERCEPT;
        }
        else if (event.type == UIEvent::Type::MOUSE_CLICK_DOUBLE)
        {
            OnItemDoubleClicked(focusable);
            return UIEventResult::INTERCEPT;
        }
        return UIEventResult::IGNORE;
    });
    explorerItem->EventEmitter<IEventsExplorerItem>::RegisterListener(this);

    p_items.PushBack(explorerItem);
    m_pathsToItem.Add(itemPath, explorerItem);
}

void Explorer::RemoveItem(const Path &itemPath)
{
    ExplorerItem *explorerItem = GetItemFromPath(itemPath);
    if (explorerItem)
    {
        explorerItem->EventEmitter<IEventsExplorerItem>::UnRegisterListener(
            DCAST<EventListener<IEventsExplorerItem> *>(this));

        p_items.Remove(explorerItem);
        m_pathsToItem.Remove(itemPath);
        GameObject::Destroy(explorerItem);
    }
}

void Explorer::GoDirectoryUp()
{
    SetCurrentPath(GetCurrentPath().GetDirectory());
}

void DuplicateImportFiles(const Path &srcPath, const Path &dstPath)
{
    ASSERT(srcPath.Exists() && dstPath.Exists());
    ASSERT(srcPath != dstPath);

    if (srcPath.IsFile())
    {
        MetaFilesManager::DuplicateMetaFile(srcPath, dstPath);
    }
    else  // IsDir()
    {
        Array<Path> srcSubPaths = srcPath.GetSubPaths(FindFlag::SIMPLE);
        const Path &srcDir = srcPath;
        const Path &dstDir = dstPath;
        for (const Path &srcSubPath : srcSubPaths)
        {
            const Path srcRelSubPath = srcSubPath.GetRelativePath(srcDir);
            const Path dstSubPath = dstDir.Append(srcRelSubPath);
            if (dstSubPath.Exists())
            {
                DuplicateImportFiles(srcSubPath, dstSubPath);
            }
        }
    }
}

void Explorer::DuplicatePathIntoDir(const Path &srcPath, const Path &dstDirPath)
{
    const Path dstPath =
        dstDirPath.Append(srcPath.GetNameExt()).GetDuplicatePath();
    File::Duplicate(srcPath, dstPath);
    DuplicateImportFiles(srcPath, dstPath);

    ForceCheckFileChanges();
    Explorer::SelectPath(dstPath);
}

void Explorer::OnRename(ExplorerItem *explorerItem)
{
    const Path &path = explorerItem->GetPath();

    String newName =
        Dialog::GetString("Rename", "Introduce the new name:", path.GetName());
    String oldExtensions = String::Join(path.GetExtensions(), ".");

    if (!newName.IsEmpty())
    {
        Path newPath = path.GetDirectory().Append(newName);

        String newExtension = newPath.GetExtension();
        if (newExtension.IsEmpty())
        {
            newPath = newPath.AppendExtension(oldExtensions);
        }

        if (newPath != path)
        {
            if (newPath.Exists())
            {
                Dialog::Error("Can't rename", "The path already exists.");
            }
            else
            {
                File::Rename(path, newPath);
                EditorFileTracker::GetInstance()->OnPathRenamed(path, newPath);
                OnPathAdded(newPath);
                explorerItem->SetPath(newPath);
                SelectPath(newPath, false, true);
            }
        }
    }

    ForceCheckFileChanges();
}

void Explorer::OnRemove(ExplorerItem *explorerItem)
{
    const Path &path = explorerItem->GetPath();
    Dialog::YesNoCancel yesNoCancel = Dialog::GetYesNoCancel(
        "Remove",
        "Are you sure you want to remove '" + path.GetNameExt() + "' ?");

    if (yesNoCancel == Dialog::YesNoCancel::YES)
    {
        File::Remove(path);
        File::Remove(MetaFilesManager::GetMetaFilepath(path));
    }

    ForceCheckFileChanges();
}

void Explorer::OnDuplicate(ExplorerItem *explorerItem)
{
    const Path &srcPath = explorerItem->GetPath();
    const Path &dstDirPath = GetCurrentPath();
    DuplicatePathIntoDir(srcPath, dstDirPath);
}

void Explorer::OnPastedOver(ExplorerItem *item)
{
    ASSERT(EditorClipboard::HasCopiedPath());

    const Path dstDirPath =
        (item->GetPath().IsDir() ? item->GetPath() : GetCurrentPath());
    ASSERT(dstDirPath.IsDir());

    const Path &srcPath = EditorClipboard::GetCopiedPath();
    DuplicatePathIntoDir(srcPath, dstDirPath);
}

void Explorer::OnDroppedToDirectory(ExplorerItem *item)
{
    BANG_UNUSED(item);
    ForceCheckFileChanges();
}

ExplorerItem *Explorer::GetSelectedItem() const
{
    for (ExplorerItem *explorerItem : p_items)
    {
        if (explorerItem->IsSelected())
        {
            return explorerItem;
        }
    }
    return nullptr;
}

ExplorerItem *Explorer::GetItemFromPath(const Path &path) const
{
    return m_pathsToItem.ContainsKey(path) ? m_pathsToItem.Get(path) : nullptr;
}

void Explorer::OnItemDoubleClicked(UIFocusable *itemFocusable)
{
    GameObject *itemGo = DCAST<UIFocusable *>(itemFocusable)->GetGameObject();
    ExplorerItem *expItem = DCAST<ExplorerItem *>(itemGo);
    ASSERT(expItem);

    const Path itemPath = expItem->GetPath();
    if (ExplorerItemFactory::CanHaveSubpaths(itemPath))
    {
        SetCurrentPath(itemPath);
    }
    else
    {
        if (itemPath.HasExtension(Extensions::GetSceneExtension()))
        {
            if (Editor::IsEditingScene())
            {
                SceneOpenerSaver::GetInstance()->OpenSceneInEditor(itemPath);
            }
        }
        else if (itemPath.HasExtension(Extensions::GetBehaviourExtensions()))
        {
            QtProjectManager::OpenBehaviourInQtCreator(itemPath);
        }
    }
}

bool Explorer::IsInsideRootPath(const Path &path) const
{
    return path.IsSubPathOf(GetRootPath());
}

void Explorer::OnValueChanged(EventEmitter<IEventsValueChanged> *)
{
    p_explorerGridLayout->SetCellSize(
        Vector2i(SCAST<int>(p_iconSizeSlider->GetValue())));
}

void Explorer::OnCreateContextMenu(MenuItem *menuRootItem)
{
    menuRootItem->SetFontSize(12);

    MenuItem *createDirItem = menuRootItem->AddItem("Create directory");
    createDirItem->SetSelectedCallback([this](MenuItem *) {
        Path newDirPath =
            GetCurrentPath().Append("New_directory").GetDuplicatePath();
        File::CreateDir(newDirPath);
        ForceCheckFileChanges();
        SelectPath(newDirPath);
    });

    MenuItem *createAssetItem = menuRootItem->AddItem("Create Asset");
    MenuBar::CreateAssetsMenuInto(createAssetItem);

    MenuItem *pasteItem = menuRootItem->AddItem("Paste");
    pasteItem->SetOverAndActionEnabled(EditorClipboard::HasCopiedPath());
    pasteItem->SetSelectedCallback([this](MenuItem *) {
        const Path &copiedPath = EditorClipboard::GetCopiedPath();
        const Path newDir = GetCurrentPath();
        DuplicatePathIntoDir(copiedPath, newDir);
    });
}

Explorer *Explorer::GetInstance()
{
    if (EditorScene *edScene = EditorSceneManager::GetEditorScene())
    {
        return edScene->GetExplorer();
    }
    return nullptr;
}

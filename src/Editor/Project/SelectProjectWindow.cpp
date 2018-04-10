#include "BangEditor/SelectProjectWindow.h"

#include "Bang/Scene.h"
#include "Bang/Camera.h"
#include "Bang/Dialog.h"
#include "Bang/UIList.h"
#include "Bang/UILabel.h"
#include "Bang/Material.h"
#include "Bang/UIButton.h"
#include "Bang/UICanvas.h"
#include "Bang/Extensions.h"
#include "Bang/GameObject.h"
#include "Bang/IconManager.h"
#include "Bang/SceneManager.h"
#include "Bang/RectTransform.h"
#include "Bang/WindowManager.h"
#include "Bang/UIScrollPanel.h"
#include "Bang/UITextRenderer.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/UILayoutElement.h"
#include "Bang/UIVerticalLayout.h"
#include "Bang/GameObjectFactory.h"
#include "Bang/UIHorizontalLayout.h"
#include "Bang/UIAspectRatioFitter.h"

#include "BangEditor/Project.h"
#include "BangEditor/EditorPaths.h"
#include "BangEditor/EditorSettings.h"
#include "BangEditor/ProjectManager.h"
#include "BangEditor/EditorSceneManager.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

Path SelectProjectWindow::OpenProjectResult = Path::EmptyPath();

SelectProjectWindow::SelectProjectWindow()
{
}

SelectProjectWindow::~SelectProjectWindow()
{
}

void SelectProjectWindow::Init()
{
    SetTitle("Bang Editor - Select Project");

    Scene *scene = GameObject::Create<SelectProjectScene>();
    EditorSceneManager::LoadScene(scene, true);
}

// ===========================================================================

SelectProjectScene::SelectProjectScene()
{
    GameObjectFactory::CreateUIGameObjectInto(this);

    GameObject *cameraGo = GameObjectFactory::CreateGameObject(true);
    Camera *camera = cameraGo->AddComponent<Camera>();
    camera->GetGameObject()->SetParent(this);
    SetCamera(camera);

    UICanvas *canvas = GameObjectFactory::CreateUICanvas();
    GameObject *canvasGo = canvas->GetGameObject();
    UIImageRenderer *bg = canvasGo->AddComponent<UIImageRenderer>();
    bg->SetTint(Color::DarkGray);
    canvas->GetGameObject()->SetParent(this);

    GameObject *mainVLGo = GameObjectFactory::CreateUIGameObject();
    UIVerticalLayout *mainVL = mainVLGo->AddComponent<UIVerticalLayout>();
    mainVL->SetPaddings(10);
    mainVL->SetSpacing(10);
    mainVLGo->SetParent(canvasGo);

    GameObject *logoContainer = GameObjectFactory::CreateUIGameObject();
    UILayoutElement *logoLE = logoContainer->AddComponent<UILayoutElement>();
    logoLE->SetFlexibleSize( Vector2(1, 0.5f) );
    logoLE->SetLayoutPriority(-5);
    logoContainer->SetParent(mainVLGo);

    UIImageRenderer *logo = GameObjectFactory::CreateUIImage();
    logo->GetMaterial()->SetUvMultiply( Vector2(1,-1) );
    Texture2D *logoTex = IconManager::GetBang2048Icon().Get();
    logo->SetImageTexture(logoTex);
    GameObject *logoGo = logo->GetGameObject();
    logoGo->GetRectTransform()->SetAnchorX( Vector2(0) );
    logoGo->GetRectTransform()->SetAnchorY( Vector2(-1) );
    logoGo->GetRectTransform()->SetPivotPosition( Vector2(0, -1) );
    UIAspectRatioFitter *logoARF = logoGo->AddComponent<UIAspectRatioFitter>();
    logoARF->SetAspectRatio( logoTex->GetSize() );
    logoARF->SetAspectRatioMode(AspectRatioMode::Keep);
    logo->GetGameObject()->SetParent(logoContainer);

    GameObjectFactory::CreateUIHSeparator()->SetParent(mainVLGo);

    UILabel *recentPLLabel = GameObjectFactory::CreateUILabel();
    UILayoutElement *recentPLLabelLE = recentPLLabel->GetGameObject()->
                                       GetComponent<UILayoutElement>();
    recentPLLabelLE->SetFlexibleSize(Vector2::Zero);
    recentPLLabel->GetText()->SetContent("Recent projects:");
    recentPLLabel->GetText()->SetTextColor(Color::White);
    recentPLLabel->GetText()->SetTextSize(16);
    recentPLLabel->GetText()->SetHorizontalAlign(HorizontalAlignment::Left);
    recentPLLabel->GetGameObject()->SetParent(mainVLGo);

    GameObject *recentPLContainer =
                        GameObjectFactory::CreateUIGameObject(true);
    UIImageRenderer *rplBG = recentPLContainer->AddComponent<UIImageRenderer>();
    rplBG->SetTint(Color::DarkGray.WithValue(0.5f));

    UILayoutElement *rplLE = recentPLContainer->AddComponent<UILayoutElement>();
    rplLE->SetFlexibleSize( Vector2(1,1) );
    recentPLContainer->SetParent(mainVLGo);

    UIList *recentProjectsList = GameObjectFactory::CreateUIList(true);
    GameObject *recentProjectsListGo  = recentProjectsList->GetGameObject();
    UIScrollPanel *rplSP = recentProjectsList->GetScrollPanel();
    rplSP->SetVerticalShowScrollMode(ShowScrollMode::WhenNeeded);
    rplSP->SetHorizontalShowScrollMode(ShowScrollMode::WhenNeeded);
    recentProjectsListGo->SetParent(recentPLContainer);

    GameObjectFactory::CreateUIHSeparator()->SetParent(mainVLGo);

    GameObject *botHLGo = GameObjectFactory::CreateUIGameObject();
    UIHorizontalLayout *botHL = botHLGo->AddComponent<UIHorizontalLayout>();
    botHL->SetSpacing(20);
    UILayoutElement *botHLLE = botHLGo->AddComponent<UILayoutElement>();
    botHLLE->SetMinHeight(30);
    botHLLE->SetFlexibleSize( Vector2(1,0) );
    botHLGo->SetParent(mainVLGo);

    GameObjectFactory::CreateUIHSpacer()->SetParent(botHLGo);

    p_newProjectButton  = GameObjectFactory::CreateUIButton();
    p_newProjectButton->GetText()->SetContent("New project...");
    p_newProjectButton->GetGameObject()->SetParent(botHLGo);
    p_newProjectButton->GetFocusable()->
                        EventEmitter<IFocusListener>::RegisterListener(this);

    p_openProjectButton = GameObjectFactory::CreateUIButton();
    p_openProjectButton->GetText()->SetContent("Open project...");
    p_openProjectButton->GetGameObject()->SetParent(botHLGo);
    p_openProjectButton->GetFocusable()->
                        EventEmitter<IFocusListener>::RegisterListener(this);

    p_openSelectedProjectButton = GameObjectFactory::CreateUIButton();
    p_openSelectedProjectButton->GetText()->SetContent("Open selected project...");
    p_openSelectedProjectButton->GetGameObject()->SetParent(botHLGo);
    p_openSelectedProjectButton->GetFocusable()->
                        EventEmitter<IFocusListener>::RegisterListener(this);
}

SelectProjectScene::~SelectProjectScene()
{

}

void SelectProjectScene::Update()
{
    Scene::Update();
}

void SelectProjectScene::NewProject()
{
    Path newProjectDirPath = Dialog::OpenDirectory("Create New Project...",
                                                   EditorPaths::GetHome());
    if (newProjectDirPath.IsDir())
    {
        String projectName = Dialog::GetString("Choose Project Name",
                                               "Please, choose your project name:",
                                               "NewProject");
        if (!projectName.IsEmpty())
        {
            Project *proj = ProjectManager::CreateNewProject(newProjectDirPath,
                                                             projectName);
            ConfirmOpenProject(proj->GetProjectFilepath());
        }
    }
}

void SelectProjectScene::OpenProject()
{
    Path projectFilepath = Dialog::OpenFilePath("Open Project...",
                                                {Extensions::GetProjectExtension()},
                                                 EditorPaths::GetHome());
    if (projectFilepath.IsFile())
    {
        ConfirmOpenProject(projectFilepath);
    }
}

void SelectProjectScene::ConfirmOpenProject(const Path &projectFilepath)
{
    SelectProjectWindow::OpenProjectResult = projectFilepath;
    WindowManager::GetInstance()->DestroyWindow( Window::GetActive() );
}

void SelectProjectScene::OnClicked(IFocusable *focusable)
{
    if (focusable == p_newProjectButton->GetFocusable())
    {
        NewProject();
    }
    else if (focusable == p_openProjectButton->GetFocusable())
    {
        OpenProject();
    }
    else if (focusable == p_openSelectedProjectButton->GetFocusable())
    {
    }
}

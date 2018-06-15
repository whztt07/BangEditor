#include "BangEditor/MaterialPreviewFactory.h"

#include "Bang/GL.h"
#include "Bang/Scene.h"
#include "Bang/Camera.h"
#include "Bang/GBuffer.h"
#include "Bang/GEngine.h"
#include "Bang/Material.h"
#include "Bang/Transform.h"
#include "Bang/GameObject.h"
#include "Bang/GLUniforms.h"
#include "Bang/Framebuffer.h"
#include "Bang/MeshRenderer.h"
#include "Bang/ShaderProgram.h"
#include "Bang/TextureFactory.h"
#include "Bang/DirectionalLight.h"
#include "Bang/GameObjectFactory.h"
#include "Bang/ShaderProgramFactory.h"

#include "BangEditor/EditorResources.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

MaterialPreviewFactory::MaterialPreviewFactory()
{
}

MaterialPreviewFactory::~MaterialPreviewFactory()
{
    delete m_auxiliarFBToCopyTextures;

    if (m_previewScene)
    {
        GameObject::Destroy(m_previewScene);
    }

    for (RH<Material> &mat : m_materials)
    {
        mat.Get()->EventEmitter<IEventsResource>::UnRegisterListener(this);
    }
}

void MaterialPreviewFactory::Init()
{
    m_auxiliarFBToCopyTextures = new Framebuffer();
    m_auxiliarFBToCopyTextures->CreateAttachmentTex2D(GL::Attachment::COLOR0,
                                                      GL::ColorFormat::RGBA8);
}

RH<Texture2D> MaterialPreviewFactory::GetPreviewTextureFor(Material *material)
{
    return MaterialPreviewFactory::GetActive()->GetPreviewTextureFor_(material);
}

RH<Texture2D> MaterialPreviewFactory::GetPreviewTextureFor_(Material *material)
{
    if (!material) { return TextureFactory::GetWhiteTexture(); }
    if (!m_previewsMap.ContainsKey(material->GetGUID()))
    {
        // Create empty preview texture
        RH<Texture2D> previewTexRH = Resources::Create<Texture2D>();
        Texture2D *previewTex = previewTexRH.Get();

        // Fill texture with preview
        FillTextureWithPreview(previewTex, material);

        // Add to preview map
        material->EventEmitter<IEventsResource>::RegisterListener(this);
        m_previewsMap.Add(material->GetGUID(), previewTexRH);
        m_materials.PushBack( RH<Material>(material) );
    }

    return m_previewsMap.Get(material->GetGUID());
}

void MaterialPreviewFactory::CreatePreviewScene()
{
    Scene *scene = GameObjectFactory::CreateScene();

    GameObject *camGo = GameObjectFactory::CreateGameObject();
    camGo->GetTransform()->SetPosition( Vector3(1.3f) );
    camGo->GetTransform()->LookAt(Vector3::Zero);
    camGo->SetParent(scene);

    Camera *camera = camGo->AddComponent<Camera>();
    camera->SetClearMode(Camera::ClearMode::SKY_BOX);
    camera->SetSkyBoxTexture( TextureFactory::GetDefaultTextureCubeMap().Get() );
    scene->SetCamera(camera);

    GameObject *dLightGo = GameObjectFactory::CreateGameObject();
    DirectionalLight *dLight = dLightGo->AddComponent<DirectionalLight>();
    dLight->SetShadowType(Light::ShadowType::NONE);
    dLightGo->SetParent(scene);

    GameObject *sphere = GameObjectFactory::CreateSphereGameObject();
    sphere->GetTransform()->SetPosition( Vector3::Zero );
    sphere->SetParent(scene);

    scene->Start();

    m_previewScene = scene;
    p_previewCamera = camera;
    p_previewMeshRenderer = sphere->GetComponent<MeshRenderer>();
}

MaterialPreviewFactory *MaterialPreviewFactory::GetActive()
{
    return EditorResources::GetInstance()->GetMaterialPreviewFactory();
}

void MaterialPreviewFactory::FillTextureWithPreview(Texture2D *texture,
                                                    Material *material)
{
    // Now we will fill the texture with the proper preview
    GL::Push(GL::Pushable::VIEWPORT);
    GL::Push(GL::BindTarget::TEXTURE_2D);
    GL::Push(GL::Pushable::FRAMEBUFFER_AND_READ_DRAW_ATTACHMENTS);

    // Prepare to draw scene
    if (!m_previewScene) { CreatePreviewScene(); }
    ASSERT(m_previewScene);
    ASSERT(p_previewCamera);
    ASSERT(p_previewMeshRenderer);

    const uint previewTextureSize = 256;
    m_auxiliarFBToCopyTextures->SetAttachmentTexture(texture,
                                                     GL::Attachment::COLOR0);
    m_auxiliarFBToCopyTextures->Resize(previewTextureSize, previewTextureSize);

    p_previewCamera->GetGBuffer()->Resize(previewTextureSize, previewTextureSize);
    p_previewMeshRenderer->SetMaterial(material);


    GL::SetViewport(0, 0, previewTextureSize, previewTextureSize);
    GEngine::GetInstance()->Render(m_previewScene, p_previewCamera);

    GL::Bind(GL::BindTarget::READ_FRAMEBUFFER, p_previewCamera->GetGBuffer()->GetGLId());
    GL::Bind(GL::BindTarget::DRAW_FRAMEBUFFER, m_auxiliarFBToCopyTextures->GetGLId());

    GL::ReadBuffer(p_previewCamera->GetGBuffer()->GetLastDrawnColorAttachment());
    GL::DrawBuffers( {GL::Attachment::COLOR0} );

    GL::BlitFramebuffer(GL::GetViewportRect(),
                        GL::GetViewportRect(),
                        GL::FilterMode::NEAREST,
                        GL::BufferBit::COLOR);

    texture->Bind();
    texture->SetFilterMode(GL::FilterMode::BILINEAR);
    // texture->GenerateMipMaps();
    // texture->SetFilterMode(GL::FilterMode::Trilinear_LL);
    texture->PropagateResourceChanged();

    // Restore OpenGL state
    GL::Pop(GL::Pushable::FRAMEBUFFER_AND_READ_DRAW_ATTACHMENTS);
    GL::Pop(GL::BindTarget::TEXTURE_2D);
    GL::Pop(GL::Pushable::VIEWPORT);
}

void MaterialPreviewFactory::OnResourceChanged(Resource *changedResource)
{
    ASSERT(m_previewsMap.ContainsKey(changedResource->GetGUID()));

    SetReceiveEvents(false);

    Material *changedMat = SCAST<Material*>(changedResource);
    Texture2D *matPreviewTex = m_previewsMap.Get(changedMat->GetGUID()).Get();
    FillTextureWithPreview(matPreviewTex, changedMat); // Update preview

    SetReceiveEvents(true);
}

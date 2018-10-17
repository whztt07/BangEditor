#include "BangEditor/RIWMaterial.h"

#include "Bang/EventEmitter.h"
#include "Bang/Extensions.h"
#include "Bang/Flags.h"
#include "Bang/GL.h"
#include "Bang/GameObject.h"
#include "Bang/GameObject.tcc"
#include "Bang/GameObjectFactory.h"
#include "Bang/Material.h"
#include "Bang/NeededUniformFlags.h"
#include "Bang/Path.h"
#include "Bang/RenderPass.h"
#include "Bang/ResourceHandle.h"
#include "Bang/Resources.h"
#include "Bang/Resources.tcc"
#include "Bang/Shader.h"
#include "Bang/ShaderProgram.h"
#include "Bang/ShaderProgramFactory.h"
#include "Bang/Texture2D.h"
#include "Bang/UICheckBox.h"
#include "Bang/UIComboBox.h"
#include "Bang/UIInputNumber.h"
#include "Bang/UISlider.h"
#include "BangEditor/MaterialPreviewFactory.h"
#include "BangEditor/PreviewViewer.h"
#include "BangEditor/RIWResource.tcc"
#include "BangEditor/UIInputColor.h"
#include "BangEditor/UIInputFileWithPreview.h"
#include "BangEditor/UIInputTexture.h"
#include "BangEditor/UIInputVector.h"

namespace Bang
{
class IEventsValueChanged;
}

namespace BangEditor
{
class ResourcePreviewFactoryParameters;
}

using namespace Bang;
using namespace BangEditor;

RIWMaterial::RIWMaterial()
{
}

RIWMaterial::~RIWMaterial()
{
}

void RIWMaterial::Init()
{
    RIWResource<Material>::Init();

    SetName("RIWMaterial");
    SetTitle("Material");

    p_albedoTextureInput = GameObject::Create<UIInputTexture>();
    p_albedoTextureInput->SetExtensions(Extensions::GetImageExtensions());
    p_albedoTextureInput->EventEmitter<IEventsValueChanged>::RegisterListener(
        this);

    p_albedoUvMultiplyInput = GameObject::Create<UIInputVector>();
    p_albedoUvMultiplyInput->SetSize(2);
    p_albedoUvMultiplyInput
        ->EventEmitter<IEventsValueChanged>::RegisterListener(this);

    p_albedoUvOffsetInput = GameObject::Create<UIInputVector>();
    p_albedoUvOffsetInput->SetSize(2);
    p_albedoUvOffsetInput->EventEmitter<IEventsValueChanged>::RegisterListener(
        this);

    p_roughnessTextureInput = GameObject::Create<UIInputTexture>();
    p_roughnessTextureInput->SetExtensions(Extensions::GetImageExtensions());
    p_roughnessTextureInput
        ->EventEmitter<IEventsValueChanged>::RegisterListener(this);

    p_metalnessTextureInput = GameObject::Create<UIInputTexture>();
    p_metalnessTextureInput->SetExtensions(Extensions::GetImageExtensions());
    p_metalnessTextureInput
        ->EventEmitter<IEventsValueChanged>::RegisterListener(this);

    p_normalMapTextureInput = GameObject::Create<UIInputTexture>();
    p_normalMapTextureInput->SetExtensions(Extensions::GetImageExtensions());
    p_normalMapTextureInput
        ->EventEmitter<IEventsValueChanged>::RegisterListener(this);

    p_normalMapUvMultiplyInput = GameObject::Create<UIInputVector>();
    p_normalMapUvMultiplyInput->SetSize(2);
    p_normalMapUvMultiplyInput
        ->EventEmitter<IEventsValueChanged>::RegisterListener(this);

    p_normalMapUvOffsetInput = GameObject::Create<UIInputVector>();
    p_normalMapUvOffsetInput->SetSize(2);
    p_normalMapUvOffsetInput
        ->EventEmitter<IEventsValueChanged>::RegisterListener(this);

    p_normalMapMultiplyFactorInput = GameObjectFactory::CreateUIInputNumber();
    p_normalMapMultiplyFactorInput
        ->EventEmitter<IEventsValueChanged>::RegisterListener(this);

    p_albedoColorInput = GameObject::Create<UIInputColor>();
    p_albedoColorInput->EventEmitter<IEventsValueChanged>::RegisterListener(
        this);

    p_receivesLightingCheckBox = GameObjectFactory::CreateUICheckBox();
    p_receivesLightingCheckBox
        ->EventEmitter<IEventsValueChanged>::RegisterListener(this);

    p_roughnessSlider = GameObjectFactory::CreateUISlider();
    p_roughnessSlider->SetMinMaxValues(0.0f, 1.0f);
    p_roughnessSlider->EventEmitter<IEventsValueChanged>::RegisterListener(
        this);

    p_metalnessSlider = GameObjectFactory::CreateUISlider();
    p_metalnessSlider->SetMinMaxValues(0.0f, 1.0f);
    p_metalnessSlider->EventEmitter<IEventsValueChanged>::RegisterListener(
        this);

    p_renderPassInput = GameObjectFactory::CreateUIComboBox();
    p_renderPassInput->AddItem("Scene", SCAST<int>(RenderPass::SCENE));
    p_renderPassInput->AddItem("Transparent",
                               SCAST<int>(RenderPass::SCENE_TRANSPARENT));
    p_renderPassInput->AddItem("Canvas", SCAST<int>(RenderPass::CANVAS));
    p_renderPassInput->AddItem("Overlay", SCAST<int>(RenderPass::OVERLAY));
    p_renderPassInput->AddItem("ScenePostProcess",
                               SCAST<int>(RenderPass::SCENE_POSTPROCESS));
    p_renderPassInput->AddItem("CanvasPostProcess",
                               SCAST<int>(RenderPass::CANVAS_POSTPROCESS));
    p_renderPassInput->AddItem("OverlayPostProcess",
                               SCAST<int>(RenderPass::OVERLAY_POSTPROCESS));
    p_renderPassInput->EventEmitter<IEventsValueChanged>::RegisterListener(
        this);

    p_neededUniformsInput = GameObjectFactory::CreateUIBoolComboBox();
    p_neededUniformsInput->AddItem("Model",
                                   SCAST<int>(NeededUniformFlag::MODEL));
    p_neededUniformsInput->AddItem("ModelInv",
                                   SCAST<int>(NeededUniformFlag::MODEL_INV));
    p_neededUniformsInput->AddItem("Normal",
                                   SCAST<int>(NeededUniformFlag::NORMAL));
    p_neededUniformsInput->AddItem("PVM", SCAST<int>(NeededUniformFlag::PVM));
    p_neededUniformsInput->AddItem("PVMInv",
                                   SCAST<int>(NeededUniformFlag::PVM_INV));
    p_neededUniformsInput->AddItem("Skyboxes",
                                   SCAST<int>(NeededUniformFlag::SKYBOXES));
    p_neededUniformsInput->AddItem(
        "MaterialAlbedo", SCAST<int>(NeededUniformFlag::MATERIAL_ALBEDO));
    p_neededUniformsInput->AddItem("MaterialPBR",
                                   SCAST<int>(NeededUniformFlag::MATERIAL_PBR));
    p_neededUniformsInput->AddItem("Time", SCAST<int>(NeededUniformFlag::TIME));
    p_neededUniformsInput->EventEmitter<IEventsValueChanged>::RegisterListener(
        this);

    p_vertexShaderInput = GameObject::Create<UIInputFileWithPreview>();
    p_vertexShaderInput->SetExtensions(Extensions::GetVertexShaderExtensions());
    p_vertexShaderInput->EventEmitter<IEventsValueChanged>::RegisterListener(
        this);
    p_vertexShaderInput->SetZoomable(false);

    p_fragmentShaderInput = GameObject::Create<UIInputFileWithPreview>();
    p_fragmentShaderInput->SetExtensions(
        Extensions::GetFragmentShaderExtensions());
    p_fragmentShaderInput->EventEmitter<IEventsValueChanged>::RegisterListener(
        this);
    p_fragmentShaderInput->SetZoomable(false);

    p_lineWidthInput = GameObjectFactory::CreateUIInputNumber();
    p_lineWidthInput->SetMinValue(1.0f);
    p_lineWidthInput->EventEmitter<IEventsValueChanged>::RegisterListener(this);

    p_cullFaceInput = GameObjectFactory::CreateUIComboBox();
    p_cullFaceInput->AddItem("None", SCAST<int>(GL::CullFaceExt::NONE));
    p_cullFaceInput->AddItem("Back", SCAST<int>(GL::CullFaceExt::BACK));
    p_cullFaceInput->AddItem("Front", SCAST<int>(GL::CullFaceExt::FRONT));
    p_cullFaceInput->AddItem("FrontAndBack",
                             SCAST<int>(GL::CullFaceExt::FRONT_AND_BACK));
    p_cullFaceInput->EventEmitter<IEventsValueChanged>::RegisterListener(this);

    p_renderWireframe = GameObjectFactory::CreateUICheckBox();
    p_renderWireframe->EventEmitter<IEventsValueChanged>::RegisterListener(
        this);

    p_materialPreviewViewer = GameObject::Create<PreviewViewer>();

    AddWidget(p_materialPreviewViewer, 256);
    AddWidget(GameObjectFactory::CreateUIHSeparator(), 10);
    AddWidget("Rec. light", p_receivesLightingCheckBox->GetGameObject());
    AddWidget("Roughness", p_roughnessSlider->GetGameObject());
    AddWidget("Roughness Tex.", p_roughnessTextureInput);
    AddWidget("Metalness", p_metalnessSlider->GetGameObject());
    AddWidget("Metalness Tex.", p_metalnessTextureInput);
    AddWidget(GameObjectFactory::CreateUIHSeparator(), 10);
    AddWidget("Albedo Color", p_albedoColorInput);
    AddWidget("Albedo Tex.", p_albedoTextureInput);
    AddWidget("Albedo Uv Offset", p_albedoUvOffsetInput);
    AddWidget("Albedo Uv Mult.", p_albedoUvMultiplyInput);
    AddWidget(GameObjectFactory::CreateUIHSeparator(), 10);
    AddWidget("Normal Map Tex.", p_normalMapTextureInput);
    AddWidget("Normal Map Uv Offset", p_normalMapUvOffsetInput);
    AddWidget("Normal Map Uv Mult.", p_normalMapUvMultiplyInput);
    AddWidget("Normal Map Factor",
              p_normalMapMultiplyFactorInput->GetGameObject());
    AddWidget(GameObjectFactory::CreateUIHSeparator(), 10);
    AddWidget("Cull Face", p_cullFaceInput->GetGameObject());
    AddWidget("Render Wireframe", p_renderWireframe->GetGameObject());
    AddWidget("Line Width", p_lineWidthInput->GetGameObject());
    AddWidget(GameObjectFactory::CreateUIHSeparator(), 10);
    AddWidget("Render pass", p_renderPassInput->GetGameObject());
    AddWidget("Needed uniforms", p_neededUniformsInput->GetGameObject());
    AddWidget("Vert shader", p_vertexShaderInput);
    AddWidget("Frag shader", p_fragmentShaderInput);
    AddWidget(GameObjectFactory::CreateUIHSeparator(), 10);

    SetLabelsWidth(130);
}

Material *RIWMaterial::GetMaterial() const
{
    return SCAST<Material *>(GetResource().Get());
}

void RIWMaterial::UpdateInputsFromResource()
{
    Texture2D *albedoTex = GetMaterial()->GetAlbedoTexture();
    p_albedoTextureInput->SetPath(albedoTex ? albedoTex->GetResourceFilepath()
                                            : Path::Empty);
    p_albedoUvOffsetInput->Set(GetMaterial()->GetAlbedoUvOffset());
    p_albedoUvMultiplyInput->Set(GetMaterial()->GetAlbedoUvMultiply());

    Texture2D *roughnessTex = GetMaterial()->GetRoughnessTexture();
    p_roughnessTextureInput->SetPath(
        roughnessTex ? roughnessTex->GetResourceFilepath() : Path::Empty);

    Texture2D *metalnessTex = GetMaterial()->GetMetalnessTexture();
    p_metalnessTextureInput->SetPath(
        metalnessTex ? metalnessTex->GetResourceFilepath() : Path::Empty);

    Texture2D *normalMapTex = GetMaterial()->GetNormalMapTexture();
    p_normalMapTextureInput->SetPath(
        normalMapTex ? normalMapTex->GetResourceFilepath() : Path::Empty);
    p_normalMapUvOffsetInput->Set(GetMaterial()->GetNormalMapUvOffset());
    p_normalMapUvMultiplyInput->Set(GetMaterial()->GetNormalMapUvMultiply());
    p_normalMapMultiplyFactorInput->SetValue(
        GetMaterial()->GetNormalMapMultiplyFactor());

    p_albedoColorInput->SetColor(GetMaterial()->GetAlbedoColor());
    p_receivesLightingCheckBox->SetChecked(
        GetMaterial()->GetReceivesLighting());
    p_roughnessSlider->SetValue(GetMaterial()->GetRoughness());
    p_metalnessSlider->SetValue(GetMaterial()->GetMetalness());
    p_renderPassInput->SetSelectionByValue(
        SCAST<int>(GetMaterial()->GetRenderPass()));

    p_renderWireframe->SetChecked(GetMaterial()->IsRenderWireframe());
    p_cullFaceInput->SetSelectionByValue(
        SCAST<int>(GetMaterial()->GetCullFace()));
    p_lineWidthInput->SetValue(GetMaterial()->GetLineWidth());

    p_materialPreviewViewer->SetPreviewImageProvider(
        [this](const ResourcePreviewFactoryParameters &params) {
            return MaterialPreviewFactory::GetPreviewTextureFor(GetMaterial(),
                                                                params);
        });

    p_neededUniformsInput->SetSelectionForFlag(
        SCAST<int>(GetMaterial()->GetNeededUniforms().GetValue()));
    ShaderProgram *sp = GetMaterial()->GetShaderProgram();
    Shader *vs = sp ? sp->GetVertexShader() : nullptr;
    Shader *fs = sp ? sp->GetFragmentShader() : nullptr;
    p_vertexShaderInput->SetPath(vs ? vs->GetResourceFilepath() : Path::Empty);
    p_fragmentShaderInput->SetPath(fs ? fs->GetResourceFilepath()
                                      : Path::Empty);
}

Texture2D *RIWMaterial::GetIconTexture() const
{
    return MaterialPreviewFactory::GetPreviewTextureFor(GetMaterial()).Get();
}

void RIWMaterial::OnValueChangedRIWResource(
    EventEmitter<IEventsValueChanged> *obj)
{
    if(!GetMaterial())
    {
        return;
    }

    Path albedoTexPath = p_albedoTextureInput->GetPath();
    if(albedoTexPath.IsFile())
    {
        RH<Texture2D> tex = Resources::Load<Texture2D>(albedoTexPath);
        GetMaterial()->SetAlbedoTexture(tex.Get());
    }
    else
    {
        GetMaterial()->SetAlbedoTexture(nullptr);
    }

    Path roughnessTexPath = p_roughnessTextureInput->GetPath();
    if(roughnessTexPath.IsFile())
    {
        RH<Texture2D> tex = Resources::Load<Texture2D>(roughnessTexPath);
        GetMaterial()->SetRoughnessTexture(tex.Get());
    }
    else
    {
        GetMaterial()->SetRoughnessTexture(nullptr);
    }

    Path metalnessTexPath = p_metalnessTextureInput->GetPath();
    if(metalnessTexPath.IsFile())
    {
        RH<Texture2D> tex = Resources::Load<Texture2D>(metalnessTexPath);
        GetMaterial()->SetMetalnessTexture(tex.Get());
    }
    else
    {
        GetMaterial()->SetMetalnessTexture(nullptr);
    }

    Path normalMapTexPath = p_normalMapTextureInput->GetPath();
    if(normalMapTexPath.IsFile())
    {
        RH<Texture2D> tex = Resources::Load<Texture2D>(normalMapTexPath);
        GetMaterial()->SetNormalMapTexture(tex.Get());
    }
    else
    {
        GetMaterial()->SetNormalMapTexture(nullptr);
    }

    GetMaterial()->SetAlbedoColor(p_albedoColorInput->GetColor());
    GetMaterial()->SetAlbedoUvOffset(p_albedoUvOffsetInput->GetVector2());
    GetMaterial()->SetAlbedoUvMultiply(p_albedoUvMultiplyInput->GetVector2());
    GetMaterial()->SetNormalMapUvOffset(p_normalMapUvOffsetInput->GetVector2());
    GetMaterial()->SetNormalMapUvMultiply(
        p_normalMapUvMultiplyInput->GetVector2());
    GetMaterial()->SetNormalMapMultiplyFactor(
        p_normalMapMultiplyFactorInput->GetValue());
    GetMaterial()->SetReceivesLighting(p_receivesLightingCheckBox->IsChecked());
    GetMaterial()->SetRoughness(p_roughnessSlider->GetValue());
    GetMaterial()->SetMetalness(p_metalnessSlider->GetValue());
    GetMaterial()->GetNeededUniforms().SetTo(SCAST<FlagsPrimitiveType>(
        p_neededUniformsInput->GetSelectedValuesForFlag()));

    if(obj == p_renderPassInput)
    {
        // Change renderPass, and shaders if the user has not changed the
        // material default shaders (if he did, auto changing this is annoying).
        RenderPass rp =
            SCAST<RenderPass>(p_renderPassInput->GetSelectedValue());
        if(GetMaterial()->GetShaderProgram() ==
           ShaderProgramFactory::GetDefault(GetMaterial()->GetRenderPass()))
        {
            GetMaterial()->SetShaderProgram(
                ShaderProgramFactory::GetDefault(rp));
        }
        GetMaterial()->SetRenderPass(rp);
    }

    GetMaterial()->SetRenderWireframe(p_renderWireframe->IsChecked());
    GetMaterial()->SetCullFace(
        SCAST<GL::CullFaceExt>(p_cullFaceInput->GetSelectedValue()));
    GetMaterial()->SetLineWidth(p_lineWidthInput->GetValue());

    Path vsPath = p_vertexShaderInput->GetPath();
    Path fsPath = p_fragmentShaderInput->GetPath();
    GetMaterial()->SetShaderProgram(ShaderProgramFactory::Get(vsPath, fsPath));
}

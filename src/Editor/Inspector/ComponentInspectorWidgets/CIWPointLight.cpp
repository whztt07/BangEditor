#include "BangEditor/CIWPointLight.h"

#include "Bang/PointLight.h"
#include "Bang/UIInputNumber.h"
#include "Bang/TextureCubeMap.h"
#include "Bang/GameObjectFactory.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

void CIWPointLight::InitInnerWidgets()
{
    CIWLight::InitInnerWidgets();

    p_rangeInput = GameObjectFactory::CreateUIInputNumber();
    p_rangeInput->EventEmitter<IValueChangedListener>::RegisterListener(this);
    p_rangeInput->SetMinMaxValues(0.0f, Math::Infinity<float>());
    AddWidget("Range", p_rangeInput->GetGameObject());
}

void CIWPointLight::UpdateFromReference()
{
    CIWLight::UpdateFromReference();

    if (!p_rangeInput->HasFocus())
    {
        p_rangeInput->SetValue( GetPointLight()->GetRange() );
    }

    /*
    TextureCubeMap *shadowMapTex = DCAST<TextureCubeMap*>(GetLight()->GetShadowMapTexture());
    if (shadowMapTex)
    {
        shadowMapTex->ToImage(GL::CubeMapDir::Front).Export( Path("test.png") );
    }
    */
}

PointLight *CIWPointLight::GetPointLight() const
{
    return SCAST<PointLight*>( GetLight() );
}

void CIWPointLight::OnValueChanged(Object *object)
{
    CIWLight::OnValueChanged(object);

    GetPointLight()->SetRange( p_rangeInput->GetValue() );
}
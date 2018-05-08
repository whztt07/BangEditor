#include "UI.frag"

uniform int B_SceneRenderMode;
uniform sampler2D B_SelectionTex;
uniform sampler2D B_SceneDepthStencilTex;

const int COLOR            = 0;
const int NORMAL           = 1;
const int ALBEDO           = 2;
const int DEPTH            = 3;
const int ROUGHNESS        = 4;
const int METALNESS        = 5;
const int RECEIVES_LIGHT   = 6;
const int RECEIVES_SHADOWS = 7;
const int SELECTION        = 8;
const int WORLD_POSITION   = 9;

void main()
{
    vec2 uv = B_FIn_AlbedoUv;
    vec3 color = vec3(1,0,1);
    switch (B_SceneRenderMode)
    {
        case COLOR:            color = B_SampleColor(uv).rgb;               break;
        case NORMAL:           color = B_SampleNormal(uv).rgb;              break;
        case ALBEDO:           color = B_SampleAlbedoColor(uv).rgb;         break;
        case WORLD_POSITION:   color = B_ComputeWorldPosition();            break;
        case ROUGHNESS:        color = vec3( B_SampleRoughness(uv) );       break;
        case METALNESS:        color = vec3( B_SampleMetalness(uv) );       break;
        case RECEIVES_LIGHT:   color = vec3( B_SampleReceivesLight(uv) );   break;
        case RECEIVES_SHADOWS: color = vec3( B_SampleReceivesShadows(uv) ); break;
        case SELECTION:        color = texture(B_SelectionTex, uv).rgb;     break;

        case DEPTH: color = vec3( texture(B_SceneDepthStencilTex, uv).r );  break;
    }

    B_GIn_Color = vec4(color, 1);
}
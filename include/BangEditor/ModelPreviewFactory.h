#ifndef MODELPREVIEWFACTORY_H
#define MODELPREVIEWFACTORY_H

#include "Bang/Bang.h"
#include "Bang/Model.h"

#include "BangEditor/BangEditor.h"
#include "BangEditor/ResourcePreviewFactory.h"

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class ModelPreviewFactory : public ResourcePreviewFactory<Model>
{
public:
	ModelPreviewFactory();
	virtual ~ModelPreviewFactory();

    static RH<Texture2D> GetPreviewTextureFor(Model *model);
    static RH<Texture2D> GetPreviewTextureFor(
                                Model *model,
                                const ResourcePreviewFactoryParameters &params);
    static ModelPreviewFactory *GetActive();

private:
   // ResourcePreviewFactory
   void OnCreateSceneFirstTime(Scene *previewScene,
                               Camera *previewCamera,
                               GameObject *previewGoContainer) override;
   void OnUpdateTextureBegin(
                Scene *previewScene,
                Camera *previewCamera,
                GameObject *previewGoContainer,
                Model *model,
                const ResourcePreviewFactoryParameters &params) override;
   void OnUpdateTextureEnd(
                Scene *previewScene,
                Camera *previewCamera,
                GameObject *previewGoContainer,
                Model *model,
                const ResourcePreviewFactoryParameters &params) override;
};

NAMESPACE_BANG_EDITOR_END

#endif // MODELPREVIEWFACTORY_H


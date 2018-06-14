#include "BangEditor/ExplorerItemFactory.h"

#include "Bang/Mesh.h"
#include "Bang/Path.h"
#include "Bang/Model.h"
#include "Bang/Material.h"
#include "Bang/Resources.h"
#include "Bang/Extensions.h"
#include "Bang/GameObject.h"
#include "Bang/ResourceHandle.h"

#include "BangEditor/ExplorerItem.h"
#include "BangEditor/ModelExplorerItem.h"
#include "BangEditor/PrefabExplorerItem.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

ExplorerItem *ExplorerItemFactory::CreateExplorerItem(const Path &path)
{
    ExplorerItem *explorerItem = nullptr;
    if (path.HasExtension(Extensions::GetModelExtensions()))
    {
        explorerItem = GameObject::Create<ModelExplorerItem>();
    }
    else if (path.HasExtension(Extensions::GetPrefabExtension()))
    {
        explorerItem = GameObject::Create<PrefabExplorerItem>();
    }
    else
    {
        explorerItem = GameObject::Create<ExplorerItem>();
    }

    explorerItem->SetPath(path);

    return explorerItem;
}

List<ExplorerItem *> ExplorerItemFactory::CreateAndGetSubPathsExplorerItems(
                                                const Path &path,
                                                bool addBackItem)
{
    List<ExplorerItem*> expItems;

    if (addBackItem)
    {
        Path prevDirPath = path.GetDirectory();
        ExplorerItem *prevDirExpItem =
                        ExplorerItemFactory::CreateExplorerItem(prevDirPath);
        prevDirExpItem->SetPathString("..");
        expItems.PushBack(prevDirExpItem);
    }

    if (path.IsDir())
    {
        List<Path> subPaths = path.GetSubPaths(Path::FindFlag::SIMPLE);
        Paths::SortPathsByExtension(&subPaths);
        Paths::SortPathsByName(&subPaths);
        for (const Path &subPath : subPaths)
        {
            ExplorerItem *childExpItem =
                            ExplorerItemFactory::CreateExplorerItem(subPath);
            expItems.PushBack(childExpItem);
        }
    }
    else if (path.IsFile())
    {
        if (path.HasExtension(Extensions::GetModelExtensions()))
        {
            RH<Model> model = Resources::Load<Model>(path);
            Resources::SetPermanent(model.Get(), true);

            for (const String& meshName : model.Get()->GetMeshesNames())
            {
                Path meshPath = path.Append(meshName);
                ExplorerItem *meshExplorerItem =
                        ExplorerItemFactory::CreateExplorerItem(meshPath);
                expItems.PushBack( meshExplorerItem );
            }

            for (const String& materialName : model.Get()->GetMaterialsNames())
            {
                Path materialPath = path.Append(materialName);
                ExplorerItem *materialExplorerItem =
                        ExplorerItemFactory::CreateExplorerItem(materialPath);
                expItems.PushBack( materialExplorerItem );
            }
        }
    }

    return expItems;
}

bool ExplorerItemFactory::CanHaveSubpaths(const Path &path)
{
    return path.IsDir() ||
           path.HasExtension(Extensions::GetModelExtensions());
}

ExplorerItemFactory::ExplorerItemFactory()
{
}

ExplorerItemFactory::~ExplorerItemFactory()
{
}


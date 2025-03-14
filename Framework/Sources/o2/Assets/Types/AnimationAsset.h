#pragma once

#include "o2/Animation/AnimationClip.h"
#include "o2/Assets/Asset.h"
#include "o2/Assets/AssetRef.h"
#include "o2/Utils/Editor/Attributes/ExpandedByDefaultAttribute.h"

namespace o2
{
	// ---------------
	// Animation asset
	// ---------------
	class AnimationAsset: public AssetWithDefaultMeta<AnimationAsset>
	{
	public:
		AnimationClip animation; // Asset data @SERIALIZABLE @EXPANDED_BY_DEFAULT

	public:
		// Default constructor
		AnimationAsset() = default;

		// Copy-constructor
		AnimationAsset(const AnimationAsset& asset);

		// Constructor with animation clip
		AnimationAsset(const AnimationClip& clip);

		// Check equals operator
		AnimationAsset& operator=(const AnimationAsset& asset);

		// Returns extensions string
		static const char* GetFileExtensions();

		// Returns editor icon
		static String GetEditorIcon() { return "ui/UI4_anim_file_icon.png"; }

		// Returns editor sorting weight
		static int GetEditorSorting() { return 98; }

		// Is this asset type is available to create from editor's assets window
		static bool IsAvailableToCreateFromEditor() { return true; }

		// Is asset reference available to contain instance inside
		static bool IsReferenceCanOwnInstance() { return true; }

		SERIALIZABLE(AnimationAsset);

		friend class Assets;
	};

	typedef Ref<AnimationAsset> AnimationAssetRef;
}

CLASS_BASES_META(o2::AnimationAsset)
{
	BASE_CLASS(o2::AssetWithDefaultMeta<AnimationAsset>);
}
END_META;
CLASS_FIELDS_META(o2::AnimationAsset)
{
	FIELD().EXPANDED_BY_DEFAULT_ATTRIBUTE().SERIALIZABLE_ATTRIBUTE().NAME(animation).PUBLIC();
}
END_META;
CLASS_METHODS_META(o2::AnimationAsset)
{

	PUBLIC_STATIC_FUNCTION(const char*, GetFileExtensions);
	PUBLIC_STATIC_FUNCTION(String, GetEditorIcon);
	PUBLIC_STATIC_FUNCTION(int, GetEditorSorting);
	PUBLIC_STATIC_FUNCTION(bool, IsAvailableToCreateFromEditor);
	PUBLIC_STATIC_FUNCTION(bool, IsReferenceCanOwnInstance);
}
END_META;

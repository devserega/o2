#pragma once

#include "o2/Assets/Types/ImageAsset.h"
#include "o2/Scene/UI/Widget.h"

namespace o2
{
	class Sprite;

	// ------------
	// Image widget
	// ------------
	class Image: public Widget
	{
	public:
		PROPERTIES(Image);
		PROPERTY(Sprite*, image, SetImage, GetImage);                      // Image sprite
		PROPERTY(ImageAssetRef, imageAsset, SetImageAsset, GetImageAsset); // Image asset
		PROPERTY(String, imageName, SetImageName, GetImageName);           // Image asset name

	public:
		// Default constructor
		Image();

		// Copy-constructor
		Image(const Image& other);

		// Assign operator
		Image& operator=(const Image& other);

		// Sets image
		void SetImage(Sprite* sprite);

		// Returns image sprite
		Sprite* GetImage();

		// Sets image asset
		void SetImageAsset(const ImageAssetRef& asset);

		// Returns image asset
		ImageAssetRef GetImageAsset() const;

		// Sets image asset name
		void SetImageName(const String& name);

		// Returns asset image name
		String GetImageName() const;

		// Returns create menu group in editor
		static String GetCreateMenuGroup();

		SERIALIZABLE(Image);

	protected:
		Sprite* mImage = nullptr; // Image layer drawable

		friend class UIButtonGroup;
	};
}

CLASS_BASES_META(o2::Image)
{
	BASE_CLASS(o2::Widget);
}
END_META;
CLASS_FIELDS_META(o2::Image)
{
	FIELD().NAME(image).PUBLIC();
	FIELD().NAME(imageAsset).PUBLIC();
	FIELD().NAME(imageName).PUBLIC();
	FIELD().DEFAULT_VALUE(nullptr).NAME(mImage).PROTECTED();
}
END_META;
CLASS_METHODS_META(o2::Image)
{

	PUBLIC_FUNCTION(void, SetImage, Sprite*);
	PUBLIC_FUNCTION(Sprite*, GetImage);
	PUBLIC_FUNCTION(void, SetImageAsset, const ImageAssetRef&);
	PUBLIC_FUNCTION(ImageAssetRef, GetImageAsset);
	PUBLIC_FUNCTION(void, SetImageName, const String&);
	PUBLIC_FUNCTION(String, GetImageName);
	PUBLIC_STATIC_FUNCTION(String, GetCreateMenuGroup);
}
END_META;

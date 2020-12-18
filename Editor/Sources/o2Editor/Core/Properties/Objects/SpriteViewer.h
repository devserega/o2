#pragma once

#include "o2/Assets/Types/ImageAsset.h"
#include "o2/Render/Sprite.h"
#include "o2Editor/Core/Properties/IObjectPropertiesViewer.h"

using namespace o2;

namespace o2
{
	class Sprite;
	class Spoiler;
}

namespace Editor
{
	class AssetProperty;
	class BorderIProperty;
	class ColorProperty;
	class EnumProperty;
	class FloatProperty;
	class ImageSlicesEditorWidget;
	class Vec2FProperty;

	// --------------------
	// Editor sprite viewer
	// --------------------
	class SpriteViewer : public TObjectPropertiesViewer<Sprite>
	{
	public:
		IOBJECT(SpriteViewer);

	protected:
		VerticalLayout* mHiddenProperties = nullptr; // Hidden properties

		AssetProperty* mImageProperty = nullptr;
		ColorProperty* mColorProperty = nullptr;
		FloatProperty* mAlphaProperty = nullptr;
		Vec2FProperty* mSizePivotProperty = nullptr;
		Vec2FProperty* mPivotProperty = nullptr;
		EnumProperty*  mModeProperty = nullptr;

		Spoiler*       mFillPropertiesSpoiler = nullptr;
		FloatProperty* mFillProperty = nullptr;

		Spoiler*         mSlicedPropertiesSpoiler = nullptr;
		BorderIProperty* mSliceBorderProperty = nullptr;

		Spoiler*       mTiledPropertiesSpoiler = nullptr;
		FloatProperty* mTileScaleProperty = nullptr;

		ImageSlicesEditorWidget* mSlicesEditor = nullptr;

	protected:
		// It is called when the viewer is refreshed, builds properties, and places them in mPropertiesContext
		void RebuildProperties(const Vector<Pair<IObject*, IObject*>>& targetObjets) override;

		// It is called when viewer is refreshed
		void OnRefreshed(const Vector<Pair<IObject*, IObject*>>& targetObjets) override;

		// It is called when type enum selected, shows required property spoiler
		void OnModeSelected();
	};
}

CLASS_BASES_META(Editor::SpriteViewer)
{
	BASE_CLASS(Editor::TObjectPropertiesViewer<Sprite>);
}
END_META;
CLASS_FIELDS_META(Editor::SpriteViewer)
{
	FIELD().DEFAULT_VALUE(nullptr).NAME(mHiddenProperties).PROTECTED();
	FIELD().DEFAULT_VALUE(nullptr).NAME(mImageProperty).PROTECTED();
	FIELD().DEFAULT_VALUE(nullptr).NAME(mColorProperty).PROTECTED();
	FIELD().DEFAULT_VALUE(nullptr).NAME(mAlphaProperty).PROTECTED();
	FIELD().DEFAULT_VALUE(nullptr).NAME(mSizePivotProperty).PROTECTED();
	FIELD().DEFAULT_VALUE(nullptr).NAME(mPivotProperty).PROTECTED();
	FIELD().DEFAULT_VALUE(nullptr).NAME(mModeProperty).PROTECTED();
	FIELD().DEFAULT_VALUE(nullptr).NAME(mFillPropertiesSpoiler).PROTECTED();
	FIELD().DEFAULT_VALUE(nullptr).NAME(mFillProperty).PROTECTED();
	FIELD().DEFAULT_VALUE(nullptr).NAME(mSlicedPropertiesSpoiler).PROTECTED();
	FIELD().DEFAULT_VALUE(nullptr).NAME(mSliceBorderProperty).PROTECTED();
	FIELD().DEFAULT_VALUE(nullptr).NAME(mTiledPropertiesSpoiler).PROTECTED();
	FIELD().DEFAULT_VALUE(nullptr).NAME(mTileScaleProperty).PROTECTED();
	FIELD().DEFAULT_VALUE(nullptr).NAME(mSlicesEditor).PROTECTED();
}
END_META;
CLASS_METHODS_META(Editor::SpriteViewer)
{

	typedef const Vector<Pair<IObject*, IObject*>>& _tmp1;
	typedef const Vector<Pair<IObject*, IObject*>>& _tmp2;

	PROTECTED_FUNCTION(void, RebuildProperties, _tmp1);
	PROTECTED_FUNCTION(void, OnRefreshed, _tmp2);
	PROTECTED_FUNCTION(void, OnModeSelected);
}
END_META;

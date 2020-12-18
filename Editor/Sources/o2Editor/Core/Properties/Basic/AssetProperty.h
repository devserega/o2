#pragma once

#include "o2/Events/KeyboardEventsListener.h"
#include "o2/Render/Text.h"
#include "o2/Scene/UI/Widget.h"
#include "o2/Utils/Editor/DragAndDrop.h"
#include "o2/Utils/FileSystem/FileSystem.h"
#include "o2Editor/AssetsWindow/AssetsIconsScroll.h"
#include "o2Editor/AssetsWindow/AssetsWindow.h"
#include "o2Editor/Core/Properties/IPropertyField.h"

using namespace o2;

namespace Editor
{
	class ObjectViewer;

	// -------------------------
	// Editor asset property box
	// -------------------------
	class AssetProperty : public TPropertyField<AssetRef>, public DragDropArea, public KeyboardEventsListener
	{
	public:
		// Default constructor
		AssetProperty();

		// Copy constructor
		AssetProperty(const AssetProperty& other);

		// Copy operator
		AssetProperty& operator=(const AssetProperty& other);

		// Sets value asset id
		void SetAssetId(const UID& id);

		// Sets asset type
		void SetAssetType(const Type* assetType);

		// Specializes field info
		void SetFieldInfo(const FieldInfo* fieldInfo) override;

		// Sets property caption
		void SetCaption(const WString& text) override;

		// Returns property caption
		WString GetCaption() const override;

		// Adds remove button
		Button* GetRemoveButton() override;

		// Returns true if point is in this object
		bool IsUnderPoint(const Vec2F& point) override;

		IOBJECT(AssetProperty);

	protected:
		Widget*  mBox = nullptr;               // Property edit box
		Text*    mNameText = nullptr;          // Asset name text
		Spoiler* mSpoiler = nullptr;           // Spoiler
		Label*   mCaption = nullptr;           // Property name caption
		Button*  mCreateInstanceBtn = nullptr; // Create instance button

		HorizontalLayout* mHeaderContainer = nullptr; // Asset controls container: create, save and remove

		ObjectViewer* mAssetObjectViewer = nullptr; // Asset instance viewer. Created if required

		const Type* mAssetType = nullptr; // Type of asset

		bool mAvailableToHaveInstance = false; // Is asset can own an instance

	protected:
		// Searches controls widgets and layers and initializes them
		void InitializeControls();

		// Sets common value asset id
		void SetCommonAssetId(const UID& id);

		// Sets asset id, checks value changed, calls onChangeCompleted
		void SetAssetIdByUser(const UID& id);

		// It is called when create instance button pressed, creates asset instance
		void OnCreateInstancePressed();

		// It is called when remove instance button pressed, removes asset instance
		void OnRemoveInstancePressed();

		// It is called when save instance button pressed, saves asset instance as asset
		void OnSaveInstancePressed();

		// It is called when type specialized during setting value proxy
		void OnTypeSpecialized(const Type& type) override;

		// Returns value from proxy
		AssetRef GetProxy(IAbstractValueProxy* proxy) const override;

		// Sets value to proxy
		void SetProxy(IAbstractValueProxy* proxy, const AssetRef& value) override;

		// Updates value view
		void UpdateValueView() override;

		// Is required refresh view every time
		bool IsAlwaysRefresh() const override;

		// It is called when cursor enters this object
		void OnCursorEnter(const Input::Cursor& cursor) override;

		// It is called when cursor exits this object
		void OnCursorExit(const Input::Cursor& cursor) override;

		// It is called when cursor pressed on this
		void OnCursorPressed(const Input::Cursor& cursor) override;

		// It is called when key was pressed
		void OnKeyPressed(const Input::Key& key) override;

		// It is called when some selectable listeners was dropped to this
		void OnDropped(ISelectableDragableObjectsGroup* group) override;

		// It is called when some drag listeners was entered to this area
		void OnDragEnter(ISelectableDragableObjectsGroup* group) override;

		// It is called when some drag listeners was exited from this area
		void OnDragExit(ISelectableDragableObjectsGroup* group) override;
	};
}

CLASS_BASES_META(Editor::AssetProperty)
{
	BASE_CLASS(Editor::TPropertyField<AssetRef>);
	BASE_CLASS(o2::DragDropArea);
	BASE_CLASS(o2::KeyboardEventsListener);
}
END_META;
CLASS_FIELDS_META(Editor::AssetProperty)
{
	FIELD().DEFAULT_VALUE(nullptr).NAME(mBox).PROTECTED();
	FIELD().DEFAULT_VALUE(nullptr).NAME(mNameText).PROTECTED();
	FIELD().DEFAULT_VALUE(nullptr).NAME(mSpoiler).PROTECTED();
	FIELD().DEFAULT_VALUE(nullptr).NAME(mCaption).PROTECTED();
	FIELD().DEFAULT_VALUE(nullptr).NAME(mCreateInstanceBtn).PROTECTED();
	FIELD().DEFAULT_VALUE(nullptr).NAME(mHeaderContainer).PROTECTED();
	FIELD().DEFAULT_VALUE(nullptr).NAME(mAssetObjectViewer).PROTECTED();
	FIELD().DEFAULT_VALUE(nullptr).NAME(mAssetType).PROTECTED();
	FIELD().DEFAULT_VALUE(false).NAME(mAvailableToHaveInstance).PROTECTED();
}
END_META;
CLASS_METHODS_META(Editor::AssetProperty)
{

	PUBLIC_FUNCTION(void, SetAssetId, const UID&);
	PUBLIC_FUNCTION(void, SetAssetType, const Type*);
	PUBLIC_FUNCTION(void, SetFieldInfo, const FieldInfo*);
	PUBLIC_FUNCTION(void, SetCaption, const WString&);
	PUBLIC_FUNCTION(WString, GetCaption);
	PUBLIC_FUNCTION(Button*, GetRemoveButton);
	PUBLIC_FUNCTION(bool, IsUnderPoint, const Vec2F&);
	PROTECTED_FUNCTION(void, InitializeControls);
	PROTECTED_FUNCTION(void, SetCommonAssetId, const UID&);
	PROTECTED_FUNCTION(void, SetAssetIdByUser, const UID&);
	PROTECTED_FUNCTION(void, OnCreateInstancePressed);
	PROTECTED_FUNCTION(void, OnRemoveInstancePressed);
	PROTECTED_FUNCTION(void, OnSaveInstancePressed);
	PROTECTED_FUNCTION(void, OnTypeSpecialized, const Type&);
	PROTECTED_FUNCTION(AssetRef, GetProxy, IAbstractValueProxy*);
	PROTECTED_FUNCTION(void, SetProxy, IAbstractValueProxy*, const AssetRef&);
	PROTECTED_FUNCTION(void, UpdateValueView);
	PROTECTED_FUNCTION(bool, IsAlwaysRefresh);
	PROTECTED_FUNCTION(void, OnCursorEnter, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorExit, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorPressed, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnKeyPressed, const Input::Key&);
	PROTECTED_FUNCTION(void, OnDropped, ISelectableDragableObjectsGroup*);
	PROTECTED_FUNCTION(void, OnDragEnter, ISelectableDragableObjectsGroup*);
	PROTECTED_FUNCTION(void, OnDragExit, ISelectableDragableObjectsGroup*);
}
END_META;

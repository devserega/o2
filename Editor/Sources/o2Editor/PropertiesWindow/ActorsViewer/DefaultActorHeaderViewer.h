#pragma once

#include "o2/Assets/Types/ActorAsset.h"
#include "o2Editor/PropertiesWindow/ActorsViewer/IActorHeaderViewer.h"

namespace o2
{
	class Toggle;
	class EditBox;
	class Button;
}

namespace Editor
{
	using namespace o2;

	class TagsProperty;
	class LayerProperty;
	class BooleanProperty;
	class StringProperty;
	class AssetProperty;

	// ---------------------------
	// Default actor header viewer
	// ---------------------------
	class DefaultActorHeaderViewer: public IActorHeaderViewer
	{
	public:
		// Default constructor
		DefaultActorHeaderViewer();

		// Virtual destructor
		~DefaultActorHeaderViewer();

		// Sets target actors
		void SetTargetActors(const Vector<Actor*>& actors);

		// Returns data widget
		Widget* GetWidget() const;

		// Updates properties values
		void Refresh();

		IOBJECT(DefaultActorHeaderViewer);

	public: 
		Vector<Actor*> mActors;

		Widget* mDataView = nullptr;

		BooleanProperty* mEnableProperty = nullptr;
		StringProperty*  mNameProperty = nullptr;
		BooleanProperty* mLockProperty = nullptr;
		AssetProperty*   mPrototypeProperty = nullptr;
		TagsProperty*    mTagsProperty = nullptr;
		LayerProperty*   mLayerProperty = nullptr;

		Button* mPrototypeApplyBtn = nullptr;
		Button* mPrototypeRevertBtn = nullptr;
		Button* mPrototypeBreakBtn = nullptr;


	protected:
		// It is called when apply button pressed, and applies changes to prototype
		void OnApplyPrototypePressed();

		// It is called when revert button pressed and reverts actor to prototype
		void OnRevertPrototypePressed();

		// It is called when break prototype buttons pressed and break prototype link
		void OnBreakPrototypePressed();

		// Returns actor applying roots
		Vector<Actor*> GetRootApplyActors();

		// It is called when some property changed, stores action for undo
		void OnPropertyChanged(const String& path, const Vector<DataDocument>& prevValue, 
							   const Vector<DataDocument>& newValue);
	};
}

CLASS_BASES_META(Editor::DefaultActorHeaderViewer)
{
	BASE_CLASS(Editor::IActorHeaderViewer);
}
END_META;
CLASS_FIELDS_META(Editor::DefaultActorHeaderViewer)
{
	FIELD().NAME(mActors).PUBLIC();
	FIELD().DEFAULT_VALUE(nullptr).NAME(mDataView).PUBLIC();
	FIELD().DEFAULT_VALUE(nullptr).NAME(mEnableProperty).PUBLIC();
	FIELD().DEFAULT_VALUE(nullptr).NAME(mNameProperty).PUBLIC();
	FIELD().DEFAULT_VALUE(nullptr).NAME(mLockProperty).PUBLIC();
	FIELD().DEFAULT_VALUE(nullptr).NAME(mPrototypeProperty).PUBLIC();
	FIELD().DEFAULT_VALUE(nullptr).NAME(mTagsProperty).PUBLIC();
	FIELD().DEFAULT_VALUE(nullptr).NAME(mLayerProperty).PUBLIC();
	FIELD().DEFAULT_VALUE(nullptr).NAME(mPrototypeApplyBtn).PUBLIC();
	FIELD().DEFAULT_VALUE(nullptr).NAME(mPrototypeRevertBtn).PUBLIC();
	FIELD().DEFAULT_VALUE(nullptr).NAME(mPrototypeBreakBtn).PUBLIC();
}
END_META;
CLASS_METHODS_META(Editor::DefaultActorHeaderViewer)
{

	PUBLIC_FUNCTION(void, SetTargetActors, const Vector<Actor*>&);
	PUBLIC_FUNCTION(Widget*, GetWidget);
	PUBLIC_FUNCTION(void, Refresh);
	PROTECTED_FUNCTION(void, OnApplyPrototypePressed);
	PROTECTED_FUNCTION(void, OnRevertPrototypePressed);
	PROTECTED_FUNCTION(void, OnBreakPrototypePressed);
	PROTECTED_FUNCTION(Vector<Actor*>, GetRootApplyActors);
	PROTECTED_FUNCTION(void, OnPropertyChanged, const String&, const Vector<DataDocument>&, const Vector<DataDocument>&);
}
END_META;

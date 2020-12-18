#pragma once

#include "o2/Render/Sprite.h"
#include "ScrollArea.h"
#include "o2/Scene/UI/Widgets/VerticalLayout.h"

namespace o2
{
	// ---------------------------------------------
	// List view widget with selection and many data
	// ---------------------------------------------
	class LongList: public ScrollArea
	{

	public:
		PROPERTIES(LongList);
		PROPERTY(int, selectedItemPos, SelectItemAt, GetSelectedItemPosition); // Selected item position property	

	public:
		Function<void(int)> onFocused; // Select item position event

		Function<int()>                   getItemsCountFunc; // Items count getting function
		Function<Vector<void*>(int, int)> getItemsRangeFunc; // Items getting in range function
		Function<void(Widget*, void*)>    setupItemFunc;     // Setup item widget function

	public:
	    // Default constructor
		LongList();

		// Copy-constructor
		LongList(const LongList& other);

		// Destructor
		~LongList();

		// Copy-operator
		LongList& operator=(const LongList& other);

		// Updates drawables, states and widget
		void Update(float dt) override;

		// Draws widget
		void Draw() override;

		// Sets item sample widget. WARNING: Removing all old items!
		void SetItemSample(Widget* sample);

		// Returns item sample widget
		Widget* GetItemSample() const;

		// Selects item at position
		void SelectItemAt(int position);

		// Returns selected item position
		int GetSelectedItemPosition() const;

		// Returns selection drawable
		Sprite* GetSelectionDrawable() const;

		// Returns hover drawable
		Sprite* GetHoverDrawable() const;

		// Sets selection drawable layout (result rectangle will be calculated by item widget absolute rectangle)
		void SetSelectionDrawableLayout(const Layout& layout);

		// Returns selection drawable layout
		Layout GetSelectionDrawableLayout() const;

		// Sets hover drawable layout (result rectangle will be calculated by item widget absolute rectangle)
		void SetHoverDrawableLayout(const Layout& layout);

		// Returns hover drawable layout
		Layout GetHoverDrawableLayout() const;

		// Updates items
		void OnItemsUpdated(bool itemsRearranged = false);

		// Updates layout
		void UpdateSelfTransform() override;

		// Returns create menu group in editor
		static String GetCreateMenuGroup();

		SERIALIZABLE(LongList);

	protected:
		Widget* mItemSample = nullptr; // Item sample widget @SERIALIZABLE

		Sprite* mSelectionDrawable = nullptr;             // Selection sprite @SERIALIZABLE
		Sprite* mHoverDrawable = nullptr;                 // Item hover drawable @SERIALIZABLE
		Layout  mSelectionLayout = Layout::BothStretch(); // Selection layout, result selection area depends on selected item @SERIALIZABLE
		Layout  mHoverLayout = Layout::BothStretch();     // Hover layout, result selection area depends on selected item @SERIALIZABLE
					 						    
		int mMinVisibleItemIdx = -1; // Visible item with minimal index
		int mMaxVisibleItemIdx = -1; // Visible item with maximal index
		int mSelectedItem = -1;      // Position of current selected item (-1 if no item isn't selected)
					 						    
		RectF mCurrentSelectionRect; // Current selection rectangle (for smoothing)
		RectF mTargetSelectionRect;  // Target selection rectangle (over selected item)
		RectF mCurrentHoverRect;     // Current hover rectangle (for smoothing)
		RectF mTargetHoverRect;      // Target hover rectangle (over selected item)
					 						    
		Vec2F mLastHoverCheckCursor;  // Last cursor position on hover check
		Vec2F mLastSelectCheckCursor; // Last cursor position on selection check
					 						    
		Vector<Widget*> mItemsPool; // Items pool

	protected:
		// It is called when object was deserialized and trying to reattach states animations target
		void OnDeserialized(const DataValue& node) override;

		// It is called when visible was changed
		void OnEnableInHierarchyChanged() override;

		// Updates transparency for this and children widgets
		void UpdateTransparency() override;

		// Calculates scroll area
		void CalculateScrollArea() override;

		// Moves scroll position and updates children widgets clipping and layout
		void MoveScrollPosition(const Vec2F& delta) override;

		// Updates visible items
		void UpdateVisibleItems();

		// It is called when cursor pressed on this
		void OnCursorPressed(const Input::Cursor& cursor) override;

		// It is called when cursor stay down during frame
		void OnCursorStillDown(const Input::Cursor& cursor) override;

		// It is called when cursor moved on this (or moved outside when this was pressed)
		void OnCursorMoved(const Input::Cursor& cursor) override;

		// It is called when cursor released (only when cursor pressed this at previous time)
		void OnCursorReleased(const Input::Cursor& cursor) override;

		// It is called when cursor pressing was broken (when scrolled scroll area or some other)
		void OnCursorPressBreak(const Input::Cursor& cursor) override;

		// It is called when cursor exits this object
		void OnCursorExit(const Input::Cursor& cursor) override;

		// Returns item widget under point and stores index in idxPtr, if not null
		Widget* GetItemUnderPoint(const Vec2F& point, int* idxPtr);

		// Updates hover
		void UpdateHover(const Vec2F& point);

		// Updates selection
		void UpdateSelection(int position);

		// It is called when selected item index was changed
		void OnSelectionChanged();

		friend class DropDown;
		friend class CustomDropDown;
	};
}

CLASS_BASES_META(o2::LongList)
{
	BASE_CLASS(o2::ScrollArea);
}
END_META;
CLASS_FIELDS_META(o2::LongList)
{
	FIELD().NAME(selectedItemPos).PUBLIC();
	FIELD().NAME(onFocused).PUBLIC();
	FIELD().NAME(getItemsCountFunc).PUBLIC();
	FIELD().NAME(getItemsRangeFunc).PUBLIC();
	FIELD().NAME(setupItemFunc).PUBLIC();
	FIELD().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(nullptr).NAME(mItemSample).PROTECTED();
	FIELD().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(nullptr).NAME(mSelectionDrawable).PROTECTED();
	FIELD().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(nullptr).NAME(mHoverDrawable).PROTECTED();
	FIELD().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(Layout::BothStretch()).NAME(mSelectionLayout).PROTECTED();
	FIELD().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(Layout::BothStretch()).NAME(mHoverLayout).PROTECTED();
	FIELD().DEFAULT_VALUE(-1).NAME(mMinVisibleItemIdx).PROTECTED();
	FIELD().DEFAULT_VALUE(-1).NAME(mMaxVisibleItemIdx).PROTECTED();
	FIELD().DEFAULT_VALUE(-1).NAME(mSelectedItem).PROTECTED();
	FIELD().NAME(mCurrentSelectionRect).PROTECTED();
	FIELD().NAME(mTargetSelectionRect).PROTECTED();
	FIELD().NAME(mCurrentHoverRect).PROTECTED();
	FIELD().NAME(mTargetHoverRect).PROTECTED();
	FIELD().NAME(mLastHoverCheckCursor).PROTECTED();
	FIELD().NAME(mLastSelectCheckCursor).PROTECTED();
	FIELD().NAME(mItemsPool).PROTECTED();
}
END_META;
CLASS_METHODS_META(o2::LongList)
{

	PUBLIC_FUNCTION(void, Update, float);
	PUBLIC_FUNCTION(void, Draw);
	PUBLIC_FUNCTION(void, SetItemSample, Widget*);
	PUBLIC_FUNCTION(Widget*, GetItemSample);
	PUBLIC_FUNCTION(void, SelectItemAt, int);
	PUBLIC_FUNCTION(int, GetSelectedItemPosition);
	PUBLIC_FUNCTION(Sprite*, GetSelectionDrawable);
	PUBLIC_FUNCTION(Sprite*, GetHoverDrawable);
	PUBLIC_FUNCTION(void, SetSelectionDrawableLayout, const Layout&);
	PUBLIC_FUNCTION(Layout, GetSelectionDrawableLayout);
	PUBLIC_FUNCTION(void, SetHoverDrawableLayout, const Layout&);
	PUBLIC_FUNCTION(Layout, GetHoverDrawableLayout);
	PUBLIC_FUNCTION(void, OnItemsUpdated, bool);
	PUBLIC_FUNCTION(void, UpdateSelfTransform);
	PUBLIC_STATIC_FUNCTION(String, GetCreateMenuGroup);
	PROTECTED_FUNCTION(void, OnDeserialized, const DataValue&);
	PROTECTED_FUNCTION(void, OnEnableInHierarchyChanged);
	PROTECTED_FUNCTION(void, UpdateTransparency);
	PROTECTED_FUNCTION(void, CalculateScrollArea);
	PROTECTED_FUNCTION(void, MoveScrollPosition, const Vec2F&);
	PROTECTED_FUNCTION(void, UpdateVisibleItems);
	PROTECTED_FUNCTION(void, OnCursorPressed, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorStillDown, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorMoved, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorReleased, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorPressBreak, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorExit, const Input::Cursor&);
	PROTECTED_FUNCTION(Widget*, GetItemUnderPoint, const Vec2F&, int*);
	PROTECTED_FUNCTION(void, UpdateHover, const Vec2F&);
	PROTECTED_FUNCTION(void, UpdateSelection, int);
	PROTECTED_FUNCTION(void, OnSelectionChanged);
}
END_META;

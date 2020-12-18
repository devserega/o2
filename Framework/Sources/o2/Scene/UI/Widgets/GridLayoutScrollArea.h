#pragma once

#include "o2/Render/Sprite.h"
#include "ScrollArea.h"
#include "o2/Scene/UI/Widgets/VerticalLayout.h"

namespace o2
{
	// ---------------------------------
	// Optimized grid layout with scroll
	// ---------------------------------
	class GridLayoutScrollArea: public ScrollArea
	{
	public:
		Function<int()>                   getItemsCountFunc; // Items count getting function
		Function<Vector<void*>(int, int)> getItemsRangeFunc; // Items getting in range function
		Function<void(Widget*, void*)>    setupItemFunc;     // Setup item widget function

	public:
	    // Default constructor
		GridLayoutScrollArea();

		// Copy-constructor
		GridLayoutScrollArea(const GridLayoutScrollArea& other);

		// Destructor
		~GridLayoutScrollArea();

		// Copy-operator
		GridLayoutScrollArea& operator=(const GridLayoutScrollArea& other);

		// Sets item sample widget. WARNING: Removing all old items!
		void SetItemSample(Widget* sample);

		// Returns item sample widget
		Widget* GetItemSample() const;

		// Sets spacing between cell widgets
		void SetItemsSpacing(const Vec2F& spacing);

		// Returns spacing between cell widgets
		const Vec2F& GetItemsSpacing() const;

		// Updates items
		void OnItemsUpdated(bool itemsRearranged = false);

		// Scroll view to item
		void ScrollTo(void* item);

		// Updates layout
		void UpdateSelfTransform() override;

		// Returns create menu group in editor
		static String GetCreateMenuGroup();

		SERIALIZABLE(GridLayoutScrollArea);

	protected:
		Widget* mItemSample = nullptr; // Item sample widget @SERIALIZABLE
		Vec2F   mItemsSpacing;         // Spacing between cell widgets @SERIALIZABLE
	    
		int mMinVisibleItemIdx = -1; // Visible item with minimal index
		int mMaxVisibleItemIdx = -1; // Visible item with maximal index

		int mPrevItemsInLine = 0; // Previous visible items on line count
					 						    
		Vector<Widget*> mItemsPool; // Items pool

	protected:
		// It is called when object was deserialized and trying to reattach states animations target
		void OnDeserialized(const DataValue& node) override;

		// It is called when visible was changed
		void OnEnableInHierarchyChanged() override;

		// Calculates scroll area
		void CalculateScrollArea() override;

		// Moves scroll position and updates children widgets clipping and layout
		void MoveScrollPosition(const Vec2F& delta) override;

		// Returns items count, calls getItemsCountFunc
		virtual int GetItemsCount() const;

		// Returns items in range from start to end, calls getItemsRangeFunc
		virtual Vector<void*> GetItemsRange(int start, int end) const;

		// Sets item widget, calls setupItemFunc
		virtual void SetupItemWidget(Widget* widget, void* item);

		// Updates visible items
		virtual void UpdateVisibleItems();

		// Returns item widget under point and stores index in idxPtr, if not null
		Widget* GetItemUnderPoint(const Vec2F& point, int* idxPtr);

		// Returns count of items in one line
		int GetItemsInLine() const;

		friend class DropDown;
		friend class CustomDropDown;
	};
}

CLASS_BASES_META(o2::GridLayoutScrollArea)
{
	BASE_CLASS(o2::ScrollArea);
}
END_META;
CLASS_FIELDS_META(o2::GridLayoutScrollArea)
{
	FIELD().NAME(getItemsCountFunc).PUBLIC();
	FIELD().NAME(getItemsRangeFunc).PUBLIC();
	FIELD().NAME(setupItemFunc).PUBLIC();
	FIELD().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(nullptr).NAME(mItemSample).PROTECTED();
	FIELD().SERIALIZABLE_ATTRIBUTE().NAME(mItemsSpacing).PROTECTED();
	FIELD().DEFAULT_VALUE(-1).NAME(mMinVisibleItemIdx).PROTECTED();
	FIELD().DEFAULT_VALUE(-1).NAME(mMaxVisibleItemIdx).PROTECTED();
	FIELD().DEFAULT_VALUE(0).NAME(mPrevItemsInLine).PROTECTED();
	FIELD().NAME(mItemsPool).PROTECTED();
}
END_META;
CLASS_METHODS_META(o2::GridLayoutScrollArea)
{

	PUBLIC_FUNCTION(void, SetItemSample, Widget*);
	PUBLIC_FUNCTION(Widget*, GetItemSample);
	PUBLIC_FUNCTION(void, SetItemsSpacing, const Vec2F&);
	PUBLIC_FUNCTION(const Vec2F&, GetItemsSpacing);
	PUBLIC_FUNCTION(void, OnItemsUpdated, bool);
	PUBLIC_FUNCTION(void, ScrollTo, void*);
	PUBLIC_FUNCTION(void, UpdateSelfTransform);
	PUBLIC_STATIC_FUNCTION(String, GetCreateMenuGroup);
	PROTECTED_FUNCTION(void, OnDeserialized, const DataValue&);
	PROTECTED_FUNCTION(void, OnEnableInHierarchyChanged);
	PROTECTED_FUNCTION(void, CalculateScrollArea);
	PROTECTED_FUNCTION(void, MoveScrollPosition, const Vec2F&);
	PROTECTED_FUNCTION(int, GetItemsCount);
	PROTECTED_FUNCTION(Vector<void*>, GetItemsRange, int, int);
	PROTECTED_FUNCTION(void, SetupItemWidget, Widget*, void*);
	PROTECTED_FUNCTION(void, UpdateVisibleItems);
	PROTECTED_FUNCTION(Widget*, GetItemUnderPoint, const Vec2F&, int*);
	PROTECTED_FUNCTION(int, GetItemsInLine);
}
END_META;

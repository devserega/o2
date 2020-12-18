#pragma once

#include "o2/Events/DrawableCursorEventsListener.h"
#include "o2/Scene/UI/Widgets/ContextMenu.h"
#include "o2/Scene/UI/Widget.h"
#include "o2/Utils/Function.h"

namespace o2
{
	class HorizontalLayout;

	// -------------
	// UI Menu panel
	// -------------
	class MenuPanel: public Widget, public DrawableCursorEventsListener
	{
	public:
		class Item: public ISerializable
		{
		public:
			WString text; // @SERIALIZABLE

			Vector<ContextMenu::Item*> subItems; // @SERIALIZABLE

			Function<void()> onClick;

		public:
			Item();
			Item(const WString& text, const Vector<ContextMenu::Item*>& subItems);
			Item(const WString& text, const Function<void()> onClick);

			bool operator==(const Item& other) const;

			SERIALIZABLE(Item);
		};

	public:
		// Default constructor
		MenuPanel();

		// Copy-constructor
		MenuPanel(const MenuPanel& other);

		// Destructor
		~MenuPanel();

		// Copy-operator
		MenuPanel& operator=(const MenuPanel& other);

		// Updates drawables, states and widget
		void Update(float dt) override;

		// Draws widget
		void Draw() override;

		// Add item
		Widget* AddItem(const Item& item);

		// Adds item by path ("node/sub node/target")
		void AddItem(const WString& path, const Function<void()>&
					 clickFunc = Function<void()>(),
					 const ImageAssetRef& icon = ImageAssetRef(),
					 const ShortcutKeys& shortcut = ShortcutKeys());

		// Adds toggle item by path ("node/sub node/target")
		void AddToggleItem(const WString& path, bool value,
						   const Function<void(bool)>& clickFunc = Function<void(bool)>(),
						   const ImageAssetRef& icon = ImageAssetRef(),
						   const ShortcutKeys& shortcut = ShortcutKeys());

		// Inserts item at position
		void InsertItem(const Item& item, int position);

		// Adds array of items
		void AddItems(Vector<Item> items);

		// Inserts array of items at position
		void InsertItems(Vector<Item> items, int position);

		// Returns item at position
		Item GetItem(int position);

		// Returns array of all items
		Vector<Item> GetItems() const;

		// Removes item at position
		void RemoveItem(int position);

		// Removes item by path
		void RemoveItem(const WString& path);

		// Removes all items
		void RemoveAllItems();

		// Returns items vertical layout
		HorizontalLayout* GetItemsLayout() const;

		// Returns item sample
		Widget* GetItemSample() const;

		// Sets item sample
		void SetItemSample(Widget* sample);

		// Returns selection drawable
		Sprite* GetSelectionDrawable() const;

		// Sets selection drawable layout
		void SetSelectionDrawableLayout(const Layout& layout);

		// Returns selection drawable layout
		Layout GetSelectionDrawableLayout() const;

		// Returns create menu group in editor
		static String GetCreateMenuGroup();

		SERIALIZABLE(MenuPanel);

	protected:
		HorizontalLayout* mLayout = nullptr; // Items layout

		Widget* mItemSample = nullptr; // Item sample @SERIALIZABLE

		Vector<Function<void()>> mClickFunctions; // Items click functions

		Sprite* mSelectionDrawable = nullptr; // Selection sprite @SERIALIZABLE
		Layout  mSelectionLayout;             // Selection layout, result selection area depends on selected item @SERIALIZABLE

		RectF mCurrentSelectionRect;  // Current selection rectangle (for smoothing)
		RectF mTargetSelectionRect;   // Target selection rectangle (over selected item)
		Vec2F mLastSelectCheckCursor; // Last cursor position on selection check

		int   mSelectedItem = -1;            // Index of selected item
		float mSelectSubContextTime = -1.0f; // Time to appearing selected sub context

		ContextMenu* mOpenedContext = nullptr; // Last opened context in menu

	protected:
		// It is called when visible was changed
		void OnEnableInHierarchyChanged() override;

		// Creates sub context menus by path
		ContextMenu* CreateSubContext(WString& path);

		// Creates item widget
		Widget* CreateItem(const Item& item);

		// Returns item info
		Item GetItemDef(int idx) const;

		// Returns item widget under point and stores index in idxPtr, if not null
		Widget* GetItemUnderPoint(const Vec2F& point, int* idxPtr);

		// Updates hover
		void UpdateHover(const Vec2F& point);

		// It is called when cursor pressed on this
		void OnCursorPressed(const Input::Cursor& cursor) override;

		// It is called when cursor stay down during frame
		void OnCursorStillDown(const Input::Cursor& cursor) override;

		// It is called when cursor released (only when cursor pressed this at previous time)
		void OnCursorReleased(const Input::Cursor& cursor) override;

		// It is called when cursor pressing was broken (when scrolled scroll area or some other)
		void OnCursorPressBreak(const Input::Cursor& cursor) override;

		// It is called when cursor moved on this (or moved outside when this was pressed)
		void OnCursorMoved(const Input::Cursor& cursor) override;

		// It is called when cursor exits this object
		void OnCursorExit(const Input::Cursor& cursor) override;
	};
}

CLASS_BASES_META(o2::MenuPanel)
{
	BASE_CLASS(o2::Widget);
	BASE_CLASS(o2::DrawableCursorEventsListener);
}
END_META;
CLASS_FIELDS_META(o2::MenuPanel)
{
	FIELD().DEFAULT_VALUE(nullptr).NAME(mLayout).PROTECTED();
	FIELD().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(nullptr).NAME(mItemSample).PROTECTED();
	FIELD().NAME(mClickFunctions).PROTECTED();
	FIELD().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(nullptr).NAME(mSelectionDrawable).PROTECTED();
	FIELD().SERIALIZABLE_ATTRIBUTE().NAME(mSelectionLayout).PROTECTED();
	FIELD().NAME(mCurrentSelectionRect).PROTECTED();
	FIELD().NAME(mTargetSelectionRect).PROTECTED();
	FIELD().NAME(mLastSelectCheckCursor).PROTECTED();
	FIELD().DEFAULT_VALUE(-1).NAME(mSelectedItem).PROTECTED();
	FIELD().DEFAULT_VALUE(-1.0f).NAME(mSelectSubContextTime).PROTECTED();
	FIELD().DEFAULT_VALUE(nullptr).NAME(mOpenedContext).PROTECTED();
}
END_META;
CLASS_METHODS_META(o2::MenuPanel)
{

	PUBLIC_FUNCTION(void, Update, float);
	PUBLIC_FUNCTION(void, Draw);
	PUBLIC_FUNCTION(Widget*, AddItem, const Item&);
	PUBLIC_FUNCTION(void, AddItem, const WString&, const Function<void()>&, const ImageAssetRef&, const ShortcutKeys&);
	PUBLIC_FUNCTION(void, AddToggleItem, const WString&, bool, const Function<void(bool)>&, const ImageAssetRef&, const ShortcutKeys&);
	PUBLIC_FUNCTION(void, InsertItem, const Item&, int);
	PUBLIC_FUNCTION(void, AddItems, Vector<Item>);
	PUBLIC_FUNCTION(void, InsertItems, Vector<Item>, int);
	PUBLIC_FUNCTION(Item, GetItem, int);
	PUBLIC_FUNCTION(Vector<Item>, GetItems);
	PUBLIC_FUNCTION(void, RemoveItem, int);
	PUBLIC_FUNCTION(void, RemoveItem, const WString&);
	PUBLIC_FUNCTION(void, RemoveAllItems);
	PUBLIC_FUNCTION(HorizontalLayout*, GetItemsLayout);
	PUBLIC_FUNCTION(Widget*, GetItemSample);
	PUBLIC_FUNCTION(void, SetItemSample, Widget*);
	PUBLIC_FUNCTION(Sprite*, GetSelectionDrawable);
	PUBLIC_FUNCTION(void, SetSelectionDrawableLayout, const Layout&);
	PUBLIC_FUNCTION(Layout, GetSelectionDrawableLayout);
	PUBLIC_STATIC_FUNCTION(String, GetCreateMenuGroup);
	PROTECTED_FUNCTION(void, OnEnableInHierarchyChanged);
	PROTECTED_FUNCTION(ContextMenu*, CreateSubContext, WString&);
	PROTECTED_FUNCTION(Widget*, CreateItem, const Item&);
	PROTECTED_FUNCTION(Item, GetItemDef, int);
	PROTECTED_FUNCTION(Widget*, GetItemUnderPoint, const Vec2F&, int*);
	PROTECTED_FUNCTION(void, UpdateHover, const Vec2F&);
	PROTECTED_FUNCTION(void, OnCursorPressed, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorStillDown, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorReleased, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorPressBreak, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorMoved, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorExit, const Input::Cursor&);
}
END_META;

CLASS_BASES_META(o2::MenuPanel::Item)
{
	BASE_CLASS(o2::ISerializable);
}
END_META;
CLASS_FIELDS_META(o2::MenuPanel::Item)
{
	FIELD().SERIALIZABLE_ATTRIBUTE().NAME(text).PUBLIC();
	FIELD().SERIALIZABLE_ATTRIBUTE().NAME(subItems).PUBLIC();
	FIELD().NAME(onClick).PUBLIC();
}
END_META;
CLASS_METHODS_META(o2::MenuPanel::Item)
{
}
END_META;

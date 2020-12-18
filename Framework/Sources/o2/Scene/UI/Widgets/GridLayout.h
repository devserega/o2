#pragma once

#include "o2/Scene/UI/Widget.h"

namespace o2
{
	// ------------------
	// Grid layout widget
	// ------------------
	class GridLayout: public Widget
	{
	public:
		PROPERTIES(GridLayout);
		PROPERTY(BaseCorner, baseCorner, SetBaseCorner, GetBaseCorner);                     // Base corder property
		PROPERTY(Vec2F, cellSize, SetCellSize, GetCellSize);                                // Grid cell size
		PROPERTY(int, arrangeAxisMaxCells, SetArrangeAxisMaxCells, GetArrangeAxisMaxCells); // Maximum cells at arranging axis
		PROPERTY(TwoDirection, arrangeAxis, SetArrangeAxis, GetArrangeAxis);                // Arrange axis

		PROPERTY(float, spacing, SetSpacing, GetSpacing);                // Space between widgets property
		PROPERTY(RectF, border, SetBorder, GetBorder);                   // Border property
		PROPERTY(float, borderLeft, SetBorderLeft, GetBorderLeft);       // Left border property
		PROPERTY(float, borderRight, SetBorderRight, GetBorderRight);    // Right border property
		PROPERTY(float, borderTop, SetBorderTop, GetBorderTop);          // Top border property
		PROPERTY(float, borderBottom, SetBorderBottom, GetBorderBottom); // Bottom border property

		PROPERTY(bool, fitByChildren, SetFitByChildren, IsFittingByChildren); // Fitting size by children property

	public:
		// Default constructor
		GridLayout();

		// Copy-constructor
		GridLayout(const GridLayout& other);

		// Destructor
		~GridLayout();

		// Copy operator
		GridLayout& operator=(const GridLayout& other);

		// Sets base corner
		void SetBaseCorner(BaseCorner baseCorner);

		// Returns base corner
		BaseCorner GetBaseCorner() const;

		// Sets space between widgets
		void SetSpacing(float spacing);

		// Returns space between widgets
		float GetSpacing() const;

		// Sets border
		void SetBorder(const RectF& border);

		// Returns border
		RectF GetBorder() const;

		// Sets left border
		void SetBorderLeft(float value);

		// Returns left border
		float GetBorderLeft() const;

		// Sets right border
		void SetBorderRight(float value);

		// Returns right border
		float GetBorderRight() const;

		// Sets top border
		void SetBorderTop(float value);

		// Returns top border
		float GetBorderTop() const;

		// Sets bottom border
		void SetBorderBottom(float value);

		// Returns bottom border
		float GetBorderBottom() const;

		// Sets cell size
		void SetCellSize(const Vec2F& size);

		// Returns cell size
		Vec2F GetCellSize() const;

		// Sets arrange type
		void SetArrangeAxis(TwoDirection type);

		// Returns arrange type
		TwoDirection GetArrangeAxis() const;

		// Sets arrange axis maximum cells count
		void SetArrangeAxisMaxCells(int count);

		// Returns arrange axis maximum cells count
		int GetArrangeAxisMaxCells() const;

		// Sets fitting size by children
		void SetFitByChildren(bool fit);

		// Returns fitting by children
		bool IsFittingByChildren() const;

		// Updates layout
		void UpdateSelfTransform() override;

		// Returns create menu group in editor
		static String GetCreateMenuGroup();

		SERIALIZABLE(GridLayout);

	protected:
		BaseCorner   mBaseCorner = BaseCorner::Left;          // Base corner of widgets arranging @SERIALIZABLE
		float        mSpacing = 0.0f;                         // Space between widgets @SERIALIZABLE
		Vec2F        mCellSize = Vec2F(50, 50);               // Grid cell size @SERIALIZABLE
		TwoDirection mArrangeAxis = TwoDirection::Horizontal; // Cells arrange axis @SERIALIZABLE
		int          mArrangeAxisMaxCells = 5;                // Arrange axis maximum cells count @SERIALIZABLE
		RectF        mBorder;                                 // Border @SERIALIZABLE
		bool         mFitByChildren = false;                  // Fitting by children @SERIALIZABLE

	protected:
		// It is called when child widget was added
		void OnChildAdded(Widget* child) override;

		// It is called when child widget was removed
		void OnChildRemoved(Widget* child) override;

		// Invokes required function for childs arranging
		void RearrangeChilds();

		// Arrange childs from left top
		void ArrangeFromLeftTop();

		// Arrange childs from top
		void ArrangeFromTop();

		// Arrange childs from right top
		void ArrangeFromRightTop();

		// Arrange childs from left
		void ArrangeFromLeft();

		// Arrange childs from center
		void ArrangeFromCenter();

		// Arrange childs from right
		void ArrangeFromRight();

		// Arrange childs from left bottom
		void ArrangeFromLeftBottom();

		// Arrange childs from bottom
		void ArrangeFromBottom();

		// Arrange childs from right bottom
		void ArrangeFromRightBottom();

		// Expands size by children
		void ExpandSizeByChilds();
	};
}

CLASS_BASES_META(o2::GridLayout)
{
	BASE_CLASS(o2::Widget);
}
END_META;
CLASS_FIELDS_META(o2::GridLayout)
{
	FIELD().NAME(baseCorner).PUBLIC();
	FIELD().NAME(cellSize).PUBLIC();
	FIELD().NAME(arrangeAxisMaxCells).PUBLIC();
	FIELD().NAME(arrangeAxis).PUBLIC();
	FIELD().NAME(spacing).PUBLIC();
	FIELD().NAME(border).PUBLIC();
	FIELD().NAME(borderLeft).PUBLIC();
	FIELD().NAME(borderRight).PUBLIC();
	FIELD().NAME(borderTop).PUBLIC();
	FIELD().NAME(borderBottom).PUBLIC();
	FIELD().NAME(fitByChildren).PUBLIC();
	FIELD().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(BaseCorner::Left).NAME(mBaseCorner).PROTECTED();
	FIELD().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(0.0f).NAME(mSpacing).PROTECTED();
	FIELD().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(Vec2F(50, 50)).NAME(mCellSize).PROTECTED();
	FIELD().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(TwoDirection::Horizontal).NAME(mArrangeAxis).PROTECTED();
	FIELD().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(5).NAME(mArrangeAxisMaxCells).PROTECTED();
	FIELD().SERIALIZABLE_ATTRIBUTE().NAME(mBorder).PROTECTED();
	FIELD().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(false).NAME(mFitByChildren).PROTECTED();
}
END_META;
CLASS_METHODS_META(o2::GridLayout)
{

	PUBLIC_FUNCTION(void, SetBaseCorner, BaseCorner);
	PUBLIC_FUNCTION(BaseCorner, GetBaseCorner);
	PUBLIC_FUNCTION(void, SetSpacing, float);
	PUBLIC_FUNCTION(float, GetSpacing);
	PUBLIC_FUNCTION(void, SetBorder, const RectF&);
	PUBLIC_FUNCTION(RectF, GetBorder);
	PUBLIC_FUNCTION(void, SetBorderLeft, float);
	PUBLIC_FUNCTION(float, GetBorderLeft);
	PUBLIC_FUNCTION(void, SetBorderRight, float);
	PUBLIC_FUNCTION(float, GetBorderRight);
	PUBLIC_FUNCTION(void, SetBorderTop, float);
	PUBLIC_FUNCTION(float, GetBorderTop);
	PUBLIC_FUNCTION(void, SetBorderBottom, float);
	PUBLIC_FUNCTION(float, GetBorderBottom);
	PUBLIC_FUNCTION(void, SetCellSize, const Vec2F&);
	PUBLIC_FUNCTION(Vec2F, GetCellSize);
	PUBLIC_FUNCTION(void, SetArrangeAxis, TwoDirection);
	PUBLIC_FUNCTION(TwoDirection, GetArrangeAxis);
	PUBLIC_FUNCTION(void, SetArrangeAxisMaxCells, int);
	PUBLIC_FUNCTION(int, GetArrangeAxisMaxCells);
	PUBLIC_FUNCTION(void, SetFitByChildren, bool);
	PUBLIC_FUNCTION(bool, IsFittingByChildren);
	PUBLIC_FUNCTION(void, UpdateSelfTransform);
	PUBLIC_STATIC_FUNCTION(String, GetCreateMenuGroup);
	PROTECTED_FUNCTION(void, OnChildAdded, Widget*);
	PROTECTED_FUNCTION(void, OnChildRemoved, Widget*);
	PROTECTED_FUNCTION(void, RearrangeChilds);
	PROTECTED_FUNCTION(void, ArrangeFromLeftTop);
	PROTECTED_FUNCTION(void, ArrangeFromTop);
	PROTECTED_FUNCTION(void, ArrangeFromRightTop);
	PROTECTED_FUNCTION(void, ArrangeFromLeft);
	PROTECTED_FUNCTION(void, ArrangeFromCenter);
	PROTECTED_FUNCTION(void, ArrangeFromRight);
	PROTECTED_FUNCTION(void, ArrangeFromLeftBottom);
	PROTECTED_FUNCTION(void, ArrangeFromBottom);
	PROTECTED_FUNCTION(void, ArrangeFromRightBottom);
	PROTECTED_FUNCTION(void, ExpandSizeByChilds);
}
END_META;

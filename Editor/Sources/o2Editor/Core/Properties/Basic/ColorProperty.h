#pragma once

#include "o2/Events/CursorEventsArea.h"
#include "o2/Utils/Property.h"
#include "o2Editor/Core/Properties/IPropertyField.h"

using namespace o2;

namespace o2
{
	class Image;
	class Widget;
}

namespace Editor
{
	// ------------------------------
	// Editor color property edit box
	// ------------------------------
	class ColorProperty: public TPropertyField<Color4>
	{
	public:
		// Default constructor
		ColorProperty();

		// Copy constructor
		ColorProperty(const ColorProperty& other);

		// Copy operator
		ColorProperty& operator=(const ColorProperty& other);

		IOBJECT(ColorProperty);

	protected:
		Widget*          mEditBox = nullptr;     // Edit box 
		Image*           mColorSprite = nullptr; // Color preview sprite
		CursorEventsArea mClickArea;             // Box click area

	protected:
		// Updates value view
		void UpdateValueView() override;

		// Searches controls widgets and layers and initializes them
		void InitializeControls();

		// It is called when box pressed and shows color picking dialog
		void OnClicked();
	};
}

CLASS_BASES_META(Editor::ColorProperty)
{
	BASE_CLASS(Editor::TPropertyField<Color4>);
}
END_META;
CLASS_FIELDS_META(Editor::ColorProperty)
{
	FIELD().DEFAULT_VALUE(nullptr).NAME(mEditBox).PROTECTED();
	FIELD().DEFAULT_VALUE(nullptr).NAME(mColorSprite).PROTECTED();
	FIELD().NAME(mClickArea).PROTECTED();
}
END_META;
CLASS_METHODS_META(Editor::ColorProperty)
{

	PROTECTED_FUNCTION(void, UpdateValueView);
	PROTECTED_FUNCTION(void, InitializeControls);
	PROTECTED_FUNCTION(void, OnClicked);
}
END_META;

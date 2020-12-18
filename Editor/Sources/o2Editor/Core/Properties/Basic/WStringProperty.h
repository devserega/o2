#pragma once

#include "o2/Utils/Property.h"
#include "o2Editor/Core/Properties/IPropertyField.h"

using namespace o2;

namespace o2
{
	class EditBox;
}

namespace Editor
{
	// -------------------------------
	// Editor string property edit box
	// -------------------------------
	class WStringProperty: public TPropertyField<WString>
	{
	public:
		// Default constructor
		WStringProperty();

		// Copy constructor
		WStringProperty(const WStringProperty& other);

		// Copy operator
		WStringProperty& operator=(const WStringProperty& other);

		IOBJECT(WStringProperty);

	protected:
		EditBox* mEditBox = nullptr; // Edit box 

	protected:
		// Updates value view
		void UpdateValueView() override;

		// Searches controls widgets and layers and initializes them
		void InitializeControls();

		// Edit box change event
		void OnEdited(const WString& data);
	};
}

CLASS_BASES_META(Editor::WStringProperty)
{
	BASE_CLASS(Editor::TPropertyField<WString>);
}
END_META;
CLASS_FIELDS_META(Editor::WStringProperty)
{
	FIELD().DEFAULT_VALUE(nullptr).NAME(mEditBox).PROTECTED();
}
END_META;
CLASS_METHODS_META(Editor::WStringProperty)
{

	PROTECTED_FUNCTION(void, UpdateValueView);
	PROTECTED_FUNCTION(void, InitializeControls);
	PROTECTED_FUNCTION(void, OnEdited, const WString&);
}
END_META;

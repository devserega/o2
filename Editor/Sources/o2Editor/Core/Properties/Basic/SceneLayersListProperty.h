#pragma once

#include "o2/Scene/SceneLayersList.h"
#include "o2Editor/Core/Properties/IPropertyField.h"

namespace o2
{
	class Button;
	class ContextMenu;
	class EditBox;
	class Widget;
}

namespace Editor
{
	// ---------------------------------
	// Editor scene layers list property
	// // ------------------------------
	class SceneLayersListProperty: public TPropertyField<SceneLayersList>
	{
	public:
		// Default constructor
		SceneLayersListProperty();

		// Copy constructor
		SceneLayersListProperty(const SceneLayersListProperty& other);

		// Copy operator
		SceneLayersListProperty& operator=(const SceneLayersListProperty& other);

		SERIALIZABLE(SceneLayersListProperty);

	protected:
		Button* mPropertyButton = nullptr; // Property box with layers' names
		Text*   mPropertyText = nullptr;   // Text layer inside property box

		ContextMenu* mLayersContext = nullptr; // Layers list toggles context

	protected:
		// Updates value view
		void UpdateValueView() override;

		// Searches controls widgets and layers and initializes them
		void InitializeControls();

		// Updates context layers list and opens
		void OpenContext();
	};
}

CLASS_BASES_META(Editor::SceneLayersListProperty)
{
	BASE_CLASS(Editor::TPropertyField<SceneLayersList>);
}
END_META;
CLASS_FIELDS_META(Editor::SceneLayersListProperty)
{
	FIELD().DEFAULT_VALUE(nullptr).NAME(mPropertyButton).PROTECTED();
	FIELD().DEFAULT_VALUE(nullptr).NAME(mPropertyText).PROTECTED();
	FIELD().DEFAULT_VALUE(nullptr).NAME(mLayersContext).PROTECTED();
}
END_META;
CLASS_METHODS_META(Editor::SceneLayersListProperty)
{

	PROTECTED_FUNCTION(void, UpdateValueView);
	PROTECTED_FUNCTION(void, InitializeControls);
	PROTECTED_FUNCTION(void, OpenContext);
}
END_META;

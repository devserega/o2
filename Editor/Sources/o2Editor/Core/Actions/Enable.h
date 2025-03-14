#pragma once

#include "o2/Utils/Types/Containers/Vector.h"
#include "o2Editor/Core/Actions/IAction.h"

using namespace o2;

namespace o2
{
	class SceneEditableObject;
}

namespace Editor
{
	// -------------------------------
	// Enable or disable scene objects
	// -------------------------------
	class EnableAction: public IAction
	{
	public:
		Vector<SceneUID> objectsIds;
		bool             enable; 

	public:
		// Default constructor
		EnableAction();

		// COnstructor with enabled or disabled objects
		EnableAction(const Vector<SceneEditableObject*>& objects, bool enable);

		// Returns name of action
		String GetName() const;

		// Enable or disable again
		void Redo();

		// Reverts objects to previous state
		void Undo();

		SERIALIZABLE(EnableAction);
	};
}

CLASS_BASES_META(Editor::EnableAction)
{
	BASE_CLASS(Editor::IAction);
}
END_META;
CLASS_FIELDS_META(Editor::EnableAction)
{
	FIELD().NAME(objectsIds).PUBLIC();
	FIELD().NAME(enable).PUBLIC();
}
END_META;
CLASS_METHODS_META(Editor::EnableAction)
{

	PUBLIC_FUNCTION(String, GetName);
	PUBLIC_FUNCTION(void, Redo);
	PUBLIC_FUNCTION(void, Undo);
}
END_META;

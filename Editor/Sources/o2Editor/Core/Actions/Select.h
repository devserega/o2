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
	// -----------------------------
	// Scene object selection action
	// -----------------------------
	class SelectAction: public IAction
	{
	public:
		Vector<SceneUID> selectedObjectsIds;
		Vector<SceneUID> prevSelectedObjectsIds;

	public:
		// Default constructor
		SelectAction();

		// CUnstructor with new and previous selected objects
		SelectAction(const Vector<SceneEditableObject*>& selectedObjects, const Vector<SceneEditableObject*>& prevSelectedObjects);

		// Returns name of action
		String GetName() const;

		// Selects objects again
		void Redo();

		// Selects previous selected objects
		void Undo();

		SERIALIZABLE(SelectAction);
	};
}

CLASS_BASES_META(Editor::SelectAction)
{
	BASE_CLASS(Editor::IAction);
}
END_META;
CLASS_FIELDS_META(Editor::SelectAction)
{
	FIELD().NAME(selectedObjectsIds).PUBLIC();
	FIELD().NAME(prevSelectedObjectsIds).PUBLIC();
}
END_META;
CLASS_METHODS_META(Editor::SelectAction)
{

	PUBLIC_FUNCTION(String, GetName);
	PUBLIC_FUNCTION(void, Redo);
	PUBLIC_FUNCTION(void, Undo);
}
END_META;

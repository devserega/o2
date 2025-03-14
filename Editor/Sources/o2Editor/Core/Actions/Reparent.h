#pragma once

#include "o2/Utils/Math/Basis.h"
#include "o2/Utils/Types/Containers/Vector.h"
#include "o2Editor/Core/Actions/IAction.h"

using namespace o2;

namespace o2
{
	class Actor;
	class SceneEditableObject;
}

namespace Editor
{
	// ------------------------------
	// Change object's parents action
	// Storing old and new parent and 
	// index in parents
	// ------------------------------
	class ReparentAction: public IAction
	{
	public:
		struct ObjectInfo
		{
			SceneUID objectId;
			SceneUID lastParentId;
			SceneUID lastPrevObjectId;
			int      objectHierarchyIdx;
			Basis    transform;
		};

		Vector<ObjectInfo*> objectsInfos;
		SceneUID            newParentId;
		SceneUID            newPrevObjectId;

	public:
		// Default constructor
		ReparentAction();

		// Constructor
		ReparentAction(const Vector<SceneEditableObject*>& beginObjects);

		// Destructor
		~ReparentAction();

		// It is called when object are reparented, stores all required data to restore old objects' parents
		void ObjectsReparented(SceneEditableObject* newParent, SceneEditableObject* prevObject);

		// Returns name of action
		String GetName() const override;

		// Sets new parents again
		void Redo() override;

		// Sets previous stored parents and index in children
		void Undo() override;

		SERIALIZABLE(ReparentAction);
	};
}

CLASS_BASES_META(Editor::ReparentAction)
{
	BASE_CLASS(Editor::IAction);
}
END_META;
CLASS_FIELDS_META(Editor::ReparentAction)
{
	FIELD().NAME(objectsInfos).PUBLIC();
	FIELD().NAME(newParentId).PUBLIC();
	FIELD().NAME(newPrevObjectId).PUBLIC();
}
END_META;
CLASS_METHODS_META(Editor::ReparentAction)
{

	PUBLIC_FUNCTION(void, ObjectsReparented, SceneEditableObject*, SceneEditableObject*);
	PUBLIC_FUNCTION(String, GetName);
	PUBLIC_FUNCTION(void, Redo);
	PUBLIC_FUNCTION(void, Undo);
}
END_META;

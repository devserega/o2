#pragma once

#include "o2Editor/Core/Properties/IPropertyField.h"
#include "o2Editor/Core/Properties/PropertiesContext.h"
#include "o2Editor/PropertiesWindow/ActorsViewer/IActorPropertiesViewer.h"

namespace Editor
{
	class IObjectPropertiesViewer;

	// -------------------------------
	// Default actor properties viewer
	// -------------------------------
	class DefaultActorPropertiesViewer: public IActorPropertiesViewer
	{
	public:
		// Default constructor. Initializes data widget
		DefaultActorPropertiesViewer();

		// Virtual destructor
		~DefaultActorPropertiesViewer();

		// Sets target actors
		void SetTargetActors(const Vector<Actor*>& actors) override;

		// Returns viewing actor type 
		const Type* GetActorType() const override;

		// Specialize viewing actor type. Creates all using properties
		void SpecializeActorType(const Type* type);

		// Updates all actor values
		void Refresh() override;

		// Returns is there no properties
		bool IsEmpty() const override;

		IOBJECT(DefaultActorPropertiesViewer);

	protected:
		Vector<Actor*> mTargetActors;        // Target actors
		const Type*    mActorType = nullptr; // Target actor type

		IObjectPropertiesViewer* mViewer = nullptr; // Actor properties viewer

	protected:
		// It is called when some property changed, marks Actor as changed and calls default Undo create callback
		void OnPropertyChanged(const String& path, const Vector<DataDocument>& before,
							   const Vector<DataDocument>& after);
	};
}

CLASS_BASES_META(Editor::DefaultActorPropertiesViewer)
{
	BASE_CLASS(Editor::IActorPropertiesViewer);
}
END_META;
CLASS_FIELDS_META(Editor::DefaultActorPropertiesViewer)
{
	FIELD().NAME(mTargetActors).PROTECTED();
	FIELD().DEFAULT_VALUE(nullptr).NAME(mActorType).PROTECTED();
	FIELD().DEFAULT_VALUE(nullptr).NAME(mViewer).PROTECTED();
}
END_META;
CLASS_METHODS_META(Editor::DefaultActorPropertiesViewer)
{

	PUBLIC_FUNCTION(void, SetTargetActors, const Vector<Actor*>&);
	PUBLIC_FUNCTION(const Type*, GetActorType);
	PUBLIC_FUNCTION(void, SpecializeActorType, const Type*);
	PUBLIC_FUNCTION(void, Refresh);
	PUBLIC_FUNCTION(bool, IsEmpty);
	PROTECTED_FUNCTION(void, OnPropertyChanged, const String&, const Vector<DataDocument>&, const Vector<DataDocument>&);
}
END_META;

#pragma once

#include "o2/Assets/Types/ActorAsset.h"
#include "o2/Scene/ActorCreationMode.h"
#include "o2/Scene/ActorRef.h"
#include "o2/Scene/ActorTransform.h"
#include "o2/Scene/Tags.h"
#include "o2/Utils/Editor/Attributes/AnimatableAttribute.h"
#include "o2/Utils/Editor/Attributes/EditorPropertyAttribute.h"
#include "o2/Utils/Editor/SceneEditableObject.h"
#include "o2/Utils/Singleton.h"
#include "o2/Utils/Types/UID.h"
#include "Scene.h"
#include "ActorRefResolver.h"

namespace o2
{
	class Component;
	class Scene;
	class SceneLayer;

#if IS_EDITOR
	typedef SceneEditableObject ActorBase;

	struct ActorDifferences;
#else
	typedef ISerializable ActorBase;
#endif

	// ---------------------------------------------------------------------------------------------
	// Scene actor. This is a basic scene object. All other scene object types is derived from this.
	// It has an unique id, name, tags, scene layer, transformation, components and children actors.
	// Can be created from prototype of other actor. 
	// When editor pragma is enabled, it is derived from SceneEditableObject and support all editor 
	// features for editing actor
	// ---------------------------------------------------------------------------------------------
	class Actor: virtual public ActorBase
	{
	public:
		enum class SceneStatus { InScene, NotInScene, WaitingAddToScene };

	public:
		PROPERTIES(Actor);
		PROPERTY(ActorAssetRef, prototype, SetPrototype, GetPrototype); // Prototype asset reference property @EDITOR_IGNORE

		GETTER(SceneUID, id, GetID);              // Actor unique id
		PROPERTY(String, name, SetName, GetName); // Actor name property @EDITOR_IGNORE @ANIMATABLE

		PROPERTY(String, layerName, SetLayer, GetLayerName); // Layer name property @EDITOR_IGNORE @ANIMATABLE

		PROPERTY(bool, enabled, SetEnabled, IsEnabled);         // Is actor enabled property @EDITOR_IGNORE @ANIMATABLE
		GETTER(bool, enabledInHierarchy, IsEnabledInHierarchy); // Is actor enabled in hierarchy getter

		GETTER(Vector<Actor*>, children, GetChildren);         // Children array getter
		GETTER(Vector<Component*>, components, GetComponents); // Components array getter

		ACCESSOR(Actor*, child, String, GetChild, GetAllChilds);                 // Children accessor
		ACCESSOR(Component*, component, String, GetComponent, GetAllComponents); // Component accessor by type name

	public:
		TagGroup              tags;      // Tags group @EDITOR_IGNORE
		ActorTransform* const transform; // Transformation of actor @EDITOR_IGNORE @ANIMATABLE

	public:
		// Default constructor
		Actor(ActorCreateMode mode = ActorCreateMode::Default);

		// Actor constructor from prototype
		Actor(const ActorAssetRef& prototype, ActorCreateMode mode = ActorCreateMode::Default);

		// Constructor with components
		Actor(Vector<Component*> components, ActorCreateMode mode = ActorCreateMode::Default);

		// Copy-constructor
		Actor(const Actor& other, ActorCreateMode mode);

		// Copy-constructor
		Actor(const Actor& other);

		// Destructor
		virtual ~Actor();

		// Assign operator
		Actor& operator=(const Actor& other);

		// Updates actor and components
		virtual void Update(float dt);

		// Updates actor and components with fixed delta time
		virtual void FixedUpdate(float dt);

		// Updates childs
		virtual void UpdateChildren(float dt);

		// Updates childs with fixed delta time
		virtual void FixedUpdateChildren(float dt);

		// Updates self transform, dependent parents and children transforms
		virtual void UpdateTransform();

		// Updates transform
		virtual void UpdateSelfTransform();

		// Updates children transforms
		virtual void UpdateChildrenTransforms();

		// Sets actor's name
		void SetName(const String& name);

		// Returns name
		const String& GetName() const;

		// Returns actor's unique id
		SceneUID GetID() const;

		// Sets id. Be carefully! Ids must be unique! Not recommended to change this
		void SetID(SceneUID id);

		// Generates new random id 
		void GenerateNewID(bool withChildren = true);

		// Returns asset id
		UID GetAssetID() const;

		// Is this from asset
		bool IsAsset() const;

		// Sets prototype and links actor to them
		void SetPrototype(ActorAssetRef asset);

		// Returns prototype from this or this parent
		ActorAssetRef GetPrototype() const;

		// Returns prototype directly from only this
		ActorAssetRef GetPrototypeDirectly() const;

		// Returns prototype link pointer
		ActorRef GetPrototypeLink() const;

		// Includes to scene and now will be update and draw automatically from scene
		void AddToScene();

		// Excludes from scene and will not be update and draw automatically from scene
		void RemoveFromScene(bool keepEditorObjects = false);

		// Is actor on scene
		bool IsOnScene() const;

		// Sets actor enabling
		virtual void SetEnabled(bool enabled);

		// Enables actor
		void Enable();

		// Disables actor
		void Disable();

		// Returns is actor enabled
		bool IsEnabled() const;

		// Returns is really enabled
		bool IsResEnabled() const;

		// Returns is actor enabled in hierarchy
		bool IsEnabledInHierarchy() const;

		// Sets parent
		void SetParent(Actor* actor, bool worldPositionStays = true);

		// Returns parent
		Actor* GetParent() const;

		// Sets index position in parent or scene
		virtual void SetIndexInSiblings(int index);

		// Add child actor
		Actor* AddChild(Actor* actor);

		// Add children actors
		void AddChildren(const Vector<Actor*>& actors);

		// Add child actor
		Actor* AddChild(Actor* actor, int index);

		// Returns child actor by path (ex "root/some node/other node/target node")
		Actor* GetChild(const String& path) const;

		// Returns child actor by name
		Actor* FindChild(const String& name) const;

		// Returns child actor by predicate
		Actor* FindChild(const Function<bool(const Actor* child)>& pred) const;

		// Returns child actor by path (ex "root/some node/other node/target node")
		template<typename _type>
		_type* GetChildByType(const String& path) const;

		// Returns child actor by name
		template<typename _type>
		_type* FindChildByTypeAndName(const String& name) const;

		// Searches child with specified type
		template<typename _type>
		_type* FindChildByType(bool searchInChildren = true);

		// Returns children array
		const Vector<Actor*>& GetChildren() const;

		// Returns all children actors with their children
		virtual void GetAllChildrenActors(Vector<Actor*>& actors);

		// Removes child and destroys him if needed
		void RemoveChild(Actor* actor, bool release = true);

		// Removes and destroys all childs
		void RemoveAllChildren(bool release = true);

		// Searches actor with id in this and this children
		virtual Actor* FindActorById(SceneUID id);

		// And new component
		template<typename _type>
		_type* AddComponent();

		// Adds new component
		Component* AddComponent(Component* component);

		// Removes component
		void RemoveComponent(Component* component, bool release = true);

		// Removes all components
		void RemoveAllComponents();

		// Returns component with type name
		Component* GetComponent(const String& typeName);

		// Returns component with type
		Component* GetComponent(const Type* type);

		// Returns component by id
		Component* GetComponent(SceneUID id);

		// Returns component with type
		template<typename _type>
		_type* GetComponent() const;

		// Returns component with type in this and children
		template<typename _type>
		_type* GetComponentInChildren() const;

		// Return all components by type
		template<typename _type>
		Vector<_type*> GetComponents() const;

		// Returns all components by type in this and children
		template<typename _type>
		Vector<_type*> GetComponentsInChildren() const;

		// Returns all components
		const Vector<Component*>& GetComponents() const;

		// Sets layer by name
		void SetLayer(const String& layerName);

		// Returns layer
		SceneLayer* GetLayer() const;

		// Returns layer name
		const String& GetLayerName() const;

		// Beginning serialization callback
		void SerializeBasicOverride(DataValue& node) const;

		// Completion deserialization callback
		void DeserializeBasicOverride(const DataValue& node);

		// Sets default actors creation mode
		static void SetDefaultCreationMode(ActorCreateMode mode);

		// Return default actors creation mode
		static ActorCreateMode GetDefaultCreationMode();

		// Returns is current mode means mode on scene. For InScene always returns true. For returns true only when default creation mode is onscene.
		static bool IsModeOnScene(ActorCreateMode mode);

		// Returns create menu category in editor
		static String GetCreateMenuCategory();

		// Returns create menu group in editor
		static String GetCreateMenuGroup();

		SERIALIZABLE(Actor);

	protected:
		struct ICopyVisitor
		{
			int depth = 0;

			virtual ~ICopyVisitor() {}
			virtual void OnCopyActor(const Actor* source, Actor* target) = 0;
			virtual void OnCopyComponent(const Component* source, Component* target) = 0;
			virtual void Finalize() {}
		};

		struct SourceToTargetMapCloneVisitor: public ICopyVisitor
		{
			Map<const Actor*, Actor*>         sourceToTargetActors;
			Map<const Component*, Component*> sourceToTargetComponents;

			void OnCopyActor(const Actor* source, Actor* target) override;
			void OnCopyComponent(const Component* source, Component* target) override;
			void Finalize() override;
		};

		struct InstantiatePrototypeCloneVisitor: public SourceToTargetMapCloneVisitor
		{
			void OnCopyActor(const Actor* source, Actor* target) override;
			void OnCopyComponent(const Component* source, Component* target) override;
		};

	protected:
		static ActorCreateMode mDefaultCreationMode;   // Default mode creation

		SceneUID mId;   // Unique actor id
		String   mName; // Name of actor @SERIALIZABLE

		ActorAssetRef mPrototype;               // Prototype asset
		ActorRef      mPrototypeLink = nullptr; // Prototype link actor. Links to source actor from prototype

		String      mLayerName = String("Default"); // Scene layer name @SERIALIZABLE
		SceneLayer* mLayer = nullptr;       // Scene layer. Empty when actor isn't on scene

		Actor*         mParent = nullptr; // Parent actor 
		Vector<Actor*> mChildren;         // Children actors 

		Vector<Component*> mComponents; // Components vector 

		bool mEnabled = true;               // Is actor enabled
		bool mResEnabled = true;            // Is actor really enabled. 
		bool mResEnabledInHierarchy = true; // Is actor enabled in hierarchy

		SceneStatus mSceneStatus = SceneStatus::NotInScene; // Actor on scene status

		bool mIsAsset = false; // Is this actor cached asset
		UID  mAssetId;         // Source asset id

		Vector<ActorRef*> mReferences; // References to this actor

		mutable ICopyVisitor* mCopyVisitor = nullptr; // Copy visitor. It is called when copying actor and calls on actor or component copying

	protected:
		// Base actor constructor with transform
		Actor(ActorTransform* transform, SceneStatus sceneStatus = SceneStatus::WaitingAddToScene,
			  const String& name = "unnamed", bool enabled = true, bool resEnabled = true, bool locked = false,
			  bool resLocked = false, const String& layerName = "", SceneLayer* layer = nullptr,
			  SceneUID id = Math::Random(), UID assetId = UID(0));

		// Default constructor with transform
		Actor(ActorTransform* transform, ActorCreateMode mode = ActorCreateMode::Default);

		// Actor constructor from prototype with transform
		Actor(ActorTransform* transform, const ActorAssetRef& prototype, ActorCreateMode mode = ActorCreateMode::Default);

		// Constructor with components with transform
		Actor(ActorTransform* transform, Vector<Component*> components, ActorCreateMode mode = ActorCreateMode::Default);

		// Copy-constructor with transform
		Actor(ActorTransform* transform, const Actor& other, ActorCreateMode mode = ActorCreateMode::Default);

		// Copies fields from source to dest
		void CopyFields(Vector<const FieldInfo*>& fields, IObject* source, IObject* dest,
						Vector<Actor**>& actorsPointers,
						Vector<Component**>& componentsPointers,
						Vector<ISerializable*>& serializableObjects);

		// Collects fixing actors and components pointers in new component
		void CollectFixingFields(Component* newComponent, Vector<Component**>& componentsPointers,
								 Vector<Actor**>& actorsPointers);

		// Collects component field, except Component class fields
		void GetComponentFields(Component* component, Vector<const FieldInfo*>& fields);

		// Fixes actors and components pointers by actors map
		void FixComponentFieldsPointers(const Vector<Actor**>& actorsPointers,
										const Vector<Component**>& componentsPointers,
										const Map<const Actor*, Actor*>& actorsMap,
										const Map<const Component*, Component*>& componentsMap);

		// Updates result read enable flag
		virtual void UpdateResEnabled();

		// Updates enabling
		virtual void UpdateResEnabledInHierarchy();

		// Regular serializing without prototype
		virtual void SerializeRaw(DataValue& node) const;

		// Regular deserializing without prototype
		virtual void DeserializeRaw(const DataValue& node);

		// Regular serializing with prototype
		virtual void SerializeWithProto(DataValue& node) const;

		// Regular deserializing with prototype
		virtual void DeserializeWithProto(const DataValue& node);

		// Returns dictionary of all children by names
		Map<String, Actor*> GetAllChilds();

		// Returns dictionary of all components by type names
		Map<String, Component*> GetAllComponents();

		// Sets parent
		void SetParentProp(Actor* actor);

		// Is is called when actor has added to scene
		virtual void OnAddToScene();

		// It is called when actor has removed from scene
		virtual void OnRemoveFromScene();

		// It is called on first update
		virtual void OnStart();

		// Is is called on update with frame dt
		virtual void OnUpdate(float dt);

		// It is called on fixed update with fixed dt
		virtual void OnFixedUpdate(float dt);

		// Is is called when actor enabled in hierarchy
		virtual void OnEnabled();

		// It is called when actor disabled in hierarchy
		virtual void OnDisabled();

		// It is called when result enable was changed
		virtual void OnEnableInHierarchyChanged();

		// It is called when transformation was updated
		virtual void OnTransformUpdated();

		// It is called when transformation was changed 
		virtual void OnTransformChanged();

		// It is called when parent changed
		virtual void OnParentChanged(Actor* oldParent);

		// It is called when child actor was added
		virtual void OnChildAdded(Actor* child);

		// It is called when child actor was removed
		virtual void OnChildRemoved(Actor* child);

		// It is called when layer was changed
		virtual void OnLayerChanged(SceneLayer* oldLayer);

		// It is called when new component has added to actor
		virtual void OnComponentAdded(Component* component);

		// It is called when component going to be removed from actor
		virtual void OnComponentRemoving(Component* component);

#if IS_EDITOR
	public:
		PROPERTY(bool, locked, SetLocked, IsLocked);          // Is actor locked property @EDITOR_IGNORE
		GETTER(bool, lockedInHierarchy, IsLockedInHierarchy); // Is actor locked in hierarchy getter

	public:
		Function<void(bool)>   onEnableChanged;         // Enable changing event
		Function<void()>       onChanged;               // Something in actor change event
		Function<void(Actor*)> onParentChanged;         // Actor change parent event
		Function<void()>       onChildHierarchyChanged; // Actor childs hierarchy change event
		Function<void(bool)>   onLockChanged;           // Locking changing event
		Function<void()>       onNameChanged;           // Name changing event

	public:
		struct MakePrototypeCloneVisitor: SourceToTargetMapCloneVisitor
		{
			void OnCopyActor(const Actor* source, Actor* target) override;
			void OnCopyComponent(const Component* source, Component* target) override;
		};

	public:
		// Sets locking. Locked actor can't be selected in editor scene view. But is can be selected in scene tree
		void SetLocked(bool locked);

		// Locks actor. Locked actor can't be selected in editor scene view. But is can be selected in scene tree
		void Lock();

		// Unlocks actor. Locked actor can't be selected in editor scene view. But is can be selected in scene tree
		void Unlock();

		// Returns is actor locked. Locked actor can't be selected in editor scene view. But is can be selected in scene tree
		bool IsLocked() const;

		// Returns is actor locked in hierarchy. Locked actor can't be selected in editor scene view. But is can be selected in scene tree
		bool IsLockedInHierarchy() const;

		// Breaks link to prototype
		void BreakPrototypeLink();

		// Applies all changes to prototype and saves it
		void ApplyChangesToPrototype();

		// Reverts all properties to prototype
		void RevertToPrototype();

		// Makes prototype asset from this actor and links this to new asset
		ActorAssetRef MakePrototype();

		// Returns is this linked to specified actor with depth links search
		bool IsLinkedToActor(Actor* actor) const;

		// Searches actor in this, what linked to linkActor
		Actor* FindLinkedActor(Actor* linkActor);

		// Returns object's link to prototype
		const SceneEditableObject* GetEditableLink() const override;

		// Returns list of object's children
		Vector<SceneEditableObject*> GetEditableChildren() const override;

		// Returns object's parent object. Return nullptr when it is a root scene object
		SceneEditableObject* GetEditableParent() const override;

		// Sets parent object. nullptr means make this object as root. idx is place in parent children. idx == -1 means last
		void SetEditableParent(SceneEditableObject* object) override;

		// Adds child. idx is place in parent children. idx == -1 means last
		void AddEditableChild(SceneEditableObject* object, int idx = -1) override;

		// Returns is that type of object can be enabled and disabled
		bool IsSupportsDisabling() const override;

		// Returns is that type of object can be locked
		bool IsSupportsLocking() const override;

		// Returns is that type of object can be transformed
		bool IsSupportsTransforming() const override;

		// Returns transform, override when it's supports
		Basis GetTransform() const override;

		// Sets transform of object, override when it's supports
		void SetTransform(const Basis& transform) override;

		// Returns is object supports pivot 
		bool IsSupportsPivot() const override;

		// Sets transform pivot point
		void SetPivot(const Vec2F& pivot) override;

		// Returns transform pivot
		Vec2F GetPivot() const override;

		// It is called when some changed in actor
		void OnChanged() override;

		// It is called when actor's locking was changed
		void OnLockChanged() override;

		// It is called when actor's name was changed
		void OnNameChanged() override;

		// It is called when child changed
		void OnChildrenChanged() override;

	protected:
		bool mLocked = false;    // Is actor locked @SERIALIZABLE
		bool mResLocked = false; // Is actor locked in hierarchy @SERIALIZABLE

	protected:
		// Applies basic actor fields and transform from source to dest  
		void CopyActorChangedFields(Actor* source, Actor* changed, Actor* dest, Vector<Actor*>& allDestChilds, bool withTransform);

		// Separates children actors to linear array, removes child and parent links
		void SeparateActors(Vector<Actor*>& separatedActors);

		// Processes reverting actor
		void ProcessReverting(Actor* dest, const Actor* source, const Vector<Actor*>& separatedActors,
							  Vector<Actor**>& actorsPointers, Vector<Component**>& componentsPointers,
							  Map<const Actor*, Actor*>& actorsMap,
							  Map<const Component*, Component*>& componentsMap,
							  Vector<ISerializable*>& serializableObjects);

		// Collects differences between this and prototype
		virtual void GetDifferences(ActorDifferences& differences) const;

		// It is called before making prototype from this object
		void BeginMakePrototype() const override;

		// It is called before instantiate from this object
		void BeginInstantiatePrototype() const override;

		// Not using prototype setter
		void SetProtytypeDummy(ActorAssetRef asset);

		// Updates locking
		void UpdateLocking();

#endif // IS_EDITOR

		friend class ActorAsset;
		friend class ActorRef;
		friend class ActorRefResolver;
		friend class ActorTransform;
		friend class Component;
		friend class ComponentRef;
		friend class DrawableComponent;
		friend class ISceneDrawable;
		friend class Scene;
		friend class SceneLayer;
		friend class Tag;
		friend class Widget;
	};
}

#include "o2/Scene/Component.h"

namespace o2
{
	template<typename _type>
	_type* Actor::FindChildByType(bool searchInChildren /*= true*/)
	{
		for (auto child : mChildren)
			if (child->GetType() == TypeOf(_type))
				return (_type*)child;

		if (searchInChildren)
		{
			for (auto child : mChildren)
				if (auto res = child->FindChildByType<_type>())
					return res;
		}

		return nullptr;
	}

	template<typename _type>
	_type* Actor::GetChildByType(const String& path) const
	{
		return dynamic_cast<_type*>(GetChild(path));
	}

	template<typename _type>
	_type* Actor::FindChildByTypeAndName(const String& name) const
	{
		return dynamic_cast<_type*>(FindChild(name));
	}

	template<typename _type>
	Vector<_type*> Actor::GetComponentsInChildren() const
	{
		Vector<_type*> res = GetComponents<_type>();

		for (auto child : mChildren)
			res.Add(child->GetComponentsInChildren<_type>());

		return res;
	}

	template<typename _type>
	_type* Actor::GetComponentInChildren() const
	{
		_type* res = GetComponent<_type*>();

		if (res)
			return res;

		for (auto child : mChildren)
		{
			res = child->GetComponentInChildren<_type>();
			if (res)
				return res;
		}

		return res;
	}

	template<typename _type>
	_type* Actor::GetComponent() const
	{
		for (auto comp : mComponents)
		{
			if (comp->GetType().IsBasedOn(TypeOf(_type)))
				return dynamic_cast<_type*>(comp);
		}

		return nullptr;
	}

	template<typename _type>
	Vector<_type*> Actor::GetComponents() const
	{
		Vector<_type*> res;
		for (auto comp : mComponents)
		{
			if (comp->GetType().IsBasedOn(TypeOf(_type)))
				res.Add(comp);
		}

		return res;
	}

	template<typename _type>
	_type* Actor::AddComponent()
	{
		if (GetComponent<_type>() != nullptr)
			return nullptr;

		_type* newComponent = mnew _type();
		AddComponent(newComponent);

		return newComponent;
	}

}

PRE_ENUM_META(o2::Actor::SceneStatus);

CLASS_BASES_META(o2::Actor)
{
	BASE_CLASS(o2::SceneEditableObject);
}
END_META;
CLASS_FIELDS_META(o2::Actor)
{
	FIELD().EDITOR_IGNORE_ATTRIBUTE().NAME(prototype).PUBLIC();
	FIELD().NAME(id).PUBLIC();
	FIELD().ANIMATABLE_ATTRIBUTE().EDITOR_IGNORE_ATTRIBUTE().NAME(name).PUBLIC();
	FIELD().ANIMATABLE_ATTRIBUTE().EDITOR_IGNORE_ATTRIBUTE().NAME(layerName).PUBLIC();
	FIELD().ANIMATABLE_ATTRIBUTE().EDITOR_IGNORE_ATTRIBUTE().NAME(enabled).PUBLIC();
	FIELD().NAME(enabledInHierarchy).PUBLIC();
	FIELD().NAME(children).PUBLIC();
	FIELD().NAME(components).PUBLIC();
	FIELD().NAME(child).PUBLIC();
	FIELD().NAME(component).PUBLIC();
	FIELD().EDITOR_IGNORE_ATTRIBUTE().NAME(tags).PUBLIC();
	FIELD().ANIMATABLE_ATTRIBUTE().EDITOR_IGNORE_ATTRIBUTE().NAME(transform).PUBLIC();
	FIELD().NAME(mId).PROTECTED();
	FIELD().SERIALIZABLE_ATTRIBUTE().NAME(mName).PROTECTED();
	FIELD().NAME(mPrototype).PROTECTED();
	FIELD().DEFAULT_VALUE(nullptr).NAME(mPrototypeLink).PROTECTED();
	FIELD().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(String("Default")).NAME(mLayerName).PROTECTED();
	FIELD().DEFAULT_VALUE(nullptr).NAME(mLayer).PROTECTED();
	FIELD().DEFAULT_VALUE(nullptr).NAME(mParent).PROTECTED();
	FIELD().NAME(mChildren).PROTECTED();
	FIELD().NAME(mComponents).PROTECTED();
	FIELD().DEFAULT_VALUE(true).NAME(mEnabled).PROTECTED();
	FIELD().DEFAULT_VALUE(true).NAME(mResEnabled).PROTECTED();
	FIELD().DEFAULT_VALUE(true).NAME(mResEnabledInHierarchy).PROTECTED();
	FIELD().DEFAULT_VALUE(SceneStatus::NotInScene).NAME(mSceneStatus).PROTECTED();
	FIELD().DEFAULT_VALUE(false).NAME(mIsAsset).PROTECTED();
	FIELD().NAME(mAssetId).PROTECTED();
	FIELD().NAME(mReferences).PROTECTED();
	FIELD().DEFAULT_VALUE(nullptr).NAME(mCopyVisitor).PROTECTED();
	FIELD().EDITOR_IGNORE_ATTRIBUTE().NAME(locked).PUBLIC();
	FIELD().NAME(lockedInHierarchy).PUBLIC();
	FIELD().NAME(onEnableChanged).PUBLIC();
	FIELD().NAME(onChanged).PUBLIC();
	FIELD().NAME(onParentChanged).PUBLIC();
	FIELD().NAME(onChildHierarchyChanged).PUBLIC();
	FIELD().NAME(onLockChanged).PUBLIC();
	FIELD().NAME(onNameChanged).PUBLIC();
	FIELD().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(false).NAME(mLocked).PROTECTED();
	FIELD().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(false).NAME(mResLocked).PROTECTED();
}
END_META;
CLASS_METHODS_META(o2::Actor)
{

	typedef const Map<const Actor*, Actor*>& _tmp1;
	typedef const Map<const Component*, Component*>& _tmp2;
	typedef Map<String, Actor*> _tmp3;
	typedef Map<String, Component*> _tmp4;
	typedef Map<const Actor*, Actor*>& _tmp5;
	typedef Map<const Component*, Component*>& _tmp6;

	PUBLIC_FUNCTION(void, Update, float);
	PUBLIC_FUNCTION(void, FixedUpdate, float);
	PUBLIC_FUNCTION(void, UpdateChildren, float);
	PUBLIC_FUNCTION(void, FixedUpdateChildren, float);
	PUBLIC_FUNCTION(void, UpdateTransform);
	PUBLIC_FUNCTION(void, UpdateSelfTransform);
	PUBLIC_FUNCTION(void, UpdateChildrenTransforms);
	PUBLIC_FUNCTION(void, SetName, const String&);
	PUBLIC_FUNCTION(const String&, GetName);
	PUBLIC_FUNCTION(SceneUID, GetID);
	PUBLIC_FUNCTION(void, SetID, SceneUID);
	PUBLIC_FUNCTION(void, GenerateNewID, bool);
	PUBLIC_FUNCTION(UID, GetAssetID);
	PUBLIC_FUNCTION(bool, IsAsset);
	PUBLIC_FUNCTION(void, SetPrototype, ActorAssetRef);
	PUBLIC_FUNCTION(ActorAssetRef, GetPrototype);
	PUBLIC_FUNCTION(ActorAssetRef, GetPrototypeDirectly);
	PUBLIC_FUNCTION(ActorRef, GetPrototypeLink);
	PUBLIC_FUNCTION(void, AddToScene);
	PUBLIC_FUNCTION(void, RemoveFromScene, bool);
	PUBLIC_FUNCTION(bool, IsOnScene);
	PUBLIC_FUNCTION(void, SetEnabled, bool);
	PUBLIC_FUNCTION(void, Enable);
	PUBLIC_FUNCTION(void, Disable);
	PUBLIC_FUNCTION(bool, IsEnabled);
	PUBLIC_FUNCTION(bool, IsResEnabled);
	PUBLIC_FUNCTION(bool, IsEnabledInHierarchy);
	PUBLIC_FUNCTION(void, SetParent, Actor*, bool);
	PUBLIC_FUNCTION(Actor*, GetParent);
	PUBLIC_FUNCTION(void, SetIndexInSiblings, int);
	PUBLIC_FUNCTION(Actor*, AddChild, Actor*);
	PUBLIC_FUNCTION(void, AddChildren, const Vector<Actor*>&);
	PUBLIC_FUNCTION(Actor*, AddChild, Actor*, int);
	PUBLIC_FUNCTION(Actor*, GetChild, const String&);
	PUBLIC_FUNCTION(Actor*, FindChild, const String&);
	PUBLIC_FUNCTION(Actor*, FindChild, const Function<bool(const Actor* child)>&);
	PUBLIC_FUNCTION(const Vector<Actor*>&, GetChildren);
	PUBLIC_FUNCTION(void, GetAllChildrenActors, Vector<Actor*>&);
	PUBLIC_FUNCTION(void, RemoveChild, Actor*, bool);
	PUBLIC_FUNCTION(void, RemoveAllChildren, bool);
	PUBLIC_FUNCTION(Actor*, FindActorById, SceneUID);
	PUBLIC_FUNCTION(Component*, AddComponent, Component*);
	PUBLIC_FUNCTION(void, RemoveComponent, Component*, bool);
	PUBLIC_FUNCTION(void, RemoveAllComponents);
	PUBLIC_FUNCTION(Component*, GetComponent, const String&);
	PUBLIC_FUNCTION(Component*, GetComponent, const Type*);
	PUBLIC_FUNCTION(Component*, GetComponent, SceneUID);
	PUBLIC_FUNCTION(const Vector<Component*>&, GetComponents);
	PUBLIC_FUNCTION(void, SetLayer, const String&);
	PUBLIC_FUNCTION(SceneLayer*, GetLayer);
	PUBLIC_FUNCTION(const String&, GetLayerName);
	PUBLIC_FUNCTION(void, SerializeBasicOverride, DataValue&);
	PUBLIC_FUNCTION(void, DeserializeBasicOverride, const DataValue&);
	PUBLIC_STATIC_FUNCTION(void, SetDefaultCreationMode, ActorCreateMode);
	PUBLIC_STATIC_FUNCTION(ActorCreateMode, GetDefaultCreationMode);
	PUBLIC_STATIC_FUNCTION(bool, IsModeOnScene, ActorCreateMode);
	PUBLIC_STATIC_FUNCTION(String, GetCreateMenuCategory);
	PUBLIC_STATIC_FUNCTION(String, GetCreateMenuGroup);
	PROTECTED_FUNCTION(void, CopyFields, Vector<const FieldInfo*>&, IObject*, IObject*, Vector<Actor**>&, Vector<Component**>&, Vector<ISerializable*>&);
	PROTECTED_FUNCTION(void, CollectFixingFields, Component*, Vector<Component**>&, Vector<Actor**>&);
	PROTECTED_FUNCTION(void, GetComponentFields, Component*, Vector<const FieldInfo*>&);
	PROTECTED_FUNCTION(void, FixComponentFieldsPointers, const Vector<Actor**>&, const Vector<Component**>&, _tmp1, _tmp2);
	PROTECTED_FUNCTION(void, UpdateResEnabled);
	PROTECTED_FUNCTION(void, UpdateResEnabledInHierarchy);
	PROTECTED_FUNCTION(void, SerializeRaw, DataValue&);
	PROTECTED_FUNCTION(void, DeserializeRaw, const DataValue&);
	PROTECTED_FUNCTION(void, SerializeWithProto, DataValue&);
	PROTECTED_FUNCTION(void, DeserializeWithProto, const DataValue&);
	PROTECTED_FUNCTION(_tmp3, GetAllChilds);
	PROTECTED_FUNCTION(_tmp4, GetAllComponents);
	PROTECTED_FUNCTION(void, SetParentProp, Actor*);
	PROTECTED_FUNCTION(void, OnAddToScene);
	PROTECTED_FUNCTION(void, OnRemoveFromScene);
	PROTECTED_FUNCTION(void, OnStart);
	PROTECTED_FUNCTION(void, OnUpdate, float);
	PROTECTED_FUNCTION(void, OnFixedUpdate, float);
	PROTECTED_FUNCTION(void, OnEnabled);
	PROTECTED_FUNCTION(void, OnDisabled);
	PROTECTED_FUNCTION(void, OnEnableInHierarchyChanged);
	PROTECTED_FUNCTION(void, OnTransformUpdated);
	PROTECTED_FUNCTION(void, OnTransformChanged);
	PROTECTED_FUNCTION(void, OnParentChanged, Actor*);
	PROTECTED_FUNCTION(void, OnChildAdded, Actor*);
	PROTECTED_FUNCTION(void, OnChildRemoved, Actor*);
	PROTECTED_FUNCTION(void, OnLayerChanged, SceneLayer*);
	PROTECTED_FUNCTION(void, OnComponentAdded, Component*);
	PROTECTED_FUNCTION(void, OnComponentRemoving, Component*);
	PUBLIC_FUNCTION(void, SetLocked, bool);
	PUBLIC_FUNCTION(void, Lock);
	PUBLIC_FUNCTION(void, Unlock);
	PUBLIC_FUNCTION(bool, IsLocked);
	PUBLIC_FUNCTION(bool, IsLockedInHierarchy);
	PUBLIC_FUNCTION(void, BreakPrototypeLink);
	PUBLIC_FUNCTION(void, ApplyChangesToPrototype);
	PUBLIC_FUNCTION(void, RevertToPrototype);
	PUBLIC_FUNCTION(ActorAssetRef, MakePrototype);
	PUBLIC_FUNCTION(bool, IsLinkedToActor, Actor*);
	PUBLIC_FUNCTION(Actor*, FindLinkedActor, Actor*);
	PUBLIC_FUNCTION(const SceneEditableObject*, GetEditableLink);
	PUBLIC_FUNCTION(Vector<SceneEditableObject*>, GetEditableChildren);
	PUBLIC_FUNCTION(SceneEditableObject*, GetEditableParent);
	PUBLIC_FUNCTION(void, SetEditableParent, SceneEditableObject*);
	PUBLIC_FUNCTION(void, AddEditableChild, SceneEditableObject*, int);
	PUBLIC_FUNCTION(bool, IsSupportsDisabling);
	PUBLIC_FUNCTION(bool, IsSupportsLocking);
	PUBLIC_FUNCTION(bool, IsSupportsTransforming);
	PUBLIC_FUNCTION(Basis, GetTransform);
	PUBLIC_FUNCTION(void, SetTransform, const Basis&);
	PUBLIC_FUNCTION(bool, IsSupportsPivot);
	PUBLIC_FUNCTION(void, SetPivot, const Vec2F&);
	PUBLIC_FUNCTION(Vec2F, GetPivot);
	PUBLIC_FUNCTION(void, OnChanged);
	PUBLIC_FUNCTION(void, OnLockChanged);
	PUBLIC_FUNCTION(void, OnNameChanged);
	PUBLIC_FUNCTION(void, OnChildrenChanged);
	PROTECTED_FUNCTION(void, CopyActorChangedFields, Actor*, Actor*, Actor*, Vector<Actor*>&, bool);
	PROTECTED_FUNCTION(void, SeparateActors, Vector<Actor*>&);
	PROTECTED_FUNCTION(void, ProcessReverting, Actor*, const Actor*, const Vector<Actor*>&, Vector<Actor**>&, Vector<Component**>&, _tmp5, _tmp6, Vector<ISerializable*>&);
	PROTECTED_FUNCTION(void, GetDifferences, ActorDifferences&);
	PROTECTED_FUNCTION(void, BeginMakePrototype);
	PROTECTED_FUNCTION(void, BeginInstantiatePrototype);
	PROTECTED_FUNCTION(void, SetProtytypeDummy, ActorAssetRef);
	PROTECTED_FUNCTION(void, UpdateLocking);
}
END_META;

#pragma once

#include "o2/Animation/AnimationClip.h"
#include "o2/Animation/AnimationState.h"
#include "o2/Animation/Editor/EditableAnimation.h"
#include "o2/Animation/Tracks/AnimationTrack.h"
#include "o2/Scene/Component.h"
#include "o2/Utils/Debug/Debug.h"
#include "o2/Utils/Editor/Attributes/DefaultTypeAttribute.h"
#include "o2/Utils/Editor/Attributes/DontDeleteAttribute.h"
#include "o2/Utils/Editor/Attributes/InvokeOnChangeAttribute.h"

namespace o2
{
	// -------------------
	// Animation component
	// -------------------
	class AnimationComponent: public Component, public IEditableAnimation
	{
	public:
		// Default constructor
		AnimationComponent();

		// Copy-constructor
		AnimationComponent(const AnimationComponent& other);

		// Destructor
		~AnimationComponent();

		// Copy-operator
		AnimationComponent& operator=(const AnimationComponent& other);

		// Updates animations, blendings and assigning blended values
		void Update(float dt) override;

		// Adds new animation state and returns him
		AnimationState* AddState(AnimationState* state);

		// Adds new animation state and returns him
		AnimationState* AddState(const String& name, const AnimationClip& animation,
								 const AnimationMask& mask, float weight);

		// Adds new animation state and returns him
		AnimationState* AddState(const String& name);

		// Removes animation state
		void RemoveState(AnimationState* state);

		// Removes animation state by name
		void RemoveState(const String& name);

		// Removes all animation states
		void RemoveAllStates();

		// Returns state with specified name. Returns nullptr if can't find state with specified name
		AnimationState* GetState(const String& name);

		// Returns all states array
		const Vector<AnimationState*>& GetStates() const;

		// Creates new state and plays him
		AnimationState* Play(const AnimationClip& animation, const String& name);

		// Creates new state and plays him
		AnimationState* Play(const AnimationClip& animation);

		// Searches animation state with name and plays him
		AnimationState* Play(const String& name);

		// Creates new state, and blends animation with duration
		AnimationState* BlendTo(const AnimationClip& animation, const String& name, float duration = 1.0f);

		// Creates new state, and blends animation with duration
		AnimationState* BlendTo(const AnimationClip& animation, float duration = 1.0f);

		// Creates new state, and blends animation with duration
		AnimationState* BlendTo(const String& name, float duration = 1.0f);

		// Plays state and blends animation with duration
		AnimationState* BlendTo(AnimationState* state, float duration = 1.0f);

		// Stops animation with name
		void Stop(const String& animationName);

		// Stops all states
		void StopAll();

		// It is called when animation started to edit. It means that animation must be deactivated
		void BeginAnimationEdit() override;

		// It is called when animation finished editing. ANimation must be reactivated
		void EndAnimationEdit() override;

		// Returns name of component
		static String GetName();

		// Returns category of component
		static String GetCategory();

		// Returns name of component icon
		static String GetIcon();

		SERIALIZABLE(AnimationComponent);

	protected:
		// -------------------------------
		// Value assigning agent interface
		// -------------------------------
		struct ITrackMixer
		{
			// Value path
			String path;

		public:
			virtual ~ITrackMixer() {}

			// Updates value and blend
			virtual void Update() = 0;

			// Removes Animation track from agent
			virtual void RemoveTrack(IAnimationTrack::IPlayer* track) = 0;

			// Returns is agent hasn't no values
			virtual bool IsEmpty() const = 0;
		};

		// ------------------------------
		// Template value assigning agent
		// ------------------------------
		template<typename _type>
		struct TrackMixer: public ITrackMixer
		{
		public:
			Vector<Pair<AnimationState*, typename AnimationTrack<_type>::Player*>> tracks; // Animation tracks associated with animation states
			
			IValueProxy<_type>* target = nullptr; // Target value proxy

		public:
			// Destructor
			~TrackMixer();

			// Updates value and blend
			void Update() override;

			// Removes Animation track from agent
			void RemoveTrack(IAnimationTrack::IPlayer* track) override;

			// Returns is agent hasn't no values
			bool IsEmpty() const override;
		};

		// ----------------------
		// Blend state parameters
		// ----------------------
		struct BlendState
		{
			Vector<AnimationState*> blendOffStates;           // Turning off states
			AnimationState*         blendOnState = nullptr;   // Turning on state
			float                   duration;                  // Blending duration
			float                   time = -1.0f;              // Current blending remaining time

			// Updates work weight by time
			void Update(float dt);
		};

	protected:
		Vector<AnimationState*> mStates; // Animation states array @SERIALIZABLE @EDITOR_PROPERTY @DEFAULT_TYPE(o2::AnimationState) @DONT_DELETE @INVOKE_ON_CHANGE(OnStatesListChanged)
		Vector<ITrackMixer*>    mValues; // Assigning value agents

		BlendState mBlend;  // Current blend parameters

		bool mInEditMode = false; // True when some state animation is editing now, disables update

	protected:
		// Registers value by path and state
		template<typename _type>
		void RegTrack(typename AnimationTrack< _type >::Player* player, const String& path, AnimationState* state);

		// Removes Animation track from agent by path
		void UnregTrack(IAnimationTrack::IPlayer* player, const String& path);

		// It is called when new track added in animation state, registers track player in mixer
		void OnStateAnimationTrackAdded(AnimationState* state, IAnimationTrack::IPlayer* player);

		// It is called when track is removing from animation state, unregisters track player from mixer
		void OnStateAnimationTrackRemoved(AnimationState* state, IAnimationTrack::IPlayer* player);

		// It is called from editor, refreshes states
		void OnStatesListChanged();

		friend class AnimationClip;
		friend class AnimationState;
		friend class IAnimationTrack;

		template<typename _type>
		friend class AnimationTrack;
	};

	template<typename _type>
	void AnimationComponent::RegTrack(typename AnimationTrack<_type>::Player* player, const String& path, AnimationState* state)
	{
		for (auto val : mValues)
		{
			if (val->path == path)
			{
				auto* agent = dynamic_cast<TrackMixer<_type>*>(val);

				if (!agent)
				{
					o2Debug.LogWarning("Different track types at: " + path);
					return;
				}

				agent->tracks.Add({ state, player });
				return;
			}
		}

		auto* newAgent = mnew TrackMixer <_type>();
		mValues.Add(newAgent);
		newAgent->path = path;
		newAgent->tracks.Add({ state, player });

		const FieldInfo* fieldInfo = nullptr;
		auto fieldPtr = (_type*)GetType().GetFieldPtr(mOwner, path, fieldInfo);

		if (!fieldInfo)
		{
			o2Debug.LogWarning("Can't animate value " + path + ": can't find field");
			return;
		}

		newAgent->target = dynamic_cast<IValueProxy<_type>*>(fieldInfo->GetType()->GetValueProxy(fieldPtr));
	}

	template<typename _type>
	AnimationComponent::TrackMixer<_type>::~TrackMixer()
	{
		if (target)
			delete target;
	}

	template<typename _type>
	bool AnimationComponent::TrackMixer<_type>::IsEmpty() const
	{
		return tracks.IsEmpty();
	}

	template<typename _type>
	void AnimationComponent::TrackMixer<_type>::RemoveTrack(IAnimationTrack::IPlayer* value)
	{
		tracks.RemoveAll([&](const auto& x) { return x.second == value; });
	}

	template<>
	void AnimationComponent::TrackMixer<bool>::Update();

	template<>
	void AnimationComponent::TrackMixer<int>::Update();

	template<typename _type>
	void AnimationComponent::TrackMixer<_type>::Update()
	{
		AnimationState* firstValueState = tracks[0].first;
		AnimationTrack<_type>::Player* firstValue = tracks[0].second;

		float weightsSum = firstValueState->mWeight*firstValueState->blend*firstValueState->mask.GetNodeWeight(path);
		_type valueSum = firstValue->GetValue();

		for (int i = 1; i < tracks.Count(); i++)
		{
			AnimationState* valueState = tracks[i].first;
			AnimationTrack<_type>::Player* value = tracks[i].second;

			weightsSum += valueState->mWeight*valueState->blend*valueState->mask.GetNodeWeight(path);
			valueSum += value->GetValue();
		}

		_type resValue = valueSum / weightsSum;
		target->SetValue(resValue);
	}
}

CLASS_BASES_META(o2::AnimationComponent)
{
	BASE_CLASS(o2::Component);
	BASE_CLASS(o2::IEditableAnimation);
}
END_META;
CLASS_FIELDS_META(o2::AnimationComponent)
{
	FIELD().DONT_DELETE_ATTRIBUTE().EDITOR_PROPERTY_ATTRIBUTE().SERIALIZABLE_ATTRIBUTE().NAME(mStates).PROTECTED();
	FIELD().NAME(mValues).PROTECTED();
	FIELD().NAME(mBlend).PROTECTED();
	FIELD().DEFAULT_VALUE(false).NAME(mInEditMode).PROTECTED();
}
END_META;
CLASS_METHODS_META(o2::AnimationComponent)
{

	PUBLIC_FUNCTION(void, Update, float);
	PUBLIC_FUNCTION(AnimationState*, AddState, AnimationState*);
	PUBLIC_FUNCTION(AnimationState*, AddState, const String&, const AnimationClip&, const AnimationMask&, float);
	PUBLIC_FUNCTION(AnimationState*, AddState, const String&);
	PUBLIC_FUNCTION(void, RemoveState, AnimationState*);
	PUBLIC_FUNCTION(void, RemoveState, const String&);
	PUBLIC_FUNCTION(void, RemoveAllStates);
	PUBLIC_FUNCTION(AnimationState*, GetState, const String&);
	PUBLIC_FUNCTION(const Vector<AnimationState*>&, GetStates);
	PUBLIC_FUNCTION(AnimationState*, Play, const AnimationClip&, const String&);
	PUBLIC_FUNCTION(AnimationState*, Play, const AnimationClip&);
	PUBLIC_FUNCTION(AnimationState*, Play, const String&);
	PUBLIC_FUNCTION(AnimationState*, BlendTo, const AnimationClip&, const String&, float);
	PUBLIC_FUNCTION(AnimationState*, BlendTo, const AnimationClip&, float);
	PUBLIC_FUNCTION(AnimationState*, BlendTo, const String&, float);
	PUBLIC_FUNCTION(AnimationState*, BlendTo, AnimationState*, float);
	PUBLIC_FUNCTION(void, Stop, const String&);
	PUBLIC_FUNCTION(void, StopAll);
	PUBLIC_FUNCTION(void, BeginAnimationEdit);
	PUBLIC_FUNCTION(void, EndAnimationEdit);
	PUBLIC_STATIC_FUNCTION(String, GetName);
	PUBLIC_STATIC_FUNCTION(String, GetCategory);
	PUBLIC_STATIC_FUNCTION(String, GetIcon);
	PROTECTED_FUNCTION(void, UnregTrack, IAnimationTrack::IPlayer*, const String&);
	PROTECTED_FUNCTION(void, OnStateAnimationTrackAdded, AnimationState*, IAnimationTrack::IPlayer*);
	PROTECTED_FUNCTION(void, OnStateAnimationTrackRemoved, AnimationState*, IAnimationTrack::IPlayer*);
	PROTECTED_FUNCTION(void, OnStatesListChanged);
}
END_META;

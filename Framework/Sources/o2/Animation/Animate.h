#pragma once
#include "o2/Animation/AnimationClip.h"
#include "o2/Animation/Tracks/AnimationFloatTrack.h"
#include "o2/Animation/Tracks/AnimationTrack.h"
#include "o2/Animation/Tracks/AnimationVec2FTrack.h"
#include "o2/Animation/Tracks/AnimationColor4Track.h"

namespace o2
{
	// --------------------------------------------
	// Class for building simple animation sequence
	// --------------------------------------------
	class Animate
	{
	public:
		// Constructor. Takes object as parameter
		Animate(IObject& object);

		// Destructor
		~Animate();

		// Animation cast operator. Needs to store as animation
		operator AnimationClip() const;

		// Inserts delay for seconds
		Animate& Wait(float seconds);

		// Applies stored transformations after seconds
		Animate& For(float seconds);

		// Splits sequence
		Animate& Then();

		// Moves object in (x,y)
		Animate& Move(float x, float y);

		// Moves object into position
		Animate& Move(const Vec2F& point);

		// Moves object by path from points
		Animate& Move(const Vector<Vec2F>& points);

		// Changes alpha
		Animate& Alpha(float alpha);

		// Shows object
		Animate& Show();

		// Hides object
		Animate& Hide();

		// Sets color
		Animate& Color(const Color4& color);

		// Sets color
		Animate& Color(int r, int g, int b, int a);

		// Sets scale
		Animate& Scale(float scale);

		// Sets scale
		Animate& Scale(const Vec2F& scale);

		// Rotates object
		Animate& Rotate(float angle);

		// Sets animation looped
		Animate& Looped();

		// Sets pin pong loop
		Animate& PingPong();

		// Changes specified parameter
		template<typename T>
		Animate& Change(T* target, const T& value);

	protected:
		// -----------------------
		// Key container interface
		// -----------------------
		struct IKeyContainer
		{
			// Virtual destructor
			virtual ~IKeyContainer() {}

			// Applies stored key to animation
			virtual void Apply(float time) = 0;
		};

		// ----------------------
		// Template key container
		// ----------------------
		template<typename T>
		struct KeyContainer: public IKeyContainer
		{
			typename AnimationTrack<T>::Key key;

			AnimationTrack<T>* animatedValue;

			~KeyContainer() {}

			void Apply(float time)
			{
				key.position = time;
				animatedValue->AddKey(key);
			}
		};

		// ---------------
		// Vec2F container
		// ---------------
		template<>
		struct KeyContainer<Vec2F>: public IKeyContainer
		{
			Curve::Key timeKey;

			AnimationTrack<Vec2F>* animatedValue;

			~KeyContainer() {}

			void Apply(float time)
			{
				timeKey.value = 0.0f;
				animatedValue->timeCurve.InsertKey(timeKey);

				timeKey.position = time;
				timeKey.value = 1.0f;
				animatedValue->timeCurve.InsertKey(timeKey);
			}
		};


	protected:
		IObject*               mTarget = nullptr;    // Target animating object
		AnimationClip          mAnimation;           // Building animation
		bool                   mKeysApplied = false; // Is stored keys was applied
		float                  mTime = 0.0f;         // Current sequence time
		Vector<IKeyContainer*> mKeyContainers;       // Stored keys that applies in For()
		Function<void()>       mFunction;            // Stored callback that applies in For()

		AnimationTrack<Color4>* mColorAnimatedValue = nullptr;    // Color Animation track, stores when needs
		AnimationTrack<Vec2F>*  mPositionAnimatedValue = nullptr; // Position Animation track, stores when needs
		AnimationTrack<Vec2F>*  mScaleAnimatedValue = nullptr;    // Scale Animation track, stores when needs
		AnimationTrack<float>*  mRotationAnimatedValue = nullptr; // Rotation Animation track, stores when needs

	protected:
		// Checks color Animation track: creates them if needed
		void CheckColorAnimatedValue();

		// Checks position Animation track: creates them if needed
		void CheckPositionAnimatedvalue();

		// Checks scale Animation track: creates them if needed
		void CheckScaleAnimatedValue();

		// Checks rotate Animation track: creates them if needed
		void CheckRotateAnimatedValue();

		// Checks applied keys: if keys was applied, clears keys containers
		void CheckAppliedKeys();

		// Applies keys and function to animation at current time
		void ApplyKeys();
	};

	template<typename T>
	Animate& Animate::Change(T* target, const T& value)
	{
		CheckAppliedKeys();

		KeyContainer<T>* container = mnew KeyContainer<T>();
		container->animatedValue = GetAnimatedValue(target);
		container->key.value = value;
		mKeyContainers.Add(container);

		return *this;
	}

}

#pragma once

#include "AnimationTrack.h"
#include "o2/Utils/Math/Vector2.h"

namespace o2
{
	// --------------------
	// Animated Vec2F value
	// --------------------
	template<>
	class AnimationTrack<o2::Vec2F>: public IAnimationTrack
	{
	public:
		typedef o2::Vec2F ValueType;
		class Key;

	public:
		PROPERTIES(AnimationTrack<o2::Vec2F>);
		PROPERTY(Vector<Key>, keys, SetKeys, GetKeysNonContant); // Keys property

	public:
		// Default constructor
		AnimationTrack();

		// Copy-constructor
		AnimationTrack(const AnimationTrack<Vec2F>& other);

		// Assign operator
		AnimationTrack<Vec2F>& operator=(const AnimationTrack<Vec2F>& other);

		// Returns value at time
		Vec2F GetValue(float position) const;

		// Returns value at time
		Vec2F GetValue(float position, bool direction, int& cacheKey, int& cacheKeyApprox) const;

		// It is called when beginning keys batch change. After this call all keys modifications will not be update pproximation
		// Used for optimizing many keys change
		void BeginKeysBatchChange() override;

		// It is called when keys batch change completed. Updates approximation
		void CompleteKeysBatchingChange() override;

		// Returns track duration
		float GetDuration() const override;

		// Creates track-type specific player
		IPlayer* CreatePlayer() const override;

		// Adds key with smoothing
		void AddKeys(Vector<Key> keys, float smooth = 1.0f);

		// Adds single key
		int AddKey(const Key& key);

		// Adds key at position
		int AddKey(const Key& key, float position);

		// Adds and smooths key
		int AddSmoothKey(const Key& key, float smooth);

		// Adds key
		int AddKey(float position, const Vec2F& value, const Vec2F& prevSupport, const Vec2F& nextSupport,
				   float leftCoef, float leftCoefPosition, float rightCoef, float rightCoefPosition);

		// Adds key at position with value and smoothing
		int AddKey(float position, const Vec2F& value, float smooth = 1.0f);

		// Removes key at position
		bool RemoveKey(float position);

		// Removes key by index
		bool RemoveKeyAt(int idx);

		// Removes all keys
		void RemoveAllKeys();

		// Returns true if animation contains key at position
		bool ContainsKey(float position) const;

		// Returns keys array
		const Vector<Key>& GetKeys() const;

		// Sets key at position
		void SetKey(int idx, const Key& key);

		// Returns key at position
		Key GetKey(float position) const;

		// Returns key at index
		Key GetKeyAt(int idx) const;

		// Returns key by uid
		Key FindKey(UInt64 uid) const;

		// Returns key index by uid
		int FindKeyIdx(UInt64 uid) const;

		// Sets keys
		void SetKeys(const Vector<Key>& keys);

		// Smooths key at position
		void SmoothKey(float position, float smooth);

		// Returns key by position
		Key operator[](float position) const;

		// Returns parametric specified Animation track
		// Sample: Parametric(someBegin, someEnd, 1.0f, 0.0f, 0.4f, 1.0f, 0.6f) 
		static AnimationTrack<Vec2F> Parametric(const Vec2F& begin, const Vec2F& end, float duration,
												float beginCoef, float beginCoefPosition,
												float endCoef, float endCoefPosition);

		// Returns tween animation from begin to end in duration with ease in
		static AnimationTrack<Vec2F> EaseIn(const Vec2F& begin, const Vec2F& end, float duration = 1.0f);

		// Returns tween animation from begin to end in duration with ease out
		static AnimationTrack<Vec2F> EaseOut(const Vec2F& begin, const Vec2F& end, float duration = 1.0f);

		// Returns tween animation from begin to end in duration with ease in-out
		static AnimationTrack<Vec2F> EaseInOut(const Vec2F& begin, const Vec2F& end, float duration = 1.0f);

		// Returns tween animation from begin to end in duration with linear transition
		static AnimationTrack<Vec2F> Linear(const Vec2F& begin, const Vec2F& end, float duration = 1.0f);

		SERIALIZABLE(AnimationTrack<Vec2F>);

	public:
		// ----------------------
		// Animation track player
		// ----------------------
		class Player: public IPlayer
		{
		public:
			PROPERTIES(Player);
			GETTER(o2::Vec2F, value, GetValue);                           // Current value getter
			SETTER(o2::Vec2F*, target, SetTarget);                        // Bind target setter
			SETTER(Function<void()>, targetDelegate, SetTargetDelegate);  // Bind target change event setter  
			SETTER(IValueProxy<o2::Vec2F>*, targetProxy, SetTargetProxy); // Bind proxy setter

		public:
			// Default constructor
			Player();

			// Destructor
			~Player();

			// Value type cast operator
			operator Vec2F() const;

			// Sets target pointer
			void SetTarget(Vec2F* value);

			// Sets target pointer and change event
			void SetTarget(Vec2F* value, const Function<void()>& changeEvent);

			// Sets target change event
			void SetTargetDelegate(const Function<void()>& changeEvent) override;

			// Sets target property pointer
			void SetTargetProxy(IValueProxy<Vec2F>* setter);

			// Sets animation track
			void SetTrack(AnimationTrack<Vec2F>* track);

			// Returns animation track
			AnimationTrack<Vec2F>* GetTrackT() const;

			// Sets target by void pointer
			void SetTargetVoid(void* target) override;

			// Sets target by void pointer and change event
			void SetTargetVoid(void* target, const Function<void()>& changeEvent) override;

			// Sets target property by void pointer
			void SetTargetProxyVoid(void* target) override;

			// Sets animation track
			void SetTrack(IAnimationTrack* track) override;

			// Returns animation track
			IAnimationTrack* GetTrack() const override;

			// Returns current value
			Vec2F GetValue() const;

			IOBJECT(Player);

		protected:
			AnimationTrack<Vec2F>* mTrack = nullptr; // Animation track

			Vec2F mCurrentValue;                // Current animation track

			float mPrevInDurationTime = 0.0f; // Previous evaluation in duration time
			int   mPrevKey = 0;               // Previous evaluation key index
			int   mPrevKeyApproximation = 0;  // Previous evaluation key approximation index

			Vec2F*              mTarget = nullptr;      // Animation target value pointer
			Function<void()>    mTargetDelegate;        // Animation target value change event
			IValueProxy<Vec2F>* mTargetProxy = nullptr; // Animation target proxy pointer

		protected:
			// Evaluates value
			void Evaluate() override;

			// Registering this in animatable value agent
			void RegMixer(AnimationState* state, const String& path) override;
		};

	public:
		// -------------
		// Animation key
		// -------------
		class Key: public ISerializable
		{
		public:
			UInt64 uid;                  // Random unique id @SERIALIZABLE
			float  position;		     // Position on time line, in seconds @SERIALIZABLE
			Vec2F  value;			     // Value @SERIALIZABLE
			Vec2F  prevSupportValue;     // Previous animation segment support value @SERIALIZABLE
			Vec2F  nextSupportValue;     // Next animation segment support value @SERIALIZABLE
			float  leftSupportValue;	 // Transition curve coefficient for previous animation segment @SERIALIZABLE
			float  leftSupportPosition;  // Transition curve coefficient position for previous animation segment (must be in 0...1) @SERIALIZABLE
			float  rightSupportValue;	 // Transition curve coefficient for next animation segment @SERIALIZABLE
			float  rightSupportPosition; // Transition curve coefficient position for next animation segment (must be in 0...1) @SERIALIZABLE

		public:
			// Default constructor
			Key();

			// Constructor from value
			Key(const Vec2F& value);

			// Constructor from position and value
			Key(float position, const Vec2F& value);

			// Constructor
			Key(float position, const Vec2F& value, const Vec2F& prevSupportValue, const Vec2F& nextSupportValue,
				float leftSupportValue, float leftSupportPosition, float rightSupportValue, float rightSupportPosition);

			// Copy-constructor
			Key(const Key& other);

			// Assign operator from other key
			Key& operator=(const Key& other);

			// Assign operator from value
			Key& operator=(const Vec2F& value);

			// Value type cast operator
			operator Vec2F() const;

			// Equals operator
			bool operator==(const Key& other) const;

			SERIALIZABLE(Key);

		public:
			static const int   mApproxValuesCount = 10;                // Transition curve approximation values count
			Vec2F              mApproxValues[mApproxValuesCount];      // Approximation values
			ApproximationValue mCurveApproxValues[mApproxValuesCount]; // Transition curve approximation values
			float              mApproxLengths[mApproxValuesCount];     // Approximation segments lengths
			float              mApproxTotalLength;                     // Total animation segment length (sum of mApproxLengths)
		};

	protected:
		bool mBatchChange = false; // It is true when began batch change
		bool mChangedKeys = false; // It is true when some keys changed during batch change

		Vector<Key> mKeys; // Animation keys @SERIALIZABLE

	protected:
		// Returns keys (for property)
		Vector<Key> GetKeysNonContant();

		// Updates keys approximation
		void UpdateApproximation();

		// Completion deserialization callback
		void OnDeserialized(const DataValue& node) override;
	};
}

CLASS_BASES_META(o2::AnimationTrack<o2::Vec2F>)
{
	BASE_CLASS(o2::IAnimationTrack);
}
END_META;
CLASS_FIELDS_META(o2::AnimationTrack<o2::Vec2F>)
{
	FIELD().NAME(keys).PUBLIC();
	FIELD().DEFAULT_VALUE(false).NAME(mBatchChange).PROTECTED();
	FIELD().DEFAULT_VALUE(false).NAME(mChangedKeys).PROTECTED();
	FIELD().SERIALIZABLE_ATTRIBUTE().NAME(mKeys).PROTECTED();
}
END_META;
CLASS_METHODS_META(o2::AnimationTrack<o2::Vec2F>)
{

	PUBLIC_FUNCTION(Vec2F, GetValue, float);
	PUBLIC_FUNCTION(Vec2F, GetValue, float, bool, int&, int&);
	PUBLIC_FUNCTION(void, BeginKeysBatchChange);
	PUBLIC_FUNCTION(void, CompleteKeysBatchingChange);
	PUBLIC_FUNCTION(float, GetDuration);
	PUBLIC_FUNCTION(IPlayer*, CreatePlayer);
	PUBLIC_FUNCTION(void, AddKeys, Vector<Key>, float);
	PUBLIC_FUNCTION(int, AddKey, const Key&);
	PUBLIC_FUNCTION(int, AddKey, const Key&, float);
	PUBLIC_FUNCTION(int, AddSmoothKey, const Key&, float);
	PUBLIC_FUNCTION(int, AddKey, float, const Vec2F&, const Vec2F&, const Vec2F&, float, float, float, float);
	PUBLIC_FUNCTION(int, AddKey, float, const Vec2F&, float);
	PUBLIC_FUNCTION(bool, RemoveKey, float);
	PUBLIC_FUNCTION(bool, RemoveKeyAt, int);
	PUBLIC_FUNCTION(void, RemoveAllKeys);
	PUBLIC_FUNCTION(bool, ContainsKey, float);
	PUBLIC_FUNCTION(const Vector<Key>&, GetKeys);
	PUBLIC_FUNCTION(void, SetKey, int, const Key&);
	PUBLIC_FUNCTION(Key, GetKey, float);
	PUBLIC_FUNCTION(Key, GetKeyAt, int);
	PUBLIC_FUNCTION(Key, FindKey, UInt64);
	PUBLIC_FUNCTION(int, FindKeyIdx, UInt64);
	PUBLIC_FUNCTION(void, SetKeys, const Vector<Key>&);
	PUBLIC_FUNCTION(void, SmoothKey, float, float);
	PUBLIC_STATIC_FUNCTION(AnimationTrack<Vec2F>, Parametric, const Vec2F&, const Vec2F&, float, float, float, float, float);
	PUBLIC_STATIC_FUNCTION(AnimationTrack<Vec2F>, EaseIn, const Vec2F&, const Vec2F&, float);
	PUBLIC_STATIC_FUNCTION(AnimationTrack<Vec2F>, EaseOut, const Vec2F&, const Vec2F&, float);
	PUBLIC_STATIC_FUNCTION(AnimationTrack<Vec2F>, EaseInOut, const Vec2F&, const Vec2F&, float);
	PUBLIC_STATIC_FUNCTION(AnimationTrack<Vec2F>, Linear, const Vec2F&, const Vec2F&, float);
	PROTECTED_FUNCTION(Vector<Key>, GetKeysNonContant);
	PROTECTED_FUNCTION(void, UpdateApproximation);
	PROTECTED_FUNCTION(void, OnDeserialized, const DataValue&);
}
END_META;

CLASS_BASES_META(o2::AnimationTrack<o2::Vec2F>::Player)
{
	BASE_CLASS(IPlayer);
}
END_META;
CLASS_FIELDS_META(o2::AnimationTrack<o2::Vec2F>::Player)
{
	FIELD().NAME(value).PUBLIC();
	FIELD().NAME(target).PUBLIC();
	FIELD().NAME(targetDelegate).PUBLIC();
	FIELD().NAME(targetProxy).PUBLIC();
	FIELD().DEFAULT_VALUE(nullptr).NAME(mTrack).PROTECTED();
	FIELD().NAME(mCurrentValue).PROTECTED();
	FIELD().DEFAULT_VALUE(0.0f).NAME(mPrevInDurationTime).PROTECTED();
	FIELD().DEFAULT_VALUE(0).NAME(mPrevKey).PROTECTED();
	FIELD().DEFAULT_VALUE(0).NAME(mPrevKeyApproximation).PROTECTED();
	FIELD().DEFAULT_VALUE(nullptr).NAME(mTarget).PROTECTED();
	FIELD().NAME(mTargetDelegate).PROTECTED();
	FIELD().DEFAULT_VALUE(nullptr).NAME(mTargetProxy).PROTECTED();
}
END_META;
CLASS_METHODS_META(o2::AnimationTrack<o2::Vec2F>::Player)
{

	PUBLIC_FUNCTION(void, SetTarget, Vec2F*);
	PUBLIC_FUNCTION(void, SetTarget, Vec2F*, const Function<void()>&);
	PUBLIC_FUNCTION(void, SetTargetDelegate, const Function<void()>&);
	PUBLIC_FUNCTION(void, SetTargetProxy, IValueProxy<Vec2F>*);
	PUBLIC_FUNCTION(void, SetTrack, AnimationTrack<Vec2F>*);
	PUBLIC_FUNCTION(AnimationTrack<Vec2F>*, GetTrackT);
	PUBLIC_FUNCTION(void, SetTargetVoid, void*);
	PUBLIC_FUNCTION(void, SetTargetVoid, void*, const Function<void()>&);
	PUBLIC_FUNCTION(void, SetTargetProxyVoid, void*);
	PUBLIC_FUNCTION(void, SetTrack, IAnimationTrack*);
	PUBLIC_FUNCTION(IAnimationTrack*, GetTrack);
	PUBLIC_FUNCTION(Vec2F, GetValue);
	PROTECTED_FUNCTION(void, Evaluate);
	PROTECTED_FUNCTION(void, RegMixer, AnimationState*, const String&);
}
END_META;

CLASS_BASES_META(o2::AnimationTrack<o2::Vec2F>::Key)
{
	BASE_CLASS(o2::ISerializable);
}
END_META;
CLASS_FIELDS_META(o2::AnimationTrack<o2::Vec2F>::Key)
{
	FIELD().SERIALIZABLE_ATTRIBUTE().NAME(uid).PUBLIC();
	FIELD().SERIALIZABLE_ATTRIBUTE().NAME(position).PUBLIC();
	FIELD().SERIALIZABLE_ATTRIBUTE().NAME(value).PUBLIC();
	FIELD().SERIALIZABLE_ATTRIBUTE().NAME(prevSupportValue).PUBLIC();
	FIELD().SERIALIZABLE_ATTRIBUTE().NAME(nextSupportValue).PUBLIC();
	FIELD().SERIALIZABLE_ATTRIBUTE().NAME(leftSupportValue).PUBLIC();
	FIELD().SERIALIZABLE_ATTRIBUTE().NAME(leftSupportPosition).PUBLIC();
	FIELD().SERIALIZABLE_ATTRIBUTE().NAME(rightSupportValue).PUBLIC();
	FIELD().SERIALIZABLE_ATTRIBUTE().NAME(rightSupportPosition).PUBLIC();
	FIELD().NAME(mApproxValues).PUBLIC();
	FIELD().NAME(mCurveApproxValues).PUBLIC();
	FIELD().NAME(mApproxLengths).PUBLIC();
	FIELD().NAME(mApproxTotalLength).PUBLIC();
}
END_META;
CLASS_METHODS_META(o2::AnimationTrack<o2::Vec2F>::Key)
{
}
END_META;

#pragma once

#include "o2/Utils/Types/CommonTypes.h"
#include "o2/Utils/Serialization/Serializable.h"
#include "o2/Utils/Types/String.h"

namespace o2
{
	class ShortcutKeys: public ISerializable
	{
	public:
		bool        control; // @SERIALIZABLE
		bool        shift;	 // @SERIALIZABLE
		bool        alt;	 // @SERIALIZABLE
		KeyboardKey key;	 // @SERIALIZABLE

	public:
		ShortcutKeys();

		ShortcutKeys(KeyboardKey key, bool control = false, bool shift = false, bool alt = false);

		bool IsPressed() const;
		bool IsDown() const;
		String AsString() const;
		bool IsEmpty() const;

		bool operator==(const ShortcutKeys& other) const;
		bool operator<(const ShortcutKeys& other) const;

		SERIALIZABLE(ShortcutKeys);
	};
}

CLASS_BASES_META(o2::ShortcutKeys)
{
	BASE_CLASS(o2::ISerializable);
}
END_META;
CLASS_FIELDS_META(o2::ShortcutKeys)
{
	FIELD().SERIALIZABLE_ATTRIBUTE().NAME(control).PUBLIC();
	FIELD().SERIALIZABLE_ATTRIBUTE().NAME(shift).PUBLIC();
	FIELD().SERIALIZABLE_ATTRIBUTE().NAME(alt).PUBLIC();
	FIELD().SERIALIZABLE_ATTRIBUTE().NAME(key).PUBLIC();
}
END_META;
CLASS_METHODS_META(o2::ShortcutKeys)
{

	PUBLIC_FUNCTION(bool, IsPressed);
	PUBLIC_FUNCTION(bool, IsDown);
	PUBLIC_FUNCTION(String, AsString);
	PUBLIC_FUNCTION(bool, IsEmpty);
}
END_META;

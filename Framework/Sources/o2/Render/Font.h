#pragma once

#include "3rdPartyLibs/FreeType/include/ft2build.h"
#include FT_FREETYPE_H

#include "o2/Render/TextureRef.h"
#include "o2/Utils/Types/CommonTypes.h"
#include "o2/Utils/Types/Containers/Vector.h"
#include "o2/Utils/Function.h"
#include "o2/Utils/Math/Rect.h"
#include "o2/Utils/Math/Vector2.h"
#include "o2/Utils/Types/String.h"

namespace o2
{
	class Mesh;
	class Render;
	class FontRef;

	// -----------------------------------------------------------
	// Font. Containing array of symbol glyphs, symbol index table
	// -----------------------------------------------------------
	class Font
	{
	protected:
		struct Character;

	public:
		Function<void()> onCharactersRebuilt;

	public:
		// Default constructor
		Font();

		// Copy-constructor
		Font(const Font& font);

		// Destructor
		virtual ~Font();

		// Returns base height in pixels for font with size
		virtual float GetHeightPx(int height) const;

		// Returns line height in pixels for font with size
		virtual float GetLineHeightPx(int height) const;

		// Returns character constant reference by id
		virtual const Character& GetCharacter(UInt16 id, int height);

		// Checks characters for preloading
		virtual void CheckCharacters(const WString& needChararacters, int height);

		// Returns font file name
		virtual String GetFileName() const;

	protected:
		// --------------------
		// Character definition
		// --------------------
		struct Character
		{
			RectF  mTexSrc;  // texture source rect
			Vec2F  mSize;    // Size of source rect
			Vec2F  mOrigin;  // Symbol origin point
			float  mAdvance; // Symbol advance
			UInt16 mId;      // Character id
			int    mHeight;  // Character height

			bool operator==(const Character& other) const;
		};

	protected:
		Vector<FontRef*>  mRefs; // Array of reference to this font

		Map<int, Map<UInt16, Character>> mCharacters; // Characters map, int - height, uint16 - id

		TextureRef mTexture;        // Texture
		RectI      mTextureSrcRect; // Texture source rectangle

		bool mReady; // True when font is ready to use

	protected:
		// Adds character and registers in cache map
		void AddCharacter(const Character& character);

		friend class Text;
		friend class FontRef;
		friend class Render;
	};
}

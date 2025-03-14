#pragma once

#include "o2/Utils/Math/Border.h"
#include "o2/Utils/Math/Rect.h"
#include "o2/Utils/Math/Vector2.h"
#include "o2/Utils/Serialization/Serializable.h"

namespace o2
{
	class Layout: public ISerializable
	{
	public:
		Vec2F anchorMin = Vec2F(0, 0); // @SERIALIZABLE
		Vec2F anchorMax = Vec2F(1, 1); // @SERIALIZABLE
		Vec2F offsetMin = Vec2F(0, 0); // @SERIALIZABLE
		Vec2F offsetMax = Vec2F(0, 0); // @SERIALIZABLE

		Layout();
		Layout(const Vec2F& anchorMin, const Vec2F& anchorMax, const Vec2F& offsetMin, const Vec2F& offsetMax);

		RectF Calculate(const RectF& source);

		bool operator==(const Layout& other) const;
		bool operator!=(const Layout& other) const;

		static Layout BothStretch(const BorderF& border);
		static Layout BothStretch(float borderLeft = 0, float borderBottom = 0, float borderRight = 0, float borderTop = 0);
		static Layout Based(BaseCorner corner, const Vec2F& size, const Vec2F& offset = Vec2F());
		static Layout HorStretch(VerAlign align, float left, float right, float height, float offsY = 0.0f);
		static Layout VerStretch(HorAlign align, float top, float bottom, float width, float offsX = 0.0f);

		SERIALIZABLE(Layout);
	};
}

CLASS_BASES_META(o2::Layout)
{
	BASE_CLASS(o2::ISerializable);
}
END_META;
CLASS_FIELDS_META(o2::Layout)
{
	FIELD().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(Vec2F(0, 0)).NAME(anchorMin).PUBLIC();
	FIELD().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(Vec2F(1, 1)).NAME(anchorMax).PUBLIC();
	FIELD().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(Vec2F(0, 0)).NAME(offsetMin).PUBLIC();
	FIELD().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(Vec2F(0, 0)).NAME(offsetMax).PUBLIC();
}
END_META;
CLASS_METHODS_META(o2::Layout)
{

	PUBLIC_FUNCTION(RectF, Calculate, const RectF&);
	PUBLIC_STATIC_FUNCTION(Layout, BothStretch, const BorderF&);
	PUBLIC_STATIC_FUNCTION(Layout, BothStretch, float, float, float, float);
	PUBLIC_STATIC_FUNCTION(Layout, Based, BaseCorner, const Vec2F&, const Vec2F&);
	PUBLIC_STATIC_FUNCTION(Layout, HorStretch, VerAlign, float, float, float, float);
	PUBLIC_STATIC_FUNCTION(Layout, VerStretch, HorAlign, float, float, float, float);
}
END_META;

#pragma once

#include "o2/Utils/Function.h"
#include "o2/Utils/Math/Rect.h"
#include "o2/Utils/Math/Vector2.h"

namespace o2
{
	// -------------------------------
	// Basic drawable object interface
	// -------------------------------
	class IDrawable
	{
	public:
		Function<void()> onDraw; // Draw event

	public:
		virtual ~IDrawable() {}

		// Draws drawable
		virtual void Draw();

		// Returns true if point is under drawable
		virtual bool IsUnderPoint(const Vec2F& point);

	protected:
		RectF mDrawingScissorRect; // Scissor rectangle at last drawing

	protected:
		// It is called when drawable was drawn. Storing render scissor rect, calling onDraw callback
		virtual void OnDrawn();
	};
}

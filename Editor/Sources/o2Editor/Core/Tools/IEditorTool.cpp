#include "o2Editor/stdafx.h"
#include "IEditorTool.h"

#include "o2/Utils/Reflection/Reflection.h"
#include "o2/Utils/Serialization/DataValue.h"
#include "o2Editor/SceneWindow/SceneEditScreen.h"

namespace Editor
{
	Toggle* IEditTool::GetPanelToggle() const
	{
		if (!mPanelToggle)
			mPanelToggle = CreatePanelToggle();

		return mPanelToggle;
	}

	Toggle* IEditTool::CreatePanelToggle() const
	{
		Toggle* toggle = mnew Toggle();
		toggle->layout->minSize = Vec2F(20, 20);
		toggle->shortcut = GetShortcut();
		auto rootLayer = toggle->AddLayer("root", nullptr);
		auto selectLayer = rootLayer->AddChildLayer("hover", nullptr);
		auto iconLayer = selectLayer->AddChildLayer("regular", mnew Sprite(GetPanelIcon()),
													Layout::Based(BaseCorner::Center, Vec2F(20, 20)));

		auto pressedIconLayer = selectLayer->AddChildLayer("pressed", mnew Sprite(GetPanelIcon()),
														   Layout::Based(BaseCorner::Center, Vec2F(20, 20)));

		toggle->AddState("hover", AnimationClip::EaseInOut("layer/root/child/hover/transparency", 1.0f, 0.5f, 0.1f))
			->offStateAnimationSpeed = 0.25f;

		toggle->AddState("pressed", AnimationClip::EaseInOut("layer/root/child/hover/child/pressed/transparency", 0.0f, 1.0f, 0.05f))
			->offStateAnimationSpeed = 0.5f;

		toggle->AddState("visible", AnimationClip::EaseInOut("transparency", 0.0f, 1.0f, 0.2f));

		toggle->AddState("value", AnimationClip::EaseInOut("layer/root/transparency", 0.3f, 1.0f, 0.1f));

		toggle->onToggle = [&](bool v) { if (v) o2EditorSceneScreen.SelectTool(this); };

		return toggle;
	}

	String IEditTool::GetPanelIcon() const
	{
		return "ui/UI4_select_tool.png";
	}

	ShortcutKeys IEditTool::GetShortcut() const
	{
		return ShortcutKeys();
	}

}

DECLARE_CLASS(Editor::IEditTool);

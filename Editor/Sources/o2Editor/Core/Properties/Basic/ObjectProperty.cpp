#include "o2Editor/stdafx.h"
#include "ObjectProperty.h"

#include "o2/Scene/UI/UIManager.h"
#include "o2/Scene/UI/Widgets/Button.h"
#include "o2/Scene/UI/Widgets/Spoiler.h"
#include "o2/Utils/Editor/Attributes/NoHeaderAttribute.h"
#include "o2/Utils/Editor/EditorScope.h"
#include "o2Editor/Core/Properties/IObjectPropertiesViewer.h"
#include "o2Editor/Core/Properties/Properties.h"

using namespace o2;

namespace Editor
{
	ObjectProperty::ObjectProperty()
	{
		InitializeControls();
	}

	ObjectProperty::ObjectProperty(const ObjectProperty& other) :
		IPropertyField(other)
	{
		InitializeControls();
	}

	ObjectProperty& ObjectProperty::operator=(const ObjectProperty& other)
	{
		IPropertyField::operator=(other);
		InitializeControls();
		return *this;
	}

	void ObjectProperty::OnFreeProperty()
	{
		if (mObjectViewer)
		{
			RemoveChild(mObjectViewer->GetSpoiler(), false);
			o2EditorProperties.FreeObjectViewer(mObjectViewer);
		}

		mObjectViewer = nullptr;
	}

	void ObjectProperty::InitializeControls()
	{
		expandHeight = true;
		expandWidth = true;
		fitByChildren = true;
	}

	void ObjectProperty::SetValueAndPrototypeProxy(const TargetsVec& targets)
	{
		for (auto& pair : mTargetObjects)
		{
			if (pair.first.isCreated)
				delete pair.first.data;

			if (pair.second.isCreated)
				delete pair.second.data;
		}

		mValuesProxies = targets;
		mTargetObjects.Clear();

		for (auto& pair : targets)
			mTargetObjects.Add({ GetObjectFromProxy(pair.first), GetObjectFromProxy(pair.second) });

		Refresh();
	}

	void ObjectProperty::Refresh()
	{
		PushEditorScopeOnStack scope;

		for (auto& pair : mTargetObjects)
		{
			pair.first.Refresh();
			pair.second.Refresh();
		}

		CheckViewer();

		if (mObjectViewer)
		{
			mObjectViewer->Refresh(mTargetObjects.Convert<Pair<IObject*, IObject*>>(
				[&](const Pair<TargetObjectData, TargetObjectData>& x)
			{
				return Pair<IObject*, IObject*>(x.first.data, x.second.data);
			}));
		}

		SetEnabled(mObjectViewer && !mObjectViewer->IsEmpty() || !mNoHeader);
	}

	void ObjectProperty::CheckViewer()
	{
		PushEditorScopeOnStack scope;

		const Type* objectType = nullptr;
		if (!mTargetObjects.IsEmpty())
		{
			auto object = mTargetObjects[0].first.data;
			if (object)
				objectType = &object->GetType();
		}

		const Type* prevObjectType = mObjectViewer ? mObjectViewer->GetViewingObjectType() : nullptr;
		if (objectType != prevObjectType)
		{
			if (mObjectViewer)
				o2EditorProperties.FreeObjectViewer(mObjectViewer);

			if (objectType)
			{
				mObjectViewer = o2EditorProperties.CreateObjectViewer(objectType, mValuesPath, onChangeCompleted, 
																	  onChanged);

				AddChild(mObjectViewer->GetSpoiler());
				mObjectViewer->SetParentContext(mParentContext);
				mObjectViewer->SetHeaderEnabled(!mNoHeader);
				mObjectViewer->SetExpanded(mExpanded); 
				mObjectViewer->GetSpoiler()->SetCaption(mCaption);
			}
		}
	}

	const Type* ObjectProperty::GetValueType() const
	{
		return GetValueTypeStatic();
	}

	const Type* ObjectProperty::GetValueTypeStatic()
	{
		return &TypeOf(IObject);
	}

	void ObjectProperty::SetFieldInfo(const FieldInfo* fieldInfo)
	{
		IPropertyField::SetFieldInfo(fieldInfo);

		if (fieldInfo)
		{
			mExpanded = fieldInfo->HasAttribute<ExpandedByDefaultAttribute>();
			mNoHeader = fieldInfo->HasAttribute<NoHeaderAttribute>();
		}
	}

	void ObjectProperty::SetCaption(const WString& text)
	{
		mCaption = text;

		if (mObjectViewer)
			mObjectViewer->GetSpoiler()->SetCaption(mCaption);;
	}

	WString ObjectProperty::GetCaption() const
	{
		return mCaption;
	}

	Button* ObjectProperty::GetRemoveButton()
	{
		if (!mRemoveBtn)
		{
			mRemoveBtn = o2UI.CreateWidget<Button>("remove small");
			*mRemoveBtn->layout = WidgetLayout::Based(BaseCorner::RightTop, Vec2F(20, 20), Vec2F(2, 0));
			AddInternalWidget(mRemoveBtn);
		}

		return mRemoveBtn;
	}

	void ObjectProperty::Expand()
	{
		SetExpanded(true);
	}

	void ObjectProperty::Collapse()
	{
		SetExpanded(false);
	}

	void ObjectProperty::SetExpanded(bool expanded)
	{
		mExpanded = expanded;

		if (mObjectViewer)
			mObjectViewer->GetSpoiler()->SetExpanded(expanded);
	}

	bool ObjectProperty::IsExpanded() const
	{
		if (mObjectViewer)
			return mObjectViewer->GetSpoiler()->IsExpanded();

		return mExpanded;
	}

	ObjectProperty::TargetObjectData ObjectProperty::GetObjectFromProxy(IAbstractValueProxy* proxy)
	{
		TargetObjectData res;

		if (!proxy)
			return res;

		res.proxy = proxy;

		const ObjectType& objectType = dynamic_cast<const ObjectType&>(proxy->GetType());
		if (auto pointerProxy = dynamic_cast<IPointerValueProxy*>(proxy))
		{
			res.data = objectType.DynamicCastToIObject(pointerProxy->GetValueVoidPointer());
			res.isCreated = false;
		}
		else
		{
			void* sample = objectType.CreateSample();
			proxy->GetValuePtr(sample);

			res.data = objectType.DynamicCastToIObject(sample);
			res.isCreated = true;
		}

		return res;
	}

	void ObjectProperty::OnPropertyChanged(const String& path, const Vector<DataDocument>& before, 
										   const Vector<DataDocument>& after)
	{
		for (auto& pair : mTargetObjects)
			pair.first.SetValue();

		onChangeCompleted(path, before, after);
	}

	void ObjectProperty::TargetObjectData::Refresh()
	{
		if (!isCreated)
			return;

		const ObjectType& objectType = dynamic_cast<const ObjectType&>(proxy->GetType());
		proxy->GetValuePtr(objectType.DynamicCastFromIObject(data));
	}

	void ObjectProperty::TargetObjectData::SetValue()
	{
		if (!isCreated)
			return;

		const ObjectType& objectType = dynamic_cast<const ObjectType&>(proxy->GetType());
		proxy->SetValuePtr(objectType.DynamicCastFromIObject(data));
	}

}

DECLARE_CLASS(Editor::ObjectProperty);

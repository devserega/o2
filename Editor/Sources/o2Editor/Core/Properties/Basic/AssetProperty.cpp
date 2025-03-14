#include "o2Editor/stdafx.h"
#include "AssetProperty.h"

#include "o2/Utils/StringUtils.h"
#include "o2Editor/AssetsWindow/AssetIcon.h"
#include "o2Editor/Core/Dialogs/System/OpenSaveDialog.h"
#include "o2Editor/Core/Properties/ObjectViewer.h"
#include "o2Editor/Core/Properties/Properties.h"

namespace Editor
{
	AssetProperty::AssetProperty()
	{}

	AssetProperty::AssetProperty(const AssetProperty& other):
		TPropertyField(other)
	{
		InitializeControls();
	}

	AssetProperty& AssetProperty::operator=(const AssetProperty& other)
	{
		TPropertyField::operator =(other);
		InitializeControls();
		return *this;
	}

	void AssetProperty::InitializeControls()
	{
		mSpoiler = GetChildByType<Spoiler>("spoiler");
		mHeaderContainer = mSpoiler->GetInternalWidgetByType<HorizontalLayout>("mainLayout/container/layout");
		mCaption = mSpoiler->GetInternalWidgetByType<Label>("mainLayout/propertyName");

		auto expandSpoilerBtn = mSpoiler->GetExpandButton();
		auto mainLayout = mSpoiler->GetInternalWidget("mainLayout");
		expandSpoilerBtn->isPointInside = [=](const Vec2F& p) {
			return expandSpoilerBtn->layout->IsPointInside(p) || mainLayout->layout->IsPointInside(p);
		};

		mBox = mSpoiler->GetInternalWidget("mainLayout/container/layout/box");
		if (mBox)
		{
			mBox->SetFocusable(true);
			mBox->onDraw += MakeFunction<DragDropArea, void>(this, &DragDropArea::OnDrawn);

			mNameText = mBox->GetLayerDrawable<Text>("caption");
			if (mNameText)
				mNameText->text = "--";
		}

		mCreateInstanceBtn = mSpoiler->GetInternalWidgetByType<Button>("mainLayout/container/layout/create");
		if (mCreateInstanceBtn)
			mCreateInstanceBtn->onClick = THIS_FUNC(OnCreateInstancePressed);

		auto saveInstanceBtn = mSpoiler->GetInternalWidgetByType<Button>("mainLayout/container/layout/save");
		if (saveInstanceBtn)
			saveInstanceBtn->onClick = THIS_FUNC(OnSaveInstancePressed);

		auto removeInstanceBtn = mSpoiler->GetInternalWidgetByType<Button>("mainLayout/container/layout/remove");
		if (removeInstanceBtn)
			removeInstanceBtn->onClick = THIS_FUNC(OnRemoveInstancePressed);
	}

	void AssetProperty::UpdateValueView()
	{
		if (mCreateInstanceBtn)
			mCreateInstanceBtn->SetEnabled(mAvailableToHaveInstance && !mValuesDifferent);

		bool allAreInstance = false;

		if (!mValuesDifferent)
		{
			if (!mCommonValue)
			{
				mNameText->text = "Null";
				if (mAssetType)
					mNameText->text += WString(": " + GetSmartName(mAssetType->GetName()));

				mBox->layer["caption"]->transparency = 0.5f;
			}
			else
			{
				auto name = o2FileSystem.GetFileNameWithoutExtension(
					o2FileSystem.GetPathWithoutDirectories(mCommonValue->GetPath()));

				mNameText->text = name;
				mBox->layer["caption"]->transparency = 1.0f;

				if (mAvailableToHaveInstance)
				{
					Vector<IObject*> targets;

					allAreInstance = true;
					for (auto proxy : mValuesProxies)
					{
						auto proxyType = dynamic_cast<const ObjectType*>(&proxy.first->GetType());
						if (auto ptrProxy = dynamic_cast<IPointerValueProxy*>(proxy.first))
						{
							void* rawAssetRefPtr = ptrProxy->GetValueVoidPointer();
							if (AssetRef* refPtr = dynamic_cast<AssetRef*>(proxyType->DynamicCastToIObject(rawAssetRefPtr)))
							{
								if (refPtr->IsInstance())
									targets.Add(refPtr->Get());
								else
								{
									allAreInstance = false;
									break;
								}
							}
						}
					}

					if (allAreInstance)
					{
						if (!mAssetObjectViewer)
						{
							mAssetObjectViewer = mnew ObjectViewer();
							mAssetObjectViewer->SetParentContext(mParentContext);
							mSpoiler->AddChild(mAssetObjectViewer);
						}

						mCreateInstanceBtn->SetEnabledForcible(false);
						mAssetObjectViewer->SetEnabled(true);
						mAssetObjectViewer->Refresh(targets);
					}
					else if (mAssetObjectViewer)
					{
						mAssetObjectViewer->SetEnabled(false);
					}
				}
			}
		}
		else
		{
			mNameText->text = "--";
			mBox->layer["caption"]->transparency = 1.0f;
		}

		SetState("instance", mAvailableToHaveInstance && allAreInstance && !mValuesDifferent);
	}

	bool AssetProperty::IsAlwaysRefresh() const
	{
		return false;
	}

	void AssetProperty::SetAssetId(const UID& id)
	{
		mCommonValue = id == 0 ? AssetRef() : AssetRef(id);

		for (auto ptr : mValuesProxies)
			SetProxy(ptr.first, mCommonValue);

		SetCommonAssetId(id);
	}

	void AssetProperty::SetAssetType(const Type* assetType)
	{
		mAssetType = assetType;
		mAvailableToHaveInstance = assetType->InvokeStatic<bool>("IsReferenceCanOwnInstance");
	}

	void AssetProperty::SetFieldInfo(const FieldInfo* fieldInfo)
	{
		IPropertyField::SetFieldInfo(fieldInfo);
	}

	void AssetProperty::SetCaption(const WString& text)
	{
		if (mCaption)
			mCaption->SetText(text);
	}

	WString AssetProperty::GetCaption() const
	{
		if (mCaption)
			return mCaption->GetText();

		return WString();
	}

	Button* AssetProperty::GetRemoveButton()
	{
		if (!mRemoveBtn)
		{
			mRemoveBtn = o2UI.CreateWidget<Button>("remove small");
			mRemoveBtn->layout->maxWidth = 20;
			mRemoveBtn->layout->minHeight = 20;
			mHeaderContainer->AddChild(mRemoveBtn, 0);
		}

		return mRemoveBtn;
	}

	void AssetProperty::SetCommonAssetId(const UID& id)
	{
		mCommonValue = id == 0 ? AssetRef() : AssetRef(id);
		mValuesDifferent = false;

		UpdateValueView();
		CheckRevertableState();
		OnValueChanged();
	}

	void AssetProperty::SetAssetIdByUser(const UID& id)
	{
		StoreValues(mBeforeChangeValues);
		SetAssetId(id);
		CheckValueChangeCompleted();
	}

	void AssetProperty::OnCreateInstancePressed()
	{
		SetState("instance", true);

		for (auto proxy : mValuesProxies)
		{
			auto proxyType = dynamic_cast<const ObjectType*>(&proxy.first->GetType());
			if (auto ptrProxy = dynamic_cast<IPointerValueProxy*>(proxy.first))
			{
				void* rawAssetRefPtr = ptrProxy->GetValueVoidPointer();
				if (AssetRef* refPtr = dynamic_cast<AssetRef*>(proxyType->DynamicCastToIObject(rawAssetRefPtr)))
					refPtr->CreateInstance();
			}
		}

		Refresh();
		mSpoiler->Expand();
	}

	void AssetProperty::OnRemoveInstancePressed()
	{
		SetState("instance", false);
		mSpoiler->Collapse();

		for (auto proxy : mValuesProxies)
		{
			auto proxyType = dynamic_cast<const ObjectType*>(&proxy.first->GetType());
			if (auto ptrProxy = dynamic_cast<IPointerValueProxy*>(proxy.first))
			{
				void* rawAssetRefPtr = ptrProxy->GetValueVoidPointer();
				if (AssetRef* refPtr = dynamic_cast<AssetRef*>(proxyType->DynamicCastToIObject(rawAssetRefPtr)))
					refPtr->RemoveInstance();
			}
		}

		Refresh();
	}

	void AssetProperty::OnSaveInstancePressed()
	{
		String assetTypeName = GetSmartName(mAssetType->GetName());
		String extesions = mAssetType->InvokeStatic<const char*>("GetFileExtensions");
		auto extension = extesions.Split(" ")[0];
		String defaultPath = o2Application.GetBinPath() + "\\" + o2Assets.GetAssetsPath().ReplacedAll("/", "\\");

		String path = GetSaveFileNameDialog("Save asset", { { assetTypeName, "*." + extension } },
											defaultPath);
		if (path.IsEmpty()) {
			return;
		}

		String relativePath = o2FileSystem.CanonicalizePath(o2FileSystem.GetPathRelativeToPath(defaultPath, path));
		relativePath.ReplaceAll("\\", "/");

		auto asset = GetProxy(mValuesProxies[0].first);
		asset->Save(relativePath + "." + extension);
	}

	void AssetProperty::OnTypeSpecialized(const Type& type)
	{
		SetAssetType(type.InvokeStatic<const Type*>("GetAssetTypeStatic"));
	}

	AssetRef AssetProperty::GetProxy(IAbstractValueProxy* proxy) const
	{
		auto proxyType = dynamic_cast<const ObjectType*>(&proxy->GetType());
		auto proxySample = proxyType->CreateSample();
		proxy->GetValuePtr(proxySample);
		auto objectSample = proxyType->DynamicCastToIObject(proxySample);
		AssetRef* assetSample = dynamic_cast<AssetRef*>(objectSample);
		AssetRef res = *assetSample;
		delete assetSample;
		return res;
	}

	void AssetProperty::SetProxy(IAbstractValueProxy* proxy, const AssetRef& value)
	{
		auto proxyType = dynamic_cast<const ObjectType*>(&proxy->GetType());
		auto proxySample = proxyType->CreateSample();
		auto objectSample = proxyType->DynamicCastToIObject(proxySample);
		AssetRef* assetSample = dynamic_cast<AssetRef*>(objectSample);
		*assetSample = value;
		proxy->SetValuePtr(proxySample);
		delete assetSample;
	}

	void AssetProperty::OnCursorPressed(const Input::Cursor& cursor)
	{
		o2UI.FocusWidget(mBox);

		if (mCommonValue)
			o2EditorAssets.ShowAssetIcon(mCommonValue->GetPath());
	}

	void AssetProperty::OnCursorExit(const Input::Cursor& cursor)
	{
		mBox->SetState("select", false);
	}

	void AssetProperty::OnCursorEnter(const Input::Cursor& cursor)
	{
		mBox->SetState("select", true);
	}

	void AssetProperty::OnKeyPressed(const Input::Key& key)
	{
		if (mBox && mBox->IsFocused() && (key == VK_DELETE || key == VK_BACK))
			SetAssetIdByUser(0);
	}

	bool AssetProperty::IsUnderPoint(const Vec2F& point)
	{
		return mBox->IsUnderPoint(point) && mBox->transparency > 0.1f;
	}

	void AssetProperty::OnDragExit(ISelectableDragableObjectsGroup* group)
	{
		o2Application.SetCursor(CursorType::Arrow);
		mBox->SetState("focused", false);
	}

	void AssetProperty::OnDragEnter(ISelectableDragableObjectsGroup* group)
	{
		auto assetIconsScroll = dynamic_cast<AssetsIconsScrollArea*>(group);
		if (!assetIconsScroll)
			return;

		if (mAssetType)
		{
			auto lastSelectedAsset = assetIconsScroll->GetSelectedAssets().Last();
			if (!lastSelectedAsset->meta->GetAssetType()->IsBasedOn(*mAssetType))
				return;
		}

		o2Application.SetCursor(CursorType::Hand);
		mBox->SetState("focused", true);
	}

	void AssetProperty::OnDropped(ISelectableDragableObjectsGroup* group)
	{
		auto assetIconsScroll = dynamic_cast<AssetsIconsScrollArea*>(group);
		if (!assetIconsScroll)
			return;

		auto lastSelectedAssetIcon = dynamic_cast<const AssetIcon*>(assetIconsScroll->GetDraggingObject());
		if (!lastSelectedAssetIcon || !lastSelectedAssetIcon->GetAssetInfo().meta->GetAssetType()->IsBasedOn(mCommonValue.GetAssetType()))
			return;

		SetAssetIdByUser(lastSelectedAssetIcon->GetAssetInfo().meta->ID());

		o2Application.SetCursor(CursorType::Arrow);
		mBox->Focus();
	}
}

DECLARE_CLASS_MANUAL(Editor::TPropertyField<AssetRef>);

DECLARE_CLASS(Editor::AssetProperty);

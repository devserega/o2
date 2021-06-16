#include "o2/stdafx.h"
#include "Asset.h"

#include "o2/Assets/Assets.h"
#include "o2/Utils/Debug/Debug.h"
#include "o2/Utils/Debug/Log/LogStream.h"

namespace o2
{
	Asset::Asset()
	{}

	Asset::Asset(const Asset& other):
		mInfo(other.mInfo)
	{}

	Asset::Asset(AssetMeta* meta)
	{
		mInfo.meta = meta;
	}

	Asset& Asset::operator=(const Asset& other)
	{
		mInfo = other.mInfo;
		return *this;
	}

	Asset::~Asset()
	{}

	const String& Asset::GetPath() const
	{
		return mInfo.path;
	}

	void Asset::SetPath(const String& path)
	{
		auto oldPath = mInfo.path;
		auto oldUID = mInfo.meta->mId;

		mInfo.path = path;
		mInfo.meta->mId.Randomize();

		o2Assets.UpdateAssetCache(this, oldPath, oldUID);
	}

	const UID& Asset::GetUID() const
	{
		return mInfo.meta->ID();
	}

	UID& Asset::ID()
	{
		return mInfo.meta->mId;
	}

	AssetMeta* Asset::GetMeta() const
	{
		return mInfo.meta;
	}

	const AssetInfo& Asset::GetInfo() const
	{
		return mInfo;
	}

	void Asset::Load(const String& path)
	{
		auto info = o2Assets.GetAssetInfo(path);

		if (info.meta->mId == 0)
		{
			GetAssetsLogStream()->Error("Failed to load asset by path (" + path + "): asset isn't exist");
			return;
		}

		Load(info);
	}

	void Asset::Load(const UID& id)
	{
		auto& info = o2Assets.GetAssetInfo(id);

		if (info.meta->mId == 0)
		{
			GetAssetsLogStream()->Error("Failed to load asset by UID (" + (WString)id + "): asset isn't exist");
			return;
		}

		Load(info);
	}

	void Asset::Load(const AssetInfo& info)
	{
		mInfo = info;
		LoadData(GetBuiltFullPath());
	}

	void Asset::Save(const String& path, bool rebuildAssetsImmediately /*= true*/)
	{
		SetPath(path);
		Save(rebuildAssetsImmediately);
	}

	void Asset::Save(bool rebuildAssetsImmediately /*= true*/)
	{
		if (ID() == 0)
			ID().Randomize();

		UID destPathAssetId = o2Assets.GetAssetId(mInfo.path);
		if (destPathAssetId != 0 && destPathAssetId != mInfo.meta->mId)
		{
			GetAssetsLogStream()->Error("Failed to save asset (" + mInfo.path + " - " + (WString)mInfo.meta->mId +
										"): another asset exist in this path");
			return;
		}

		DataDocument metaData;
		metaData.Set(mInfo.meta);
		metaData.SaveToFile(GetMetaFullPath());

		SaveData(GetFullPath());

		if (rebuildAssetsImmediately)
			o2Assets.RebuildAssets();
	}

	const char* Asset::GetFileExtensions()
	{
		return "";
	}

	String Asset::GetFullPath() const
	{
		return (mInfo.tree ? mInfo.tree->assetsPath : String()) + mInfo.path;
	}

	String Asset::GetBuiltFullPath() const
	{
		return (mInfo.tree ? mInfo.tree->builtAssetsPath : String()) + mInfo.path;
	}

	String Asset::GetMetaFullPath() const
	{
		return GetFullPath() + ".meta";
	}

	LogStream* Asset::GetAssetsLogStream() const
	{
		return o2Assets.mLog;
	}

	void Asset::SetMeta(AssetMeta* meta)
	{}

	void Asset::LoadData(const String& path)
	{
		DataDocument data;
		data.LoadFromFile(path);
		Deserialize(data);
	}

	void Asset::SaveData(const String& path) const
	{
		DataDocument data;
		Serialize(data);
		data.SaveToFile(path);
	}

}

DECLARE_CLASS(o2::Asset);

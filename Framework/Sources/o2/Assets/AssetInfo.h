#pragma once
#include "o2/Assets/Meta.h"
#include "o2/Utils/Serialization/Serializable.h"
#include "o2/Utils/System/Time/TimeStamp.h"

namespace o2
{
	class AssetsTree;

	// -----------------
	// Asset information
	// -----------------
	struct AssetInfo : public ISerializable
	{
		const AssetsTree* tree = nullptr; // Owner asset tree
		
		String    path;     // Path of asset @SERIALIZABLE
		TimeStamp editTime; // Asset edited time @SERIALIZABLE		

		AssetMeta* meta = nullptr; // Asset meta data @SERIALIZABLE

		AssetInfo*         parent = nullptr;   // Parent asset info
		Vector<AssetInfo*> children;           // Children assets infos @SERIALIZABLE
		bool               ownChildren = true; // Is children assets is owned by this asset info

	public:
		// Default constructor
		AssetInfo();

		// Default constructor with meta
		AssetInfo(AssetMeta* meta);

		// Copy-constructor
		AssetInfo(const AssetInfo& other);

		// Destructor
		~AssetInfo();

		// Copy-operator
		AssetInfo& operator=(const AssetInfo& other);

		// Check equal operator
		bool operator==(const AssetInfo& other) const;

		// Returns is asset info valid - checks id for empty
		operator bool() const;

		// Adds new child node and returns him
		AssetInfo* AddChild(AssetInfo* node);

		// Remove child node and releases him if needs
		void RemoveChild(AssetInfo* node, bool release = true);

		// Sets parent node
		void SetParent(AssetInfo* parent);

		// Sets asset tree, adds in tree's allAssets map
		void SetTree(AssetsTree* tree);

		// Returns is asset valid - checks id for empty
		bool IsValid() const;

		static AssetInfo empty;

		SERIALIZABLE(AssetInfo);

	private:
		// Completion deserialization callback, reads asset type name
		void OnDeserialized(const DataValue& node) override;

		friend class AssetsBuilder;
		friend class AssetsTree;
		friend class Asset;
	};
}

CLASS_BASES_META(o2::AssetInfo)
{
	BASE_CLASS(o2::ISerializable);
}
END_META;
CLASS_FIELDS_META(o2::AssetInfo)
{
	FIELD().DEFAULT_VALUE(nullptr).NAME(tree).PUBLIC();
	FIELD().SERIALIZABLE_ATTRIBUTE().NAME(path).PUBLIC();
	FIELD().SERIALIZABLE_ATTRIBUTE().NAME(editTime).PUBLIC();
	FIELD().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(nullptr).NAME(meta).PUBLIC();
	FIELD().DEFAULT_VALUE(nullptr).NAME(parent).PUBLIC();
	FIELD().SERIALIZABLE_ATTRIBUTE().NAME(children).PUBLIC();
	FIELD().DEFAULT_VALUE(true).NAME(ownChildren).PUBLIC();
}
END_META;
CLASS_METHODS_META(o2::AssetInfo)
{

	PUBLIC_FUNCTION(AssetInfo*, AddChild, AssetInfo*);
	PUBLIC_FUNCTION(void, RemoveChild, AssetInfo*, bool);
	PUBLIC_FUNCTION(void, SetParent, AssetInfo*);
	PUBLIC_FUNCTION(void, SetTree, AssetsTree*);
	PUBLIC_FUNCTION(bool, IsValid);
	PRIVATE_FUNCTION(void, OnDeserialized, const DataValue&);
}
END_META;

#pragma once

#include "CoreInc.h"
#include "IO/File.h"

namespace rapidxml
{
	template<class CharT>
	class xml_node;

	template<class CharT>
	class xml_attribute;

	template<class CharT>
	class xml_document;
}

namespace XE
{
	struct FXmlAttribute;
	
	struct CORE_API FXmlNode
	{
		friend class FXmlDocument;

		FXmlNode() = default;
		FXmlNode(rapidxml::xml_document<Char> * DocumentIn, rapidxml::xml_node<Char> * NodeIn) : Document(DocumentIn), Node(NodeIn)
		{

#ifdef _DEBUG
			DebugName = GetName();
#endif
		}
		bool IsValid() const { return !!Node; }
		explicit operator bool() const { return !!Node; }
		
		FXmlNode GetParent() const;
		FStringView GetName() const;

		FXmlNode GetFirstChild() const;
		FXmlNode GetFirstChild(FStringView Name) const;

		size_t GetChildrenCount() const;
		TVector<FXmlNode> GetChildren() const;
		TVector<FXmlNode> GetChildren(FStringView Name) const;

		TVector<FXmlAttribute> GetAttributes() const;
		TVector<FXmlAttribute> GetAttributes(FStringView Name) const;

		FStringView GetAttribute(FStringView Name, FStringView DefaultValue = FStringView()) const;
		bool SetAttribute(FStringView Name, FStringView Value) const;

		void AppendChild(FXmlNode & Child);
		FXmlNode CreateChild(FStringView Name);

		void RemoveChild(FXmlNode & Child);
		void RemoveAllChildren();

	private:
#ifdef _DEBUG
		FStringView DebugName;
#endif
		rapidxml::xml_document<Char> * Document = nullptr;
		rapidxml::xml_node<Char> * Node = nullptr;
	};

	struct CORE_API FXmlAttribute
	{
		friend class FXmlDocument;

		FXmlAttribute() = default;
		FXmlAttribute(rapidxml::xml_document<Char> * DocumentIn, rapidxml::xml_attribute<Char> * AttributeIn) : Document(DocumentIn), Attribute(AttributeIn)
		{

#ifdef _DEBUG
			DebugName = GetName();
#endif
		}
		bool IsValid() const { return !!Attribute; }
		explicit operator bool() const { return !!Attribute; }

		FXmlNode GetOwner() const;
		FStringView GetName() const;
		FStringView GetValue() const;

	private:
#ifdef _DEBUG
		FStringView DebugName;
#endif
		rapidxml::xml_document<Char> * Document = nullptr;
		rapidxml::xml_attribute<Char> * Attribute = nullptr;
	};


	class CORE_API FXmlDocument
	{
	public:
		FXmlDocument();
		FXmlDocument(FStringView FileName) { LoadFromFile(FileName); }
		~FXmlDocument();

	public:
		FXmlNode GetRootNode() const { return RootNode; }
		FXmlNode FindRootNode(FStringView Name) const;

		FXmlNode CreateRootNode(FStringView Name);

	public:
		bool LoadFromFile(FStringView FileName);
		bool SaveToFile(FStringView FileName);

	private:
		FXmlNode RootNode;
	};

	class CORE_API FXmlAttributes
	{
	public:
		FXmlAttributes() = delete;

	public:
		static FString GetString(FStringView XmlPath, FStringView NodePath, FStringView AttributeName);
		static bool GetBool(FStringView XmlPath, FStringView NodePath, FStringView AttributeName)
		{
			return Strings::ToBool(GetString(XmlPath, NodePath, AttributeName));
		}
	};
}

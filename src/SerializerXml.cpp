//----------------------------------------------------------)
//
//  MSerializerXml.cpp
//
//  Designed by Andrey Martynov
//  mailto:rek@.rsdn.ru
//
//----------------------------------------------------------)


#if !defined(_INC_TCHAR) && !defined(_TCHAR_H_) && !defined(_TCHAR_H)
    #include <tchar.h>
#endif

#if !defined(_FUNCTIONAL_) && !defined(_STLP_FUNCTIONAL) && !defined(__STD_FUNCTIONAL__) && !defined(_CPP_FUNCTIONAL) && !defined(_GLIBCXX_FUNCTIONAL)
    #include <functional>
#endif


#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS  // some CString constructors will be explicit

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN        // Exclude rarely-used stuff from Windows headers
#endif

//#include <afx.h>
//#include <afxwin.h>         // MFC core and standard components
//#include <afxext.h>         // MFC extensions
//#include <afxdtctl.h>     // MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
//#include <afxcmn.h>           // MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS  // some CString constructors will be explicit


#ifndef __ATLBASE_H__
    #include <atlbase.h>
#endif



#if !defined(_VECTOR_) && !defined(_STLP_VECTOR) && !defined(__STD_VECTOR__) && !defined(_CPP_VECTOR) && !defined(_GLIBCXX_VECTOR)
    #include <vector>
#endif

#if !defined(_MAP_) && !defined(_STLP_MAP) && !defined(__STD_MAP__) && !defined(_CPP_MAP) && !defined(_GLIBCXX_MAP)
    #include <map>
#endif

#if !defined(_STRING_) && !defined(_STLP_STRING) && !defined(__STD_STRING) && !defined(_CPP_STRING) && !defined(_GLIBCXX_STRING)
    #include <string>
#endif

#if !defined(_MEMORY_) && !defined(_STLP_MEMORY) && !defined(__STD_MEMORY__) && !defined(_CPP_MEMORY) && !defined(_GLIBCXX_MEMORY)
    #include <memory>
#endif

#if !defined(_ALGORITHM_) && !defined(_STLP_ALGORITHM) && !defined(__STD_ALGORITHM) && !defined(_CPP_ALGORITHM) && !defined(_GLIBCXX_ALGORITHM)
    #include <algorithm>
#endif

#if !defined(_INC_FLOAT) && !defined(_FLOAT_H_) && !defined(_FLOAT_H)
    #include <float.h>
#endif


#include <msxml2.h>
#include "SerializerXml.h"


#ifdef _DEBUG
//#define new DEBUG_NEW
#endif

namespace Rsdn       {
namespace Serializer {
namespace Tree       {
namespace Xml        {

//-----------------------------------------------------------------------------
NodeArrayImpl::NodeArrayImpl(IXMLDOMNodeList* pNodeList)
{
    long length;
    if (FAILED(pNodeList->get_length(&length)))
        throw XmlStorageException();

    for (int idx = 0; idx != length; idx++)
    {
        CComPtr<IXMLDOMNode> pXmlNode;
        if (FAILED(pNodeList->get_item(idx, &pXmlNode)))
            throw XmlStorageException();

        m_nodes.push_back(new NodeImpl(pXmlNode));
    }
}

//-----------------------------------------------------------------------------
NamedNodeListImpl::NamedNodeListImpl(IXMLDOMNodeList* pNodeList)
{
    long length;
    if (FAILED(pNodeList->get_length(&length)))
        throw XmlStorageException();

    for (long idx = 0; idx != length; idx++)
    {
        CComPtr<IXMLDOMNode> pNode; 
        if (FAILED(pNodeList->get_item(idx, &pNode)))
            throw XmlStorageException();

        CComBSTR name; 
        if (FAILED(pNode->get_nodeName(&name)))
            throw XmlStorageException();

        m_mapNamedNodes[tstring(CW2T(name))] = new NodeImpl(pNode);
    }
}

//-----------------------------------------------------------------------------
NamedNodeListImpl::NamedNodeListImpl(IXMLDOMNamedNodeMap* pNodeList)
{
    long length;
    if (FAILED(pNodeList->get_length(&length)))
        throw XmlStorageException();

    for (long idx = 0; idx != length; idx++)
    {
        CComPtr<IXMLDOMNode> pNode; 
        if (FAILED(pNodeList->get_item(idx, &pNode)))
            throw XmlStorageException();

        CComBSTR name; 
        if (FAILED(pNode->get_nodeName(&name)))
            throw XmlStorageException();

        m_mapNamedNodes[tstring(CW2T(name))] = new NodeImpl(pNode);
    }
}

//-----------------------------------------------------------------------------
Serializer::INode* NamedNodeListImpl::GetByName(const tstring& name) const
{
    NamedNodes::const_iterator it = m_mapNamedNodes.find(name);
    if (it == m_mapNamedNodes.end())
        return NULL;
    return it->second;
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
NodeImpl::NodeImpl(IXMLDOMNode* pNode)
        : m_pXmlNode(pNode)
    {}

INode* NodeImpl::AddElement( const tstring& tag                             
                           , long        // index
                           )
{
    CComPtr<IXMLDOMDocument> pDoc;
    if (FAILED(m_pXmlNode->get_ownerDocument(&pDoc)))
        throw XmlStorageException();

    CComPtr<IXMLDOMElement> pElement;
    if (FAILED(pDoc->createElement(CComBSTR(tag.c_str()), &pElement)))
        throw XmlStorageException();

    if (FAILED(m_pXmlNode->appendChild(pElement, NULL)))
        throw XmlStorageException();

    return new NodeImpl(pElement);
}

INode* NodeImpl::AddAttribute( const tstring& tag)
{
    HRESULT hr;

    DOMNodeType nodeType;
    if (FAILED(m_pXmlNode->get_nodeType(&nodeType)))
        throw XmlStorageException();

    ATLASSERT(nodeType == NODE_ELEMENT);

    CComPtr<IXMLDOMElement> pElement;
    if (FAILED(m_pXmlNode.QueryInterface(&pElement)))
        throw XmlStorageException();

    CComPtr<IXMLDOMDocument> pDoc;
    if (FAILED(m_pXmlNode->get_ownerDocument(&pDoc)))
        throw XmlStorageException();

    CComPtr<IXMLDOMAttribute> pAttribute;
    if (FAILED(pDoc->createAttribute(CComBSTR(tag.c_str()), &pAttribute)))
        throw XmlStorageException();


    hr = pElement->setAttributeNode(pAttribute, NULL);
    if (FAILED(hr))
        throw XmlStorageException();

    return new NodeImpl(pAttribute);
}

Serializer::INamedNodeList* NodeImpl::ChildElements() const
{
    CComPtr<IXMLDOMNodeList> pXmlNodeList;
    if (FAILED(m_pXmlNode->get_childNodes(&pXmlNodeList)))
        throw XmlStorageException();
    return new NamedNodeListImpl(pXmlNodeList);
}
/*
Serializer::INamedNodeList* NodeImpl::ThisAsNamedNodeList() const
{

}
*/
Serializer::INodeArray* NodeImpl::ChildElements(const tstring& name) const
{
    CComPtr<IXMLDOMNodeList> pXmlNodeList;
    if (FAILED(m_pXmlNode->selectNodes(CComBSTR(name.c_str()), &pXmlNodeList)))
        throw XmlStorageException();
    return new NodeArrayImpl(pXmlNodeList);
}
        
Serializer::INamedNodeList* NodeImpl::Attributes() const
{
    CComPtr<IXMLDOMNamedNodeMap> pNamedNodeMap;
    if (FAILED(m_pXmlNode->get_attributes(&pNamedNodeMap)))
        throw XmlStorageException();
    return new NamedNodeListImpl(pNamedNodeMap);
}

tstring NodeImpl::ReadText() const
{
    CComBSTR text;
    if (FAILED(m_pXmlNode->get_text(&text)))
        throw XmlStorageException();
    return (LPCTSTR)CW2CT(text);
}

void NodeImpl::WriteText(const tstring& text)
{
    m_pXmlNode->put_text(CComBSTR(text.c_str()));
}

} // namespace Xml {
} // namespace TreeStorage {
} //namespace Serializer {
} // namespace Rsdn {
//==========================================================)
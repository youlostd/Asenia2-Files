#include "stdafx.h"
#include "../gamelib/ItemManager.h"
#include "PythonPrivateShopSearch.h"
#include "PythonCharacterManager.h"


CPythonPrivateShopSearch::CPythonPrivateShopSearch()
{
}


CPythonPrivateShopSearch::~CPythonPrivateShopSearch()
{
}

void CPythonPrivateShopSearch::AddItemData (const TSearchItemData& rItemData)
{
	m_ItemInstanceVector.push_back (rItemData);
	m_ItemInstance10Vector.push_back(rItemData);
	ResultFilterSelectedPage(1);
}

void CPythonPrivateShopSearch::ClearItemData()
{
	m_ItemInstanceVector.clear();
	m_ItemInstance10Vector.clear();
}

DWORD CPythonPrivateShopSearch::GetItemDataPtr (DWORD index, TSearchItemData** ppInstance)
{
	if (index < GetItem10DataCount())
	{
		*ppInstance = &m_ItemInstance10Vector.at(index);
		return 1;
	}
	return 0;
}

PyObject* privateShopSearchGetSearchItemMetinSocket (PyObject* poSelf, PyObject* poArgs)
{
	int iSlotIndex;
	if (!PyTuple_GetInteger (poArgs, 0, &iSlotIndex))
	{
		return Py_BadArgument();
	}
	int iSocketIndex;
	if (!PyTuple_GetInteger (poArgs, 1, &iSocketIndex))
	{
		return Py_BadArgument();
	}

	if (iSocketIndex >= ITEM_SOCKET_SLOT_MAX_NUM)
	{
		return Py_BuildException();
	}

	CPythonPrivateShopSearch::TSearchItemData* pItemData;
	if (!CPythonPrivateShopSearch::Instance().GetItemDataPtr (iSlotIndex, &pItemData))
	{
		return Py_BuildException();
	}

	return Py_BuildValue ("i", pItemData->alSockets[iSocketIndex]);
}

PyObject* privateShopSearchGetSearchItemAttribute (PyObject* poSelf, PyObject* poArgs)
{
	int iSlotIndex;
	if (!PyTuple_GetInteger (poArgs, 0, &iSlotIndex))
	{
		return Py_BuildException();
	}
	int iAttrSlotIndex;
	if (!PyTuple_GetInteger (poArgs, 1, &iAttrSlotIndex))
	{
		return Py_BuildException();
	}

	if (iAttrSlotIndex >= 0 && iAttrSlotIndex < ITEM_ATTRIBUTE_SLOT_MAX_NUM)
	{
		CPythonPrivateShopSearch::TSearchItemData* pItemData;
		if (CPythonPrivateShopSearch::Instance().GetItemDataPtr (iSlotIndex, &pItemData))
		{
			return Py_BuildValue ("ii", pItemData->aAttr[iAttrSlotIndex].bType, pItemData->aAttr[iAttrSlotIndex].sValue);
		}
	}

	return Py_BuildValue ("ii", 0, 0);
}

PyObject* privateShopSearchGetItemCount (PyObject* poSelf, PyObject* poArgs)
{
	return Py_BuildValue ("i", CPythonPrivateShopSearch::Instance().GetItemDataCount());
}

PyObject* privateShopSearchGetSearchItemShopVID (PyObject* poSelf, PyObject* poArgs)
{
	int ipos;
	if (!PyTuple_GetInteger (poArgs, 0, &ipos))
	{
		return Py_BadArgument();
	}

	CPythonPrivateShopSearch::TSearchItemData* pInstance;
	if (!CPythonPrivateShopSearch::Instance().GetItemDataPtr (ipos, &pInstance))
	{
		return Py_BuildException();
	}

	return Py_BuildValue ("i", pInstance->vid);
}

PyObject* privateShopSearchGetSearchItemPos (PyObject* poSelf, PyObject* poArgs)
{
	int ipos;
	if (!PyTuple_GetInteger (poArgs, 0, &ipos))
	{
		return Py_BadArgument();
	}

	CPythonPrivateShopSearch::TSearchItemData* pInstance;
	if (!CPythonPrivateShopSearch::Instance().GetItemDataPtr (ipos, &pInstance))
	{
		return Py_BuildException();
	}

	return Py_BuildValue ("i", pInstance->Cell);
}


PyObject* privateShopSearchGetSearchItemVnum (PyObject* poSelf, PyObject* poArgs)
{
	int ipos;
	if (!PyTuple_GetInteger (poArgs, 0, &ipos))
	{
		return Py_BadArgument();
	}

	CPythonPrivateShopSearch::TSearchItemData* pInstance;
	if (!CPythonPrivateShopSearch::Instance().GetItemDataPtr (ipos, &pInstance))
	{
		return Py_BuildException();
	}

	return Py_BuildValue ("i", pInstance->vnum);
}

PyObject* privateShopSearchGetSearchItemEvolution(PyObject* poSelf, PyObject* poArgs)
{
	int ipos;
	if (!PyTuple_GetInteger(poArgs, 0, &ipos))
	{
		return Py_BadArgument();
	}

	CPythonPrivateShopSearch::TSearchItemData* pInstance;
	if (!CPythonPrivateShopSearch::Instance().GetItemDataPtr(ipos, &pInstance))
	{
		return Py_BuildException();
	}

	return Py_BuildValue("i", pInstance->evolution);
}
PyObject* privateShopSearchGetPrivateShopSearchResult(PyObject* poSelf, PyObject* poArgs)
{
	int ipos;
	if (!PyTuple_GetInteger(poArgs, 0, &ipos))
	{
		return Py_BadArgument();
	}

	CPythonPrivateShopSearch::TSearchItemData* pInstance;
	if (!CPythonPrivateShopSearch::Instance().GetItemDataPtr(ipos, &pInstance))
	{
		// return Py_BuildValue("ssiii", "","",0,0,0);
		return Py_BuildValue("ssiii", "", "", 0, 0, 0);
	}

	return Py_BuildValue("ssiii", pInstance->itemName, pInstance->ownerName, pInstance->count, pInstance->price, pInstance->price_cheque);
}

PyObject* privateShopSearchGetPrivateShopSearchSelectPage(PyObject* poSelf, PyObject* poArgs)
{
	int page;
	if (!PyTuple_GetInteger(poArgs, 0, &page))
	{
		return Py_BadArgument();
	}

	CPythonPrivateShopSearch::Instance().ResultFilterSelectedPage(page);
	
	return Py_BuildNone();
}

void CPythonPrivateShopSearch::ResultFilterSelectedPage(int page)
{
	m_ItemInstance10Vector.clear();
	//CPythonPrivateShopSearch::TSearchItemData item;
	for (const auto &item : m_ItemInstanceVector) {
		if (item.page == page)
		{
			m_ItemInstance10Vector.push_back(item);
		}
	}
}
PyObject* privateShopSearchGetPrivateShopSearchResultPage(PyObject* poSelf, PyObject* poArgs)
{
	CPythonPrivateShopSearch::TSearchItemData* pInstance;
	if (!CPythonPrivateShopSearch::Instance().GetItemDataPtr(0, &pInstance))
	{
		return Py_BuildException();
	}

	return Py_BuildValue("i", pInstance->page);
}

void initprivateShopSearch()
{
	static PyMethodDef s_methods[] =
	{
		{ "GetPrivateShopSearchResultMaxCount",		privateShopSearchGetItemCount,			METH_VARARGS },

		{ "GetSearchItemVnum",			privateShopSearchGetSearchItemVnum,				METH_VARARGS },

		{ "GetSearchItemShopVID",			privateShopSearchGetSearchItemShopVID,				METH_VARARGS },

		{ "GetSearchItemMetinSocket",		privateShopSearchGetSearchItemMetinSocket,			METH_VARARGS },
		{ "GetSearchItemAttribute",		privateShopSearchGetSearchItemAttribute,			METH_VARARGS },
		{ "GetSearchItemEvolution",	privateShopSearchGetSearchItemEvolution,		METH_VARARGS },
		{ "GetSearchItemPos", privateShopSearchGetSearchItemPos, METH_VARARGS	},
		{ "GetPrivateShopSearchResult", privateShopSearchGetPrivateShopSearchResult, METH_VARARGS	},
		{ "GetPrivateShopSearchResultSelectPage", privateShopSearchGetPrivateShopSearchSelectPage, METH_VARARGS	},
		{ "GetPrivateShopSearchResultPage", privateShopSearchGetPrivateShopSearchResultPage, METH_VARARGS	},
		{ NULL,							NULL,									NULL },
	};

	PyObject* poModule = Py_InitModule ("privateShopSearch", s_methods);

}

#include "StdAfx.h"
#include "PythonItem.h"

#include "../gamelib/ItemManager.h"
#ifdef ENABLE_DS_SET
#include "../GameLib/DragonSoulTable.h"
#endif
#include "InstanceBase.h"
#include "AbstractApplication.h"
#include "Locale_inc.h"

extern int TWOHANDED_WEWAPON_ATT_SPEED_DECREASE_VALUE;

PyObject * itemSetUseSoundFileName(PyObject * poSelf, PyObject * poArgs)
{
	int iUseSound;
	if (!PyTuple_GetInteger(poArgs, 0, &iUseSound))
		return Py_BadArgument();
	
	char* szFileName;
	if (!PyTuple_GetString(poArgs, 1, &szFileName))
		return Py_BadArgument();

	CPythonItem& rkItem=CPythonItem::Instance();
	rkItem.SetUseSoundFileName(iUseSound, szFileName);
	return Py_BuildNone();
}

PyObject * itemSetDropSoundFileName(PyObject * poSelf, PyObject * poArgs)
{
	int iDropSound;
	if (!PyTuple_GetInteger(poArgs, 0, &iDropSound))
		return Py_BadArgument();
	
	char* szFileName;
	if (!PyTuple_GetString(poArgs, 1, &szFileName))
		return Py_BadArgument();

	CPythonItem& rkItem=CPythonItem::Instance();
	rkItem.SetDropSoundFileName(iDropSound, szFileName);
	return Py_BuildNone();
}

PyObject * itemSelectItem(PyObject * poSelf, PyObject * poArgs)
{
	int iIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iIndex))
		return Py_BadArgument();

	if (!CItemManager::Instance().SelectItemData(iIndex))
	{
		//TraceError("Cannot find item by %d", iIndex);
		CItemManager::Instance().SelectItemData(60001);
	}

	return Py_BuildNone();
}

PyObject * itemGetItemName(PyObject * poSelf, PyObject * poArgs)
{
	CItemData * pItemData = CItemManager::Instance().GetSelectedItemDataPointer();
	if (!pItemData)
		return Py_BuildException("no selected item data");

	return Py_BuildValue("s", pItemData->GetName());
}

PyObject * itemGetItemDescription(PyObject * poSelf, PyObject * poArgs)
{
	CItemData * pItemData = CItemManager::Instance().GetSelectedItemDataPointer();
	if (!pItemData)
		return Py_BuildException("no selected item data");

	return Py_BuildValue("s", pItemData->GetDescription());
}

PyObject * itemGetItemSummary(PyObject * poSelf, PyObject * poArgs)
{
	CItemData * pItemData = CItemManager::Instance().GetSelectedItemDataPointer();
	if (!pItemData)
		return Py_BuildException("no selected item data");

	return Py_BuildValue("s", pItemData->GetSummary());
}
PyObject * itemGetItemRed(PyObject * poSelf, PyObject * poArgs)
{
	CItemData * pItemData = CItemManager::Instance().GetSelectedItemDataPointer();
	if (!pItemData)
		return Py_BuildException("no selected item data");

	return Py_BuildValue("s", pItemData->GetRed());
}
PyObject * itemGetItemBlue(PyObject * poSelf, PyObject * poArgs)
{
	CItemData * pItemData = CItemManager::Instance().GetSelectedItemDataPointer();
	if (!pItemData)
		return Py_BuildException("no selected item data");

	return Py_BuildValue("s", pItemData->GetBlue());
}
PyObject * itemGetItemGreen(PyObject * poSelf, PyObject * poArgs)
{
	CItemData * pItemData = CItemManager::Instance().GetSelectedItemDataPointer();
	if (!pItemData)
		return Py_BuildException("no selected item data");

	return Py_BuildValue("s", pItemData->GetGreen());
}
PyObject * itemGetItemYellow(PyObject * poSelf, PyObject * poArgs)
{
	CItemData * pItemData = CItemManager::Instance().GetSelectedItemDataPointer();
	if (!pItemData)
		return Py_BuildException("no selected item data");

	return Py_BuildValue("s", pItemData->GetYellow());
}
PyObject * itemGetItemOrange(PyObject * poSelf, PyObject * poArgs)
{
	CItemData * pItemData = CItemManager::Instance().GetSelectedItemDataPointer();
	if (!pItemData)
		return Py_BuildException("no selected item data");

	return Py_BuildValue("s", pItemData->GetOrange());
}
PyObject * itemGetItemPink(PyObject * poSelf, PyObject * poArgs)
{
	CItemData * pItemData = CItemManager::Instance().GetSelectedItemDataPointer();
	if (!pItemData)
		return Py_BuildException("no selected item data");

	return Py_BuildValue("s", pItemData->GetPink());
}
PyObject * itemGetItemPurple(PyObject * poSelf, PyObject * poArgs)
{
	CItemData * pItemData = CItemManager::Instance().GetSelectedItemDataPointer();
	if (!pItemData)
		return Py_BuildException("no selected item data");

	return Py_BuildValue("s", pItemData->GetPurple());
}
PyObject * itemGetIconImage(PyObject * poSelf, PyObject * poArgs)
{
	CItemData * pItemData = CItemManager::Instance().GetSelectedItemDataPointer();
	if (!pItemData)
		return Py_BuildException("no selected item data");

//	if (CItemData::ITEM_TYPE_SKILLBOOK == pItemData->GetType())
//	{
//		char szItemName[64+1];
//		_snprintf(szItemName, "d:/ymir work/ui/items/etc/book_%02d.sub", );
//		CGraphicImage * pImage = (CGraphicImage *)CResourceManager::Instance().GetResourcePointer(szItemName);
//	}

	return Py_BuildValue("i", pItemData->GetIconImage());
}

PyObject * itemGetIconImageFileName(PyObject * poSelf, PyObject * poArgs)
{
	CItemData * pItemData = CItemManager::Instance().GetSelectedItemDataPointer();
	if (!pItemData)
		return Py_BuildException("no selected item data");

	CGraphicSubImage * pImage = pItemData->GetIconImage();
	if (!pImage)
		return Py_BuildValue("s", "Noname");

	return Py_BuildValue("s", pImage->GetFileName());
}

PyObject * itemGetItemSize(PyObject * poSelf, PyObject * poArgs)
{
	CItemData * pItemData = CItemManager::Instance().GetSelectedItemDataPointer();
	if (!pItemData)
		return Py_BuildException("no selected item data");

	return Py_BuildValue("(ii)", 1, pItemData->GetSize());
}

PyObject * itemGetItemType(PyObject * poSelf, PyObject * poArgs)
{
	CItemData * pItemData = CItemManager::Instance().GetSelectedItemDataPointer();
	if (!pItemData)
		return Py_BuildException("no selected item data");

	return Py_BuildValue("i", pItemData->GetType());
}

PyObject * itemGetItemSubType(PyObject * poSelf, PyObject * poArgs)
{
	CItemData * pItemData = CItemManager::Instance().GetSelectedItemDataPointer();
	if (!pItemData)
		return Py_BuildException("no selected item data");

	return Py_BuildValue("i", pItemData->GetSubType());
}

PyObject * itemGetIBuyItemPrice(PyObject * poSelf, PyObject * poArgs)
{
	CItemData * pItemData = CItemManager::Instance().GetSelectedItemDataPointer();
	
	if (!pItemData)
		return Py_BuildException("no selected item data");

	return Py_BuildValue("i", pItemData->GetIBuyItemPrice());
}

PyObject * itemGetISellItemPrice(PyObject * poSelf, PyObject * poArgs)
{
	CItemData * pItemData = CItemManager::Instance().GetSelectedItemDataPointer();
	
	if (!pItemData)
		return Py_BuildException("no selected item data");

	return Py_BuildValue("i", pItemData->GetISellItemPrice());
}


PyObject * itemIsAntiFlag(PyObject * poSelf, PyObject * poArgs)
{
	int iFlag;
	if (!PyTuple_GetInteger(poArgs, 0, &iFlag))
		return Py_BadArgument();

	CItemData * pItemData = CItemManager::Instance().GetSelectedItemDataPointer();
	if (!pItemData)
		return Py_BuildException("no selected item data");

	return Py_BuildValue("i", pItemData->IsAntiFlag(iFlag));
}

PyObject * itemIsFlag(PyObject * poSelf, PyObject * poArgs)
{
	int iFlag;
	if (!PyTuple_GetInteger(poArgs, 0, &iFlag))
		return Py_BadArgument();

	CItemData * pItemData = CItemManager::Instance().GetSelectedItemDataPointer();
	if (!pItemData)
		return Py_BuildException("no selected item data");

	return Py_BuildValue("i", pItemData->IsFlag(iFlag));
}

PyObject * itemIsWearableFlag(PyObject * poSelf, PyObject * poArgs)
{
	int iFlag;
	if (!PyTuple_GetInteger(poArgs, 0, &iFlag))
		return Py_BadArgument();

	CItemData * pItemData = CItemManager::Instance().GetSelectedItemDataPointer();
	if (!pItemData)
		return Py_BuildException("no selected item data");

	return Py_BuildValue("i", pItemData->IsWearableFlag(iFlag));
}

PyObject * itemIs1GoldItem(PyObject * poSelf, PyObject * poArgs)
{
	CItemData * pItemData = CItemManager::Instance().GetSelectedItemDataPointer();
	if (!pItemData)
		return Py_BuildException("no selected item data");

	return Py_BuildValue("i", pItemData->IsFlag(CItemData::ITEM_FLAG_COUNT_PER_1GOLD));
}

PyObject * itemGetLimit(PyObject * poSelf, PyObject * poArgs)
{
	int iValueIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iValueIndex))
		return Py_BadArgument();

	CItemData * pItemData = CItemManager::Instance().GetSelectedItemDataPointer();
	if (!pItemData)
		return Py_BuildException("Not yet select item data");

	CItemData::TItemLimit ItemLimit;
	if (!pItemData->GetLimit(iValueIndex, &ItemLimit))
		return Py_BuildException();

	return Py_BuildValue("ii", ItemLimit.bType, ItemLimit.lValue);
}

PyObject * itemGetAffect(PyObject * poSelf, PyObject * poArgs)
{
	int iValueIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iValueIndex))
		return Py_BadArgument();

	CItemData * pItemData = CItemManager::Instance().GetSelectedItemDataPointer();
	if (!pItemData)
		return Py_BuildException("Not yet select item data");

	CItemData::TItemApply ItemApply;
	if (!pItemData->GetApply(iValueIndex, &ItemApply))
		return Py_BuildException();

	if ((CItemData::APPLY_ATT_SPEED == ItemApply.bType) && (CItemData::ITEM_TYPE_WEAPON == pItemData->GetType()) && (CItemData::WEAPON_TWO_HANDED == pItemData->GetSubType()))
	{
		ItemApply.lValue -= TWOHANDED_WEWAPON_ATT_SPEED_DECREASE_VALUE;
	}

	return Py_BuildValue("ii", ItemApply.bType, ItemApply.lValue);
}

PyObject * itemGetValue(PyObject * poSelf, PyObject * poArgs)
{
	int iValueIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iValueIndex))
		return Py_BadArgument();

	CItemData * pItemData = CItemManager::Instance().GetSelectedItemDataPointer();
	if (!pItemData)
		return Py_BuildException("Not yet select item data");

	return Py_BuildValue("i", pItemData->GetValue(iValueIndex));
}

PyObject * itemGetSocket(PyObject * poSelf, PyObject * poArgs)
{
	int iValueIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iValueIndex))
		return Py_BadArgument();

	CItemData * pItemData = CItemManager::Instance().GetSelectedItemDataPointer();
	if (!pItemData)
		return Py_BuildException("Not yet select item data");

	return Py_BuildValue("i", pItemData->GetSocket(iValueIndex));
}

PyObject * itemGetIconInstance(PyObject * poSelf, PyObject * poArgs)
{
	CItemData * pItemData = CItemManager::Instance().GetSelectedItemDataPointer();
	if (!pItemData)
		return Py_BuildException("Not yet select item data");

	CGraphicSubImage * pImage = pItemData->GetIconImage();
	if (!pImage)
		return Py_BuildException("Cannot get icon image by %d", pItemData->GetIndex());

	CGraphicImageInstance * pImageInstance = CGraphicImageInstance::New();
	pImageInstance->SetImagePointer(pImage);

	return Py_BuildValue("i", pImageInstance);
}

PyObject * itemDeleteIconInstance(PyObject * poSelf, PyObject * poArgs)
{
	int iHandle;
	if (!PyTuple_GetInteger(poArgs, 0, &iHandle))
		return Py_BadArgument();

	CGraphicImageInstance::Delete((CGraphicImageInstance *) iHandle);

	return Py_BuildNone();
}

PyObject * itemIsEquipmentVID(PyObject * poSelf, PyObject * poArgs)
{
	int iItemVID;
	if (!PyTuple_GetInteger(poArgs, 0, &iItemVID))
		return Py_BadArgument();

	CItemManager::Instance().SelectItemData(iItemVID);
	CItemData * pItemData = CItemManager::Instance().GetSelectedItemDataPointer();
	if (!pItemData)
		return Py_BuildException("Not yet select item data");

	return Py_BuildValue("i", pItemData->IsEquipment());
}

// 2005.05.20.myevan.통합 USE_TYPE 체크
PyObject* itemGetUseType(PyObject * poSelf, PyObject * poArgs)
{
	int iItemVID;
	if (!PyTuple_GetInteger(poArgs, 0, &iItemVID))
		return Py_BadArgument();

	CItemManager::Instance().SelectItemData(iItemVID);
	CItemData * pItemData = CItemManager::Instance().GetSelectedItemDataPointer();
	if (!pItemData)
		return Py_BuildException("Can't find select item data");
	
	return Py_BuildValue("s", pItemData->GetUseTypeString());	
}

PyObject * itemIsRefineScroll(PyObject * poSelf, PyObject * poArgs)
{
	int iItemIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iItemIndex))
		return Py_BadArgument();

	CItemManager::Instance().SelectItemData(iItemIndex);
	CItemData * pItemData = CItemManager::Instance().GetSelectedItemDataPointer();
	if (!pItemData)
		return Py_BuildException("Can't find select item data");

	if (pItemData->GetType() != CItemData::ITEM_TYPE_USE)
		return Py_BuildValue("i", FALSE);
	
	switch (pItemData->GetSubType())
	{
		case CItemData::USE_TUNING:
			return Py_BuildValue("i", TRUE);
			break;
	}
	
	return Py_BuildValue("i", FALSE);
}

PyObject * itemIsDetachScroll(PyObject * poSelf, PyObject * poArgs)
{
	int iItemIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iItemIndex))
		return Py_BadArgument();

	CItemManager::Instance().SelectItemData(iItemIndex);
	CItemData * pItemData = CItemManager::Instance().GetSelectedItemDataPointer();
	if (!pItemData)
		return Py_BuildException("Can't find select item data");

	int iType = pItemData->GetType();
	int iSubType = pItemData->GetSubType();
	if (iType == CItemData::ITEM_TYPE_USE)
	if (iSubType == CItemData::USE_DETACHMENT)
	{
		return Py_BuildValue("i", TRUE);
	}

	return Py_BuildValue("i", FALSE);
}

PyObject * itemCanAddToQuickSlotItem(PyObject * poSelf, PyObject * poArgs)
{
	int iItemIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iItemIndex))
		return Py_BadArgument();

	CItemManager::Instance().SelectItemData(iItemIndex);
	CItemData * pItemData = CItemManager::Instance().GetSelectedItemDataPointer();
	if (!pItemData)
		return Py_BuildException("Can't find select item data");

#ifdef ENABLE_NEW_PET_SYSTEM
	if (CItemData::ITEM_TYPE_USE == pItemData->GetType() || CItemData::ITEM_TYPE_QUEST == pItemData->GetType() || CItemData::ITEM_TYPE_PET == pItemData->GetType())
#else
	if (CItemData::ITEM_TYPE_USE == pItemData->GetType() || CItemData::ITEM_TYPE_QUEST == pItemData->GetType())
#endif
	{
		return Py_BuildValue("i", TRUE);
	}

	return Py_BuildValue("i", FALSE);
}

PyObject * itemIsKey(PyObject * poSelf, PyObject * poArgs)
{
	int iItemIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iItemIndex))
		return Py_BadArgument();

	CItemManager::Instance().SelectItemData(iItemIndex);
	CItemData * pItemData = CItemManager::Instance().GetSelectedItemDataPointer();
	if (!pItemData)
		return Py_BuildException("Can't find select item data");

	if (CItemData::ITEM_TYPE_TREASURE_KEY == pItemData->GetType())
	{
		return Py_BuildValue("i", TRUE);
	}

	return Py_BuildValue("i", FALSE);
}

PyObject * itemIsMetin(PyObject * poSelf, PyObject * poArgs)
{
	int iItemIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iItemIndex))
		return Py_BadArgument();

	CItemManager::Instance().SelectItemData(iItemIndex);
	CItemData * pItemData = CItemManager::Instance().GetSelectedItemDataPointer();
	if (!pItemData)
		return Py_BuildException("Can't find select item data");

	if (CItemData::ITEM_TYPE_METIN == pItemData->GetType())
	{
		return Py_BuildValue("i", TRUE);
	}

	return Py_BuildValue("i", FALSE);
}

PyObject * itemRender(PyObject * poSelf, PyObject * poArgs)
{
	CPythonItem::Instance().Render();
	return Py_BuildNone();
}

PyObject * itemUpdate(PyObject * poSelf, PyObject * poArgs)
{
	IAbstractApplication& rkApp=IAbstractApplication::GetSingleton();

	POINT ptMouse;
	rkApp.GetMousePosition(&ptMouse);	

	CPythonItem::Instance().Update(ptMouse);
	return Py_BuildNone();
}

PyObject * itemCreateItem(PyObject * poSelf, PyObject * poArgs)
{
	int iVirtualID;
	if (!PyTuple_GetInteger(poArgs, 0, &iVirtualID))
		return Py_BadArgument();
	int iVirtualNumber;
	if (!PyTuple_GetInteger(poArgs, 1, &iVirtualNumber))
		return Py_BadArgument();

	float x;
	if (!PyTuple_GetFloat(poArgs, 2, &x))
		return Py_BadArgument();
	float y;
	if (!PyTuple_GetFloat(poArgs, 3, &y))
		return Py_BadArgument();
	float z;
	if (!PyTuple_GetFloat(poArgs, 4, &z))
		return Py_BadArgument();
	
	bool bDrop = true;
	PyTuple_GetBoolean(poArgs, 5, &bDrop);

	CPythonItem::Instance().CreateItem(iVirtualID, iVirtualNumber, x, y, z, bDrop);

	return Py_BuildNone();
}

PyObject * itemDeleteItem(PyObject * poSelf, PyObject * poArgs)
{
	int iVirtualID;
	if (!PyTuple_GetInteger(poArgs, 0, &iVirtualID))
		return Py_BadArgument();

	CPythonItem::Instance().DeleteItem(iVirtualID);
	return Py_BuildNone();
}

PyObject * itemPick(PyObject * poSelf, PyObject * poArgs)
{
	DWORD dwItemID;
	if (CPythonItem::Instance().GetPickedItemID(&dwItemID))
		return Py_BuildValue("i", dwItemID);
	else
		return Py_BuildValue("i", -1);
}

PyObject* itemLoadItemTable(PyObject* poSelf, PyObject* poArgs)
{
	char * szFileName;
	if (!PyTuple_GetString(poArgs, 0, &szFileName))
		return Py_BadArgument();

	CItemManager::Instance().LoadItemTable(szFileName);
	return Py_BuildNone();
}
PyObject * itemGetMaskType(PyObject * poSelf, PyObject * poArgs)
{
	CItemData * pItemData = CItemManager::Instance().GetSelectedItemDataPointer();
	if (!pItemData)
		return Py_BuildException("no selected item data");

	return Py_BuildValue("i", pItemData->GetMaskType());
}

PyObject * itemGetMaskSubType(PyObject * poSelf, PyObject * poArgs)
{
	CItemData * pItemData = CItemManager::Instance().GetSelectedItemDataPointer();
	if (!pItemData)
		return Py_BuildException("no selected item data");

	return Py_BuildValue("i", pItemData->GetMaskSubType());
}

#ifdef ENABLE_DS_SET
PyObject* itemGetDSSetWeight(PyObject* poSelf, PyObject* poArgs)
{
	BYTE iSetGrade;
	if (!PyTuple_GetInteger(poArgs, 0, &iSetGrade))
		return Py_BadArgument();

	CDragonSoulTable* dsTable = CItemManager::Instance().GetDragonSoulTable();
	if (!dsTable)
		return Py_BuildException("DragonSoulTable not initalized");

	return Py_BuildValue("i", dsTable->GetDSSetWeight(iSetGrade));
}

PyObject* itemGetDSBasicApplyCount(PyObject* poSelf, PyObject* poArgs)
{
	BYTE iDSType;
	if (!PyTuple_GetInteger(poArgs, 0, &iDSType))
		return Py_BadArgument();

	CDragonSoulTable* dsTable = CItemManager::Instance().GetDragonSoulTable();
	if (!dsTable)
		return Py_BuildException("DragonSoulTable not initalized");

	return Py_BuildValue("i", dsTable->GetDSBasicApplyCount(iDSType));
}

PyObject* itemGetDSBasicApplyValue(PyObject* poSelf, PyObject* poArgs)
{
	BYTE iDSType;
	if (!PyTuple_GetInteger(poArgs, 0, &iDSType))
		return Py_BadArgument();

	WORD iApplyType;
	if (!PyTuple_GetInteger(poArgs, 1, &iApplyType))
		return Py_BadArgument();

	CDragonSoulTable* dsTable = CItemManager::Instance().GetDragonSoulTable();
	if (!dsTable)
		return Py_BuildException("DragonSoulTable not initalized");

	return Py_BuildValue("i", dsTable->GetDSBasicApplyValue(iDSType, iApplyType));
}

PyObject* itemGetDSAdditionalApplyValue(PyObject* poSelf, PyObject* poArgs)
{
	BYTE iDSType;
	if (!PyTuple_GetInteger(poArgs, 0, &iDSType))
		return Py_BadArgument();

	WORD iApplyType;
	if (!PyTuple_GetInteger(poArgs, 1, &iApplyType))
		return Py_BadArgument();

	CDragonSoulTable* dsTable = CItemManager::Instance().GetDragonSoulTable();
	if (!dsTable)
		return Py_BuildException("DragonSoulTable not initalized");

	return Py_BuildValue("i", dsTable->GetDSAdditionalApplyValue(iDSType, iApplyType));
}
#endif


PyObject * itemIsUpgradeTimeScroll(PyObject * poSelf, PyObject * poArgs)
{
	int iItemIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iItemIndex))
		return Py_BadArgument();
	
	if (iItemIndex == 91014)
		return Py_BuildValue("i", 1);
	else if (iItemIndex == 91015)
		return Py_BuildValue("i", 2);
	else if (iItemIndex == 91016)
		return Py_BuildValue("i", 3);
	else if (iItemIndex == 91017)
		return Py_BuildValue("i", 4);

	return Py_BuildValue("i", false);
}

#ifdef ENABLE_NEW_PET_SYSTEM
PyObject* itemGtVnum(PyObject* poSelf, PyObject* poArgs)
{
	CItemData* pItemData = CItemManager::Instance().GetSelectedItemDataPointer();
	if (!pItemData)
		return Py_BuildException("no selected item data");

	return Py_BuildValue("i", pItemData->GetIndex());
}

PyObject* itemIsNewPetBooks(PyObject* poSelf, PyObject* poArgs)
{
	int iItemIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iItemIndex))
		return Py_BadArgument();

	CItemManager::Instance().SelectItemData(iItemIndex);
	CItemData * pItemData = CItemManager::Instance().GetSelectedItemDataPointer();
	if (!pItemData)
		return Py_BuildException("Can't find select item data");

	if (pItemData->GetIndex() >= 55010 && pItemData->GetIndex() <= 55027)
		return Py_BuildValue("i", 1);
	if (pItemData->GetIndex() >= 55034 && pItemData->GetIndex() <= 55040)
		return Py_BuildValue("i", 1);

	//Pet Revertus
	if (pItemData->GetIndex() == 55030)
		return Py_BuildValue("i", 1);

	return Py_BuildValue("i", 0);
}
#endif

void initItem()
{
	static PyMethodDef s_methods[] =
	{	
		{ "SetUseSoundFileName",			itemSetUseSoundFileName,				METH_VARARGS },
		{ "SetDropSoundFileName",			itemSetDropSoundFileName,				METH_VARARGS },
		{ "SelectItem",						itemSelectItem,							METH_VARARGS },

		{ "GetItemName",					itemGetItemName,						METH_VARARGS },
		{ "GetItemDescription",				itemGetItemDescription,					METH_VARARGS },
		{ "GetItemSummary",					itemGetItemSummary,						METH_VARARGS },
		{ "GetItemRed",						itemGetItemRed,							METH_VARARGS },
		{ "GetItemBlue",					itemGetItemBlue,						METH_VARARGS },
		{ "GetItemGreen",					itemGetItemGreen,						METH_VARARGS },
		{ "GetItemYellow",					itemGetItemYellow,						METH_VARARGS },
		{ "GetItemOrange",					itemGetItemOrange,						METH_VARARGS },
		{ "GetItemPink",					itemGetItemPink,						METH_VARARGS },
		{ "GetItemPurple",					itemGetItemPurple,						METH_VARARGS },
		{ "GetIconImage",					itemGetIconImage,						METH_VARARGS },
		{ "GetIconImageFileName",			itemGetIconImageFileName,				METH_VARARGS },
		{ "GetItemSize",					itemGetItemSize,						METH_VARARGS },
		{ "GetItemType",					itemGetItemType,						METH_VARARGS },
		{ "GetItemSubType",					itemGetItemSubType,						METH_VARARGS },
		{ "GetIBuyItemPrice",				itemGetIBuyItemPrice,					METH_VARARGS },
		{ "GetISellItemPrice",				itemGetISellItemPrice,					METH_VARARGS },
		{ "IsAntiFlag",						itemIsAntiFlag,							METH_VARARGS },
		{ "IsFlag",							itemIsFlag,								METH_VARARGS },
		{ "IsWearableFlag",					itemIsWearableFlag,						METH_VARARGS },
		{ "Is1GoldItem",					itemIs1GoldItem,						METH_VARARGS },
		{ "GetLimit",						itemGetLimit,							METH_VARARGS },
		{ "GetAffect",						itemGetAffect,							METH_VARARGS },
		{ "GetValue",						itemGetValue,							METH_VARARGS },
		{ "GetSocket",						itemGetSocket,							METH_VARARGS },
		{ "GetIconInstance",				itemGetIconInstance,					METH_VARARGS },
		{ "GetUseType",						itemGetUseType,							METH_VARARGS },
		{ "DeleteIconInstance",				itemDeleteIconInstance,					METH_VARARGS },
		{ "IsEquipmentVID",					itemIsEquipmentVID,						METH_VARARGS },		
		{ "IsRefineScroll",					itemIsRefineScroll,						METH_VARARGS },
		{ "IsDetachScroll",					itemIsDetachScroll,						METH_VARARGS },
		{ "IsKey",							itemIsKey,								METH_VARARGS },
		{ "IsMetin",						itemIsMetin,							METH_VARARGS },
		{ "CanAddToQuickSlotItem",			itemCanAddToQuickSlotItem,				METH_VARARGS },

		{ "Update",							itemUpdate,								METH_VARARGS },
		{ "Render",							itemRender,								METH_VARARGS },
		{ "CreateItem",						itemCreateItem,							METH_VARARGS },
		{ "DeleteItem",						itemDeleteItem,							METH_VARARGS },
		{ "Pick",							itemPick,								METH_VARARGS },

		{ "LoadItemTable",					itemLoadItemTable,						METH_VARARGS },
		{"IsUpgradeTimeScroll",				itemIsUpgradeTimeScroll,				METH_VARARGS},

		{ "GetMaskType",					itemGetMaskType,						METH_VARARGS },
		{ "GetMaskSubType",					itemGetMaskSubType,						METH_VARARGS },

#ifdef ENABLE_DS_SET
		{ "GetDSSetWeight",					itemGetDSSetWeight,						METH_VARARGS },
		{ "GetDSBasicApplyCount",			itemGetDSBasicApplyCount,				METH_VARARGS },
		{ "GetDSBasicApplyValue",			itemGetDSBasicApplyValue,				METH_VARARGS },
		{ "GetDSAdditionalApplyValue",		itemGetDSAdditionalApplyValue,			METH_VARARGS },
#endif
#ifdef ENABLE_NEW_PET_SYSTEM
		{ "GetVnum",						itemGtVnum,								METH_VARARGS},
		{ "IsNewPetBooks",					itemIsNewPetBooks,						METH_VARARGS },
#endif

		{ NULL,								NULL,									NULL		 },
	};

	PyObject * poModule = Py_InitModule("item", s_methods);

	PyModule_AddIntConstant(poModule, "USESOUND_ACCESSORY",			CPythonItem::USESOUND_ACCESSORY);
	PyModule_AddIntConstant(poModule, "USESOUND_ARMOR",				CPythonItem::USESOUND_ARMOR);
	PyModule_AddIntConstant(poModule, "USESOUND_BOW",				CPythonItem::USESOUND_BOW);
	PyModule_AddIntConstant(poModule, "USESOUND_DEFAULT",			CPythonItem::USESOUND_DEFAULT);
	PyModule_AddIntConstant(poModule, "USESOUND_WEAPON",			CPythonItem::USESOUND_WEAPON);
	PyModule_AddIntConstant(poModule, "USESOUND_POTION",			CPythonItem::USESOUND_POTION);
	PyModule_AddIntConstant(poModule, "USESOUND_PORTAL",			CPythonItem::USESOUND_PORTAL);

	PyModule_AddIntConstant(poModule, "DROPSOUND_ACCESSORY",		CPythonItem::DROPSOUND_ACCESSORY);
	PyModule_AddIntConstant(poModule, "DROPSOUND_ARMOR",			CPythonItem::DROPSOUND_ARMOR);
	PyModule_AddIntConstant(poModule, "DROPSOUND_BOW",				CPythonItem::DROPSOUND_BOW);
	PyModule_AddIntConstant(poModule, "DROPSOUND_DEFAULT",			CPythonItem::DROPSOUND_DEFAULT);
	PyModule_AddIntConstant(poModule, "DROPSOUND_WEAPON",			CPythonItem::DROPSOUND_WEAPON);

	PyModule_AddIntConstant(poModule, "EQUIPMENT_COUNT",			c_Equipment_Count);
	PyModule_AddIntConstant(poModule, "EQUIPMENT_HEAD",				c_Equipment_Head);
	PyModule_AddIntConstant(poModule, "EQUIPMENT_BODY",				c_Equipment_Body);
	PyModule_AddIntConstant(poModule, "EQUIPMENT_WEAPON",			c_Equipment_Weapon);
	PyModule_AddIntConstant(poModule, "EQUIPMENT_WRIST",			c_Equipment_Wrist);
	PyModule_AddIntConstant(poModule, "EQUIPMENT_SHOES",			c_Equipment_Shoes);
	PyModule_AddIntConstant(poModule, "EQUIPMENT_NECK",				c_Equipment_Neck);
	PyModule_AddIntConstant(poModule, "EQUIPMENT_EAR",				c_Equipment_Ear);
	PyModule_AddIntConstant(poModule, "EQUIPMENT_UNIQUE1",			c_Equipment_Unique1);
	PyModule_AddIntConstant(poModule, "EQUIPMENT_UNIQUE2",			c_Equipment_Unique2);
	PyModule_AddIntConstant(poModule, "EQUIPMENT_ARROW",			c_Equipment_Arrow);
#ifdef ENABLE_GLOVE_SYSTEM
	PyModule_AddIntConstant(poModule, "EQUIPMENT_GLOVE", c_Equipment_Glove);
#endif
	PyModule_AddIntConstant(poModule, "EQUIPMENT_Element",			c_Equipment_Element);

#ifdef ENABLE_NEW_EQUIPMENT_SYSTEM
	PyModule_AddIntConstant(poModule, "EQUIPMENT_RING1",			c_Equipment_Ring1);
	PyModule_AddIntConstant(poModule, "EQUIPMENT_RING2",			c_Equipment_Ring2);
	PyModule_AddIntConstant(poModule, "EQUIPMENT_BELT",				c_Equipment_Belt);
	PyModule_AddIntConstant(poModule, "EQUIPMENT_PET", c_Equipment_Pet);


	PyModule_AddIntConstant(poModule, "ITEM_TYPE_NONE",				CItemData::ITEM_TYPE_NONE);
	PyModule_AddIntConstant(poModule, "ITEM_TYPE_WEAPON",			CItemData::ITEM_TYPE_WEAPON);
	PyModule_AddIntConstant(poModule, "ITEM_TYPE_ARMOR",			CItemData::ITEM_TYPE_ARMOR);
	PyModule_AddIntConstant(poModule, "ITEM_TYPE_GIFTBOX", CItemData::ITEM_TYPE_GIFTBOX);
	PyModule_AddIntConstant(poModule, "ITEM_TYPE_USE",				CItemData::ITEM_TYPE_USE);
	PyModule_AddIntConstant(poModule, "ITEM_TYPE_AUTOUSE",			CItemData::ITEM_TYPE_AUTOUSE);
	PyModule_AddIntConstant(poModule, "ITEM_TYPE_MATERIAL",			CItemData::ITEM_TYPE_MATERIAL);
	PyModule_AddIntConstant(poModule, "ITEM_TYPE_SPECIAL",			CItemData::ITEM_TYPE_SPECIAL);
	PyModule_AddIntConstant(poModule, "ITEM_TYPE_TOOL",				CItemData::ITEM_TYPE_TOOL);
	PyModule_AddIntConstant(poModule, "ITEM_TYPE_LOTTERY",			CItemData::ITEM_TYPE_LOTTERY);
	PyModule_AddIntConstant(poModule, "ITEM_TYPE_ELK",				CItemData::ITEM_TYPE_ELK);
	PyModule_AddIntConstant(poModule, "ITEM_TYPE_METIN",			CItemData::ITEM_TYPE_METIN);
	PyModule_AddIntConstant(poModule, "ITEM_TYPE_CONTAINER",		CItemData::ITEM_TYPE_CONTAINER);
	PyModule_AddIntConstant(poModule, "ITEM_TYPE_FISH",				CItemData::ITEM_TYPE_FISH);
	PyModule_AddIntConstant(poModule, "ITEM_TYPE_ROD",				CItemData::ITEM_TYPE_ROD);
	PyModule_AddIntConstant(poModule, "ITEM_TYPE_RESOURCE",			CItemData::ITEM_TYPE_RESOURCE);
	PyModule_AddIntConstant(poModule, "ITEM_TYPE_CAMPFIRE",			CItemData::ITEM_TYPE_CAMPFIRE);
	PyModule_AddIntConstant(poModule, "ITEM_TYPE_UNIQUE",			CItemData::ITEM_TYPE_UNIQUE);
	PyModule_AddIntConstant(poModule, "ITEM_TYPE_SKILLBOOK",		CItemData::ITEM_TYPE_SKILLBOOK);
	PyModule_AddIntConstant(poModule, "ITEM_TYPE_QUEST",			CItemData::ITEM_TYPE_QUEST);
	PyModule_AddIntConstant(poModule, "ITEM_TYPE_POLYMORPH",		CItemData::ITEM_TYPE_POLYMORPH);
	PyModule_AddIntConstant(poModule, "ITEM_TYPE_TREASURE_BOX",		CItemData::ITEM_TYPE_TREASURE_BOX);
	PyModule_AddIntConstant(poModule, "ITEM_TYPE_TREASURE_KEY",		CItemData::ITEM_TYPE_TREASURE_KEY);
	PyModule_AddIntConstant(poModule, "ITEM_TYPE_PICK",				CItemData::ITEM_TYPE_PICK);
	PyModule_AddIntConstant(poModule, "ITEM_TYPE_BLEND",			CItemData::ITEM_TYPE_BLEND);
	PyModule_AddIntConstant(poModule, "ITEM_TYPE_DS",				CItemData::ITEM_TYPE_DS);
	PyModule_AddIntConstant(poModule, "ITEM_TYPE_SPECIAL_DS",		CItemData::ITEM_TYPE_SPECIAL_DS);
	PyModule_AddIntConstant(poModule, "ITEM_TYPE_RING",				CItemData::ITEM_TYPE_RING);
	PyModule_AddIntConstant(poModule, "ITEM_TYPE_BELT",				CItemData::ITEM_TYPE_BELT);
	PyModule_AddIntConstant(poModule, "ITEM_TYPE_GIFTBOX",			CItemData::ITEM_TYPE_GIFTBOX);
#ifdef ENABLE_CHEQUE_SYSTEM
	PyModule_AddIntConstant(poModule, "ITEM_TYPE_WON",				CItemData::ITEM_TYPE_WON);
#endif

#ifdef ENABLE_NEW_PET_SYSTEM
	PyModule_AddIntConstant(poModule, "ITEM_TYPE_PET",				CItemData::ITEM_TYPE_PET);
#endif
#ifdef ENABLE_COSTUME_SYSTEM
	PyModule_AddIntConstant(poModule, "ITEM_TYPE_COSTUME",			CItemData::ITEM_TYPE_COSTUME);

	// Item Sub Type
	PyModule_AddIntConstant(poModule, "COSTUME_TYPE_BODY",			CItemData::COSTUME_BODY);
	PyModule_AddIntConstant(poModule, "COSTUME_TYPE_HAIR",			CItemData::COSTUME_HAIR);
	#ifdef ENABLE_SASH_SYSTEM
	PyModule_AddIntConstant(poModule, "COSTUME_TYPE_SASH", CItemData::COSTUME_SASH);
	#endif
#ifdef ENABLE_COSTUME_WEAPON_SYSTEM
	PyModule_AddIntConstant(poModule, "COSTUME_TYPE_WEAPON", CItemData::COSTUME_WEAPON);
#endif
	PyModule_AddIntConstant(poModule, "COSTUME_TYPE_MOUNT",			CItemData::COSTUME_MOUNT);
#ifdef ENABLE_AURA_SYSTEM
	PyModule_AddIntConstant(poModule, "COSTUME_TYPE_AURA", CItemData::COSTUME_AURA);
#endif
	// 인벤토리 및 장비창에서의 슬롯 번호
	PyModule_AddIntConstant(poModule, "COSTUME_SLOT_START",			c_Costume_Slot_Start);
	PyModule_AddIntConstant(poModule, "COSTUME_SLOT_COUNT",			c_Costume_Slot_Count);
	PyModule_AddIntConstant(poModule, "COSTUME_SLOT_BODY",			c_Costume_Slot_Body);
	PyModule_AddIntConstant(poModule, "COSTUME_SLOT_HAIR",			c_Costume_Slot_Hair);
	#ifdef ENABLE_SASH_SYSTEM
	PyModule_AddIntConstant(poModule, "COSTUME_SLOT_SASH", c_Costume_Slot_Sash);
	#endif
#ifdef ENABLE_COSTUME_WEAPON_SYSTEM
	PyModule_AddIntConstant(poModule, "COSTUME_SLOT_WEAPON", c_Costume_Slot_Weapon);
#endif


	PyModule_AddIntConstant(poModule, "COSTUME_SLOT_MOUNT",			c_Costume_Slot_Mount);
#ifdef ENABLE_AURA_SYSTEM
	PyModule_AddIntConstant(poModule, "COSTUME_SLOT_AURA", c_Costume_Slot_Aura);
#endif

#ifdef NEW_COSTUME_SOCKET_RING
	PyModule_AddIntConstant(poModule, "COSTUME_TYPE_RING_SOCKET", CItemData::COSTUME_RING_SOCKET);
	PyModule_AddIntConstant(poModule, "COSTUME_SLOT_RING_SOCKET", c_Costume_Slot_Ring_Socket);

	PyModule_AddIntConstant(poModule, "COSTUME_TYPE_RING_SOCKET_2", CItemData::COSTUME_RING_SOCKET_2);
	PyModule_AddIntConstant(poModule, "COSTUME_SLOT_RING_SOCKET_2", c_Costume_Slot_Ring_Socket_2);

	PyModule_AddIntConstant(poModule, "COSTUME_TYPE_RING_SOCKET_3", CItemData::COSTUME_RING_SOCKET_3);
	PyModule_AddIntConstant(poModule, "COSTUME_SLOT_RING_SOCKET_3", c_Costume_Slot_Ring_Socket_3);

	PyModule_AddIntConstant(poModule, "COSTUME_TYPE_RING_SOCKET_4", CItemData::COSTUME_RING_SOCKET_4);
	PyModule_AddIntConstant(poModule, "COSTUME_SLOT_RING_SOCKET_4", c_Costume_Slot_Ring_Socket_4);

	PyModule_AddIntConstant(poModule, "COSTUME_TYPE_RING_SOCKET_5", CItemData::COSTUME_RING_SOCKET_5);
	PyModule_AddIntConstant(poModule, "COSTUME_SLOT_RING_SOCKET_5", c_Costume_Slot_Ring_Socket_5);

	PyModule_AddIntConstant(poModule, "COSTUME_TYPE_RING_SOCKET_6", CItemData::COSTUME_RING_SOCKET_6);
	PyModule_AddIntConstant(poModule, "COSTUME_SLOT_RING_SOCKET_6", c_Costume_Slot_Ring_Socket_6);
	
	
	PyModule_AddIntConstant(poModule, "COSTUME_TYPE_RING_SOCKET_7", CItemData::COSTUME_RING_SOCKET_7);
	PyModule_AddIntConstant(poModule, "COSTUME_SLOT_RING_SOCKET_7", c_Costume_Slot_Ring_Socket_7);
	
	PyModule_AddIntConstant(poModule, "COSTUME_TYPE_RING_SOCKET_8", CItemData::COSTUME_RING_SOCKET_8);
	PyModule_AddIntConstant(poModule, "COSTUME_SLOT_RING_SOCKET_8", c_Costume_Slot_Ring_Socket_8);
	
	PyModule_AddIntConstant(poModule, "COSTUME_TYPE_RING_SOCKET_9", CItemData::COSTUME_RING_SOCKET_9);
	PyModule_AddIntConstant(poModule, "COSTUME_SLOT_RING_SOCKET_9", c_Costume_Slot_Ring_Socket_9);
	
	PyModule_AddIntConstant(poModule, "COSTUME_TYPE_RING_SOCKET_10", CItemData::COSTUME_RING_SOCKET_10);
	PyModule_AddIntConstant(poModule, "COSTUME_SLOT_RING_SOCKET_10", c_Costume_Slot_Ring_Socket_10);
	
	PyModule_AddIntConstant(poModule, "COSTUME_TYPE_RING_SOCKET_11", CItemData::COSTUME_RING_SOCKET_11);
	PyModule_AddIntConstant(poModule, "COSTUME_SLOT_RING_SOCKET_11", c_Costume_Slot_Ring_Socket_11);
	
	PyModule_AddIntConstant(poModule, "COSTUME_TYPE_RING_SOCKET_12", CItemData::COSTUME_RING_SOCKET_12);
	PyModule_AddIntConstant(poModule, "COSTUME_SLOT_RING_SOCKET_12", c_Costume_Slot_Ring_Socket_12);


	PyModule_AddIntConstant(poModule, "COSTUME_TYPE_RING_SOCKET_13", CItemData::COSTUME_RING_SOCKET_13);
	PyModule_AddIntConstant(poModule, "COSTUME_SLOT_RING_SOCKET_13", c_Costume_Slot_Ring_Socket_13);
	
	PyModule_AddIntConstant(poModule, "COSTUME_TYPE_RING_SOCKET_14", CItemData::COSTUME_RING_SOCKET_14);
	PyModule_AddIntConstant(poModule, "COSTUME_SLOT_RING_SOCKET_14", c_Costume_Slot_Ring_Socket_14);
	
	PyModule_AddIntConstant(poModule, "COSTUME_TYPE_RING_SOCKET_15", CItemData::COSTUME_RING_SOCKET_15);
	PyModule_AddIntConstant(poModule, "COSTUME_SLOT_RING_SOCKET_15", c_Costume_Slot_Ring_Socket_15);
	
	PyModule_AddIntConstant(poModule, "COSTUME_TYPE_RING_SOCKET_16", CItemData::COSTUME_RING_SOCKET_16);
	PyModule_AddIntConstant(poModule, "COSTUME_SLOT_RING_SOCKET_16", c_Costume_Slot_Ring_Socket_16);
	
	PyModule_AddIntConstant(poModule, "COSTUME_TYPE_RING_SOCKET_17", CItemData::COSTUME_RING_SOCKET_17);
	PyModule_AddIntConstant(poModule, "COSTUME_SLOT_RING_SOCKET_17", c_Costume_Slot_Ring_Socket_17);
	
	PyModule_AddIntConstant(poModule, "COSTUME_TYPE_RING_SOCKET_18", CItemData::COSTUME_RING_SOCKET_18);
	PyModule_AddIntConstant(poModule, "COSTUME_SLOT_RING_SOCKET_18", c_Costume_Slot_Ring_Socket_18);
	
	
	PyModule_AddIntConstant(poModule, "COSTUME_TYPE_RING_SOCKET_19", CItemData::COSTUME_RING_SOCKET_19);
	PyModule_AddIntConstant(poModule, "COSTUME_SLOT_RING_SOCKET_19", c_Costume_Slot_Ring_Socket_19);
	
	PyModule_AddIntConstant(poModule, "COSTUME_TYPE_RING_SOCKET_20", CItemData::COSTUME_RING_SOCKET_20);
	PyModule_AddIntConstant(poModule, "COSTUME_SLOT_RING_SOCKET_20", c_Costume_Slot_Ring_Socket_20);
	
	PyModule_AddIntConstant(poModule, "COSTUME_TYPE_RING_SOCKET_21", CItemData::COSTUME_RING_SOCKET_21);
	PyModule_AddIntConstant(poModule, "COSTUME_SLOT_RING_SOCKET_21", c_Costume_Slot_Ring_Socket_21);
	
	PyModule_AddIntConstant(poModule, "COSTUME_TYPE_RING_SOCKET_22", CItemData::COSTUME_RING_SOCKET_22);
	PyModule_AddIntConstant(poModule, "COSTUME_SLOT_RING_SOCKET_22", c_Costume_Slot_Ring_Socket_22);
	
	PyModule_AddIntConstant(poModule, "COSTUME_TYPE_RING_SOCKET_23", CItemData::COSTUME_RING_SOCKET_23);
	PyModule_AddIntConstant(poModule, "COSTUME_SLOT_RING_SOCKET_23", c_Costume_Slot_Ring_Socket_23);
	
	PyModule_AddIntConstant(poModule, "COSTUME_TYPE_RING_SOCKET_24", CItemData::COSTUME_RING_SOCKET_24);
	PyModule_AddIntConstant(poModule, "COSTUME_SLOT_RING_SOCKET_24", c_Costume_Slot_Ring_Socket_24);
	
	
	PyModule_AddIntConstant(poModule, "COSTUME_TYPE_RING_SOCKET_25", CItemData::COSTUME_RING_SOCKET_25);
	PyModule_AddIntConstant(poModule, "COSTUME_SLOT_RING_SOCKET_25", c_Costume_Slot_Ring_Socket_25);
	
	PyModule_AddIntConstant(poModule, "COSTUME_TYPE_RING_SOCKET_26", CItemData::COSTUME_RING_SOCKET_26);
	PyModule_AddIntConstant(poModule, "COSTUME_SLOT_RING_SOCKET_26", c_Costume_Slot_Ring_Socket_26);
	
	PyModule_AddIntConstant(poModule, "COSTUME_TYPE_RING_SOCKET_27", CItemData::COSTUME_RING_SOCKET_27);
	PyModule_AddIntConstant(poModule, "COSTUME_SLOT_RING_SOCKET_27", c_Costume_Slot_Ring_Socket_27);
	
	PyModule_AddIntConstant(poModule, "COSTUME_TYPE_RING_SOCKET_28", CItemData::COSTUME_RING_SOCKET_28);
	PyModule_AddIntConstant(poModule, "COSTUME_SLOT_RING_SOCKET_28", c_Costume_Slot_Ring_Socket_28);
	
	PyModule_AddIntConstant(poModule, "COSTUME_TYPE_RING_SOCKET_29", CItemData::COSTUME_RING_SOCKET_29);
	PyModule_AddIntConstant(poModule, "COSTUME_SLOT_RING_SOCKET_29", c_Costume_Slot_Ring_Socket_29);
	
	PyModule_AddIntConstant(poModule, "COSTUME_TYPE_RING_SOCKET_30", CItemData::COSTUME_RING_SOCKET_30);
	PyModule_AddIntConstant(poModule, "COSTUME_SLOT_RING_SOCKET_30", c_Costume_Slot_Ring_Socket_30);
#endif
	PyModule_AddIntConstant(poModule, "COSTUME_SLOT_END",			c_Costume_Slot_End);
#endif



#ifdef ENABLE_NEW_EQUIPMENT_SYSTEM
	PyModule_AddIntConstant(poModule, "BELT_INVENTORY_SLOT_START",			c_Belt_Inventory_Slot_Start);
	PyModule_AddIntConstant(poModule, "BELT_INVENTORY_SLOT_COUNT",			c_Belt_Inventory_Slot_Count);
	PyModule_AddIntConstant(poModule, "BELT_INVENTORY_SLOT_END",			c_Belt_Inventory_Slot_End);

#endif

	PyModule_AddIntConstant(poModule, "WEAPON_SWORD",				CItemData::WEAPON_SWORD);
	PyModule_AddIntConstant(poModule, "WEAPON_DAGGER",				CItemData::WEAPON_DAGGER);
	PyModule_AddIntConstant(poModule, "WEAPON_BOW",					CItemData::WEAPON_BOW);
	PyModule_AddIntConstant(poModule, "WEAPON_TWO_HANDED",			CItemData::WEAPON_TWO_HANDED);
	PyModule_AddIntConstant(poModule, "WEAPON_BELL",				CItemData::WEAPON_BELL);
	PyModule_AddIntConstant(poModule, "WEAPON_FAN",					CItemData::WEAPON_FAN);
	PyModule_AddIntConstant(poModule, "WEAPON_ARROW",				CItemData::WEAPON_ARROW);
	PyModule_AddIntConstant(poModule, "WEAPON_CLAW", CItemData::WEAPON_CLAW);
#ifdef ENABLE_NEW_ARROW_SYSTEM
	PyModule_AddIntConstant(poModule, "WEAPON_UNLIMITED_ARROW", CItemData::WEAPON_UNLIMITED_ARROW);
#endif
	PyModule_AddIntConstant(poModule, "WEAPON_NUM_TYPES",			CItemData::WEAPON_NUM_TYPES);

	PyModule_AddIntConstant(poModule, "USE_POTION",					CItemData::USE_POTION);
	PyModule_AddIntConstant(poModule, "USE_TALISMAN",				CItemData::USE_TALISMAN);
	PyModule_AddIntConstant(poModule, "USE_TUNING",					CItemData::USE_TUNING);
	PyModule_AddIntConstant(poModule, "USE_MOVE",					CItemData::USE_MOVE);
	PyModule_AddIntConstant(poModule, "USE_TREASURE_BOX",			CItemData::USE_TREASURE_BOX);
	PyModule_AddIntConstant(poModule, "USE_MONEYBAG",				CItemData::USE_MONEYBAG);
	PyModule_AddIntConstant(poModule, "USE_BAIT",					CItemData::USE_BAIT);
	PyModule_AddIntConstant(poModule, "USE_ABILITY_UP",				CItemData::USE_ABILITY_UP);
	PyModule_AddIntConstant(poModule, "USE_AFFECT",					CItemData::USE_AFFECT);
	PyModule_AddIntConstant(poModule, "USE_CREATE_STONE",			CItemData::USE_CREATE_STONE);
	PyModule_AddIntConstant(poModule, "USE_SPECIAL",				CItemData::USE_SPECIAL);
	PyModule_AddIntConstant(poModule, "USE_POTION_NODELAY",			CItemData::USE_POTION_NODELAY);
	PyModule_AddIntConstant(poModule, "USE_CLEAR",					CItemData::USE_CLEAR);
	PyModule_AddIntConstant(poModule, "USE_INVISIBILITY",			CItemData::USE_INVISIBILITY);
	PyModule_AddIntConstant(poModule, "USE_DETACHMENT",				CItemData::USE_DETACHMENT);
	PyModule_AddIntConstant(poModule, "USE_TIME_CHARGE_PER",		CItemData::USE_TIME_CHARGE_PER);
	PyModule_AddIntConstant(poModule, "USE_TIME_CHARGE_FIX",		CItemData::USE_TIME_CHARGE_FIX);
	PyModule_AddIntConstant(poModule, "USE_PET", CItemData::USE_PET);
	PyModule_AddIntConstant(poModule, "USE_COSTUME_MOUNT_SKIN",		CItemData::USE_COSTUME_MOUNT_SKIN);

	PyModule_AddIntConstant(poModule, "MATERIAL_DS_REFINE_NORMAL",	CItemData::MATERIAL_DS_REFINE_NORMAL);
	PyModule_AddIntConstant(poModule, "MATERIAL_DS_REFINE_BLESSED",	CItemData::MATERIAL_DS_REFINE_BLESSED);
	PyModule_AddIntConstant(poModule, "MATERIAL_DS_REFINE_HOLLY",	CItemData::MATERIAL_DS_REFINE_HOLLY);

	PyModule_AddIntConstant(poModule, "METIN_NORMAL",				CItemData::METIN_NORMAL);
	PyModule_AddIntConstant(poModule, "METIN_GOLD",					CItemData::METIN_GOLD);

	PyModule_AddIntConstant(poModule, "LIMIT_NONE",					CItemData::LIMIT_NONE);
	PyModule_AddIntConstant(poModule, "LIMIT_LEVEL",				CItemData::LIMIT_LEVEL);
	PyModule_AddIntConstant(poModule, "LIMIT_STR",					CItemData::LIMIT_STR);
	PyModule_AddIntConstant(poModule, "LIMIT_DEX",					CItemData::LIMIT_DEX);
	PyModule_AddIntConstant(poModule, "LIMIT_INT",					CItemData::LIMIT_INT);
	PyModule_AddIntConstant(poModule, "LIMIT_CON",					CItemData::LIMIT_CON);
	PyModule_AddIntConstant(poModule, "LIMIT_PCBANG",				CItemData::LIMIT_PCBANG);
	PyModule_AddIntConstant(poModule, "LIMIT_REAL_TIME",			CItemData::LIMIT_REAL_TIME);
	PyModule_AddIntConstant(poModule, "LIMIT_REAL_TIME_START_FIRST_USE",	CItemData::LIMIT_REAL_TIME_START_FIRST_USE);
	PyModule_AddIntConstant(poModule, "LIMIT_TIMER_BASED_ON_WEAR",	CItemData::LIMIT_TIMER_BASED_ON_WEAR);
	PyModule_AddIntConstant(poModule, "LIMIT_TYPE_MAX_NUM",			CItemData::LIMIT_MAX_NUM);
	PyModule_AddIntConstant(poModule, "LIMIT_MAX_NUM",				CItemData::ITEM_LIMIT_MAX_NUM);

	PyModule_AddIntConstant(poModule, "ITEM_ANTIFLAG_FEMALE",		CItemData::ITEM_ANTIFLAG_FEMALE);
	PyModule_AddIntConstant(poModule, "ITEM_ANTIFLAG_MALE",			CItemData::ITEM_ANTIFLAG_MALE);
	PyModule_AddIntConstant(poModule, "ITEM_ANTIFLAG_WARRIOR",		CItemData::ITEM_ANTIFLAG_WARRIOR);
	PyModule_AddIntConstant(poModule, "ITEM_ANTIFLAG_ASSASSIN",		CItemData::ITEM_ANTIFLAG_ASSASSIN);
	PyModule_AddIntConstant(poModule, "ITEM_ANTIFLAG_SURA",			CItemData::ITEM_ANTIFLAG_SURA);
	PyModule_AddIntConstant(poModule, "ITEM_ANTIFLAG_SHAMAN",		CItemData::ITEM_ANTIFLAG_SHAMAN);
	PyModule_AddIntConstant(poModule, "ITEM_ANTIFLAG_WOLFMAN",		CItemData::ITEM_ANTIFLAG_WOLFMAN);
	PyModule_AddIntConstant(poModule, "ITEM_ANTIFLAG_GET",			CItemData::ITEM_ANTIFLAG_GET);
	PyModule_AddIntConstant(poModule, "ITEM_ANTIFLAG_DROP",			CItemData::ITEM_ANTIFLAG_DROP);
	PyModule_AddIntConstant(poModule, "ITEM_ANTIFLAG_SELL",			CItemData::ITEM_ANTIFLAG_SELL);
	PyModule_AddIntConstant(poModule, "ITEM_ANTIFLAG_EMPIRE_A",		CItemData::ITEM_ANTIFLAG_EMPIRE_A);
	PyModule_AddIntConstant(poModule, "ITEM_ANTIFLAG_EMPIRE_B",		CItemData::ITEM_ANTIFLAG_EMPIRE_B);
	PyModule_AddIntConstant(poModule, "ITEM_ANTIFLAG_EMPIRE_R",		CItemData::ITEM_ANTIFLAG_EMPIRE_R);
	PyModule_AddIntConstant(poModule, "ITEM_ANTIFLAG_SAVE",			CItemData::ITEM_ANTIFLAG_SAVE);
	PyModule_AddIntConstant(poModule, "ITEM_ANTIFLAG_GIVE",			CItemData::ITEM_ANTIFLAG_GIVE);
	PyModule_AddIntConstant(poModule, "ITEM_ANTIFLAG_PKDROP",		CItemData::ITEM_ANTIFLAG_PKDROP);
	PyModule_AddIntConstant(poModule, "ITEM_ANTIFLAG_STACK",		CItemData::ITEM_ANTIFLAG_STACK);
	PyModule_AddIntConstant(poModule, "ITEM_ANTIFLAG_MYSHOP",		CItemData::ITEM_ANTIFLAG_MYSHOP);
	PyModule_AddIntConstant(poModule, "ITEM_ANTIFLAG_SAFEBOX",		CItemData::ITEM_ANTIFLAG_SAFEBOX);
	PyModule_AddIntConstant(poModule, "ITEM_ANTIFLAG_SELL_WITH_METIN",			CItemData::ITEM_ANTIFLAG_SELL_WITH_METIN);
	PyModule_AddIntConstant(poModule, "ITEM_FLAG_RARE",				CItemData::ITEM_FLAG_RARE);
	PyModule_AddIntConstant(poModule, "ITEM_FLAG_UNIQUE",			CItemData::ITEM_FLAG_UNIQUE);
	PyModule_AddIntConstant(poModule, "ITEM_FLAG_CONFIRM_WHEN_USE",	CItemData::ITEM_FLAG_CONFIRM_WHEN_USE);

	PyModule_AddIntConstant(poModule, "ANTIFLAG_FEMALE",			CItemData::ITEM_ANTIFLAG_FEMALE);
	PyModule_AddIntConstant(poModule, "ANTIFLAG_MALE",				CItemData::ITEM_ANTIFLAG_MALE);
	PyModule_AddIntConstant(poModule, "ANTIFLAG_WARRIOR",			CItemData::ITEM_ANTIFLAG_WARRIOR);
	PyModule_AddIntConstant(poModule, "ANTIFLAG_ASSASSIN",			CItemData::ITEM_ANTIFLAG_ASSASSIN);
	PyModule_AddIntConstant(poModule, "ANTIFLAG_SURA",				CItemData::ITEM_ANTIFLAG_SURA);
	PyModule_AddIntConstant(poModule, "ANTIFLAG_SHAMAN",			CItemData::ITEM_ANTIFLAG_SHAMAN);
	PyModule_AddIntConstant(poModule, "ANTIFLAG_WOLFMAN",			CItemData::ITEM_ANTIFLAG_WOLFMAN);
	PyModule_AddIntConstant(poModule, "ANTIFLAG_GET",				CItemData::ITEM_ANTIFLAG_GET);
	PyModule_AddIntConstant(poModule, "ANTIFLAG_DROP",				CItemData::ITEM_ANTIFLAG_DROP);
	PyModule_AddIntConstant(poModule, "ANTIFLAG_SELL",				CItemData::ITEM_ANTIFLAG_SELL);
	PyModule_AddIntConstant(poModule, "ANTIFLAG_EMPIRE_A",			CItemData::ITEM_ANTIFLAG_EMPIRE_A);
	PyModule_AddIntConstant(poModule, "ANTIFLAG_EMPIRE_B",			CItemData::ITEM_ANTIFLAG_EMPIRE_B);
	PyModule_AddIntConstant(poModule, "ANTIFLAG_EMPIRE_R",			CItemData::ITEM_ANTIFLAG_EMPIRE_R);
	PyModule_AddIntConstant(poModule, "ANTIFLAG_SAVE",				CItemData::ITEM_ANTIFLAG_SAVE);
	PyModule_AddIntConstant(poModule, "ANTIFLAG_GIVE",				CItemData::ITEM_ANTIFLAG_GIVE);
	PyModule_AddIntConstant(poModule, "ANTIFLAG_PKDROP",			CItemData::ITEM_ANTIFLAG_PKDROP);
	PyModule_AddIntConstant(poModule, "ANTIFLAG_STACK",				CItemData::ITEM_ANTIFLAG_STACK);
	PyModule_AddIntConstant(poModule, "ANTIFLAG_MYSHOP",			CItemData::ITEM_ANTIFLAG_MYSHOP);
	PyModule_AddIntConstant(poModule, "ANTIFLAG_SAFEBOX",			CItemData::ITEM_ANTIFLAG_SAFEBOX);
	PyModule_AddIntConstant(poModule, "ANTIFLAG_SELL_WITH_METIN",				CItemData::ITEM_ANTIFLAG_SELL_WITH_METIN);
	PyModule_AddIntConstant(poModule, "WEARABLE_BODY",				CItemData::WEARABLE_BODY);
	PyModule_AddIntConstant(poModule, "WEARABLE_HEAD",				CItemData::WEARABLE_HEAD);
	PyModule_AddIntConstant(poModule, "WEARABLE_FOOTS",				CItemData::WEARABLE_FOOTS);
	PyModule_AddIntConstant(poModule, "WEARABLE_WRIST",				CItemData::WEARABLE_WRIST);
	PyModule_AddIntConstant(poModule, "WEARABLE_WEAPON",			CItemData::WEARABLE_WEAPON);
	PyModule_AddIntConstant(poModule, "WEARABLE_NECK",				CItemData::WEARABLE_NECK);
	PyModule_AddIntConstant(poModule, "WEARABLE_EAR",				CItemData::WEARABLE_EAR);
	PyModule_AddIntConstant(poModule, "WEARABLE_UNIQUE",			CItemData::WEARABLE_UNIQUE);
	PyModule_AddIntConstant(poModule, "WEARABLE_SHIELD",			CItemData::WEARABLE_SHIELD);
	PyModule_AddIntConstant(poModule, "WEARABLE_ARROW",				CItemData::WEARABLE_ARROW);

	PyModule_AddIntConstant(poModule, "ARMOR_BODY",					CItemData::ARMOR_BODY);
	PyModule_AddIntConstant(poModule, "ARMOR_HEAD",					CItemData::ARMOR_HEAD);
	PyModule_AddIntConstant(poModule, "ARMOR_SHIELD",				CItemData::ARMOR_SHIELD);
	PyModule_AddIntConstant(poModule, "ARMOR_WRIST",				CItemData::ARMOR_WRIST);
	PyModule_AddIntConstant(poModule, "ARMOR_FOOTS",				CItemData::ARMOR_FOOTS);
	PyModule_AddIntConstant(poModule, "ARMOR_NECK",					CItemData::ARMOR_NECK);
	PyModule_AddIntConstant(poModule, "ARMOR_EAR",					CItemData::ARMOR_EAR);
	PyModule_AddIntConstant(poModule, "ARMOR_ELEMENT",					CItemData::ARMOR_ELEMENT);	
	
	PyModule_AddIntConstant(poModule, "ITEM_APPLY_MAX_NUM",			CItemData::ITEM_APPLY_MAX_NUM);
	PyModule_AddIntConstant(poModule, "ITEM_SOCKET_MAX_NUM",		CItemData::ITEM_SOCKET_MAX_NUM);

	PyModule_AddIntConstant(poModule, "APPLY_NONE",					CItemData::APPLY_NONE);
	PyModule_AddIntConstant(poModule, "APPLY_STR",					CItemData::APPLY_STR);
	PyModule_AddIntConstant(poModule, "APPLY_DEX",					CItemData::APPLY_DEX);
	PyModule_AddIntConstant(poModule, "APPLY_CON",					CItemData::APPLY_CON);
	PyModule_AddIntConstant(poModule, "APPLY_INT",					CItemData::APPLY_INT);
	PyModule_AddIntConstant(poModule, "APPLY_MAX_HP",				CItemData::APPLY_MAX_HP);
	PyModule_AddIntConstant(poModule, "APPLY_MAX_SP",				CItemData::APPLY_MAX_SP);
	PyModule_AddIntConstant(poModule, "APPLY_HP_REGEN",				CItemData::APPLY_HP_REGEN);
	PyModule_AddIntConstant(poModule, "APPLY_SP_REGEN",				CItemData::APPLY_SP_REGEN);
	PyModule_AddIntConstant(poModule, "APPLY_DEF_GRADE_BONUS",		CItemData::APPLY_DEF_GRADE_BONUS);
	PyModule_AddIntConstant(poModule, "APPLY_ATT_GRADE_BONUS",		CItemData::APPLY_ATT_GRADE_BONUS);
	PyModule_AddIntConstant(poModule, "APPLY_ATT_SPEED",			CItemData::APPLY_ATT_SPEED);
	PyModule_AddIntConstant(poModule, "APPLY_MOV_SPEED",			CItemData::APPLY_MOV_SPEED);
	PyModule_AddIntConstant(poModule, "APPLY_CAST_SPEED",			CItemData::APPLY_CAST_SPEED);
	PyModule_AddIntConstant(poModule, "APPLY_MAGIC_ATT_GRADE",		CItemData::APPLY_MAGIC_ATT_GRADE);
	PyModule_AddIntConstant(poModule, "APPLY_MAGIC_DEF_GRADE",		CItemData::APPLY_MAGIC_DEF_GRADE);
	PyModule_AddIntConstant(poModule, "APPLY_SKILL",				CItemData::APPLY_SKILL);
    PyModule_AddIntConstant(poModule, "APPLY_ATTBONUS_ANIMAL",		CItemData::APPLY_ATTBONUS_ANIMAL);
    PyModule_AddIntConstant(poModule, "APPLY_ATTBONUS_UNDEAD",		CItemData::APPLY_ATTBONUS_UNDEAD);
    PyModule_AddIntConstant(poModule, "APPLY_ATTBONUS_DEVIL", 		CItemData::APPLY_ATTBONUS_DEVIL);
    PyModule_AddIntConstant(poModule, "APPLY_ATTBONUS_HUMAN",		CItemData::APPLY_ATTBONUS_HUMAN); // bonuslari neden cliente eklememisler ki
    PyModule_AddIntConstant(poModule, "APPLY_BOW_DISTANCE", 		CItemData::APPLY_BOW_DISTANCE);
    PyModule_AddIntConstant(poModule, "APPLY_RESIST_BOW", 			CItemData::APPLY_RESIST_BOW);
    PyModule_AddIntConstant(poModule, "APPLY_RESIST_FIRE", 			CItemData::APPLY_RESIST_FIRE);
    PyModule_AddIntConstant(poModule, "APPLY_RESIST_ELEC", 			CItemData::APPLY_RESIST_ELEC);
    PyModule_AddIntConstant(poModule, "APPLY_RESIST_MAGIC", 		CItemData::APPLY_RESIST_MAGIC);
    PyModule_AddIntConstant(poModule, "APPLY_POISON_PCT",			CItemData::APPLY_POISON_PCT);
    PyModule_AddIntConstant(poModule, "APPLY_SLOW_PCT", 			CItemData::APPLY_SLOW_PCT);
    PyModule_AddIntConstant(poModule, "APPLY_STUN_PCT", 			CItemData::APPLY_STUN_PCT);
	PyModule_AddIntConstant(poModule, "APPLY_CRITICAL_PCT",			CItemData::APPLY_CRITICAL_PCT);			// n% 확률로 두배 타격
	PyModule_AddIntConstant(poModule, "APPLY_PENETRATE_PCT",		CItemData::APPLY_PENETRATE_PCT);		// n% 확률로 적의 방어력 무시
	PyModule_AddIntConstant(poModule, "APPLY_ATTBONUS_ORC",			CItemData::APPLY_ATTBONUS_ORC);			// 웅귀에게 n% 추가 데미지
	PyModule_AddIntConstant(poModule, "APPLY_ATTBONUS_MILGYO",		CItemData::APPLY_ATTBONUS_MILGYO);		// 밀교에게 n% 추가 데미지
	PyModule_AddIntConstant(poModule, "APPLY_ATTBONUS_UNDEAD",		CItemData::APPLY_ATTBONUS_UNDEAD);		// 시체에게 n% 추가 데미지
	PyModule_AddIntConstant(poModule, "APPLY_ATTBONUS_DEVIL",		CItemData::APPLY_ATTBONUS_DEVIL);		// 악마에게 n% 추가 데미지
	PyModule_AddIntConstant(poModule, "APPLY_STEAL_HP",				CItemData::APPLY_STEAL_HP);				// n% 확률로 타격의 10% 를 생명력으로 흡수
	PyModule_AddIntConstant(poModule, "APPLY_STEAL_SP",				CItemData::APPLY_STEAL_SP);				// n% 확률로 타격의 10% 를 정신력으로 흡수
	PyModule_AddIntConstant(poModule, "APPLY_MANA_BURN_PCT",		CItemData::APPLY_MANA_BURN_PCT);		// n% 확률로 상대의 마나를 깎는다
	PyModule_AddIntConstant(poModule, "APPLY_DAMAGE_SP_RECOVER",	CItemData::APPLY_DAMAGE_SP_RECOVER);	// n% 확률로 정신력 2 회복
	PyModule_AddIntConstant(poModule, "APPLY_BLOCK",				CItemData::APPLY_BLOCK);				// n% 확률로 물리공격 완벽 방어
	PyModule_AddIntConstant(poModule, "APPLY_DODGE",				CItemData::APPLY_DODGE);				// n% 확률로 물리공격 완벽 회피
	PyModule_AddIntConstant(poModule, "APPLY_RESIST_SWORD",			CItemData::APPLY_RESIST_SWORD);			// 한손검에 의한 피해를 n% 감소
	PyModule_AddIntConstant(poModule, "APPLY_RESIST_TWOHAND",		CItemData::APPLY_RESIST_TWOHAND);		// 양손검에 의한 피해를 n% 감소
	PyModule_AddIntConstant(poModule, "APPLY_RESIST_DAGGER",		CItemData::APPLY_RESIST_DAGGER);		// 단도에 의한 피해를 n% 감소
	PyModule_AddIntConstant(poModule, "APPLY_RESIST_BELL",			CItemData::APPLY_RESIST_BELL);			// 방울에 의한 피해를 n% 감소
	PyModule_AddIntConstant(poModule, "APPLY_RESIST_FAN",			CItemData::APPLY_RESIST_FAN);			// 부채에 의한 피해를 n% 감소
	PyModule_AddIntConstant(poModule, "APPLY_RESIST_WIND",			CItemData::APPLY_RESIST_WIND);			// 바람에 의한 피해를 n% 감소
	PyModule_AddIntConstant(poModule, "APPLY_RESIST_CLAW", 			CItemData::APPLY_RESIST_CLAW);
	PyModule_AddIntConstant(poModule, "APPLY_REFLECT_MELEE",		CItemData::APPLY_REFLECT_MELEE);		// 근접 타격 n% 를 적에게 되돌린다
	PyModule_AddIntConstant(poModule, "APPLY_REFLECT_CURSE",		CItemData::APPLY_REFLECT_CURSE);		// 적이 나에게 저주 사용시 n% 확률로 되돌린다
	PyModule_AddIntConstant(poModule, "APPLY_POISON_REDUCE",		CItemData::APPLY_POISON_REDUCE);		// 독에 의한 데미지 감소
	PyModule_AddIntConstant(poModule, "APPLY_KILL_SP_RECOVER",		CItemData::APPLY_KILL_SP_RECOVER);		// 적을 죽였을때 n% 확률로 정신력 10 회복
	PyModule_AddIntConstant(poModule, "APPLY_EXP_DOUBLE_BONUS",		CItemData::APPLY_EXP_DOUBLE_BONUS);		// n% 확률로 경험치 획득량 2배
	PyModule_AddIntConstant(poModule, "APPLY_GOLD_DOUBLE_BONUS",	CItemData::APPLY_GOLD_DOUBLE_BONUS);	// n% 확률로 돈 획득량 2배
	PyModule_AddIntConstant(poModule, "APPLY_ITEM_DROP_BONUS",		CItemData::APPLY_ITEM_DROP_BONUS);		// n% 확률로 아이템 획득량 2배
	PyModule_AddIntConstant(poModule, "APPLY_POTION_BONUS",			CItemData::APPLY_POTION_BONUS);			// 물약 복용시 n% 만큼 성능 증대
	PyModule_AddIntConstant(poModule, "APPLY_KILL_HP_RECOVER",		CItemData::APPLY_KILL_HP_RECOVER);		// 죽일때마다 생명력 회복 
	PyModule_AddIntConstant(poModule, "APPLY_IMMUNE_STUN",			CItemData::APPLY_IMMUNE_STUN);			// 기절 하지 않는다
	PyModule_AddIntConstant(poModule, "APPLY_IMMUNE_SLOW",			CItemData::APPLY_IMMUNE_SLOW);			// 느려지지 않는다
	PyModule_AddIntConstant(poModule, "APPLY_IMMUNE_FALL",			CItemData::APPLY_IMMUNE_FALL);			// 넘어지지 않는다
	PyModule_AddIntConstant(poModule, "APPLY_MAX_STAMINA",			CItemData::APPLY_MAX_STAMINA);			// 최대 스테미너 증가
	PyModule_AddIntConstant(poModule, "APPLY_ATTBONUS_WARRIOR",		CItemData::APPLY_ATT_BONUS_TO_WARRIOR);
	PyModule_AddIntConstant(poModule, "APPLY_ATTBONUS_ASSASSIN",	CItemData::APPLY_ATT_BONUS_TO_ASSASSIN);
	PyModule_AddIntConstant(poModule, "APPLY_ATTBONUS_SURA",		CItemData::APPLY_ATT_BONUS_TO_SURA);
	PyModule_AddIntConstant(poModule, "APPLY_ATTBONUS_SHAMAN",		CItemData::APPLY_ATT_BONUS_TO_SHAMAN);
	PyModule_AddIntConstant(poModule, "APPLY_ATTBONUS_WOLFMAN",		CItemData::APPLY_ATT_BONUS_TO_WOLFMAN);
	PyModule_AddIntConstant(poModule, "APPLY_ATTBONUS_MONSTER",		CItemData::APPLY_ATT_BONUS_TO_MONSTER);
	PyModule_AddIntConstant(poModule, "APPLY_MALL_ATTBONUS",		CItemData::APPLY_MALL_ATTBONUS);
	PyModule_AddIntConstant(poModule, "APPLY_MALL_DEFBONUS",		CItemData::APPLY_MALL_DEFBONUS);
	PyModule_AddIntConstant(poModule, "APPLY_MALL_EXPBONUS",		CItemData::APPLY_MALL_EXPBONUS);
	PyModule_AddIntConstant(poModule, "APPLY_MALL_ITEMBONUS",		CItemData::APPLY_MALL_ITEMBONUS);
	PyModule_AddIntConstant(poModule, "APPLY_MALL_GOLDBONUS",		CItemData::APPLY_MALL_GOLDBONUS);
	PyModule_AddIntConstant(poModule, "APPLY_MAX_HP_PCT",			CItemData::APPLY_MAX_HP_PCT);
	PyModule_AddIntConstant(poModule, "APPLY_MAX_SP_PCT",			CItemData::APPLY_MAX_SP_PCT);
	PyModule_AddIntConstant(poModule, "APPLY_SKILL_DAMAGE_BONUS",		CItemData::APPLY_SKILL_DAMAGE_BONUS);
	PyModule_AddIntConstant(poModule, "APPLY_NORMAL_HIT_DAMAGE_BONUS",	CItemData::APPLY_NORMAL_HIT_DAMAGE_BONUS);
	PyModule_AddIntConstant(poModule, "APPLY_SKILL_DEFEND_BONUS",		CItemData::APPLY_SKILL_DEFEND_BONUS);
	PyModule_AddIntConstant(poModule, "APPLY_NORMAL_HIT_DEFEND_BONUS",	CItemData::APPLY_NORMAL_HIT_DEFEND_BONUS);
#ifdef ENABLE_GLOVE_SYSTEM
	PyModule_AddIntConstant(poModule, "APPLY_RANDOM",	CItemData::APPLY_RANDOM);
#endif

	PyModule_AddIntConstant(poModule, "APPLY_PC_BANG_EXP_BONUS",	CItemData::APPLY_PC_BANG_EXP_BONUS);
	PyModule_AddIntConstant(poModule, "APPLY_PC_BANG_DROP_BONUS",	CItemData::APPLY_PC_BANG_DROP_BONUS);

	PyModule_AddIntConstant(poModule, "APPLY_RESIST_WARRIOR",	CItemData::APPLY_RESIST_WARRIOR );
	PyModule_AddIntConstant(poModule, "APPLY_RESIST_ASSASSIN",	CItemData::APPLY_RESIST_ASSASSIN );
	PyModule_AddIntConstant(poModule, "APPLY_RESIST_SURA",		CItemData::APPLY_RESIST_SURA );
	PyModule_AddIntConstant(poModule, "APPLY_RESIST_SHAMAN",	CItemData::APPLY_RESIST_SHAMAN );
	PyModule_AddIntConstant(poModule, "APPLY_RESIST_WOLFMAN",	CItemData::APPLY_RESIST_WOLFMAN);
	PyModule_AddIntConstant(poModule, "APPLY_RESIST_CLAW",		CItemData::APPLY_RESIST_CLAW);
	PyModule_AddIntConstant(poModule, "APPLY_ENERGY",	CItemData::APPLY_ENERGY );		// 기력
	PyModule_AddIntConstant(poModule, "APPLY_COSTUME_ATTR_BONUS",	CItemData::APPLY_COSTUME_ATTR_BONUS );		

	PyModule_AddIntConstant(poModule, "APPLY_MAGIC_ATTBONUS_PER",	CItemData::APPLY_MAGIC_ATTBONUS_PER );		
	PyModule_AddIntConstant(poModule, "APPLY_MELEE_MAGIC_ATTBONUS_PER",	CItemData::APPLY_MELEE_MAGIC_ATTBONUS_PER );		
	PyModule_AddIntConstant(poModule, "APPLY_RESIST_ICE",	CItemData::APPLY_RESIST_ICE );		
	PyModule_AddIntConstant(poModule, "APPLY_RESIST_EARTH",	CItemData::APPLY_RESIST_EARTH );		
	PyModule_AddIntConstant(poModule, "APPLY_RESIST_DARK",	CItemData::APPLY_RESIST_DARK );		
	PyModule_AddIntConstant(poModule, "APPLY_ANTI_CRITICAL_PCT",	CItemData::APPLY_ANTI_CRITICAL_PCT );		
	PyModule_AddIntConstant(poModule, "APPLY_ANTI_PENETRATE_PCT",	CItemData::APPLY_ANTI_PENETRATE_PCT );		
#ifdef WJ_MAGIC_REDUCION_BONUS
	PyModule_AddIntConstant(poModule, "APPLY_ANTI_RESIST_MAGIC", CItemData::APPLY_ANTI_RESIST_MAGIC);
#endif
#ifdef ADD_NEW_BONUS
	PyModule_AddIntConstant(poModule, "APPLY_ATTBONUS_METIN", CItemData::APPLY_ATTBONUS_METIN);
	PyModule_AddIntConstant(poModule, "APPLY_RESIST_MONSTER", CItemData::APPLY_RESIST_MONSTER);
	PyModule_AddIntConstant(poModule, "APPLY_ATTBONUS_ELEC", CItemData::APPLY_ATTBONUS_ELEC);
	PyModule_AddIntConstant(poModule, "APPLY_ATTBONUS_FIRE", CItemData::APPLY_ATTBONUS_FIRE);
	PyModule_AddIntConstant(poModule, "APPLY_ATTBONUS_ICE", CItemData::APPLY_ATTBONUS_ICE);
	PyModule_AddIntConstant(poModule, "MASK_ITEM_TYPE_NONE", CItemData::MASK_ITEM_TYPE_NONE);
	PyModule_AddIntConstant(poModule, "MASK_ITEM_TYPE_MOUNT_PET", CItemData::MASK_ITEM_TYPE_MOUNT_PET);
	PyModule_AddIntConstant(poModule, "MASK_ITEM_TYPE_EQUIPMENT_WEAPON", CItemData::MASK_ITEM_TYPE_EQUIPMENT_WEAPON);
	PyModule_AddIntConstant(poModule, "MASK_ITEM_TYPE_EQUIPMENT_ARMOR", CItemData::MASK_ITEM_TYPE_EQUIPMENT_ARMOR);
	PyModule_AddIntConstant(poModule, "MASK_ITEM_TYPE_EQUIPMENT_JEWELRY", CItemData::MASK_ITEM_TYPE_EQUIPMENT_JEWELRY);
	PyModule_AddIntConstant(poModule, "MASK_ITEM_TYPE_TUNING", CItemData::MASK_ITEM_TYPE_TUNING);
	PyModule_AddIntConstant(poModule, "MASK_ITEM_TYPE_POTION", CItemData::MASK_ITEM_TYPE_POTION);
	PyModule_AddIntConstant(poModule, "MASK_ITEM_TYPE_FISHING_PICK", CItemData::MASK_ITEM_TYPE_FISHING_PICK);
	PyModule_AddIntConstant(poModule, "MASK_ITEM_TYPE_DRAGON_STONE", CItemData::MASK_ITEM_TYPE_DRAGON_STONE);
	PyModule_AddIntConstant(poModule, "MASK_ITEM_TYPE_COSTUMES", CItemData::MASK_ITEM_TYPE_COSTUMES);
	PyModule_AddIntConstant(poModule, "MASK_ITEM_TYPE_SKILL", CItemData::MASK_ITEM_TYPE_SKILL);
	PyModule_AddIntConstant(poModule, "MASK_ITEM_TYPE_UNIQUE", CItemData::MASK_ITEM_TYPE_UNIQUE);
	PyModule_AddIntConstant(poModule, "MASK_ITEM_TYPE_ETC", CItemData::MASK_ITEM_TYPE_ETC);
	PyModule_AddIntConstant(poModule, "MASK_ITEM_TYPE_MAX", CItemData::MASK_ITEM_TYPE_MAX);

	PyModule_AddIntConstant(poModule, "MASK_ITEM_SUBTYPE_MOUNT_PET_MOUNT", CItemData::MASK_ITEM_SUBTYPE_MOUNT_PET_MOUNT);
	PyModule_AddIntConstant(poModule, "MASK_ITEM_SUBTYPE_MOUNT_PET_CHARGED_PET", CItemData::MASK_ITEM_SUBTYPE_MOUNT_PET_CHARGED_PET);
	PyModule_AddIntConstant(poModule, "MASK_ITEM_SUBTYPE_MOUNT_PET_FREE_PET", CItemData::MASK_ITEM_SUBTYPE_MOUNT_PET_FREE_PET);
	PyModule_AddIntConstant(poModule, "MASK_ITEM_SUBTYPE_MOUNT_PET_EGG", CItemData::MASK_ITEM_SUBTYPE_MOUNT_PET_EGG);

	PyModule_AddIntConstant(poModule, "MASK_ITEM_SUBTYPE_WEAPON_WEAPON_SWORD", CItemData::MASK_ITEM_SUBTYPE_WEAPON_WEAPON_SWORD);
	PyModule_AddIntConstant(poModule, "MASK_ITEM_SUBTYPE_WEAPON_WEAPON_DAGGER", CItemData::MASK_ITEM_SUBTYPE_WEAPON_WEAPON_DAGGER);
	PyModule_AddIntConstant(poModule, "MASK_ITEM_SUBTYPE_WEAPON_WEAPON_BOW", CItemData::MASK_ITEM_SUBTYPE_WEAPON_WEAPON_BOW);
	PyModule_AddIntConstant(poModule, "MASK_ITEM_SUBTYPE_WEAPON_WEAPON_TWO_HANDED", CItemData::MASK_ITEM_SUBTYPE_WEAPON_WEAPON_TWO_HANDED);
	PyModule_AddIntConstant(poModule, "MASK_ITEM_SUBTYPE_WEAPON_WEAPON_BELL", CItemData::MASK_ITEM_SUBTYPE_WEAPON_WEAPON_BELL);
	PyModule_AddIntConstant(poModule, "MASK_ITEM_SUBTYPE_WEAPON_WEAPON_CLAW", CItemData::MASK_ITEM_SUBTYPE_WEAPON_WEAPON_CLAW);
	PyModule_AddIntConstant(poModule, "MASK_ITEM_SUBTYPE_WEAPON_WEAPON_FAN", CItemData::MASK_ITEM_SUBTYPE_WEAPON_WEAPON_FAN);
	PyModule_AddIntConstant(poModule, "MASK_ITEM_SUBTYPE_WEAPON_WEAPON_MOUNT_SPEAR", CItemData::MASK_ITEM_SUBTYPE_WEAPON_WEAPON_MOUNT_SPEAR);
	PyModule_AddIntConstant(poModule, "MASK_ITEM_SUBTYPE_WEAPON_WEAPON_ARROW", CItemData::MASK_ITEM_SUBTYPE_WEAPON_WEAPON_ARROW);
	PyModule_AddIntConstant(poModule, "MASK_ITEM_SUBTYPE_WEAPON_WEAPON_QUIVER", CItemData::MASK_ITEM_SUBTYPE_WEAPON_WEAPON_QUIVER);

	PyModule_AddIntConstant(poModule, "MASK_ITEM_SUBTYPE_ARMOR_ARMOR_BODY", CItemData::MASK_ITEM_SUBTYPE_ARMOR_ARMOR_BODY);
	PyModule_AddIntConstant(poModule, "MASK_ITEM_SUBTYPE_ARMOR_ARMOR_HEAD", CItemData::MASK_ITEM_SUBTYPE_ARMOR_ARMOR_HEAD);
	PyModule_AddIntConstant(poModule, "MASK_ITEM_SUBTYPE_ARMOR_ARMOR_SHIELD", CItemData::MASK_ITEM_SUBTYPE_ARMOR_ARMOR_SHIELD);

	PyModule_AddIntConstant(poModule, "MASK_ITEM_SUBTYPE_JEWELRY_ARMOR_WRIST", CItemData::MASK_ITEM_SUBTYPE_JEWELRY_ARMOR_WRIST);
	PyModule_AddIntConstant(poModule, "MASK_ITEM_SUBTYPE_JEWELRY_ARMOR_FOOTS", CItemData::MASK_ITEM_SUBTYPE_JEWELRY_ARMOR_FOOTS);
	PyModule_AddIntConstant(poModule, "MASK_ITEM_SUBTYPE_JEWELRY_ARMOR_NECK", CItemData::MASK_ITEM_SUBTYPE_JEWELRY_ARMOR_NECK);
	PyModule_AddIntConstant(poModule, "MASK_ITEM_SUBTYPE_JEWELRY_ARMOR_EAR", CItemData::MASK_ITEM_SUBTYPE_JEWELRY_ARMOR_EAR);
	PyModule_AddIntConstant(poModule, "MASK_ITEM_SUBTYPE_JEWELRY_ITEM_BELT", CItemData::MASK_ITEM_SUBTYPE_JEWELRY_ITEM_BELT);
	PyModule_AddIntConstant(poModule, "MASK_ITEM_SUBTYPE_JEWELRY_ITEM_ELEMENT", CItemData::MASK_ITEM_SUBTYPE_JEWELRY_ITEM_ELEMENT);

	PyModule_AddIntConstant(poModule, "MASK_ITEM_SUBTYPE_TUNING_RESOURCE", CItemData::MASK_ITEM_SUBTYPE_TUNING_RESOURCE);
	PyModule_AddIntConstant(poModule, "MASK_ITEM_SUBTYPE_TUNING_STONE", CItemData::MASK_ITEM_SUBTYPE_TUNING_STONE);
	PyModule_AddIntConstant(poModule, "MASK_ITEM_SUBTYPE_TUNING_ETC", CItemData::MASK_ITEM_SUBTYPE_TUNING_ETC);

	PyModule_AddIntConstant(poModule, "MASK_ITEM_SUBTYPE_POTION_ABILITY", CItemData::MASK_ITEM_SUBTYPE_POTION_ABILITY);
	PyModule_AddIntConstant(poModule, "MASK_ITEM_SUBTYPE_POTION_HAIRDYE", CItemData::MASK_ITEM_SUBTYPE_POTION_HAIRDYE);
	PyModule_AddIntConstant(poModule, "MASK_ITEM_SUBTYPE_POTION_ETC", CItemData::MASK_ITEM_SUBTYPE_POTION_ETC);

	PyModule_AddIntConstant(poModule, "MASK_ITEM_SUBTYPE_FISHING_PICK_FISHING_POLE", CItemData::MASK_ITEM_SUBTYPE_FISHING_PICK_FISHING_POLE);
	PyModule_AddIntConstant(poModule, "MASK_ITEM_SUBTYPE_FISHING_PICK_EQUIPMENT_PICK", CItemData::MASK_ITEM_SUBTYPE_FISHING_PICK_EQUIPMENT_PICK);
	PyModule_AddIntConstant(poModule, "MASK_ITEM_SUBTYPE_FISHING_PICK_FOOD", CItemData::MASK_ITEM_SUBTYPE_FISHING_PICK_FOOD);
	PyModule_AddIntConstant(poModule, "MASK_ITEM_SUBTYPE_FISHING_PICK_STONE", CItemData::MASK_ITEM_SUBTYPE_FISHING_PICK_STONE);
	PyModule_AddIntConstant(poModule, "MASK_ITEM_SUBTYPE_FISHING_PICK_ETC", CItemData::MASK_ITEM_SUBTYPE_FISHING_PICK_ETC);

	PyModule_AddIntConstant(poModule, "MASK_ITEM_SUBTYPE_DRAGON_STONE_DRAGON_DIAMOND", CItemData::MASK_ITEM_SUBTYPE_DRAGON_STONE_DRAGON_DIAMOND);
	PyModule_AddIntConstant(poModule, "MASK_ITEM_SUBTYPE_DRAGON_STONE_DRAGON_RUBY", CItemData::MASK_ITEM_SUBTYPE_DRAGON_STONE_DRAGON_RUBY);
	PyModule_AddIntConstant(poModule, "MASK_ITEM_SUBTYPE_DRAGON_STONE_DRAGON_JADE", CItemData::MASK_ITEM_SUBTYPE_DRAGON_STONE_DRAGON_JADE);
	PyModule_AddIntConstant(poModule, "MASK_ITEM_SUBTYPE_DRAGON_STONE_DRAGON_SAPPHIRE", CItemData::MASK_ITEM_SUBTYPE_DRAGON_STONE_DRAGON_SAPPHIRE);
	PyModule_AddIntConstant(poModule, "MASK_ITEM_SUBTYPE_DRAGON_STONE_DRAGON_GARNET", CItemData::MASK_ITEM_SUBTYPE_DRAGON_STONE_DRAGON_GARNET);
	PyModule_AddIntConstant(poModule, "MASK_ITEM_SUBTYPE_DRAGON_STONE_DRAGON_ONYX", CItemData::MASK_ITEM_SUBTYPE_DRAGON_STONE_DRAGON_ONYX);
	PyModule_AddIntConstant(poModule, "MASK_ITEM_SUBTYPE_DRAGON_STONE_ETC", CItemData::MASK_ITEM_SUBTYPE_DRAGON_STONE_ETC);

	PyModule_AddIntConstant(poModule, "MASK_ITEM_SUBTYPE_COSTUMES_COSTUME_WEAPON", CItemData::MASK_ITEM_SUBTYPE_COSTUMES_COSTUME_WEAPON);
	PyModule_AddIntConstant(poModule, "MASK_ITEM_SUBTYPE_COSTUMES_COSTUME_BODY", CItemData::MASK_ITEM_SUBTYPE_COSTUMES_COSTUME_BODY);
	PyModule_AddIntConstant(poModule, "MASK_ITEM_SUBTYPE_COSTUMES_COSTUME_HAIR", CItemData::MASK_ITEM_SUBTYPE_COSTUMES_COSTUME_HAIR);
	PyModule_AddIntConstant(poModule, "MASK_ITEM_SUBTYPE_COSTUMES_SASH", CItemData::MASK_ITEM_SUBTYPE_COSTUMES_SASH);
	PyModule_AddIntConstant(poModule, "MASK_ITEM_SUBTYPE_COSTUMES_ETC", CItemData::MASK_ITEM_SUBTYPE_COSTUMES_ETC);

	PyModule_AddIntConstant(poModule, "MASK_ITEM_SUBTYPE_SKILL_PAHAE", CItemData::MASK_ITEM_SUBTYPE_SKILL_PAHAE);
	PyModule_AddIntConstant(poModule, "MASK_ITEM_SUBTYPE_SKILL_SKILL_BOOK", CItemData::MASK_ITEM_SUBTYPE_SKILL_SKILL_BOOK);
	PyModule_AddIntConstant(poModule, "MASK_ITEM_SUBTYPE_SKILL_BOOK_OF_OBLIVION", CItemData::MASK_ITEM_SUBTYPE_SKILL_BOOK_OF_OBLIVION);
	PyModule_AddIntConstant(poModule, "MASK_ITEM_SUBTYPE_SKILL_ETC", CItemData::MASK_ITEM_SUBTYPE_SKILL_ETC);

	PyModule_AddIntConstant(poModule, "MASK_ITEM_SUBTYPE_UNIQUE_ABILITY", CItemData::MASK_ITEM_SUBTYPE_UNIQUE_ABILITY);
	PyModule_AddIntConstant(poModule, "MASK_ITEM_SUBTYPE_UNIQUE_ETC", CItemData::MASK_ITEM_SUBTYPE_UNIQUE_ETC);

	PyModule_AddIntConstant(poModule, "MASK_ITEM_SUBTYPE_ETC_GIFTBOX", CItemData::MASK_ITEM_SUBTYPE_ETC_GIFTBOX);
	PyModule_AddIntConstant(poModule, "MASK_ITEM_SUBTYPE_ETC_MATRIMORY", CItemData::MASK_ITEM_SUBTYPE_ETC_MATRIMORY);
	PyModule_AddIntConstant(poModule, "MASK_ITEM_SUBTYPE_ETC_EVENT", CItemData::MASK_ITEM_SUBTYPE_ETC_EVENT);
	PyModule_AddIntConstant(poModule, "MASK_ITEM_SUBTYPE_ETC_SEAL", CItemData::MASK_ITEM_SUBTYPE_ETC_SEAL);
	PyModule_AddIntConstant(poModule, "MASK_ITEM_SUBTYPE_ETC_PARTI", CItemData::MASK_ITEM_SUBTYPE_ETC_PARTI);
	PyModule_AddIntConstant(poModule, "MASK_ITEM_SUBTYPE_ETC_POLYMORPH", CItemData::MASK_ITEM_SUBTYPE_ETC_POLYMORPH);
	PyModule_AddIntConstant(poModule, "MASK_ITEM_SUBTYPE_ETC_RECIPE", CItemData::MASK_ITEM_SUBTYPE_ETC_RECIPE);
	PyModule_AddIntConstant(poModule, "MASK_ITEM_SUBTYPE_ETC_ETC", CItemData::MASK_ITEM_SUBTYPE_ETC_ETC);

	PyModule_AddIntConstant(poModule, "APPLY_ATTBONUS_WIND", CItemData::APPLY_ATTBONUS_WIND);
	PyModule_AddIntConstant(poModule, "APPLY_ATTBONUS_EARTH", CItemData::APPLY_ATTBONUS_EARTH);
	PyModule_AddIntConstant(poModule, "APPLY_ATTBONUS_DARK", CItemData::APPLY_ATTBONUS_DARK);
	PyModule_AddIntConstant(poModule, "APPLY_ANTI_SWORD", CItemData::APPLY_ANTI_SWORD);
	PyModule_AddIntConstant(poModule, "APPLY_ANTI_TWOHAND", CItemData::APPLY_ANTI_TWOHAND);
	PyModule_AddIntConstant(poModule, "APPLY_ANTI_DAGGER", CItemData::APPLY_ANTI_DAGGER);
	PyModule_AddIntConstant(poModule, "APPLY_ANTI_BELL", CItemData::APPLY_ANTI_BELL);
	PyModule_AddIntConstant(poModule, "APPLY_ANTI_FAN", CItemData::APPLY_ANTI_FAN);
	PyModule_AddIntConstant(poModule, "APPLY_ANTI_BOW", CItemData::APPLY_ANTI_BOW);
	PyModule_AddIntConstant(poModule, "APPLY_ANTI_HUMAN", CItemData::APPLY_ANTI_HUMAN);
	PyModule_AddIntConstant(poModule, "APPLY_ATT_MONSTER_NEW", CItemData::APPLY_ATT_MONSTER_NEW);
	PyModule_AddIntConstant(poModule, "APPLY_ATT_BOSS", CItemData::APPLY_ATT_BOSS);
	PyModule_AddIntConstant(poModule, "APPLY_ATTBONUS_ELEMENT", CItemData::APPLY_ATTBONUS_ELEMENT);
#endif	
#ifdef ENABLE_NEW_PET_SYSTEM
	PyModule_AddIntConstant (poModule, "PET_HATCHING_MONEY", PET_HATCHING_MONEY);
	PyModule_AddIntConstant (poModule, "PET_NAME_MIN_SIZE", PET_NAME_MIN_SIZE);
	PyModule_AddIntConstant (poModule, "PET_NAME_MAX_SIZE", PET_NAME_MAX_SIZE);
#endif
#endif
}

import uiScriptLocale
import item
ROOT = "d:/ymir work/ui/game/"
EQUIPMENT_START_INDEX = 180

window = {
	"name" : "InventoryWindow",

	## 600 - (width + ���������� ���� ���� 24 px)
	"x" : SCREEN_WIDTH - 176,
	"y" : SCREEN_HEIGHT - 37 - 565,

	"style" : ("movable", "float",),

	"width" : 176,
	"height" : 540,

	"children" :
	(
		## Inventory, Equipment Slots
		{
			"name" : "board",
			"type" : "board",
			"style" : ("attach",),

			"x" : 0,
			"y" : 0,

			"width" : 176,
			"height" : 540,

			"children" :
			(
			
				{
						"name" : "SeparateBaseImage",
						"type" : "image",
						"style" : ("attach",),

						"x" : 8,
						"y" : 7,

						"image" : "d:/ymir work/ui/pattern/titlebar_inv_refresh_baseframe.tga",

						"children" :
						(
							## Separate Button (38x24)
							{
								"name" : "SeparateButton",
								"type" : "button",

								"x" : 0,
								"y" : 0,

								"tooltip_text" : "Envanteri Duzenle",

								"default_image" : "d:/ymir work/ui/pattern/titlebar_inv_refresh_baseframe.tga",
								"over_image" : "d:/ymir work/ui/pattern/titlebar_inv_refresh_baseframe_over.tga",
								"down_image" : "d:/ymir work/ui/pattern/titlebar_inv_refresh_baseframe_down.tga",
								"disable_image" : "d:/ymir work/ui/pattern/titlebar_inv_refresh_baseframe.tga",
							},
						),
					},
				## Title
				{
					"name" : "TitleBar",
					"type" : "titlebar",
					"style" : ("attach",),

					"x" : 8+35,
					"y" : 7,

					"width" : 161-35,
					"color" : "yellow",

					"children" :
					(
						{ "name":"TitleName", "type":"text", "x":77, "y":3, "text":uiScriptLocale.INVENTORY_TITLE, "text_horizontal_align":"center" },
					),
				},

				## Equipment Slot
				{
					"name" : "Equipment_Base",
					"type" : "expanded_image",

					"x" : 10,
					"y" : 33,

					"image" : "d:/ymir work/ui/new_bonus_inventory.tga",
					#"image" : "d:/ymir work/ui/equipment_bg_without_ring.tga",
					#"image" : "d:/ymir work/ui/equipment_bg_with_ring.tga",

					"children" :
					(

						{
							"name" : "EquipmentSlot",
							"type" : "slot",

							"x" : 3,
							"y" : 3,

							"width" : 150,
							"height" : 182,

							"slot" : (
										{"index":EQUIPMENT_START_INDEX+0, "x":39, "y":37, "width":32, "height":64},
										{"index":EQUIPMENT_START_INDEX+1, "x":39, "y":2, "width":32, "height":32},
										{"index":EQUIPMENT_START_INDEX+2, "x":39, "y":145, "width":32, "height":32},
										{"index":EQUIPMENT_START_INDEX+3, "x":75, "y":67, "width":32, "height":32},
										{"index":EQUIPMENT_START_INDEX+4, "x":3, "y":3, "width":32, "height":96},
										{"index":EQUIPMENT_START_INDEX+5, "x":114, "y":67, "width":32, "height":32},
										{"index":EQUIPMENT_START_INDEX+6, "x":114, "y":35, "width":32, "height":32},
										{"index":EQUIPMENT_START_INDEX+7, "x":2, "y":145, "width":32, "height":32},
										{"index":EQUIPMENT_START_INDEX+8, "x":75, "y":145, "width":32, "height":32},
										{"index":EQUIPMENT_START_INDEX+9, "x":114, "y":2, "width":32, "height":32},
										{"index":EQUIPMENT_START_INDEX+10, "x":75, "y":35, "width":32, "height":32},
										{"index":EQUIPMENT_START_INDEX+11, "x":3, "y":106, "width":32, "height":32},
										## �� ����1
										##{"index":item.EQUIPMENT_RING1, "x":2, "y":106, "width":32, "height":32},
										## �� ����2
										##{"index":item.EQUIPMENT_RING2, "x":75, "y":106, "width":32, "height":32},
										## �� ��Ʈ
										{"index":item.EQUIPMENT_BELT, "x":39, "y":106, "width":32, "height":32},
										{"index":item.EQUIPMENT_PET, "x":115, "y":106, "width":32, "height":32},
										{"index":item.EQUIPMENT_GLOVE, "x":75, "y":106, "width":32, "height":32},#guc eldiveni
									),
						},
						## Dragon Soul Button
						{
							"name" : "DSSButton",
							"type" : "button",

							"x" : 118,
							"y" : 148,

							"tooltip_text" : uiScriptLocale.TASKBAR_DRAGON_SOUL,

							"default_image" : uiScriptLocale.LOCALE_UISCRIPT_PATH + "inventar/alchimie_01.jpg",
							"over_image" : uiScriptLocale.LOCALE_UISCRIPT_PATH + "inventar/alchimie_02.jpg",
							"down_image" : uiScriptLocale.LOCALE_UISCRIPT_PATH + "inventar/alchimie_03.jpg",
						},
						## MallButton
						{
							"name" : "MallButton",
							"type" : "button",

							"x" : 132,
							"y" : 162,

							"tooltip_text" : uiScriptLocale.MALL_TITLE,

							"default_image" : uiScriptLocale.LOCALE_UISCRIPT_PATH + "inventar/item_shop_01.jpg",
							"over_image" : uiScriptLocale.LOCALE_UISCRIPT_PATH + "inventar/item_shop_02.jpg",
							"down_image" : uiScriptLocale.LOCALE_UISCRIPT_PATH + "inventar/item_shop_03.jpg",
						},
						##DEPOZIT Cryptex
						{
							"name" : "SpecialStorageButton",
							"type" : "button",

							"x" : 118,
							"y" : 162,

							"tooltip_text" : uiScriptLocale.DEP_TITLE,

							"default_image" : uiScriptLocale.LOCALE_UISCRIPT_PATH + "inventar/depozit_01.jpg",
							"over_image" : uiScriptLocale.LOCALE_UISCRIPT_PATH + "inventar/depozit_02.jpg",
							"down_image" : uiScriptLocale.LOCALE_UISCRIPT_PATH + "inventar/depozit_03.jpg",
						},
						##NPC BUTTON
						{
							"name" : "npcacamk",
							"type" : "button",

							"x" : 132,
							"y" : 148,

							"tooltip_text" : uiScriptLocale.NPC_TITLE,

							"default_image" : uiScriptLocale.LOCALE_UISCRIPT_PATH + "inventar/shop_01.jpg",
							"over_image" : uiScriptLocale.LOCALE_UISCRIPT_PATH + "inventar/shop_02.jpg",
							"down_image" : uiScriptLocale.LOCALE_UISCRIPT_PATH + "inventar/shop_03.jpg",
						},
						## CostumeButton
						{
							"name" : "CostumeButton",
							"type" : "button",

							"x" : 78,
							"y" : 5,

							"tooltip_text" : uiScriptLocale.COSTUME_TITLE,

							"default_image" : "d:/ymir work/ui/game/taskbar/costume_Button_01.tga",
							"over_image" : "d:/ymir work/ui/game/taskbar/costume_Button_02.tga",
							"down_image" : "d:/ymir work/ui/game/taskbar/costume_Button_03.tga",
						},						
						{
							"name" : "Equipment_Tab_01",
							"type" : "radio_button",

							"x" : 86,
							"y" : 161,

							"default_image" : "d:/ymir work/ui/game/windows/tab_button_small_01.sub",
							"over_image" : "d:/ymir work/ui/game/windows/tab_button_small_02.sub",
							"down_image" : "d:/ymir work/ui/game/windows/tab_button_small_03.sub",

							"children" :
							(
								{
									"name" : "Equipment_Tab_01_Print",
									"type" : "text",

									"x" : 0,
									"y" : 0,

									"all_align" : "center",

									"text" : "I",
								},
							),
						},
						{
							"name" : "Equipment_Tab_02",
							"type" : "radio_button",

							"x" : 86 + 32,
							"y" : 161,

							"default_image" : "d:/ymir work/ui/game/windows/tab_button_small_01.sub",
							"over_image" : "d:/ymir work/ui/game/windows/tab_button_small_02.sub",
							"down_image" : "d:/ymir work/ui/game/windows/tab_button_small_03.sub",

							"children" :
							(
								{
									"name" : "Equipment_Tab_02_Print",
									"type" : "text",

									"x" : 0,
									"y" : 0,

									"all_align" : "center",

									"text" : "II",
								},
							),
						},

					),
				},

				{
					"name" : "Inventory_Tab_01",
					"type" : "radio_button",

					"x" : 10,
					"y" : 33 + 191,

					"default_image" : "d:/ymir work/ui/game/windows/tab_button_large_half_01.sub",
					"over_image" : "d:/ymir work/ui/game/windows/tab_button_large_half_02.sub",
						"down_image" : "d:/ymir work/ui/game/windows/tab_button_large_half_03.sub",
					"tooltip_text" : uiScriptLocale.INVENTORY_PAGE_BUTTON_TOOLTIP_1,

					"children" :
					(
						{
							"name" : "Inventory_Tab_01_Print",
							"type" : "text",

							"x" : 0,
							"y" : 0,

							"all_align" : "center",

							"text" : "I",
						},
					),
				},
				{
					"name" : "Inventory_Tab_02",
					"type" : "radio_button",

					#"x" : 10 + 78,
					"x" : 10 + 39,
					"y" : 33 + 191,

					"default_image" : "d:/ymir work/ui/game/windows/tab_button_large_half_01.sub",
					"over_image" : "d:/ymir work/ui/game/windows/tab_button_large_half_02.sub",
					"down_image" : "d:/ymir work/ui/game/windows/tab_button_large_half_03.sub",
					"tooltip_text" : uiScriptLocale.INVENTORY_PAGE_BUTTON_TOOLTIP_2,

					"children" :
					(
						{
							"name" : "Inventory_Tab_02_Print",
							"type" : "text",

							"x" : 0,
							"y" : 0,

							"all_align" : "center",

							"text" : "II",
						},
					),
				},
					
				{
					"name" : "Inventory_Tab_03",
					"type" : "radio_button",

					"x" : 10 + 39 + 39,
					"y" : 33 + 191,

					"default_image" : "d:/ymir work/ui/game/windows/tab_button_large_half_01.sub",
					"over_image" : "d:/ymir work/ui/game/windows/tab_button_large_half_02.sub",
					"down_image" : "d:/ymir work/ui/game/windows/tab_button_large_half_03.sub",
					"tooltip_text" : "3.Envanter",

					"children" :
					(
						{
							"name" : "Inventory_Tab_03_Print",
							"type" : "text",

							"x" : 0,
							"y" : 0,

							"all_align" : "center",

							"text" : "III",
						},
					),
				},
					
				{
					"name" : "Inventory_Tab_04",
					"type" : "radio_button",

					"x" : 10 + 39 + 39 + 39,
					"y" : 33 + 191,

					"default_image" : "d:/ymir work/ui/game/windows/tab_button_large_half_01.sub",
					"over_image" : "d:/ymir work/ui/game/windows/tab_button_large_half_02.sub",
					"down_image" : "d:/ymir work/ui/game/windows/tab_button_large_half_03.sub",
					"tooltip_text" : "4.Envanter",

					"children" :
					(
						{
							"name" : "Inventory_Tab_04_Print",
							"type" : "text",

							"x" : 0,
							"y" : 0,

							"all_align" : "center",

							"text" : "IV",
						},
					),
				},

				## Item Slot
				{
					"name" : "ItemSlot",
					"type" : "grid_table",

					"x" : 8,
					"y" : 246,

					"start_index" : 0,
					"x_count" : 5,
					"y_count" : 9,
					"x_step" : 32,
					"y_step" : 32,

					"image" : "d:/ymir work/ui/public/Slot_Base.sub"
				},

			),
		},
	),
}

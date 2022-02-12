import uiScriptLocale
BUTTON_ROOT = "d:/ymir work/ui/public/"
window = {
	"name" : "SpecialStorageWindow",
	"x" : 200,
	"y" : 20,
	"style" : ("movable", "float",),
	"width" : 239,
	"height" : 395+10+20-17 + 23,
	"children" :
	(
		{
			"name" : "board",
			"type" : "board",
			"x" : 0,
			"y" : 0,
			"width" : 239,
			"height" : 395+10+20-17 + 23,
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

								"tooltip_text" : "Envanteri Düzenle",

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

					"width" : 175 + 80-45 - 10 - 10,
					"color" : "yellow",
					"children" :
					(
						{ "name":"TitleName", "type":"text", "x":0, "y":-1, "text":uiScriptLocale.UPGRADE_STORAGE_TITLE, "all_align":"center" },
					),
				},
				
				{
					"name" : "test",
					"type" : "thinboard_circle",
					"x" : 175, "y" : 30,
					"width" :53,
					"height" : 395+10+20-17-68 - 45,
				},
				
				## Button
				{
					"name" : "Category_Tab_01",
					"type" : "radio_button",
					"x" : 185,
					"y" : 37 + (38*2) - 5,
					"tooltip_text" : "Yükseltme",
					"default_image" : "arti1.png",
					"over_image" : "arti2.png",
					"down_image" : "arti2.png",
				},
				{
					"name" : "Category_Tab_02",
					"type" : "radio_button",
					"x" : 185,
					"y" : 37 + (38*3) - 5,
					"tooltip_text" : "Gaya",
					"default_image" : "gaya.png",
					"over_image" : "gaya2.png",
					"down_image" : "gaya2.png",
				},
				{
					"name" : "Category_Tab_03",
					"type" : "radio_button",
					"x" : 185,
					"y" : 37 + (38*4) - 5,
					"tooltip_text" : "Sandýk",

					"default_image" : "chest1.png",
					"over_image" : "chest2.png",
					"down_image" : "chest2.png",
				},
				{
					"name" : "Category_Tab_04",
					"type" : "radio_button",
					"x" : 185,
					"y" : 37 + (38*5) - 5,
					"tooltip_text" : "Cevher",

					"default_image" : "cevher1.png",
					"over_image" : "cevher2.png",
					"down_image" : "cevher2.png",
				},
				
				## Item Slot
				{
					"name" : "ItemSlot",
					"type" : "grid_table",

					"x" : 8,
					"y" : 30,

					"start_index" : 0,
					"x_count" : 5,
					"y_count" : 9,
					"x_step" : 32,
					"y_step" : 32,

					"image" : "d:/ymir work/ui/public/Slot_Base.sub",
				},

				{
					"name":"horizontalbar1", "type":"horizontalbar", "x":15, "y":380, "width":215,
					"children":
					(
						{ "name":"question", "type":"text", "text" : "Envanter ile acilsin mi ?", "x":0, "y":0, "r":1.0, "g":1.0, "b":1.0, "a":1.0, "all_align":"center" },
					),
				},

				{
					"name" : "acceptbutton",
					"type" : "radio_button",
					"x" : 22 + 23,
					"y" : 400,
					"tooltip_text" : "Envanterle açýlsýn",
					# "vertical_align" : "bottom",
					"default_image" : "d:/ymir work/ui/public/acceptbutton00.sub",
					"over_image" : "d:/ymir work/ui/public/acceptbutton01.sub",
					"down_image" : "d:/ymir work/ui/public/acceptbutton02.sub",
				},
				{
					"name" : "cancelbutton",
					"type" : "radio_button",
					"x" : 22+26+52-5 + 23,
					"y" : 400,
					"tooltip_text" : "Envanterle açýlmasýn",
					# "vertical_align" : "bottom",
					"default_image" : "d:/ymir work/ui/public/canclebutton00.sub",
					"over_image" : "d:/ymir work/ui/public/canclebutton01.sub",
					"down_image" : "d:/ymir work/ui/public/canclebutton02.sub",
				},
				{
					"name" : "test2",
					"type" : "thinboard_circle",
					"x" : 10, "y" : 335,
					"width" :175 + 39,
					"height" : 33,
				},
				{
					"name" : "Inventory_Tab_01",
					"type" : "radio_button",

					"x" : 34,
					"y" : 341,

					"default_image" : "d:/ymir work/ui/game/windows/tab_button_large_half_01.sub",
					"over_image" : "d:/ymir work/ui/game/windows/tab_button_large_half_02.sub",
					"down_image" : "d:/ymir work/ui/game/windows/tab_button_large_half_03.sub",
					"text" : "I",
					"tooltip_text" : uiScriptLocale.INVENTORY_PAGE_BUTTON_TOOLTIP_1,

					"children" :
					(
						{
							"name" : "Inventory_Tab_01_Print",
							"type" : "text",

							"x" : 0,
							"y" : 0,

							"all_align" : "center",


						},
					),
				},
				{
					"name" : "Inventory_Tab_02",
					"type" : "radio_button",

					"x" : 34+39,
					"y" : 341,

					"default_image" : "d:/ymir work/ui/game/windows/tab_button_large_half_01.sub",
					"over_image" : "d:/ymir work/ui/game/windows/tab_button_large_half_02.sub",
					"down_image" : "d:/ymir work/ui/game/windows/tab_button_large_half_03.sub",
					"text" : "II",

					"tooltip_text" : uiScriptLocale.INVENTORY_PAGE_BUTTON_TOOLTIP_2,

					"children" :
					(
						{
							"name" : "Inventory_Tab_02_Print",
							"type" : "text",

							"x" : 0,
							"y" : 0,

							"all_align" : "center",

						},
					),
				},
				{
					"name" : "Inventory_Tab_03",
					"type" : "radio_button",

					"x" : 34+39+39,
					"y" : 341,

					"default_image" : "d:/ymir work/ui/game/windows/tab_button_large_half_01.sub",
					"over_image" : "d:/ymir work/ui/game/windows/tab_button_large_half_02.sub",
					"down_image" : "d:/ymir work/ui/game/windows/tab_button_large_half_03.sub",
					"text" : "III",
					"tooltip_text" : uiScriptLocale.INVENTORY_PAGE_BUTTON_TOOLTIP_3,

					"children" :
					(
						{
							"name" : "Inventory_Tab_03_Print",
							"type" : "text",

							"x" : 0,
							"y" : 0,

							"all_align" : "center",

						},
					),
				},
				{
					"name" : "Inventory_Tab_04",
					"type" : "radio_button",

					"x" : 34+39+39+39,
					"y" : 341,

					"default_image" : "d:/ymir work/ui/game/windows/tab_button_large_half_01.sub",
					"over_image" : "d:/ymir work/ui/game/windows/tab_button_large_half_02.sub",
					"down_image" : "d:/ymir work/ui/game/windows/tab_button_large_half_03.sub",
					"text" : "IV",
					"tooltip_text" : uiScriptLocale.INVENTORY_PAGE_BUTTON_TOOLTIP_4,
					"children" :
					(
						{
							"name" : "Inventory_Tab_04_Print",
							"type" : "text",

							"x" : 0,
							"y" : 0,

							"all_align" : "center",
						},
					),
				},

				# EnvanterInfo
				# {
					# "name" : "MalzemeDeposuInfo",
					# "type" : "button",

					# "x" : 131,
					# "y" : 9,

					# "default_image" : "d:/ymir work/ui/pattern/q_mark_01.tga",
					# "over_image" : "d:/ymir work/ui/pattern/q_mark_02.tga",
					# "down_image" : "d:/ymir work/ui/pattern/q_mark_01.tga",
				# },
			),
		},
	),
}

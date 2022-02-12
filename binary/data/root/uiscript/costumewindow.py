import uiScriptLocale
import item
import app

COSTUME_START_INDEX = item.COSTUME_SLOT_START

window = {
	"name" : "CostumeWindow",

	"x" : SCREEN_WIDTH - 180 - 140, # "x" : SCREEN_WIDTH - 210 - 140, ENABLE_SIDEBAR_INVENTORY:
	"y" : SCREEN_HEIGHT - 40 - 565,

	"style" : ("movable", "float",),

	"width" : 140,
	"height" : (180 + 47 + 32), 

	"children" :
	(
		{
			"name" : "board",
			"type" : "board",
			"style" : ("attach",),

			"x" : 10,
			"y" : 2,

			"width" : 140,
			"height" : (180 + 47 + 32),

			"children" :
			(
				## Title
				{
					"name" : "TitleBar",
					"type" : "titlebar",
					"style" : ("attach",),

					"x" : 8,
					"y" : 8,

					"width" : 126,
					"color" : "yellow",

					"children" :
					(
						{ "name":"TitleName", "type":"text", "x":60, "y":3, "text":uiScriptLocale.COSTUME_WINDOW_TITLE, "text_horizontal_align":"center" },
					),
				},

				## Equipment Slot
				{
					"name" : "Costume_Base",
					"type" : "image",

					"x" : 13,
					"y" : 38,

					"image" : uiScriptLocale.LOCALE_UISCRIPT_PATH + "costume/new_costume_bg_with_ring.jpg",					

					"children" :
					(
						{
							"name" : "CostumeSlot",
							"type" : "slot",

							"x" : 3,
							"y" : 3,

							"width" : 127,
							"height" : 188,

							"slot" : (
										{"index":COSTUME_START_INDEX+0, "x":62, "y":45, "width":32, "height":64},#KOSTÜM
										{"index":COSTUME_START_INDEX+1, "x":62, "y": 9, "width":32, "height":32},#BAŞLIK
										{"index":COSTUME_START_INDEX+2, "x":70, "y":127, "width":32, "height":32},#KUŞAK
										{"index":COSTUME_START_INDEX+3, "x":13, "y":13, "width":32, "height":96}, #SİLAH KOSTÜMÜ
										{"index":COSTUME_START_INDEX+4, "x":5, "y":127, "width":32, "height":32},#BİNEK
										{"index":COSTUME_START_INDEX+5, "x":38, "y":127, "width":32, "height":32},#AURA
										{"index":item.EQUIPMENT_RING1, 	"x":12,	"y":167, "width":32, "height":32},#YÜZÜK1
										{"index":item.EQUIPMENT_RING2, 	"x":63,	"y":167, "width":32, "height":32},#YÜZÜK2
									),
						},
							#ENABLE_COSTUME_HIDE_SYSTEM
							{
								"name" : "kostumgizle", "type" : "button",
								"x" : 65, "y" : 48,

								"tooltip_text" : "Goster/Gizle",

								"default_image" : "d:/ymir work/ui/game/ekstra/image/custume_window/new_costume_buttons.tga",
								"over_image" : "d:/ymir work/ui/game/ekstra/image/custume_window/new_costume_buttons2.tga",
								"down_image" : "d:/ymir work/ui/game/ekstra/image/custume_window/new_costume_buttons3.tga",
							},
							{
								"name" : "sacgizle", "type" : "button",
								"x" : 65, "y" : 9+3,

								"tooltip_text" : "Goster/Gizle",

								"default_image" : "d:/ymir work/ui/game/ekstra/image/custume_window/new_costume_buttons.tga",
								"over_image" : "d:/ymir work/ui/game/ekstra/image/custume_window/new_costume_buttons2.tga",
								"down_image" : "d:/ymir work/ui/game/ekstra/image/custume_window/new_costume_buttons3.tga",
							},
							{
								"name" : "silahgizle", "type" : "button",
								"x" : 16, "y" : 13+3,

								"tooltip_text" : "Goster/Gizle",

								"default_image" : "d:/ymir work/ui/game/ekstra/image/custume_window/new_costume_buttons.tga",
								"over_image" : "d:/ymir work/ui/game/ekstra/image/custume_window/new_costume_buttons2.tga",
								"down_image" : "d:/ymir work/ui/game/ekstra/image/custume_window/new_costume_buttons3.tga",
							},
							#ENABLE_COSTUME_HIDE_SYSTEM
					),
				},
			),
		},
	),
}

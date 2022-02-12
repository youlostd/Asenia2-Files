import uiScriptLocale
import item
import app

window = {
	"name" : "KygnAura",
	"style" : ("movable", "float",),

	"x" : SCREEN_WIDTH / 2 - 124,
	"y" : SCREEN_HEIGHT / 2 - 147,


	"width" : 248,
	"height" : 294,

	"children" :
	(
		{
			"name" : "board",
			"type" : "board",
			"style" : ("attach",),

			"x" : 0,
			"y" : 0,

			"width" : 248,
			"height" : 294,
		
			"children" :
			(
				## Title
				{
					"name" : "TitleBar",
					"type" : "titlebar",
					"style" : ("attach",),

					"x" : 6,
					"y" : 6,

					"width" : 235,
					"color" : "yellow",

					"children" :
					(
						{ "name":"TitleName", "type":"text", "x":0, "y":3, "horizontal_align":"center", "text_horizontal_align":"center", "text": uiScriptLocale.AURA_YUKSELTPANEL, },
					),
				},

				## Slot
				{
					"name" : "Aura_Levelup",
					"type" : "image",
					
					"x" : 13,
					"y" : 35,
					
					"image" : "d:/ymir work/ui/aura/aura_levelup_bg.sub",
					
					"children" :
					(
						## Main
						{"name" : "Main", "type" : "text", "x" : 66, "y" : 10, "text":uiScriptLocale.AURA_MAIN, "text_horizontal_align" : "center"},
						## Sub
						{"name" : "Main", "type" : "text", "x" : 158, "y" : 10, "text":uiScriptLocale.AURA_SUB, "text_horizontal_align" : "center"},
						## Slot
						{
							"name" : "AuraSlot",
							"type" : "slot",
					
							"x" : 3,
							"y" : 3,
					
							"width" : 190,
							"height" : 200,
					
							"slot" : (
								{"index":0, "x":46, "y":28, "width":31, "height":31},  # ¸ŞÀÎ
								{"index":1, "x":138, "y":28, "width":31, "height":31},  # ¼­ºê
							),
						},
					),
				},


				## Line
				{
					"name" : "Aura_ToopTip_Line",
					"type" : "image",
					
					"x" : 27,
					"y" : 176,
					
					"image" : "d:/ymir work/ui/aura/aura_levelup_line.sub",
				},

				{"name" : "AuraText1", "type" : "text", "x" : 34, "y" : 122, "text_horizontal_align" : "left", "text" : " "},
				{"name" : "AuraText2", "type" : "text", "x" : 34, "y" : 140, "text_horizontal_align" : "left", "text" : " "},
				{"name" : "AuraText3", "type" : "text", "x" : 34, "y" : 158, "text_horizontal_align" : "left", "text" : " "},
				{"name" : "AuraText4", "type" : "text", "x" : 34, "y" : 190, "text_horizontal_align" : "left", "text" : " "},
				{"name" : "AuraText5", "type" : "text", "x" : 34, "y" : 208, "text_horizontal_align" : "left", "text" : " "},
				{"name" : "AuraText6", "type" : "text", "x" : 34, "y" : 226, "text_horizontal_align" : "left", "text" : " "},


				## Button
				# {
					# "name" : "AuraSlotButon",
					# "type" : "button",

					# "x" : 13 + 49,
					# "y" : 35 + 31,

					# "default_image" : "d:/ymir work/ui/aura/bos.tga",
					# "over_image" : "d:/ymir work/ui/aura/bos.tga",
					# "down_image" : "d:/ymir work/ui/aura/bos.tga",
				# },
				{
					"name" : "YukseltButon",
					"type" : "button",

					"x" : 36,
					"y" : 260,

					"text" : uiScriptLocale.OK,

					"default_image" : "d:/ymir work/ui/public/large_button_01.sub",
					"over_image" : "d:/ymir work/ui/public/large_button_02.sub",
					"down_image" : "d:/ymir work/ui/public/large_button_03.sub",
				},
				{
					"name" : "KapatButon",
					"type" : "button",

					"x" : 124,
					"y" : 260,

					"text" : uiScriptLocale.CANCEL,

					"default_image" : "d:/ymir work/ui/public/large_button_01.sub",
					"over_image" : "d:/ymir work/ui/public/large_button_02.sub",
					"down_image" : "d:/ymir work/ui/public/large_button_03.sub",
				},				
			),
		},
	),
}


import uiScriptLocale

window = {
	"name" : "InputDialog",

	"x" : 0,
	"y" : 0,

	"style" : ("movable", "float",),

	"width" : 230,
	"height" : 245,

	"children" :
	(
		{
			"name" : "Board",
			"type" : "board_with_titlebar",

			"x" : 0,
			"y" : 0,

			"width" : 230,
			"height" : 245,

			"title" : uiScriptLocale.GUILD_WAR_DECLARE,

			"children" :
			(
				## Input Slot
                {
					"name" : "InputName",
					"type" : "text",

					"x" : 15,
					"y" : 40,

					"text" : uiScriptLocale.GUILD_WAR_ENEMY,
				},
				{
					"name" : "InputSlot",
					"type" : "slotbar",

					"x" : 80,
					"y" : 37,
					"width" : 130,
					"height" : 18,

					"children" :
					(
						{
							"name" : "InputValue",
							"type" : "editline",

							"x" : 3,
							"y" : 3,

							"width" : 90,
							"height" : 18,

							"input_limit" : 12,
						},
					),
				},
				## Input Slot
				{
					"name" : "GameType", "x" : 15, "y" : 65, "width" : 65+45*4, "height" : 20,
                   
					"children" :
					(
						{"name" : "GameTypeLabel", "type" : "text", "x" : 0, "y" : 3, "text" : uiScriptLocale.GUILD_WAR_BATTLE_TYPE,},
						{
							"name" : "NormalButton",
							"type" : "radio_button",

							"x" : 65,
							"y" : 0,

							"text" : uiScriptLocale.GUILD_WAR_NORMAL,

							"default_image" : "d:/ymir work/ui/public/small_button_01.sub",
							"over_image" : "d:/ymir work/ui/public/small_button_02.sub",
							"down_image" : "d:/ymir work/ui/public/small_button_03.sub",
						},
						{
							"name" : "WarpButton",
							"type" : "radio_button",

							"x" : 65+45*1,
							"y" : 0,

							"text" : uiScriptLocale.GUILD_WAR_WARP,
                           
							"default_image" : "d:/ymir work/ui/public/small_button_01.sub",
							"over_image" : "d:/ymir work/ui/public/small_button_02.sub",
							"down_image" : "d:/ymir work/ui/public/small_button_03.sub",
						},
						{
							"name" : "CTFButton",
							"type" : "radio_button",

							"x" : 65+45*2,
							"y" : 0,

							"text" : uiScriptLocale.GUILD_WAR_CTF,

							"default_image" : "d:/ymir work/ui/public/small_button_01.sub",
							"over_image" : "d:/ymir work/ui/public/small_button_02.sub",
							"down_image" : "d:/ymir work/ui/public/small_button_03.sub",
                        },
                        {"name" : "GameTypeLabel", "type" : "text", "x" : 0, "y" : 30, "text" : "Binek",},               
					),
				},
				## Input Slot
				{
					"name" : "InputNameScore",
					"type" : "text",

					"x" : 15,
					"y" : 125,

					"text" : "Maks. Skor",
				},
				{
					"name" : "InputSlotScore",
					"type" : "slotbar",

					"x" : 80,
					"y" : 122,
					"width" : 130,
					"height" : 18,

					"children" :
					(
						{
							"name" : "InputValueScore",
							"type" : "editline",

							"x" : 3,
							"y" : 3,

							"width" : 90,
							"height" : 18,

							"input_limit" : 12,
						},
					),
				},
				## Input Slot
				{
					"name" : "InputNameUser",
					"type" : "text",

					"x" : 15,
					"y" : 155,

					"text" : "Maks. Oyuncu",
				},
				{
					"name" : "InputSlotUser",
					"type" : "slotbar",

					"x" : 80,
					"y" : 152,
					"width" : 130,
					"height" : 18,

					"children" :
					(
						{
							"name" : "InputValueUser",
							"type" : "editline",

							"x" : 3,
							"y" : 3,

							"width" : 90,
							"height" : 18,

							"input_limit" : 12,
						},
					),
				},
				## Input Slot
				{
					"name" : "InputNameMinSeviye",
					"type" : "text",

					"x" : 15,
					"y" : 185,

					"text" : "Min. Seviye",
				},
				{
					"name" : "InputSlotMinSeviye",
					"type" : "slotbar",

					"x" : 80,
					"y" : 182,
					"width" : 130,
					"height" : 18,

					"children" :
					(
						{
							"name" : "InputValueMinSeviye",
							"type" : "editline",

							"x" : 3,
							"y" : 3,

							"width" : 90,
							"height" : 18,

							"input_limit" : 12,
						},
					),
				},               
				## Button
				{
					"name" : "AcceptButton",
					"type" : "button",

					"x" : - 61 - 5 + 30,
					"y" : 215,
					"horizontal_align" : "center",

					"text" : uiScriptLocale.OK,

					"default_image" : "d:/ymir work/ui/public/middle_button_01.sub",
					"over_image" : "d:/ymir work/ui/public/middle_button_02.sub",
					"down_image" : "d:/ymir work/ui/public/middle_button_03.sub",
				},
				{
					"name" : "CancelButton",
					"type" : "button",

					"x" : 5 + 30,
					"y" : 215,
					"horizontal_align" : "center",

					"text" : uiScriptLocale.CANCEL,

					"default_image" : "d:/ymir work/ui/public/middle_button_01.sub",
					"over_image" : "d:/ymir work/ui/public/middle_button_02.sub",
					"down_image" : "d:/ymir work/ui/public/middle_button_03.sub",
				},
				{
					"name" : "Acikbinek",
					"type" : "radio_button",

					"x" : -12,
					"y" : 92,
					"horizontal_align" : "center",

					"text" : "Acik",

					"default_image" : "d:/ymir work/ui/public/small_button_01.sub",
					"over_image" : "d:/ymir work/ui/public/small_button_02.sub",
					"down_image" : "d:/ymir work/ui/public/small_button_03.sub",
				},
				{
					"name" : "Kapalibinek",
					"type" : "radio_button",

					"x" : 32,
					"y" : 92,
					"horizontal_align" : "center",

					"text" : "Kapali",

					"default_image" : "d:/ymir work/ui/public/small_button_01.sub",
					"over_image" : "d:/ymir work/ui/public/small_button_02.sub",
					"down_image" : "d:/ymir work/ui/public/small_button_03.sub",
				},               
			),
		},
	),
}
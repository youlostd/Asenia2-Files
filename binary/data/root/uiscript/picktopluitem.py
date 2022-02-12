import uiScriptLocale, localeInfo

BOARD_WIDTH = 190
BOARD_HEIGHT = 112
window = {
	"name" : "PickMoneyDialog",

	"x" : 100,
	"y" : 100,

	"style" : ("movable", "float",),

	"width" : 170,
	"height" : 105,

	"children" :
	(
		{
			"name" : "board",
			"type" : "board_with_titlebar",

			"x" : 0,
			"y" : 0,

			"width" : 170,
			"height" : 105,
			"title" : uiScriptLocale.PICK_MONEY_TITLE,

			"children" :
			(

				## Money Slot
				{
					"name" : "money_slot",
					"type" : "image",

					"x" : 20,
					"y" : 34,

					"image" : "d:/ymir work/ui/public/Parameter_Slot_02.sub",

					"children" :
					(
						{
							"name" : "money_value","type" : "editline","x" : 3,"y" : 2,
							"width" : 60, "height" : 18, "input_limit" : 6, "only_number" : 1, "text" : "1",
						},
						{"name" : "max_value","type" : "text", "x" : 63, "y" : 3, "text" : "/ 999999",},
					),
				},
				
				{
					"name" : "cBoxText","type" : "window","style" : ("attach",),
					"x" : 25,"y" : 55,"width" : 29,"height" : 18,
					"children" :({
							"name" : "cBoxText","type" : "text","x" : 13,"y" : 0,"text_horizontal_align" : "left","text" : localeInfo.HEPSINI_AYIR,
						},
					),
				},
				{
					"name" : "cBoxToplu","type" : "toggle_button","x" : 20,"y" : 56,
					# "tooltip_text" : "Toplu ayir","tooltip_x" : 55,"tooltip_y" : -15,
					"default_image" : "d:/ymir work/new/checkBoxKapali.tga",
					"over_image" : "d:/ymir work/new/checkBoxUstunde.tga",
					"down_image" : "d:/ymir work/new/checkBoxAktif.tga",
				},

				## Button
				{
					"name" : "accept_button",
					"type" : "button",

					"x" : 170/2 - 61 - 5,
					"y" : 73,

					"text" : uiScriptLocale.OK,

					"default_image" : "d:/ymir work/ui/public/middle_button_01.sub",
					"over_image" : "d:/ymir work/ui/public/middle_button_02.sub",
					"down_image" : "d:/ymir work/ui/public/middle_button_03.sub",
				},
				{
					"name" : "cancel_button",
					"type" : "button",

					"x" : 170/2 + 5,
					"y" : 73,

					"text" : uiScriptLocale.CANCEL,

					"default_image" : "d:/ymir work/ui/public/middle_button_01.sub",
					"over_image" : "d:/ymir work/ui/public/middle_button_02.sub",
					"down_image" : "d:/ymir work/ui/public/middle_button_03.sub",
				},
			),
		},
	),
}
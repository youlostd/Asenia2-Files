import uiScriptLocale

ROOT_PATH = "d:/ymir work/ui/public/"

TEMPORARY_X = +13
TEXT_TEMPORARY_X = -10
BUTTON_TEMPORARY_X = 5
PVP_X = -10

window = {
	"name" : "SystemOptionDialog",
	"style" : ("movable", "float",),

	"x" : 0,
	"y" : 0,

	"width" : 250,
	"height" : 100,

	"children" :
	(
		{
			"name" : "board",
			"type" : "board",

			"x" : 0,
			"y" : 0,

			"width" : 250,
			"height" : 100,

			"children" :
			(
				## Title
				{
					"name" : "titlebar",
					"type" : "titlebar",
					"style" : ("attach",),

					"x" : 8,
					"y" : 8,

					"width" : 230,
					"color" : "gray",

					"children" :
					(
						{ 
						"name":"titlename", "type":"text", "x":0, "y":3, 
						"horizontal_align":"center", "text_horizontal_align":"center",
						"text" : "Efsun Botu",
						 },
					),
				},
				{
					"type" : "image",

					"x" : 60,
					"y" : 35,

					"image" : "icon/item/71084.tga",
				},
				{
					"type" : "image",

					"x" : 150,
					"y" : 35,

					"image" : "icon/item/71084.tga",
				},
				{
					"name" : "bgm_button2",
					"type" : "button",

					"x" : 30,
					"y" : 70,

					"text" : "Efsun Botu(1)",

					"default_image" : ROOT_PATH + "Large_Button_01.sub",
					"over_image" : ROOT_PATH + "Large_Button_02.sub",
					"down_image" : ROOT_PATH + "Large_Button_03.sub",
				},
				{
					"name" : "bgm_button3",
					"type" : "button",

					"x" : 120,
					"y" : 70,

					"text" : "Efsun Botu(2)",

					"default_image" : ROOT_PATH + "Large_Button_01.sub",
					"over_image" : ROOT_PATH + "Large_Button_02.sub",
					"down_image" : ROOT_PATH + "Large_Button_03.sub",
				},
			),
		},
	),
}

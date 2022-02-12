import uiScriptLocale

ROOT_PATH = "d:/ymir work/ui/public/"
ROOT = "d:/ymir work/ui/public/"
FACE_SLOT_FILE = "d:/ymir work/ui/game/windows/box_face.sub"

TEMPORARY_X = +13
TEXT_TEMPORARY_X = -10
BUTTON_TEMPORARY_X = 5
PVP_X = -10

window = {
	"name" : "SystemOptionDialog",
	"style" : ("movable", "float",),

	"x" : 0,
	"y" : 0,

	"width" : 127,
	"height" : 490,

	"children" :
	(
		{
			"name" : "board",
			"type" : "board",

			"x" : 0,
			"y" : 0,

			"width" : 127,
			"height" : 445,

			"children" :
			(
				## Title
				{
					"name" : "titlebar",
					"type" : "titlebar",
					"style" : ("attach",),

					"x" : 8,
					"y" : 8,

					"width" : 120,
					"color" : "gray",

					"children" :
					(
						{ 
						"name":"titlename", "type":"text", "x":0, "y":3, 
						"horizontal_align":"center", "text_horizontal_align":"center",
						"text" : "Hava Durumu",
						 },
					),
				},
				{
					"name" : "skybox_button1",
					"type" : "radio_button",

					"x" : 20,
					"y" : 50+40-40,

					"text" : "Normal",

					"default_image" : ROOT + "Large_Button_01.sub",
					"over_image" : ROOT + "Large_Button_02.sub",
					"down_image" : ROOT + "Large_Button_03.sub",
				},
				{
					"name" : "skybox_button2",
					"type" : "radio_button",

					"x" : 20,
					"y" : 80+40-40,

					"text" : uiScriptLocale.ibrahim_sky1,

					"default_image" : ROOT + "Large_Button_01.sub",
					"over_image" : ROOT + "Large_Button_02.sub",
					"down_image" : ROOT + "Large_Button_03.sub",
				},
				{
					"name" : "skybox_button3",
					"type" : "radio_button",

					"x" : 20,
					"y" : 80+40+30-40,

					"text" : uiScriptLocale.ibrahim_sky2,

					"default_image" : ROOT + "Large_Button_01.sub",
					"over_image" : ROOT + "Large_Button_02.sub",
					"down_image" : ROOT + "Large_Button_03.sub",
				},
				{
					"name" : "skybox_button4",
					"type" : "radio_button",

					"x" : 20,
					"y" : 140+40-40,

					"text" : "Gece",

					"default_image" : ROOT + "Large_Button_01.sub",
					"over_image" : ROOT + "Large_Button_02.sub",
					"down_image" : ROOT + "Large_Button_03.sub",
				},
				{
					"name" : "skybox_button5",
					"type" : "radio_button",

					"x" : 20,
					"y" : 170+40-40,

					"text" : "Sabah", 

					"default_image" : ROOT + "Large_Button_01.sub",
					"over_image" : ROOT + "Large_Button_02.sub",
					"down_image" : ROOT + "Large_Button_03.sub",
				},
				{
					"name" : "skybox_button6",
					"type" : "radio_button",

					"x" : 20,
					"y" : 200+40-40,

					"text" : uiScriptLocale.ibrahim_sky3,

					"default_image" : ROOT + "Large_Button_01.sub",
					"over_image" : ROOT + "Large_Button_02.sub",
					"down_image" : ROOT + "Large_Button_03.sub",
				},
				{
					"name" : "skybox_button7",
					"type" : "radio_button",

					"x" : 20,
					"y" : 230+40-40,

					"text" : uiScriptLocale.ibrahim_sky4,

					"default_image" : ROOT + "Large_Button_01.sub",
					"over_image" : ROOT + "Large_Button_02.sub",
					"down_image" : ROOT + "Large_Button_03.sub",
				},
				{
					"name" : "skybox_button8",
					"type" : "radio_button",

					"x" : 20,
					"y" : 260+40-40,

					"text" : "Sisli 1",

					"default_image" : ROOT + "Large_Button_01.sub",
					"over_image" : ROOT + "Large_Button_02.sub",
					"down_image" : ROOT + "Large_Button_03.sub",
				},
				{
					"name" : "skybox_button9",
					"type" : "radio_button",

					"x" : 20,
					"y" : 290+40-40,

					"text" : uiScriptLocale.ibrahim_sky5,

					"default_image" : ROOT + "Large_Button_01.sub",
					"over_image" : ROOT + "Large_Button_02.sub",
					"down_image" : ROOT + "Large_Button_03.sub",
				},
				{
					"name" : "skybox_button10",
					"type" : "radio_button",

					"x" : 20,
					"y" : 320+40-40,

					"text" : "Sisli 2",

					"default_image" : ROOT + "Large_Button_01.sub",
					"over_image" : ROOT + "Large_Button_02.sub",
					"down_image" : ROOT + "Large_Button_03.sub",
				},
				{
					"name" : "skybox_button11",
					"type" : "radio_button",

					"x" : 20,
					"y" : 350+40-40,

					"text" : "Bulutlu",

					"default_image" : ROOT + "Large_Button_01.sub",
					"over_image" : ROOT + "Large_Button_02.sub",
					"down_image" : ROOT + "Large_Button_03.sub",
				},
				{
					"name" : "skybox_button12",
					"type" : "radio_button",

					"x" : 20,
					"y" : 380+40-40,

					"text" : uiScriptLocale.ibrahim_sky6,

					"default_image" : ROOT + "Large_Button_01.sub",
					"over_image" : ROOT + "Large_Button_02.sub",
					"down_image" : ROOT + "Large_Button_03.sub",
				},
				{
					"name" : "skybox_button13",
					"type" : "radio_button",

					"x" : 20,
					"y" : 410+40-40,

					"text" : uiScriptLocale.ibrahim_sky7,

					"default_image" : ROOT + "Large_Button_01.sub",
					"over_image" : ROOT + "Large_Button_02.sub",
					"down_image" : ROOT + "Large_Button_03.sub",
				},
			),
		},
	),
}

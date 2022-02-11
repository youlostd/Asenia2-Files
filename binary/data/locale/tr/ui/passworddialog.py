import uiScriptLocale

window = {
	"name" : "PasswordDialog",

	"x" : 0,
	"y" : 0,

	"style" : ("movable", "float",),

	"width" : 355,
	"height" : 220,

	"children" :
	(
		{
			"name" : "board",
			"type" : "board",

			"x" : 0,
			"y" : 0,

			"width" : 355,
			"height" : 220,

			"children" :
			(
				#
				{
					"name" : "Desc1",
					"type" : "text",

					"x" : 355/2,
					"y" : 35,

					"text" : uiScriptLocale.PASSWORD_DESC_1,
					"text_horizontal_align":"center"
				},
				#³»¿ë2
				{
					"name" : "Desc2",
					"type" : "text",

					"x" : 355/2,
					"y" : 47,

					"text" : uiScriptLocale.PASSWORD_DESC_2,
					"text_horizontal_align":"center"
				},
				## Title
				{
					"name" : "titlebar",
					"type" : "titlebar",
					"style" : ("attach",),

					"x" : 8,
					"y" : 8,
 
					"width" : 339,
					"color" : "gray",

					"children" :
					(
						{
							"name" : "TitleName",
							"type" : "text",

							"x" : 339/2,
							"y" : 3,

							"text" : uiScriptLocale.PASSWORD_TITLE,
							"text_horizontal_align":"center"
						},
					),
				},

				## Password Slot
				{
					"name" : "password_slot",
					"type" : "image",

					"x" : 0,
					"y" : 63,
					"horizontal_align" : "center",

					"image" : "d:/ymir work/ui/public/Parameter_Slot_02.sub",

					"children" :
					(
						{
							"name" : "password_value",
							"type" : "editline",

							"x" : 3,
							"y" : 3,

							"width" : 60,
							"height" : 18,

							"input_limit" : 6,
							"secret_flag" : 1,
						},
					),
				},
				{
					"name" : "Desc3",
					"type" : "text",

					"x" : 355/2,
					"y" : 140, # 80

					"text" : uiScriptLocale.PASSWORD_DESC_3,
					"text_horizontal_align":"center"
				},
				{
					"name" : "Desc4",
					"type" : "text",

					"x" : 355/2,
					"y" : 152,

					"text" : uiScriptLocale.PASSWORD_DESC_4,
					"text_horizontal_align":"center"
				},
				{
					"name" : "Desc5",
					"type" : "text",

					"x" : 355/2,
					"y" : 164,

					"text" : uiScriptLocale.PASSWORD_DESC_5,
					"text_horizontal_align":"center"
				},
				## Button
				{
					"name" : "accept_button",
					"type" : "button",

					"x" : 355/2 - 61 - 5,
					"y" : 180,

					"text" : uiScriptLocale.OK,

					"default_image" : "d:/ymir work/ui/public/middle_button_01.sub",
					"over_image" : "d:/ymir work/ui/public/middle_button_02.sub",
					"down_image" : "d:/ymir work/ui/public/middle_button_03.sub",
				},
				{
					"name" : "cancel_button",
					"type" : "button",

					"x" : 355/2 + 5,
					"y" : 180,

					"text" : uiScriptLocale.CANCEL,

					"default_image" : "d:/ymir work/ui/public/middle_button_01.sub",
					"over_image" : "d:/ymir work/ui/public/middle_button_02.sub",
					"down_image" : "d:/ymir work/ui/public/middle_button_03.sub",
				},
				## Button
				{
					"name" : "normal_button",
					"type" : "radio_button",

					"x" : 20,
					"y" : 96,

					"text" : "",

					"default_image" : "d:/ymir work/ui/depo_nesne/depo_1.tga",
					"over_image" : "d:/ymir work/ui/depo_nesne/depo_2.tga",
					"down_image" : "d:/ymir work/ui/depo_nesne/depo_3.tga",
				},
				{
					"name" : "nesne_button",
					"type" : "radio_button",

					"x" : 185,
					"y" : 96,

					"text" : "",

					"default_image" : "d:/ymir work/ui/depo_nesne/nesne_1.tga",
					"over_image" : "d:/ymir work/ui/depo_nesne/nesne_2.tga",
					"down_image" : "d:/ymir work/ui/depo_nesne/nesne_3.tga",
				},
			),
		},
	),
}
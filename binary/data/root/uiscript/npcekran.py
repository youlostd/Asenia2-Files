import uiScriptLocale,localeInfo
GOLD_COLOR	= 0xFFFEE3AE
BUTTON_ADRESS = "d:/ymir work/ui/game/myshop_deco/"

window = {
	"name" : "GameOptionDialog",
	"style" : ("movable", "float",),

	"x" : (SCREEN_WIDTH/2) - (190/2),
	"y" : (SCREEN_HEIGHT/2) - 100,	

	"width" : 391,
	"height" : 265+160,

	"children" :
	(
		{
			"name" : "board",
			"type" : "board",

			"x" : 0,
			"y" : 0,

			"width" : 391,
			"height" : 265+150,

			"children" :
			(
				## Title
				{
					"name" : "titlebar",
					"type" : "titlebar",
					"style" : ("attach",),

					"x" : 6,
					"y" : 7,

					"width" : 391-13,
					"color" : "gray",

					"children" :
					(
						{ "name":"titlename", "type":"text", "x":0, "y":3, 
						"text" : "Uzaktan Market", 
						"horizontal_align":"center", "text_horizontal_align":"center" },
					),
				},

				{
					"name" : "BlackBoard1",
					"type" : "thinboard_circle",
					"x" : 10, "y" : 36, "width" : 190-(13*2), "height" : 368,
					"children":
					(
						{
							"name" : "genel", "type" : "button",
							"x" : 7, "y" : 9, "text": "Genel Market",

							"default_image" : BUTTON_ADRESS+ "select_btn_01.sub",
							"over_image" : BUTTON_ADRESS +"select_btn_02.sub",
							"down_image" : BUTTON_ADRESS +"select_btn_03.sub",
						},
						{
							"name" : "bilet", "type" : "button",
							"x" : 7, "y" : 9+30, "text": "Bilet Market",

							"default_image" : BUTTON_ADRESS+ "select_btn_01.sub",
							"over_image" : BUTTON_ADRESS +"select_btn_02.sub",
							"down_image" : BUTTON_ADRESS +"select_btn_03.sub",
						},
						{
							"name" : "arti", "type" : "button",
							"x" : 7, "y" : 9+(30*2), "text": "Yükseltme Eþyalarý",

							"default_image" : BUTTON_ADRESS+ "select_btn_01.sub",
							"over_image" : BUTTON_ADRESS +"select_btn_02.sub",
							"down_image" : BUTTON_ADRESS +"select_btn_03.sub",
						},
						{
							"name" : "iksir", "type" : "button",
							"x" : 7, "y" : 9+(30*3), "text": "Ýksir Market",

							"default_image" : BUTTON_ADRESS+ "select_btn_01.sub",
							"over_image" : BUTTON_ADRESS +"select_btn_02.sub",
							"down_image" : BUTTON_ADRESS +"select_btn_03.sub",
						},
						{
							"name" : "tas", "type" : "button",
							"x" : 7, "y" : 9+(30*4), "text": "Deðerli Taþ Tüccarý",

							"default_image" : BUTTON_ADRESS+ "select_btn_01.sub",
							"over_image" : BUTTON_ADRESS +"select_btn_02.sub",
							"down_image" : BUTTON_ADRESS +"select_btn_03.sub",
						},
						{
							"name" : "bio", "type" : "button",
							"x" : 7, "y" : 9+(30*5), "text": "Biyolog Malzemeleri",

							"default_image" : BUTTON_ADRESS+ "select_btn_01.sub",
							"over_image" : BUTTON_ADRESS +"select_btn_02.sub",
							"down_image" : BUTTON_ADRESS +"select_btn_03.sub",
						},
						{
							"name" : "gaya", "type" : "button",
							"x" : 7, "y" : 9+(30*6), "text": "Boss Gaya Maðazasý",

							"default_image" : BUTTON_ADRESS+ "select_btn_01.sub",
							"over_image" : BUTTON_ADRESS +"select_btn_02.sub",
							"down_image" : BUTTON_ADRESS +"select_btn_03.sub",
						},
						{
							"name" : "gaya2", "type" : "button",
							"x" : 7, "y" : 9+(30*7), "text": "Metin Gaya Maðazasý",

							"default_image" : BUTTON_ADRESS+ "select_btn_01.sub",
							"over_image" : BUTTON_ADRESS +"select_btn_02.sub",
							"down_image" : BUTTON_ADRESS +"select_btn_03.sub",
						},
						{
							"name" : "yil", "type" : "button",
							"x" : 7, "y" : 9+(30*8), "text": "Simya Market",

							"default_image" : BUTTON_ADRESS+ "select_btn_01.sub",
							"over_image" : BUTTON_ADRESS +"select_btn_02.sub",
							"down_image" : BUTTON_ADRESS +"select_btn_03.sub",
						},
						{
							"name" : "silah", "type" : "button",
							"x" : 7, "y" : 9+(30*9), "text": "Silah satýcýsý",

							"default_image" : BUTTON_ADRESS+ "select_btn_01.sub",
							"over_image" : BUTTON_ADRESS +"select_btn_02.sub",
							"down_image" : BUTTON_ADRESS +"select_btn_03.sub",
						},
						{
							"name" : "zirh", "type" : "button",
							"x" : 7, "y" : 9+(30*10), "text": "Zýrh Satýcýsý",

							"default_image" : BUTTON_ADRESS+ "select_btn_01.sub",
							"over_image" : BUTTON_ADRESS +"select_btn_02.sub",
							"down_image" : BUTTON_ADRESS +"select_btn_03.sub",
						},
						{
							"name" : "balikci", "type" : "button",
							"x" : 7, "y" : 9+(30*11), "text": "Balýkçý",

							"default_image" : BUTTON_ADRESS+ "select_btn_01.sub",
							"over_image" : BUTTON_ADRESS +"select_btn_02.sub",
							"down_image" : BUTTON_ADRESS +"select_btn_03.sub",
						},
					),
				},		
						{
							"name" : "ModelView",
							"type" : "image",
							
							"x" : 182, "y" : 36,
							"image" : "d:/ymir work/ui/game/myshop_deco/model_view_title.sub",
							
							"children" :
							(
								{ "name" : "ModelName", "type" : "text", "x" : 0, "y" : 0, "text" : "Market Önizleme", "all_align":"center" },
							),
						},
					),
				},
			),
		}

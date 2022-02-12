import uiScriptLocale
import item
import app


window = {
		"name" : "MenuWindow",

		"x" : SCREEN_WIDTH - 75 - 58,
		"y" : SCREEN_HEIGHT - 37 - 565 + 209 + 32,

		"width" : 40,
		"height" : 415,

		"type" : "image",
		"image" : "d:/ymir work/ui/game/belt_inventory/bg.tga",
		

#		"children" :
#		(
#
#			
#			{
#				"name" : "menuboard",
#				"type" : "board",
#				"style" : ("attach","float"),
#
#				"x" : 13,
#				"y" : 0,
#
#				"width" : 40,
#				"height" : 312,
#
				"children" :
				(

					{
						"name" : "patron",
						"type" : "button",

						"x" : 19,
						"y" : 140,
						"tooltip_text" : "Offlineshop",
						"tooltip_x" : -50,
						"tooltip_y" : 10,

						"default_image" : "locale/tr/board/boss.png",
						"over_image" : "locale/tr/board/boss2.png",
						"down_image" : "locale/tr/board/boss3.png",

					},
					{
						"name" : "efsun",
						"type" : "button",

						"x" : 19,
						"y" : 171,
						"tooltip_text" : "Efsun Botu",
						"tooltip_x" : -50,
						"tooltip_y" : 10,
								

						"default_image" : "locale/tr/board/efsun.png",
						"over_image" : "locale/tr/board/efsun2.png",
						"down_image" : "locale/tr/board/efsun3.png",
					},
					
					{
						"name" : "Biyolog",
						"type" : "button",

						"x" : 19,
						"y" : 202,
						"tooltip_text" : "Biyolog",
						"tooltip_x" : -50,
						"tooltip_y" : 10,
						
						"default_image" : "locale/tr/board/biyolog.png",
						"over_image" : "locale/tr/board/biyolog2.png",
						"down_image" : "locale/tr/board/biyolog3.png",
						
					},
					
					
					{
						"name" : "guvenlik",
						"type" : "button",

						"x" : 19,
						"y" : 233,
						"tooltip_text" : "Güvenlik Sistemi",
						"tooltip_x" : -50,
						"tooltip_y" : 10,

						"default_image" : "locale/tr/board/guvenlik.png",
						"over_image" : "locale/tr/board/guvenlik2.png",
						"down_image" : "locale/tr/board/guvenlik3.png",
					},

					{
						"name" : "stonesellbutton",
						"type" : "button",

						"x" : 19,
						"y" : 264,
						"tooltip_text" : "Güvenli Pc",
						"tooltip_x" : -50,
						"tooltip_y" : 10,

						"default_image" : "locale/tr/board/gpc.png",
						"over_image" : "locale/tr/board/gpc2.png",
						"down_image" : "locale/tr/board/gpc3.png",
					},
					{
						"name" : "dungeon",
						"type" : "button",

						"x" : 19,
						"y" : 295,
						"tooltip_text" : "Zindan Takip",
						"tooltip_x" : -50,
						"tooltip_y" : 10,

						"default_image" : "locale/tr/board/zindantakip.png",
						"over_image" : "locale/tr/board/zindantakip2.png",
						"down_image" : "locale/tr/board/zindantakip3.png",
					},
#					{
#						"name" : "special",
#						"type" : "button",
#
#						"x" : 7,
#						"y" : 190,
#						"tooltip_text" : "K Envanter",
#						"tooltip_x" : -50,
#						"tooltip_y" : 10,
#
#						"default_image" : "locale/tr/board/kenvanter.png",
#						"over_image" : "locale/tr/board/kenvanter2.png",
#						"down_image" : "locale/tr/board/kenvanter3.png",
#					},
				
					{
						"name" : "thanos",
						"type" : "button",

						"x" : 19,
						"y" : 326,
						"tooltip_text" : "Patron takip",
						"tooltip_x" : -50,
						"tooltip_y" : 10,

						"default_image" : "locale/tr/board/thanos.png",
						"over_image" : "locale/tr/board/thanos2.png",
						"down_image" : "locale/tr/board/thanos3.png",
					},				
					{
						"name" : "won_ac",
						"type" : "button",

						"x" : 19,
						"y" : 357,
						"tooltip_text" : "Won Takas",
						"tooltip_x" : -50,
						"tooltip_y" : 10,

						"default_image" : "locale/tr/board/won.png",
						"over_image" : "locale/tr/board/won2.png",
						"down_image" : "locale/tr/board/won3.png",
					},					
					
				)
			}


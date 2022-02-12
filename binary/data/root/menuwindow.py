import uiScriptLocale
import item
import app


window = {
		"name" : "MenuWindow",

		"x" : SCREEN_WIDTH - 176 - 58,
		"y" : SCREEN_HEIGHT - 37 - 565 + 209 + 32,

		"width" : 40,
		"height" : 510,

		"type" : "image",
		"image" : "d:/ymir work/ui/game/belt_inventory/bg.tga",
		

		"children" :
		(

			
			{
				"name" : "menuboard",
				"type" : "board",
				"style" : ("attach","float"),

				"x" : 0,
				"y" : 0,

				"width" : 40,
				"height" : 438,

				"children" :
				(

					{
						"name" : "patron",
						"type" : "button",

						"x" : 10,
						"y" : 10,
						"tooltip_text" : "Patron Takip",
						"tooltip_x" : -50,
						"tooltip_y" : 10,

						"default_image" : "locale/tr/board/boss.tga",
						"over_image" : "locale/tr/board/boss2.tga",
						"down_image" : "locale/tr/board/boss3.tga",

					},
					{
						"name" : "DepoButton",
						"type" : "button",

						"x" : 10,
						"y" : 45,
						"tooltip_text" : "Depo",
						"tooltip_x" : -50,
						"tooltip_y" : 10,
								

						"default_image" : "locale/tr/board/depo.tga",
						"over_image" : "locale/tr/board/depo2.tga",
						"down_image" : "locale/tr/board/depo3.tga",
					},
					
					{
						"name" : "efsun",
						"type" : "button",

						"x" : 10,
						"y" : 80,
						"tooltip_text" : "Efsun Botu",
						"tooltip_x" : -50,
						"tooltip_y" : 10,
								

						"default_image" : "locale/tr/board/efsun.tga",
						"over_image" : "locale/tr/board/efsun2.tga",
						"down_image" : "locale/tr/board/efsun3.tga",
					},
					
					{
						"name" : "Biyolog",
						"type" : "button",

						"x" : 10,
						"y" : 115,
						"tooltip_text" : "Biyolog",
						"tooltip_x" : -50,
						"tooltip_y" : 10,
						
						"default_image" : "locale/tr/board/biyolog.tga",
						"over_image" : "locale/tr/board/biyolog2.tga",
						"down_image" : "locale/tr/board/biyolog3.tga",
						
					},
					
					
					{
						"name" : "guvenlik",
						"type" : "button",

						"x" : 10,
						"y" : 150,
						"tooltip_text" : "Güvenlik Sistemi",
						"tooltip_x" : -50,
						"tooltip_y" : 10,

						"default_image" : "locale/tr/board/guvenlik.tga",
						"over_image" : "locale/tr/board/guvenlik2.tga",
						"down_image" : "locale/tr/board/guvenlik3.tga",
					},

					{
						"name" : "stonesellbutton",
						"type" : "button",

						"x" : 10,
						"y" : 185,
						"tooltip_text" : "Güvenli Pc",
						"tooltip_x" : -50,
						"tooltip_y" : 10,

						"default_image" : "locale/tr/board/tassat.tga",
						"over_image" : "locale/tr/board/tassat2.tga",
						"down_image" : "locale/tr/board/tassat3.tga",
					},
					{
						"name" : "dungeon",
						"type" : "button",

						"x" : 10,
						"y" : 220,
						"tooltip_text" : "Zindan Takip",
						"tooltip_x" : -50,
						"tooltip_y" : 10,

						"default_image" : "locale/tr/board/zindantakip.tga",
						"over_image" : "locale/tr/board/zindantakip2.tga",
						"down_image" : "locale/tr/board/zindantakip3.tga",
					},
					{
						"name" : "special",
						"type" : "button",

						"x" : 10,
						"y" : 255,
						"tooltip_text" : "K Envanter",
						"tooltip_x" : -50,
						"tooltip_y" : 10,

						"default_image" : "locale/tr/board/kenvanter.tga",
						"over_image" : "locale/tr/board/kenvanter2.tga",
						"down_image" : "locale/tr/board/kenvanter3.tga",
					},
				
					{
						"name" : "thanos",
						"type" : "button",

						"x" : 10,
						"y" : 290,
						"tooltip_text" : "Çakra Penceresi",
						"tooltip_x" : -50,
						"tooltip_y" : 10,

						"default_image" : "locale/tr/board/thanos.tga",
						"over_image" : "locale/tr/board/thanos2.tga",
						"down_image" : "locale/tr/board/thanos3.tga",
					},				
					{
						"name" : "npc",
						"type" : "button",

						"x" : 10,
						"y" : 325,
						"tooltip_text" : "Uzak Npc",
						"tooltip_x" : -50,
						"tooltip_y" : 10,

						"default_image" : "locale/tr/board/uzaknpc.tga",
						"over_image" : "locale/tr/board/uzaknpc2.tga",
						"down_image" : "locale/tr/board/uzaknpc3.tga",
					},
					
					{
						"name" : "won_ac",
						"type" : "button",

						"x" : 10,
						"y" : 360,
						"tooltip_text" : "Won Takas",
						"tooltip_x" : -50,
						"tooltip_y" : 10,

						"default_image" : "locale/tr/board/won.tga",
						"over_image" : "locale/tr/board/won2.tga",
						"down_image" : "locale/tr/board/won3.tga",
					},					
					{
						"name" : "OfflineShopButton",
						"type" : "button",

						"x" : 10,
						"y" : 395,
						"tooltip_text" : "Offline Shop" ,
						"tooltip_x" : -50,
						"tooltip_y" : 10,

						"default_image" : "locale/tr/board/offline.tga",
						"over_image" : "locale/tr/board/offline2.tga",
						"down_image" : "locale/tr/board/offline3.tga",

					},
					
                    
				)
			},

		),
	}


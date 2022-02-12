import uiScriptLocale

window = {
	"name" : "OfflineShopBankWindow",
	
	"style" : ("movable", "float", ),
	
	"x" : 0,
	"y" : 0,
	
	"width" : 300,
	"height" : 240,
	
	"children" :
	(
		{
			"name" : "Board",
			"type" : "board_with_titlebar",
			
			"style" : ("attach", ),
			
			"x" : 0,
			"y" : 0,
			
			"width" : 300,
			"height" : 240,
			
			"title" : uiScriptLocale.CT_BANKA_BASLIK,
			
			"children" :
			(	
				# bilgilendirme
				{
					"name" : "bilgilendirme",
					"type" : "thinboard",

					"x" : 0,
					"y" : 35,
					"width" : 270,
					"height" : 20,
					"horizontal_align" : "center",

					"children" :
					(
						{
							"name" : "text1",
							"type" : "text",
							"x" : 15,
							"y" : 9,
							"text" : uiScriptLocale.CT_BANKA_INFO,
						},
					),
				},
				# Yazilar
				{
					"name" : "para",
					"type" : "thinboard",

					"x" : 0,
					"y" : 72,
					"width" : 270,
					"height" : 110,
					"horizontal_align" : "center",

					"children" :
					(
						{
							"name" : "text1",
							"type" : "text",
							"x" : 10,
							"y" : 10,
							"text" : uiScriptLocale.CT_BANKA_MEVCUT_YANG,
						},
						{
							"name" : "CurrentMoneySlot",
							"type" : "slotbar",
							
							"x" : 10,
							"y" : 31,
							
							"width" : 110,
							"height" : 15,
							
							"children" :
							(
								{
									"name" : "CurrentMoneyLine",
									"type" : "text",
									
									"x" : 3,
									"y" : 3,
									
									"width" : 210,
									"height" : 15,
									
									"input_limit" : 12,
									"text" : "0123456789",
								},
							),
						},
						{
							"name" : "text2",
							"type" : "text",
							"x" : 150,
							"y" : 10,
							"text" : uiScriptLocale.CT_BANKA_MEVCUT_WON,
						},
						{
							"name" : "CurrentChequeSlot",
							"type" : "slotbar",
							
							"x" : 150,
							"y" : 31,
							
							"width" : 110,
							"height" : 15,
							
							"children" :
							(
								{
									"name" : "CurrentChequeLine",
									"type" : "text",
									
									"x" : 3,
									"y" : 3,
									
									"width" : 210,
									"height" : 15,
									
									"input_limit" : 12,
									"text" : "0123456789",
								},
							),
						},
						{
							"name" : "text3",
							"type" : "text",
							"x" : 150,
							"y" : 56,
							"text" : uiScriptLocale.CT_BANKA_CEKILMEK_ISTENEN_MIKTAR,
						},
						{
							"name" : "RequiredChequeSlot",
							"type" : "slotbar",
							
							"x" : 150,
							"y" : 76,
							
							"width" : 110,
							"height" : 15,
							
							"children" :
							(
								{
									"name" : "RequiredChequeLine",
									"type" : "editline",
									
									"x" : 3,
									"y" : 3,
									
									"width" : 210,
									"height" : 15,
									
									"input_limit" : 12,
									"text" : "0123456789",
								},
							),
						},
						{
							"name" : "text1",
							"type" : "text",
							"x" : 10,
							"y" : 56,
							"text" : uiScriptLocale.CT_BANKA_CEKILMEK_ISTENEN_MIKTAR,
						},
						{
							"name" : "RequiredMoneySlot",
							"type" : "slotbar",
							
							"x" : 10,
							"y" : 76,
							
							"width" : 110,
							"height" : 15,
							
							"children" :
							(
								{
									"name" : "RequiredMoneyLine",
									"type" : "editline",
									
									"x" : 3,
									"y" : 3,
									
									"width" : 210,
									"height" : 15,
									
									"input_limit" : 12,
									"text" : "0123456789",
								},
							),
						},
					),
				},
				
				# butonlar
				{
					"name" : "butonlar",
					"type" : "thinboard",

					"x" : 0,
					"y" : 187,
					"width" : 270,
					"height" : 40,
					"horizontal_align" : "center",

					"children" :
					(
						{
							"name" : "withdraw_button",
							"type" : "button",
							
							"x" : 35,
							"y" : 10,
							
							"text" : uiScriptLocale.CT_BANKA_YANG_CEK,
							
							"default_image" : "d:/ymir work/ui/public/large_button_01.sub",
							"over_image" : "d:/ymir work/ui/public/large_button_02.sub",
							"down_image" : "d:/ymir work/ui/public/large_button_03.sub",
						},
						
						{
							"name" : "cancel_button",
							"type" : "button",
							
							"x" : 145,
							"y" : 10,
							
							"text" : uiScriptLocale.CT_BANKA_VAZGEC,
							
							"default_image" : "d:/ymir work/ui/public/large_button_01.sub",
							"over_image" : "d:/ymir work/ui/public/large_button_02.sub",
							"down_image" : "d:/ymir work/ui/public/large_button_03.sub",					
						},
					),
				},
			),
		},
	),
}
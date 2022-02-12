import uiScriptLocale

window = {
	"name" : "OfflineShopAdminPanelWindow",
	
	"x" : 0,
	"y" : 0,
	
	"style" : ("movable", "float",),
	
	"width" : 364,
	"height" : 240,
	
	"children":
	(
		{
			"name" : "Board",
			"type" : "board_with_titlebar",
			"style" : ("attach",),
			
			"x" : 0,
			"y" : 0,
			
			"width" : 364,
			"height" : 240,
			
			"title" : uiScriptLocale.CT_ADMIN_PANEL_BASLIK,
			
			"children" :
			(	
				# Yazilar
				{
					"name" : "yazilar",
					"type" : "thinboard",

					"x" : -53,
					"y" : 48,
					"width" : 240,
					"height" : 150,
					"horizontal_align" : "center",

					"children" :
					(
						#{
					#"name" : "Icon",
					#"type" : "expanded_image",
					
					#"x" : 40,
					#"y" : -62,
					
					#"image" : uiScriptLocale.LOCALE_OFFLINESHOP_PATH + "/55lvlmt2.tga",
						#},		
						# User Name
						{
							"name" : "UserName",
							"type" : "text",
							
							"x" : 10,
							"y" : 27,
							
							"text" : uiScriptLocale.CT_ADMIN_MERHABA,
						},
						
						{
							"name" : "yazi3",
							"type" : "text",
							
							"x" : 10,
							"y" : 47,
							
							"text" : uiScriptLocale.CT_ADMIN_MERHABA,
						},
						
						{
							"name" : "yazi1",
							"type" : "text",
							
							"x" : 10,
							"y" : 67,
							
							"text" : uiScriptLocale.CT_ADMIN_MERHABA,
						},
						
						{
							"name" : "yazi2",
							"type" : "text",
							
							"x" : 10,
							"y" : 87,
							
							"text" : uiScriptLocale.CT_ADMIN_MERHABA,
						},
						
						{
							"name" : "yazi4",
							"type" : "text",
							
							"x" : 10,
							"y" : 107,
							
							"text" : uiScriptLocale.CT_ADMIN_MERHABA,
						},
						
						{
							"name" : "yazi5",
							"type" : "text",
							
							"x" : 10,
							"y" : 127,
							
							"text" : uiScriptLocale.CT_ADMIN_MERHABA,
						},
											
					),

				},
				
				# butonlar
				{
					"name" : "butonlar",
					"type" : "thinboard",

					"x" : 120,
					"y" : 35,
					"width" : 100,
					"height" : 193,
					"horizontal_align" : "center",

					"children" :
					(
						# Open Offline Shop
#						{
#							"name" : "OpenOfflineShopButton",
#							"type" : "button",
#							
#							"x" : 5,
#							"y" : 7,
#							
#							"text" : uiScriptLocale.CT_ADMIN_PAZAR_AC,
#							
#							"default_image" : "d:/ymir work/ui/public/large_button_01.sub",
#							"over_image" : "d:/ymir work/ui/public/large_button_02.sub",
#							"down_image" : "d:/ymir work/ui/public/large_button_03.sub",
#						},
						# Close Offline Shop
						{
							"name" : "CloseOfflineShopButton",
							"type" : "button",
							
							"x" : 5,
							"y" : 37,
							
							"text" : uiScriptLocale.CT_ADMIN_PAZAR_KAPAT,
							
							"default_image" : "d:/ymir work/ui/public/large_button_01.sub",
							"over_image" : "d:/ymir work/ui/public/large_button_02.sub",
							"down_image" : "d:/ymir work/ui/public/large_button_03.sub",
						},
						# My Bank
						{
							"name" : "MyBankButton",
							"type" : "button",
							
							"x" : 5,
							"y" : 67,
							
							"text" : uiScriptLocale.CT_ADMIN_BANKA,
							
							"default_image" : "d:/ymir work/ui/public/large_button_01.sub",
							"over_image" : "d:/ymir work/ui/public/large_button_02.sub",
							"down_image" : "d:/ymir work/ui/public/large_button_03.sub",										
						},
						# Satilmayanlar
						{
							"name" : "SatilmayanlarButtons",
							"type" : "button",
							
							"x" : 5,
							"y" : 97,
							
							"text" : uiScriptLocale.CT_ADMIN_SATILMAYANLAR,
							
							"default_image" : "d:/ymir work/ui/public/large_button_01.sub",
							"over_image" : "d:/ymir work/ui/public/large_button_02.sub",
							"down_image" : "d:/ymir work/ui/public/large_button_03.sub",
						},
						# TezgahaBak
						{
							"name" : "TezgahaBakButton",
							"type" : "button",
							
							"x" : 5,
							"y" : 127,
							
							"text" : uiScriptLocale.CT_ADMIN_TEZGAHIM,
							
							"default_image" : "d:/ymir work/ui/public/large_button_01.sub",
							"over_image" : "d:/ymir work/ui/public/large_button_02.sub",
							"down_image" : "d:/ymir work/ui/public/large_button_03.sub",
						},
						# TezgahaBak
						{
							"name" : "TezgahaBakButton1",
							"type" : "button",
							
							"x" : 5,
							"y" : 157,
							
							"text" : "Esya Ekle",
							
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
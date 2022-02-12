import uiScriptLocale

window = {
	"name" : "OfflineShopInputDialog",
	
	"x" : 0,
	"y" : 0,
	
	"style" : ("movable", "float",),
	
	"width" : 300,
	"height" : 120,
	
	"children" :
	(
		# board
		{
			"name" : "board",
			"type" : "board_with_titlebar",
			
			"style" : ("attach",),
			
			"x" : 0,
			"y" : 0,
			
			"width" : 300,
			"height" : 120,
			
			"title" : uiScriptLocale.CT_KUR_BASLIK,
			
			"children":
			(	
				{
					"name" : "oneHour",
					"type" : "radio_button",
					
					"x" : 30,
					"y" : 37,
					
					"tooltip_text" : uiScriptLocale.CT_UCRET_1M,
					
					"default_image" : "d:/ymir work/new/normal.tga",
					"over_image" : "d:/ymir work/new/hover.tga",
					"down_image" : "d:/ymir work/new/active.tga",
				},

				{
					"name" : "twoHours",
					"type" : "radio_button",
					
					"tooltip_text" : uiScriptLocale.CT_UCRET_2M,
					
					"x" : 90,
					"y" : 37,
					
					"default_image" : "d:/ymir work/new/normal.tga",
					"over_image" : "d:/ymir work/new/hover.tga",
					"down_image" : "d:/ymir work/new/active.tga",
				},

				{
					"name" : "fourHours",
					"type" : "radio_button",
					
					"tooltip_text" : uiScriptLocale.CT_UCRET_3M,
					
					"x" : 150,
					"y" : 37,
					
					"default_image" : "d:/ymir work/new/normal.tga",
					"over_image" : "d:/ymir work/new/hover.tga",
					"down_image" : "d:/ymir work/new/active.tga",
				},
				{
					"name" : "unlimited",
					"type" : "radio_button",
					
					"tooltip_text" : uiScriptLocale.CT_UCRET_4M,
					
					"x" : 210,
					"y" : 37,
					
					"default_image" : "d:/ymir work/new/normal.tga",
					"over_image" : "d:/ymir work/new/hover.tga",
					"down_image" : "d:/ymir work/new/active.tga",
				},
				{
					"name" : "oneHourText",
					"type" : "text",
					
					"x" : 55,
					"y" : 39,
					
					"text" : uiScriptLocale.CT_SURE_1_SAAT,
				},
				
				{
					"name" : "twoHoursText",
					"type" : "text",
					
					"x" : 115,
					"y" : 39,
					
					"text" : uiScriptLocale.CT_SURE_2_SAAT,
				},
				
				{
					"name" : "fourHoursText",
					"type" : "text",
					
					"x" : 175,
					"y" : 39,
					
					"text" : uiScriptLocale.CT_SURE_4_SAAT,
				},				
				{
					"name" : "unlimitedText",
					"type" : "text",
					
					"x" : 235,
					"y" : 39,
					
					"text" : uiScriptLocale.CT_SURE_SURESIZ_SAAT,
				},
				
				##Shop Decoration
				{
					"name" : "DecorationButton",
					"type" : "button",
					
					"x" : 196,
					"y" : 88,
					
					"tooltip_text" : "Dekorasyon", 
					
					"default_image" : "d:/ymir work/ui/public/middle_button_01.sub",
					"over_image" : "d:/ymir work/ui/public/middle_button_02.sub",
					"down_image" : "d:/ymir work/ui/public/middle_button_03.sub",
					
					"text" : "Decorasyon",
				},
				##Shop Decoration
				## Input Slot
				{
					"name" : "InputSlot",
					"type" : "slotbar",

					"x" : -3,
					"y" : 64,
					
					"width" : 235,
					"height" : 15,
					
					"horizontal_align" : "center",
					
					"children" :
					(
						{
							"name" : "InputValue",
							"type" : "editline",

							"x" : 1,
							"y" : 1,

							"width" : 235,
							"height" : 15,

							"input_limit" : 32,
						},
					),
				},

				{
					"name" : "AgreeButton",
					"type" : "button",
					
					"x" : 38,
					"y" : 88,
					
					"default_image" : "d:/ymir work/ui/public/middle_button_01.sub",
					"over_image" : "d:/ymir work/ui/public/middle_button_02.sub",
					"down_image" : "d:/ymir work/ui/public/middle_button_03.sub",
					
					"text" : uiScriptLocale.CT_SURE_TAMAM,
				},
				
				{
					"name" : "CancelButton",
					"type" : "button",
					
					"x" : 116,
					"y" : 88,
					
					"default_image" : "d:/ymir work/ui/public/middle_button_01.sub",
					"over_image" : "d:/ymir work/ui/public/middle_button_02.sub",
					"down_image" : "d:/ymir work/ui/public/middle_button_03.sub",
					
					"text" : uiScriptLocale.CT_SURE_VAZGEC,
				},
			),
		},
	),
}
import uiScriptLocale

window = {
	"name" : "OfflineShopChangeTime",
	
	"style" : ("movable", "float", ),
	
	"x" : 0,
	"y" : 0,
	
	"width" : 466,
	"height" : 174,
	
	"children" :
	(
		# Board
		{
			"name" : "Board",
			"type" : "board_with_titlebar",
			
			"style" : ("attach", ),
			
			"x" : 0,
			"y" : 0,
			
			"width" : 466,
			"height" : 174,
			
			"title" : "Choose to the time!",
			
			"children" :
			(
				{
					"name" : "Icon",
					"type" : "expanded_image",
					
					"x" : 0,
					"y" : 22,
					
					"image" : uiScriptLocale.LOCALE_OFFLINESHOP_PATH + "/time.tga",
				},
				
				{
					"name" : "AgreeButton",
					"type" : "button",
					
					"x" : 151,
					"y" : 137,
					
					"text" : "Pazar Ac",
					
					"default_image" : "d:/ymir work/ui/public/large_button_01.sub",
					"over_image" : "d:/ymir work/ui/public/large_button_02.sub",
					"down_image" : "d:/ymir work/ui/public/large_button_03.sub",
				},
				
				{
					"name" : "CancelButton",
					"type" : "button",
					
					"x" : 243,
					"y" : 137,
					
					"text" : "Kapat",
					
					"default_image" : "d:/ymir work/ui/public/large_button_01.sub",
					"over_image" : "d:/ymir work/ui/public/large_button_02.sub",
					"down_image" : "d:/ymir work/ui/public/large_button_03.sub",					
				},
				
				{
					"name" : "oneHour",
					"type" : "radio_button",
					
					"x" : 125,
					"y" : 66,
					
					"default_image" : uiScriptLocale.LOCALE_OFFLINESHOP_PATH + "/new_checkbox_default.sub",
					"over_image" : uiScriptLocale.LOCALE_OFFLINESHOP_PATH + "/new_checkbox_over.sub",
					"down_image" : uiScriptLocale.LOCALE_OFFLINESHOP_PATH + "/new_checkbox_down.sub",
				},
				
				{
					"name" : "twoHours",
					"type" : "radio_button",
					
					"x" : 204,
					"y" : 66,
					
					"default_image" : uiScriptLocale.LOCALE_OFFLINESHOP_PATH + "/new_checkbox_default.sub",
					"over_image" : uiScriptLocale.LOCALE_OFFLINESHOP_PATH + "/new_checkbox_over.sub",
					"down_image" : uiScriptLocale.LOCALE_OFFLINESHOP_PATH + "/new_checkbox_down.sub",
				},

				{
					"name" : "fourHours",
					"type" : "radio_button",
					
					"x" : 289,
					"y" : 66,
					
					"default_image" : uiScriptLocale.LOCALE_OFFLINESHOP_PATH + "/new_checkbox_default.sub",
					"over_image" : uiScriptLocale.LOCALE_OFFLINESHOP_PATH + "/new_checkbox_over.sub",
					"down_image" : uiScriptLocale.LOCALE_OFFLINESHOP_PATH + "/new_checkbox_down.sub",
				},

				{
					"name" : "oneHourText",
					"type" : "text",
					
					"x" : 158,
					"y" : 66,
					
					"text" : "1 Saat",
				},
				
				{
					"name" : "twoHoursText",
					"type" : "text",
					
					"x" : 237,
					"y" : 66,
					
					"text" : "2 Saat",
				},
				
				{
					"name" : "fourHoursText",
					"type" : "text",
					
					"x" : 323,
					"y" : 66,
					
					"text" : "4 Saat",
				},
			),
		},
	),
}
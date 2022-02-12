import uiScriptLocale

ROOT = "d:/ymir work/ui/public/"
ROOT_PATH = "d:/ymir work/ui/game/akira_event/"

window = {
	"name" : "AkiraEventDialog",
	"style" : ("float",),
	
	"x" : SCREEN_WIDTH - 136 - 210,
	"y" : 78,
	
	"width" : 210,
	"height" : 120,
	
	"children" :
	(
		{
			"name" : "board",
			"type" : "thinboard",

			"x" : 0,
			"y" : 0,

			"width" : 210,
			"height" : 105,
			
			"children" :
			(
				{
					"name" : "BlackBoard",
					"type" : "thinboard_circle",
					"x" : 10, "y" : 10, "width" : 210-20, "height" : 105-20,
				},
				{
					"name" : "titleBar01",
					"type" : "image",
					"x" : 18,
					"y" : 20,
					"image" : ROOT_PATH+"racename_btn.sub",
					"children" :
					(
						{ "name" : "text01", "type" : "text", "text_horizontal_align":"center", "x" : 88, "y" : 5, "text" : uiScriptLocale.AKIRAEVENT_TEXT05, "color":0xFFFEE3AE },
					),
				},
				{
					"name" : "titleBar02",
					"type" : "image",
					"x" : 18,
					"y" : 40,
					"image" : ROOT_PATH+"racename_btn.sub",
					"children" :
					(
						{ "name" : "text02", "type" : "text", "text_horizontal_align":"center", "x" : 90, "y" : 5, "text" : uiScriptLocale.AKIRAEVENT_TEXT06, "color":0xFFFEE3AE },
					),
				},
				{
					"name" : "close_button",
					"type" : "button",
					
					"x" : 16,
					"y" : 55,
					
					"text" : uiScriptLocale.CLOSE,
					
					"default_image" : ROOT + "XLarge_Button_01.sub",
					"over_image" : ROOT + "XLarge_Button_02.sub",
					"down_image" : ROOT + "XLarge_Button_03.sub",
				},
			),
		},		
	),	
}

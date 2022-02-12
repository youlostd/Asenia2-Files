import uiScriptLocale
ROOT_PATH = "d:/ymir work/ui/public/"

window = {
	"name" : "NewPetWinow",
	"style" : ("movable", "float",),
	
	"x" : SCREEN_WIDTH - 200,
	"y" : SCREEN_HEIGHT - 595,	

	"width" : 460-200,
	"height" : 300+30-220,

	"children" :
	(
		{
			"name" : "board",
			"type" : "board",
			"style" : ("attach",),

			"x" : 0,
			"y" : 0,

			"width" : 460-200,
			"height" : 300+30-220,

			"children" :
			(
				{
					"name" : "TitleBarPet",
					"type" : "titlebar",
					"style" : ("attach",),
					"x" : 6,
					"y" : 7,
					"width" : 448-200,
					"color" : "gray",
					"children" :
					(
						{ "name":"TitleName", "type":"text", "x":0, "y":0, "text":"", "all_align":"center" },
					),
				},
								
				{
					"name" : "title2",
					"type" : "text",
					
					"x" : 61-40,
					"y" : 20+7+9+3,
					
					"text" : "Aranan Nesne: ",
				},
				
				{
					"name" : "InputSlot",
					"type" : "slotbar",

					"x" : 24-0,
					"y" : 20+7+9,

					"width" : 235-100,
					"height" : 20,
					
					"horizontal_align" : "center",
					
					"children" :
					(

						{
							"name" : "InputValue",
							"type" : "editline",

							"x" : 3,
							"y" : 3,

							"width" : 235-100,
							"height" : 20,

							"input_limit" : 32,
						},
					),
				},
				# {
					# "name" : "ScrollBar",
					# "type" : "scrollbar",
					# "x" : 10+4,
					# "y" : 20+15+3+15+10,
					# "size" : 220,
					# "horizontal_align" : "left",
				# },
				# {
					# "name" : "ListBox",
					# "type" : "listboxex",
					# "x" : 40,
					# "y" : 20+30+6+15+10,
					# "width" : 386,
					# "height" : 220,
					# "item_align" : 0,
				# },
				{
					"name" : "okButton",
					"type" : "button",

					"x" : 170+24-32-100-25,
					"y" : 280+18-230,
					"text" : "Kapat",
					"default_image" : ROOT_PATH + "Middle_Button_01.sub",
					"over_image" : ROOT_PATH + "Middle_Button_02.sub",
					"down_image" : ROOT_PATH + "Middle_Button_03.sub",
				},
				{
					"name" : "temizle",
					"type" : "button",

					"x" : 170+24+32-100-25,
					"y" : 280+18-230,
					"text" : "Temizle",
					"default_image" : ROOT_PATH + "Middle_Button_01.sub",
					"over_image" : ROOT_PATH + "Middle_Button_02.sub",
					"down_image" : ROOT_PATH + "Middle_Button_03.sub",
				},
				{
					"name" : "arama",
					"type" : "button",

					"x" : 170+24+32-100+32+10,
					"y" : 280+18-230,
					"text" : "Ara",
					"default_image" : ROOT_PATH + "Middle_Button_01.sub",
					"over_image" : ROOT_PATH + "Middle_Button_02.sub",
					"down_image" : ROOT_PATH + "Middle_Button_03.sub",
				},
				{
					"name" : "button3",
					"type" : "button",

					"x" : 434+2-150,
					"y" : 10,
					"tooltip_text" : uiScriptLocale.CLOSE, 
					"default_image" : "d:/ymir work/ui/public/close_button_01.sub",
					"over_image" : "d:/ymir work/ui/public/close_button_02.sub",
					"down_image" : "d:/ymir work/ui/public/close_button_03.sub",
				},
			),
		},
	),
}

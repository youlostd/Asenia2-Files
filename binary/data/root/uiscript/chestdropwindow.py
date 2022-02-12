import uiscriptlocale
BUTTON_ROOT = "d:/ymir work/ui/public/"
BUTTON_ROOTT = "d:/ymir work/ui/game/"
FELIX_PATH = "d:/ymir work/ui/"
ROOT_PATH2 = "d:/ymir work/ui/public/"

window = {
	"name" : "ChestDropWindow",

	"x" : 0,
	"y" : 0,

	"style" : ("movable", "float",),


	"width" : 675,
	"height" : 446,

	"children" :
	(
		{
			"name" : "board",
			"type" : "board",
			"style" : ("attach",),

			"x" : 0,
			"y" : 0,

			"width" : 675,
			"height" : 446,
		
			"children" :
			(
				## Title
				{
					"name" : "TitleBar",
					"type" : "titlebar",
					"style" : ("attach",),

					"x" : 6,
					"y" : 6,

					"width" : 675-15,
					"color" : "yellow",

					"children" :
					(
						{ "name":"TitleName", "type":"text", "x": 0, "y":-1, "text": "Sandık Aynası", "all_align":"center" },
					),
				},
				{
                    "name" : "header",
                    "type" : "image",
                    "image" : "chest_header.tga",
                    "x" : 8,
                    "y" : 31,
                },
				
				{ 
					"name" : "SlotBG", 
					"type" : "expanded_image", 
					"style" : ("attach",), 
					
                    "x" : 16,
                    "y" : 213,
					
					"image" : "d:/ymir work/ui/pet/Pet_Incu_slot_001.tga",
					
					"children" :
					(
						{
							"name" : "OpenItemSlot",
							"type" : "slot",
								
							"x" : 4,
							"y" : 4,
							"width" : 32,
							"height" : 32,
		
							"slot" : (
								{"index":0, "x":0, "y":0, "width":32, "height":32,},
							),
						},
					),
				},
				
				{
					"name" : "OpenCountController",
					"type" : "sliderbar",

					"x" : 65,
					"y" : 215,
				},	
									
				{
					"name" : "OpenChestButtonSingle",
					"type" : "button",

					"x" : 202,
					"y" : 189,
					# "text" : "|Eemoji/key_lclick|e",

					# "default_image" : BUTTON_ROOT + "middle_button_01.sub",
					"over_image" : BUTTON_ROOT + "middle_button_02.sub",
					# "down_image" : BUTTON_ROOT + "middle_button_03.sub",
				},
				
				{
					"name" : "OpenChestButtonMultiple",
					"type" : "button",

					"x" : 202,
					"y" : 216,
					# "text" : "|Eemoji/key_lclick|e",

					# "default_image" : BUTTON_ROOT + "middle_button_01.sub",
					# "over_image" : BUTTON_ROOT + "middle_button_02.sub",
					# "down_image" : BUTTON_ROOT + "middle_button_03.sub",
					},
{
					"name" : "OpenChestButtonMultiple1",
					"type" : "button",

					"x" : 202,
					"y" : 243,
					# "text" : "|Eemoji/key_lclick|e",
					
					# "default_image" : BUTTON_ROOT + "middle_button_01.sub",
					# "over_image" : BUTTON_ROOT + "middle_button_02.sub",
					# "down_image" : BUTTON_ROOT + "middle_button_03.sub",
				},
				{
					"name" : "prev_button", 
					"type" : "button",
					
					"x" : 460 - 50, 
					"y" : 225,

					"default_image" : ROOT_PATH2 + "small_Button_01.sub",
					"over_image" : ROOT_PATH2 + "small_Button_02.sub",
					"down_image" : ROOT_PATH2 + "small_Button_03.sub",

					"children" :
					(
						{
							"name" : "b",
							"type" : "text",
							"text" : "<<",
							"font_name" : "Arial : 12",
							"color" : 0xffffd200,
							"x" : 0, "y" : 0,
							"all_align" : "center",
						},
					),
				},
				
				{
					"name" : "CurrentPageBack",
					"type" : "thinboard_circle",
					
					"x" : 460, 
					"y" : 226,
					
					"width" : 31,
					"height" : 20,
					
					"children" :
					(
						{
							"name" : "CurrentPage",
							"type" : "text",
							
							"x" : 0,
							"y" : 0,
							
							"vertical_align" : "center",
							"horizontal_align" : "center",
							
							"text_vertical_align" : "center",
							"text_horizontal_align" : "center",

							"text" : "1",
						},
					),
				},
				
				{
					"name" : "next_button", 
					"type" : "button",

					"x" : 460 + 20 + 17, 
					"y" : 225,


					"default_image" : ROOT_PATH2 + "small_Button_01.sub",
					"over_image" : ROOT_PATH2 + "small_Button_02.sub",
					"down_image" : ROOT_PATH2 + "small_Button_03.sub",
					"children" :
					(
						{
							"name" : "b",
							"type" : "text",
							"text" : ">>",
							"font_name" : "Arial : 12",
							"color" : 0xffffd200,
							"x" : 0, "y" : 0,
							"all_align" : "center",
						},
					),
				},
			),
		},
	),
}


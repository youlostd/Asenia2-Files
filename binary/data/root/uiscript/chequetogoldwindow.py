import uiScriptLocale

BUTTON_ROOT = "d:/ymir work/ui/public/"

window = {
	"name" : "ChequeTicketWindow",
	
	"style" : ("movable", "float", ),
	
	"x" : 0,
	"y" : 0,
	
	"width" 	: 200,
	"height"	: 140,
	
	"children" : 
	(
		{
			"name" 	: "main_board",
			"type"	: "board_with_titlebar",
			
			"style" : ("attach",),
			
			"x"		: 0,
			"y"		: 0,
			
			"width"	: 200,
			"height" : 140,
			
			"title" : "Won Bozdur",
			
			"children" : 
			(
				{
					"name" : "missionback",
					"type" : "image",
					"x": 10,
					"y": 33,
					"image" : "system/chequearka.png",
				},
				{
					"name" : "InputSlot",
					"type" : "slotbar",
					
					"x" : 35,
					"y" : 45,
					
					"width" : 34,
					"height" : 20,
					
					"children" : 
					(
						{
							"name" : "InputValue",
							"type" : "editline",
							
							"x" : 20,
							"y" : 4,
							
							"width" : 0,
							"height" : 0,
							
							"text" : "1",
							"input_limit" : 1,
							"only_number" : 1,
						},
						{
							"name" : "ChequeIcon",
							"type" : "image",
							
							"x" : 2,
							"y" : 3,
							
							"image" : "d:/ymir work/ui/game/windows/cheque_icon.sub",
						},
					),
				},
				{
					"name" : "text",
					"type" : "text",
					
					"x" : 35+44,
					"y" : 47,
					
					"text" : "=",
				},
				{
					"name" : "InputSlot2",
					"type" : "slotbar",
					
					"x" : 95,
					"y" : 45,
					
					"width" : 70,
					"height" : 20,
					
					"children" : 
					(
						{
							"name" : "InputValue2",
							"type" : "editline",
							
							"x" : 20,
							"y" : 4,
							
							"width" : 0,
							"height" : 0,
							
							"text" : "100000000",
							"input_limit" : 9,
							"only_number" : 1,
						},
						{
							"name" : "MoneyIcon",
							"type" : "image",
							
							"x" : 2,
							"y" : 3,
							
							"image" : "d:/ymir work/ui/game/windows/money_icon.sub",
						},
					),
				},
				{
					"name" : "Lbutton",
					"type" : "button",
					"x" : 30+40,
					"y" : 80-4,
					
					"default_image" : BUTTON_ROOT+"AcceptButton00.sub",
					"over_image" : BUTTON_ROOT+"AcceptButton01.sub",
					"down_image" : BUTTON_ROOT+"AcceptButton02.sub",
				},
				{
					"name" : "image_1",
					"type" : "image",

					"x" : 2,
					"y" : 110,

					"image" : "system/won1.png",
				},
				{
					"name" : "goldtochequewindowBtn",
					"type" : "button",

					"x" : 20+30,
					"y" : 116,

					"default_image" : "system/saydam.png",
					"over_image" : "system/saydam.png",
					"down_image" : "system/saydam.png",
				},
			),
		},
	),
}
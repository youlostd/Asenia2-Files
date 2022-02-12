import uiScriptLocale
import localeInfo

window = {
	"name" : "Battlepass",

	"x" : 0,
	"y" : 0,

	"style" : ("movable", "float",),

	"width" : 537,
	"height" : 297,

	"children" :
	(
		## Board and buttons
		{
			"name" : "board",
			"type" : "board",
			"style" : ("attach",),

			"x" : 0,
			"y" : 0,

			"width" : 537,
			"height" : 297,

			"children" :
			(
				{
					"name" : "board_misiuni","type" : "image","style" : ("attach",),"x" : 10,"y" : 30,"image" : "d:/ymir work/battle_pass/mission_board.tga"
				},
				{
					"name" : "ScrollBar","type" : "scrollbar","x" : 303,"y" : 33,"size" : 253,
				},
				{
					"name" : "DesignTop","type" : "image","style" : ("attach",),"x" : 299+18, "y" : 30,"image" : "d:/ymir work/battle_pass/info_bg.tga",	
				},
				{
					"name" : "BpassStatus", "type" : "text", "x" : 370+10, "y" : 35, "text" : "G�rev Bilgileri", 
				},
				{
					"name" : "Text1Info","type" : "text","x" : 320+3,"y" : 54,"text" : "- G�rev Ad�: ",
				},
				{
					"name" : "Text2Info","type" : "text","x" : 320+3,"y" : 54+20,"text" : "- Durumu: ",
				},
				{
					"name" : "Text3Info","type" : "text","x" : 320+3,"y" : 74+20,"text" : "- �lerleme: ",
				},
				{
					"name" : "Text4Info","type" : "text","x" : 378,"y" : 94+20,"text" : "|cffffff00G�rev Tan�m�",
				},
				{
					"name" : "Text5Info","type" : "text","x" : 320+5,"y" : 114+20,"text" : "",
				},
				{
					"name" : "Text6Info","type" : "text","x" : 320+5,"y" : 134 + 20,"text" : "",
				},
				{
					"name" : "Text7Info","type" : "text","x" : 378,"y" : 154 + 24,"text" : "|cffffff00Son �d�l",
				},
				{
					"name" : "FinalReward","type" : "button","x" : 330,"y" : 254,"default_image" : "d:/ymir work/battle_pass/reward_normal.tga","over_image" : "d:/ymir work/battle_pass/reward_over.tga","down_image" : "d:/ymir work/battle_pass/reward_down.tga",
				},
				## TitleBar
				{"name" : "TitleBar","type" : "titlebar","style" : ("attach",),"x" : 8,"y" : 8,"width" : 537 - 15,"color" : "gray",
					"children" :
					(
						{ 
							"name":"TitleName", "type":"text", "x" : (537 - 15) / 2, "y":4, "text":"Sava� Bileti", "text_horizontal_align":"center" 
						},
					),
				},
				
			),
		},
	),
}
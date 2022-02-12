import uiScriptLocale

BOARD_WIDTH = 190
BOARD_HEIGHT = 78
LEFT = 11

window = {
	"name" : "CostumEffectDialog",
	"style" : ("movable", "float", "ltr"),

	"x" : (SCREEN_WIDTH/2) - (190/2),
	"y" : (SCREEN_HEIGHT/2) - 100,

	"width" : BOARD_WIDTH,
	"height" : BOARD_HEIGHT,

	"children" :
	(
		{
			"name" : "CostumEffectBoard",
			"type" : "board",
			"style" : ("attach", "ltr"),

			"x" : 0,
			"y" : 0,

			"width" : BOARD_WIDTH,
			"height" : BOARD_HEIGHT,
			"horizontal_align" : "center",

			"children" :
			(
				## Title Bar
				{
					"name" : "CostumEffectTitle",
					"type" : "titlebar",
					"style" : ("attach",),

					"x" : 6, "y" : 7, "width" : BOARD_WIDTH - 13,

					"children" :
					(
						{ "name":"TitleName", "type":"text", "x":BOARD_WIDTH/2-5, "y":3, "text":uiScriptLocale.ADD_EFFECT_TITLE, "text_horizontal_align":"center" },
					),
				},
				{
					"name" : "BlackBoard",
					"type" : "thinboard_circle",
					"x" : 13, "y" : 36, "width" : 163, "height" : BOARD_HEIGHT-73,
				},
				{
					"name" : "AcceptButton",
					"type" : "button",
					"text" : uiScriptLocale.ADD_EFFECT_OK,
					"x" : LEFT+104,
					"y" : BOARD_HEIGHT-28,
					"default_image" : "d:/ymir work/ui/public/large_button_01.sub",
					"over_image" : "d:/ymir work/ui/public/large_button_02.sub",
					"down_image" : "d:/ymir work/ui/public/large_button_03.sub",
				},
			),
		},
	),
}


import uiScriptLocale

BOARD_WIDTH = 300
BOARD_HEIGHT = 125+(2*32)
ITEM_SLOT_Y = 47
ROOT = "d:/ymir work/ui/public/"

window = {
	"name" : "AkiraEventInfoWindow",

	"x" : 0,
	"y" : 0,

	"style" : ("movable", "float",),

	"width" : BOARD_WIDTH,
	"height" : BOARD_HEIGHT,

	"children" :
	(
		{
			"name" : "board",
			"type" : "board",
			"style" : ("attach",),

			"x" : 0,
			"y" : 0,

			"width" : BOARD_WIDTH,
			"height" : BOARD_HEIGHT,
		
			"children" :
			(
				## Title
				{
					"name" : "TitleBar",
					"type" : "titlebar",
					"style" : ("attach",),

					"x" : 6,
					"y" : 6,

					"width" : BOARD_WIDTH-13,
					"color" : "yellow",

					"children" :
					(
						{ "name":"TitleName", "type":"text", "x":BOARD_WIDTH/2, "y":3, "text":"", "text_horizontal_align":"center" },
					),
				},
				{
					"name" : "BlackBoard",
					"type" : "thinboard_circle",
					"x" : 13, "y" : 36, "width" : BOARD_WIDTH-28, "height" : BOARD_HEIGHT-48,
				},
				{
					"name" : "ItemGrid", "type" : "grid_table",
					"x" : BOARD_WIDTH/2-16,
					"y" : ITEM_SLOT_Y,
					"start_index" : 0,
					"x_count" : 1,
					"y_count" : 3,
					"x_step" : 32,
					"y_step" : 32,
					"x_blank" : 0,
					"y_blank" : 0,
					"image" : ROOT+"Slot_Base.sub",
				},
				{
					"name" : "InfoImage",
					"type" : "image",
					"x" : BOARD_WIDTH/2-60,
					"y" : ITEM_SLOT_Y+40+(2*32),
					"image" : "d:/ymir work/ui/game/akira_event/name_img.sub",
					"children" :
					(
						{ "name" : "InfoText", "type" : "text", "text_horizontal_align":"center", "x" : 60, "y" : 5, "text" : uiScriptLocale.AKIRAEVENT_TEXT07, "color":0xFFFEE3AE },
					),
				},
			),
		},
	),
}


import uiScriptLocale

BOARD_WIDTH = 392
BOARD_HEIGHT = 65+7*24+18
PAGE_BUTTON = BOARD_WIDTH/2
ROOT_PATH = "d:/ymir work/ui/game/akira_event/"

window = {
	"name" : "AkiraAutoEventWindow",

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
						{ "name":"TitleName", "type":"text", "x":BOARD_WIDTH/2, "y":3, "text": uiScriptLocale.AKIRAEVENT_WINDOWTITLE01, "text_horizontal_align":"center" },
					),
				},
				{
					"name" : "BlackBoard",
					"type" : "thinboard_circle",
					"x" : 13, "y" : 36, "width" : BOARD_WIDTH-28, "height" : BOARD_HEIGHT-48,
				},
				{
					"name" : "AkiraAutoEventTitleBar",
					"type" : "image",
					"x" : 20,
					"y" : 41,
					"image" : ROOT_PATH+"akira_event_list_menu.tga",
					"children" :
					(
						## Text
						{ "name" : "Day", "type" : "text", "x" : 35, "y" : 4,  "text" : uiScriptLocale.AKIRAEVENT_TEXT01, },
						{ "name" : "EventName", "type" : "text", "x" : 145, "y" : 4, "text" : uiScriptLocale.AKIRAEVENT_TEXT02, },
						{ "name" : "EventStartTime", "type" : "text", "x" : 232, "y" : 4, "text" : uiScriptLocale.AKIRAEVENT_TEXT03, },
						{ "name" : "EventFinish", "type" : "text", "x" : 297, "y" : 4, "text" : uiScriptLocale.AKIRAEVENT_TEXT04, },
					),
				},
			),
		},
	),
}


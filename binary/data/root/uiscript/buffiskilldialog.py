import uiScriptLocale

MAX_SKILL_COUNT = 3
BOARD_WIDTH = 42*MAX_SKILL_COUNT+30
BOARD_HEIGHT = 93

window = {
	"name" : "AssistantSkillDialog",

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
						{ "name":"TitleName", "type":"text", "x":BOARD_WIDTH/2-10, "y":4, "text": "Buffi Becerileri", "text_horizontal_align":"center" },
					),
				},
				{
					"name" : "skillSlot", "type" : "grid_table", "x" : BOARD_WIDTH/2-53, "y" : 44, "start_index" : 0,
					"x_count" : MAX_SKILL_COUNT, "y_count" : 1, "x_step" : 32, "y_step" : 32, "x_blank" : 5, "y_blank" : 4,
					"image" : "d:/ymir work/ui/Public/Slot_Base.sub", "horizontal_align" : "center",
				},
			),
		},
	),
}
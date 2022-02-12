import uiScriptLocale
import item

BG_WIDTH_WINDOW = 585

window = {
	"name" : "ThanosWindow",

	"x" : SCREEN_WIDTH - 175 - 295,
	"y" : SCREEN_HEIGHT - 37 - 575,

	"style" : ("movable", "float",),

	"width" : BG_WIDTH_WINDOW,
	"height" : BG_WIDTH_WINDOW,

	"children" :
	(
		{
			"name" : "board",
			"type" : "image",
			"style" : ("movable","attach",),

			"x" : 0,
			"y" : 0,
			"image" : "d:/ymir work/ui/bg_thanos.png",

			"width" : BG_WIDTH_WINDOW,
			"height" : BG_WIDTH_WINDOW,

			"children" :
			(
				{
					"name" : "ThanosSlot",
					"type" : "slot",

					"x" : 3,
					"y" : 3,

					"width" : BG_WIDTH_WINDOW,
					"height" : BG_WIDTH_WINDOW,

					"slot" : (
								{"index":item.EQUIPMENT_THANOS, "x":118, "y":270, "width":32, "height":32},
								{"index":item.EQUIPMENT_THANOS_RED, "x":451, "y":270, "width":32, "height":32},
								{"index":item.EQUIPMENT_THANOS_BLUE, "x":284, "y":270, "width":32, "height":32},
								{"index":item.EQUIPMENT_THANOS_GREEN, "x":115, "y":560, "width":32, "height":32},
								{"index":item.EQUIPMENT_THANOS_YELLOW, "x":451, "y":560, "width":32, "height":32},
								{"index":item.EQUIPMENT_THANOS_BLACK, "x":284, "y":560, "width":32, "height":32},
							),
				},
				{
					"name" : "TitleBar",
					"type" : "titlebar",
					"style" : ("movable","attach",),
				
					"x" : 30,
					"y" : 20,
				
					"width" : BG_WIDTH_WINDOW-50,
					"color" : "yellow",
				
					"children" :
					(
						{
							"name" : "TitleName",
							"type" : "text",
			
							"x" : 0,
							"y" : 5,
			
							"horizontal_align" : "center",
			
							"text" : "",
			
							"text_horizontal_align" : "center",
							"color" : 0xFFFEE3AE,
						},
					),
				},
			),
		},
	),
}


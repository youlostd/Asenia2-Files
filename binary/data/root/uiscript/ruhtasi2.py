import uiScriptLocale

ROOT_PATH = "d:/ymir work/ui/public/"

window = {
	"name" : "RuhTasiDialog",
	"style" : ("movable", "float",),

	"x" : 0,
	"y" : 0,

	"width" : 347,
	"height" : 195*2 - 15,

	"children" :
	(
		{
			"name" : "board",
			"type" : "board",

			"x" : 0,
			"y" : 0,

			"width" : 347,
			"height" : 195*2 - 15,

			"children" :
			(
				## Title
				{
					"name" : "titlebar",
					"type" : "titlebar",
					"style" : ("attach",),

					"x" : 8,
					"y" : 8,

					"width" : 347 - 16,
					"color" : "gray",

					"children" :
					(
						{ "name":"titlename", "type":"text", "x":0, "y":3, 
						"text" : "H�zl� ruh ta�� okuma penceresi", 
						"horizontal_align":"center", "text_horizontal_align":"center" },
					),
				},
				{
					"name" : "background",
					"type" : "image",

					"x" : 4,
					"y" : 35,

					"image" : "d:/ymir work/ui/public/hidragame/oku2.png",
				},
				{
					"name" : "derece2_circle",
					"type" : "window",

					"x" : 55,
					"y" : 101,
					"width" : 180+48,
                    "height" : 20+15,
                    "children" :
                    (
                        {
                            "name" : "derece2",
                            "type" : "text",

                            "x" : 20,
                            "y" : 10,

                            "text" : "",

                        },
                        {
                            "name" : "derece",
                            "type" : "text",

                            "x" : 140,
                            "y" : 10,

                            "text" : "Zalim",

                        },
                    ),

				},
				

				{
					"name" : "ruhslot",
					"type" : "grid_table",

					"start_index" : 0,

					"x" : 160,
					"y" : 144,

					"x_count" : 1,
					"y_count" : 1,
					"x_step" : 32,
					"y_step" : 32,
					"x_blank" : 0,
					"y_blank" : 0,

					"image" : "d:/ymir work/ui/public/slot_base.sub",
				},	

				{
					"name" : "zenslot",
					"type" : "grid_table",

					"start_index" : 1,

					"x" : 115,
					"y" : 189,

					"x_count" : 1,
					"y_count" : 1,
					"x_step" : 32,
					"y_step" : 32,
					"x_blank" : 0,
					"y_blank" : 0,

					"image" : "d:/ymir work/ui/public/slot_base.sub",
				},
				{
					"name" : "kotuslot",
					"type" : "grid_table",

					"start_index" : 2,

					"x" : 160,
					"y" : 189,

					"x_count" : 1,
					"y_count" : 1,
					"x_step" : 32,
					"y_step" : 32,
					"x_blank" : 0,
					"y_blank" : 0,

					"image" : "d:/ymir work/ui/public/slot_base.sub",
				},	
				{
					"name" : "munzevislot",
					"type" : "grid_table",

					"start_index" : 3,

					"x" : 205,
					"y" : 189,

					"x_count" : 1,
					"y_count" : 1,
					"x_step" : 32,
					"y_step" : 32,
					"x_blank" : 0,
					"y_blank" : 0,

					"image" : "d:/ymir work/ui/public/slot_base.sub",
				},	

				
				{
					"name" : "skillslot",
					"type" : "slot",

					"x" : 64-6,
					"y" : 280,

					"width" : 223,
					"height" : 223,
					"image" : "d:/ymir work/ui/public/slot_base.sub",

					"slot" :	(
									{"index": 0, "x": 0, "y":  0, "width":35, "height":35},
									{"index": 1, "x": 38+1, "y":  0, "width":35, "height":35},
									{"index": 2, "x": 38*2 + 2, "y":  0, "width":35, "height":35},
									{"index": 3, "x": 38*3 + 3, "y":  0, "width":35, "height":35},
									{"index": 4, "x": 38*4+3 , "y":  0, "width":35, "height":35},
									{"index": 5, "x": 38*5+5, "y":  0, "width":35, "height":35},

							),
				},
				
				{
					"name" : "tek",
					"type" : "button",

					"x" : 64-6,
					"y" : 320,
					
					"text"	:	"Oku",

					"default_image" : ROOT_PATH + "large_button_01.sub",
					"over_image" : ROOT_PATH + "large_button_02.sub",
					"down_image" : ROOT_PATH + "large_button_03.sub",
				},	

				{
					"name" : "hepsi",
					"type" : "button",

					"x" : 64-6 + (38*3 + 3) + 29,
					"y" : 320,
					
					"text"	:	"Hepsini Oku",

					"default_image" : ROOT_PATH + "large_button_01.sub",
					"over_image" : ROOT_PATH + "large_button_02.sub",
					"down_image" : ROOT_PATH + "large_button_03.sub",
				},				
								
				
				
				
			),
		},
	),
}

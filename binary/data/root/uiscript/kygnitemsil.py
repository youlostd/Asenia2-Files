
window = {
	"name" : "KygnRemoveORSell",
	"style" : ("movable", "float",),

	"x" : SCREEN_WIDTH / 2 - 170,
	"y" : SCREEN_HEIGHT / 2 - 133,

	"width" : 339,
	"height" : 222 + 45,


	"children" :
	(
		{
			"name" : "board",
			"type" : "board",

			"x" : 0,
			"y" : 0,

			"width" : 339,
			"height" : 222 + 45,

			"children" :
			(
				{
					"name" : "titlebar",
					"type" : "titlebar",
					"style" : ("attach",),

					"x" : 5,
					"y" : 6,

					"width" : 339-12,
					"color" : "gray",

					"children" :
					(
						{ "name":"titlename", "type":"text", "x":0, "y":3, 
						"text" : "Hýzlý Sil-Sat", 
						"horizontal_align":"center", "text_horizontal_align":"center" },
					),
				},

				{
					"name" : "itemslot",
					"type" : "grid_table",
					"x" : 8,
					"y" : 30,
					"start_index" : 0,
					"x_count" : 10,
					"y_count" : 6,
					"x_step" : 32,
					"y_step" : 32,
					"image" : "d:/ymir work/ui/public/Slot_Base.sub"
				},

				{
					"name" : "ItemSilmeButonu",
					"type" : "button",

					"x" : 10,
					"y" : 222 + 10,

					"text": "Itemleri Sil",

					"default_image" : "d:/ymir work/ui/button/offical_button.tga",
					"over_image" : "d:/ymir work/ui/button/offical_button_bastim.tga",
					"down_image" : "d:/ymir work/ui/button/offical_button.tga",
				},

				{
					"name" : "ItemSatmaButonu",
					"type" : "button",

					"x" : 181,
					"y" : 222 + 10,

					"text": "Itemleri Sat",

					"default_image" : "d:/ymir work/ui/button/offical_button.tga",
					"over_image" : "d:/ymir work/ui/button/offical_button_bastim.tga",
					"down_image" : "d:/ymir work/ui/button/offical_button.tga",
				},

			),
		},
	),
}

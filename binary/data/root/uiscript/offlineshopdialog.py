import uiScriptLocale

window = {
	"name" : "OfflineShopDialog",

	"x" : SCREEN_WIDTH - 400,
	"y" : 10,

	"style" : ("movable", "float",),

	"width" : 225+445,
	"height" : 328 + 26+50,

	"children" :
	(
		{
			"name" : "board",
			"type" : "board",
			"style" : ("attach",),

			"x" : 0,
			"y" : 0,

			"width" : 225+445,
			"height" : 328 + 26+50,

			"children" :
			(
				## Title
				{
					"name" : "TitleBar",
					"type" : "titlebar",
					"style" : ("attach",),

					"x" : 8,
					"y" : 8,

					"width" : 169+492,
					"color" : "gray",

					"children" :
					(
						{ "name":"TitleName", "type":"text", "x":300, "y":4, "text":uiScriptLocale.PRIVATE_SHOP_TITLE, "text_horizontal_align":"center" },
					),
				},

				## Item Slot
				{
					"name" : "ItemSlot",
					"type" : "grid_table",

					"x" : 12,
					"y" : 10 + 26,

					"start_index" : 0,
					"x_count" : 20,
					"y_count" : 10,
					"x_step" : 32,
					"y_step" : 32,

					"image" : "d:/ymir work/ui/public/Slot_Base.sub",
				},

				## Buy
				{
					"name" : "BuyButton",
					"type" : "toggle_button",

					"x" : 296,
					"y" : 295+50+23,
	
					"text" : uiScriptLocale.SHOP_BUY,

					"default_image" : "d:/ymir work/ui/public/large_button_01.sub",
					"over_image" : "d:/ymir work/ui/public/large_button_02.sub",
					"down_image" : "d:/ymir work/ui/public/large_button_03.sub",
				},
			),
		},
	),
}
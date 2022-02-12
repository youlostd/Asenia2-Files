import uiScriptLocale

window = {
	"name" : "ShopDialog",

	"x" : SCREEN_WIDTH - 400 -200,
	"y" : 10,

	"style" : ("movable", "float",),

	"width" : 345,
	"height" : 390,

	"children" :
	(

		{
			"name" : "board",
			"type" : "board",
			"style" : ("attach",),

			"x" : 0,
			"y" : 0,

			"width" : 345,
			"height" : 390,

			"children" :
			(
				## Title
				{
					"name" : "TitleBar",
					"type" : "titlebar",
					"style" : ("attach",),

					"x" : 8,
					"y" : 8,

					"width" : 328,
					"color" : "gray",

					"children" :
					(
						{ "name":"TitleName", "type":"text", "x":164, "y":4, "text":uiScriptLocale.SHOP_TITLE, "text_horizontal_align":"center" },
					),
				},

				## Item Slot
				{
					"name" : "ItemSlot",
					"type" : "grid_table",

					"x" : 12,
					"y" : 34,

					"start_index" : 0,
					"x_count" : 10,
					"y_count" : 10,
					"x_step" : 32,
					"y_step" : 32,

					"image" : "d:/ymir work/ui/public/Slot_Base.sub",
				},


				## Buy
				{
					"name" : "BuyButton",
					"type" : "toggle_button",

					"x" : 100,
					"y" : 360,

					"width" : 61,
					"height" : 21,

					"text" : uiScriptLocale.SHOP_BUY,

					"default_image" : "d:/ymir work/ui/public/middle_button_01.sub",
					"over_image" : "d:/ymir work/ui/public/middle_button_02.sub",
					"down_image" : "d:/ymir work/ui/public/middle_button_03.sub",
				},

				## Sell
				{
					"name" : "SellButton",
					"type" : "toggle_button",

					"x" : 183,
					"y" : 360,

					"width" : 61,
					"height" : 21,

					"text" : uiScriptLocale.SHOP_SELL,

					"default_image" : "d:/ymir work/ui/public/middle_button_01.sub",
					"over_image" : "d:/ymir work/ui/public/middle_button_02.sub",
					"down_image" : "d:/ymir work/ui/public/middle_button_03.sub",
				},

				## Close
				{
					"name" : "CloseButton",
					"type" : "button",

					"x" : 200,
					"y" : 360,

					"width" : 61,
					"height" : 21,
					
					#"horizontal_align" : "center",

					"text" : uiScriptLocale.PRIVATE_SHOP_CLOSE_BUTTON,

					"default_image" : "d:/ymir work/ui/public/large_button_01.sub",
					"over_image" : "d:/ymir work/ui/public/large_button_02.sub",
					"down_image" : "d:/ymir work/ui/public/large_button_03.sub",
				},

				## MiddleTab1
                {
                    "name" : "MiddleTab1",
                    "type" : "radio_button",

                    "x" : 21,
                    "y" : 295,

                    "width" : 61,
                    "height" : 21,

                    "default_image" : "d:/ymir work/ui/public/middle_button_01.sub",
                    "over_image" : "d:/ymir work/ui/public/middle_button_02.sub",
                    "down_image" : "d:/ymir work/ui/public/middle_button_03.sub",
                },

                ## MiddleTab2
                {
                    "name" : "MiddleTab2",
                    "type" : "radio_button",

                    "x" : 104,
                    "y" : 295,

                    "width" : 61,
                    "height" : 21,

                    "default_image" : "d:/ymir work/ui/public/middle_button_01.sub",
                    "over_image" : "d:/ymir work/ui/public/middle_button_02.sub",
                    "down_image" : "d:/ymir work/ui/public/middle_button_03.sub",
                },

                ## SmallTab1
                {
                    "name" : "SmallTab1",
                    "type" : "radio_button",

                    "x" : 21,
                    "y" : 295,

                    "width" : 43,
                    "height" : 21,

                    "default_image" : "d:/ymir work/ui/public/small_button_01.sub",
                    "over_image" : "d:/ymir work/ui/public/small_button_02.sub",
                    "down_image" : "d:/ymir work/ui/public/small_button_03.sub",
                },

                ## SmallTab2
                {
                    "name" : "SmallTab2",
                    "type" : "radio_button",

                    "x" : 71,
                    "y" : 295,

                    "width" : 43,
                    "height" : 21,

                    "default_image" : "d:/ymir work/ui/public/small_button_01.sub",
                    "over_image" : "d:/ymir work/ui/public/small_button_02.sub",
                    "down_image" : "d:/ymir work/ui/public/small_button_03.sub",
                },

                ## SmallTab3
                {
                    "name" : "SmallTab3",
                    "type" : "radio_button",

                    "x" : 120,
                    "y" : 295,

                    "width" : 43,
                    "height" : 21,

                    "default_image" : "d:/ymir work/ui/public/small_button_01.sub",
                    "over_image" : "d:/ymir work/ui/public/small_button_02.sub",
                    "down_image" : "d:/ymir work/ui/public/small_button_03.sub",
                },

			),
		},
	),
}
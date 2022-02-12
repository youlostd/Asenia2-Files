ROOT = "d:/ymir work/ui/minimap/new/"

window = {
	"name" : "MiniMap",

	"x" : SCREEN_WIDTH - 136,
	"y" : 0,

	"width" : 136,
	"height" : 137,

	"children" :
	(
		## OpenWindow
		{
			"name" : "OpenWindow",
			"type" : "window",

			"x" : 0,
			"y" : 0,

			"width" : 136,
			"height" : 137,

			"children" :
			(
				{
					"name" : "OpenWindowBGI",
					"type" : "image",
					"x" : -13,
					"y" : -8,
					"image" : ROOT + "minimap.png",
				},
				## MiniMapWindow
				{
					"name" : "MiniMapWindow",
					"type" : "window",

					"x" : 4,
					"y" : 5,

					"width" : 128,
					"height" : 128,
				},
				## ScaleUpButton
				{
					"name" : "ScaleUpButton",
					"type" : "button",

					"x" : 95,
					"y" : 110,

					"default_image" : ROOT + "plus_normal.png",
					"over_image" : ROOT + "plus_hover.png",
					"down_image" : ROOT + "plus_normal.png",
				},
				## ScaleDownButton
				{
					"name" : "ScaleDownButton",
					"type" : "button",

					"x" : 111,
					"y" : 95,

					"default_image" : ROOT + "minus_normal.png",
					"over_image" : ROOT + "minus_hover.png",
					"down_image" : ROOT + "minus_normal.png",
				},
				## MiniMapHideButton
				{
					"name" : "MiniMapHideButton",
					"type" : "button",

					"x" : 100,
					"y" : 7,

					"default_image" : ROOT + "btn_cl_normal.png",
					"over_image" : ROOT + "btn_cl_hover.png",
					"down_image" : ROOT + "btn_cl_normal.png",
				},
				##Sosyalmedyabtuon
				{
					"name" : "Facebook",
					"type" : "button",


					"x" : -3,
					"y" : 50,


					"default_image" : "d:/ymir work/ui/Sosyal/minimap_face_01.png",
					"over_image" : "d:/ymir work/ui/Sosyal/minimap_face_02.png",
					"down_image" : "d:/ymir work/ui/Sosyal/minimap_face_03.png",
				},
				{
					"name" : "Twitter",
					"type" : "button",


					"x" : 3,
					"y" : 83,


					"default_image" : "d:/ymir work/ui/Sosyal/minimap_discord_01.png",
					"over_image" : "d:/ymir work/ui/Sosyal/minimap_discord_01.png",
					"down_image" : "d:/ymir work/ui/Sosyal/minimap_discord_01.png",
				},
				{
					"name" : "Youtube",
					"type" : "button",


					"x" : 23,
					"y" : 106,


					"default_image" : "d:/ymir work/ui/Sosyal/minimap_youtube_01.png",
					"over_image" : "d:/ymir work/ui/Sosyal/minimap_youtube_01.png",
					"down_image" : "d:/ymir work/ui/Sosyal/minimap_youtube_01.png",
				},
				{
					"name" : "Site",
					"type" : "button",


					"x" : 55,
					"y" : 115,


					"default_image" : "d:/ymir work/ui/Sosyal/minimap_home_01.png",
					"over_image" : "d:/ymir work/ui/Sosyal/minimap_home_01.png",
				"down_image" : "d:/ymir work/ui/Sosyal/minimap_home_01.png",
				},
				## AtlasShowButton
				{
					"name" : "AtlasShowButton",
					"type" : "button",

					"x" : 15,
					"y" : 12,

					"default_image" : ROOT + "btn_m_normal.png",
					"over_image" : ROOT + "btn_m_hover.png",
					"down_image" : ROOT + "btn_m_down.png",
				},
				## ServerInfo
				{
					"name" : "ServerInfo",
					"type" : "text",
					
					"text_horizontal_align" : "center",

					"outline" : 1,

					"x" : 70,
					"y" : 140,

					"text" : "",
				},
				## PositionInfo
				{
					"name" : "PositionInfo",
					"type" : "text",
					
					"text_horizontal_align" : "center",

					"outline" : 1,

					"x" : 70,
					"y" : 160,

					"text" : "",
				},
				{
					"name" : "dungeontimewindow",
					"type" : "thinboard",
					


					"x" : 15,
					"y" : 200,
					
					"width" : 110,
					"height" : 48,
					
					"children" :
					(
						{
							"name" : "kalansure",
							"type" : "text",

							"x" : 32,
							"y" : 10,
							

							"text" : "Kalan Süre",
						},
						
						{
							"name" : "kalanzaman",
							"type" : "text",

							"x" : 38,
							"y" : 25,
							

							"text" : "00:00:00",
						},
					),

				},
				{
					"name" : "hydradirekcan",
					"type" : "thinboard",
					


					"x" : 15,
					"y" : 250+20,
					
					"width" : 110,
					"height" : 48,
					
					"children" :
					(
						{
							"name" : "kalancann",
							"type" : "text",

							"x" : 15,
							"y" : 10,
							

							"text" : "Gemi Direði : %100",
						},
					),

				},
				## ObserverCount
				{
					"name" : "ObserverCount",
					"type" : "text",
					
					"text_horizontal_align" : "center",

					"outline" : 1,

					"x" : 70,
					"y" : 180,

					"text" : "",
				},

				##Ping
				{
					"name" : "pingBackground",
					"type" : "image",
					"x" : 29,
					"y" : 143,
					"image" : ROOT + "ping_time/0.png",
				},

				{
					"name" : "pingPicture_1",
					"type" : "image",
					"x" : 41,
					"y" : 170,
					"image" : ROOT + "ping_time/1.png",
				},

				{
					"name" : "pingPicture_2",
					"type" : "image",
					"x" : 41,
					"y" : 170,
					"image" : ROOT + "ping_time/2.png",
				},

				{
					"name" : "pingPicture_3",
					"type" : "image",
					"x" : 41,
					"y" : 170,
					"image" : ROOT + "ping_time/3.png",
				},

				{
					"name" : "pingPicture_4",
					"type" : "image",
					"x" : 41,
					"y" : 170,
					"image" : ROOT + "ping_time/4.png",
				},

				{
					"name" : "pingPicture_5",
					"type" : "image",
					"x" : 41,
					"y" : 170,
					"image" : ROOT + "ping_time/5.png",
				},
			),
		},
		{
			"name" : "CloseWindow",
			"type" : "window",

			"x" : 0,
			"y" : 0,

			"width" : 132,
			"height" : 48,

			"children" :
			(
				## ShowButton
				{
					"name" : "MiniMapShowButton",
					"type" : "button",

					"x" : 100,
					"y" : 4,

					"default_image" : ROOT + "minimap_open_default.sub",
					"over_image" : ROOT + "minimap_open_default.sub",
					"down_image" : ROOT + "minimap_open_default.sub",
				},
			),
		},
	),
}

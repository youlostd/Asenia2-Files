import uiScriptLocale

INTERFACE_PATH = "d:/ymir work/ui/intro/loading/"

window = {

	"x" : 0,
	"y" : 0,

	"width" : SCREEN_WIDTH,
	"height" : SCREEN_HEIGHT,

	"children" :
	(
		## Board
		{
			"name" : "BackGround",
			"type" : "expanded_image",

			"x" : 0,
			"y" : 0,

			"image" : "d:/ymir work/ui/intro/pattern/Line_Pattern.tga",

			"x_scale" : float(SCREEN_WIDTH) / 1920.0,
			"y_scale" : float(SCREEN_HEIGHT) / 1080.0,
		},
		{ 
			"name":"ErrorMessage", 
			"type":"text", "x":10, "y":10, 
			"text": uiScriptLocale.LOAD_ERROR, 
		},
		# {
			# "name" : "InfoMess",
			# "type" : "expanded_image",

			# "x" : float(SCREEN_WIDTH) / 5  - 100/3,
			# "y" : 650,

			# "image" : "d:/ymir work/ui/help_gui.png",
			# "children" :
			# (
				# {
					# "name" : "LoadingTip",
					# "type" : "text",

					# "x"	: 565,
					# "y" : 60,

					# "fontsize" : "LARGE",
					# "color" : 0xff54D2FF,
					# "text_horizontal_align" : "center",
				# },
			# ),
		# },
		{
			"name" : "GageBoard",
			"type" : "window",
			"x" : float(SCREEN_WIDTH) / 2  - 268/2,
			"y" : float(SCREEN_HEIGHT) / 2 - 50,
			"width" : 268, 
			"height": 268,

			"children" :
			(
				{
					"name" : "BackGage",
					"type" : "ani_image",


					"x" : -160,
					"y" : -210,

					"delay" : 3,

					"images" :
					(
						INTERFACE_PATH + "bar_load_01.png",
						INTERFACE_PATH + "bar_load_02.png",
						INTERFACE_PATH + "bar_load_03.png",
						INTERFACE_PATH + "bar_load_04.png",
						INTERFACE_PATH + "bar_load_05.png",
						INTERFACE_PATH + "bar_load_06.png",
						INTERFACE_PATH + "bar_load_07.png",
						INTERFACE_PATH + "bar_load_08.png",
						INTERFACE_PATH + "bar_load_09.png",
						INTERFACE_PATH + "bar_load_10.png",
						INTERFACE_PATH + "bar_load_01.png",
						INTERFACE_PATH + "bar_load_02.png",
						INTERFACE_PATH + "bar_load_03.png",
						INTERFACE_PATH + "bar_load_04.png",
						INTERFACE_PATH + "bar_load_05.png",
						INTERFACE_PATH + "bar_load_06.png",
						INTERFACE_PATH + "bar_load_07.png",
						INTERFACE_PATH + "bar_load_07.png",
						INTERFACE_PATH + "bar_load_07.png",
						INTERFACE_PATH + "bar_load_07.png",
					)
				},
			),
		},
	),
}

import localeInfo
import uiScriptLocale

LOCALE_PATH = uiScriptLocale.WINDOWS_PATH
ROOT_PATH2 = "d:/ymir work/ui/public/"

MAINBOARD_WIDTH = 244
MAINBOARD_HEIGHT = 405#361

LABEL_START_X = 159
LABEL_START_Y = 58
LABEL_WIDTH = 50
LABEL_HEIGHT = 19
LABEL_GAP = LABEL_HEIGHT+5
LABEL_NAME_POS_X = 15
TITLE_BAR_POS_X = 30
TITLE_BAR_WIDTH = 163

window = {
	"name" : "CharacterDetailsWindow",
	"style" : ("movable"),
	
	"x" : 274, #24+253-3,
	"y" : (SCREEN_HEIGHT - 398) / 2,

	"width" : MAINBOARD_WIDTH,
	"height" : MAINBOARD_HEIGHT,
	
	"children" :
	(
		{
			"name" : "MainBoard",
			"type" : "board",
			"style" : ("attach","ltr"),

			"x" : 0,
			"y" : 0,

			"width" : MAINBOARD_WIDTH,
			"height" : MAINBOARD_HEIGHT,

			"children" :
			(

				# ????
				{
					"name" : "ibo_adamdir",
					"type" : "thinboard_circle",
					"x" : 10,
					"y" : 45,
					"width" : MAINBOARD_WIDTH-20,
					"height" : MAINBOARD_HEIGHT-55,
				},

				{
					"name" : "killbonuspage",
					"type" : "window",
					"x" : 15,
					"y" : 25,
					"width" : MAINBOARD_WIDTH,
					"height" : MAINBOARD_HEIGHT,
					"children" : 
					(

				
				{
					"name" : "bosses_title_bg", "type" : "thinboard_circle", "x" : 15, "y" : 50+27*0, "width" : 120, "height" : 24,
					
					"children" : ( 
						{ "name" : "bosses_text", "type" : "text", "x" : 0, "y" : 0, "text" : "Kesilen Patron", "color": 0xff007bff, "all_align" : "center", },
					),
				},
				{
					"name" : "bosses_kills_bg", "type" : "thinboard_circle", "x" : 15+120+2, "y" : 50+20*0, "width" : 169-120-2, "height" : 24,
					
					"children" : ( 
						{ "name" : "bosses_kills", "type" : "text", "x" : 0, "y" : 0, "text" : "0", "all_align" : "center", },
					),
				},	

				{
					"name" : "stones_title_bg", "type" : "thinboard_circle", "x" : 15, "y" : 50+27*1, "width" : 120, "height" : 24,
					
					"children" : ( 
						{ "name" : "stones_text", "type" : "text", "x" : 0, "y" : 0, "text" : "Kesilen Metin", "color" : 0xffff1723, "all_align" : "center", },
					),
				},
				{
					"name" : "stones_kills_bg", "type" : "thinboard_circle", "x" : 15+120+2, "y" : 50+27*1, "width" : 169-120-2, "height" : 24,
					
					"children" : ( 
						{ "name" : "stones_kills", "type" : "text", "x" : 0, "y" : 0, "text" : "0", "all_align" : "center", },
					),
				},		

					),
				},
				{
					"name" : "karakterbonuspage",
					"type" : "window",
					"x" : 0,
					"y" : 0,
					"width" : MAINBOARD_WIDTH,
					"height" : MAINBOARD_HEIGHT,
					"children" :
					(
						## ??? ?
						{
							"name" : "ScrollBar",
							"type" : "scrollbar",

							"x" : 30,
							"y" : 44,
							"size" : MAINBOARD_HEIGHT - 65,
							"horizontal_align" : "right",
						},
						
						## ??? ?
						{ 
							"name" : "horizontalbar0", "type":"thinboard_circle", "x":TITLE_BAR_POS_X, "y":LABEL_START_Y+LABEL_GAP*0, "width":TITLE_BAR_WIDTH, "height" : 20,
							"children" : ( { "name" : "horizontalbarName0", "type" : "text", "x" : 0, "y" : 0, "text" : "", "all_align" : "center", }, ),
						},
						{ 
							"name" : "horizontalbar1", "type":"thinboard_circle", "x":TITLE_BAR_POS_X, "y":LABEL_START_Y+LABEL_GAP*1, "width":TITLE_BAR_WIDTH, "height" : 20,
							"children" : ( { "name" : "horizontalbarName1", "type" : "text", "x" : 0, "y" : 0, "text" : "", "all_align" : "center", }, ),
						},
						{ 
							"name" : "horizontalbar2", "type":"thinboard_circle", "x":TITLE_BAR_POS_X, "y":LABEL_START_Y+LABEL_GAP*2, "width":TITLE_BAR_WIDTH, "height" : 20,
							"children" : ( { "name" : "horizontalbarName2", "type" : "text", "x" : 0, "y" : 0, "text" : "", "all_align" : "center", }, ),
						},
						{ 
							"name" : "horizontalbar3", "type":"thinboard_circle", "x":TITLE_BAR_POS_X, "y":LABEL_START_Y+LABEL_GAP*3, "width":TITLE_BAR_WIDTH, "height" : 20,
							"children" : ( { "name" : "horizontalbarName3", "type" : "text", "x" : 0, "y" : 0, "text" : "", "all_align" : "center", }, ),
						},
						{ 
							"name" : "horizontalbar4", "type":"thinboard_circle", "x":TITLE_BAR_POS_X, "y":LABEL_START_Y+LABEL_GAP*4, "width":TITLE_BAR_WIDTH, "height" : 20,
							"children" : ( { "name" : "horizontalbarName4", "type" : "text", "x" : 0, "y" : 0, "text" : "", "all_align" : "center", }, ),
						},
						{ 
							"name" : "horizontalbar5", "type":"thinboard_circle", "x":TITLE_BAR_POS_X, "y":LABEL_START_Y+LABEL_GAP*5, "width":TITLE_BAR_WIDTH, "height" : 20,
							"children" : ( { "name" : "horizontalbarName5", "type" : "text", "x" : 0, "y" : 0, "text" : "", "all_align" : "center", }, ),
						},
						{ 
							"name" : "horizontalbar6", "type":"thinboard_circle", "x":TITLE_BAR_POS_X, "y":LABEL_START_Y+LABEL_GAP*6, "width":TITLE_BAR_WIDTH, "height" : 20,
							"children" : ( { "name" : "horizontalbarName6", "type" : "text", "x" : 0, "y" : 0, "text" : "", "all_align" : "center", }, ),
						},
						{ 
							"name" : "horizontalbar7", "type":"thinboard_circle", "x":TITLE_BAR_POS_X, "y":LABEL_START_Y+LABEL_GAP*7, "width":TITLE_BAR_WIDTH, "height" : 20,
							"children" : ( { "name" : "horizontalbarName7", "type" : "text", "x" : 0, "y" : 0, "text" : "", "all_align" : "center", }, ),
						},
						{ 
							"name" : "horizontalbar8", "type":"thinboard_circle", "x":TITLE_BAR_POS_X, "y":LABEL_START_Y+LABEL_GAP*8, "width":TITLE_BAR_WIDTH, "height" : 20,
							"children" : ( { "name" : "horizontalbarName8", "type" : "text", "x" : 0, "y" : 0, "text" : "", "all_align" : "center", }, ),
						},
						{ 
							"name" : "horizontalbar9", "type":"thinboard_circle", "x":TITLE_BAR_POS_X, "y":LABEL_START_Y+LABEL_GAP*9, "width":TITLE_BAR_WIDTH, "height" : 20,
							"children" : ( { "name" : "horizontalbarName9", "type" : "text", "x" : 0, "y" : 0, "text" : "", "all_align" : "center", }, ),
						},
						{ 
							"name" : "horizontalbar10", "type":"thinboard_circle", "x":TITLE_BAR_POS_X, "y":LABEL_START_Y+LABEL_GAP*10, "width":TITLE_BAR_WIDTH, "height" : 20,
							"children" : ( { "name" : "horizontalbarName10", "type" : "text", "x" : 0, "y" : 0, "text" : "", "all_align" : "center", }, ),
						},
						{ 
							"name" : "horizontalbar11", "type":"thinboard_circle", "x":TITLE_BAR_POS_X, "y":LABEL_START_Y+LABEL_GAP*11, "width":TITLE_BAR_WIDTH, "height" : 20,
							"children" : ( { "name" : "horizontalbarName11", "type" : "text", "x" : 0, "y" : 0, "text" : "", "all_align" : "center", }, ),
						},
						{ 
							"name" : "horizontalbar12", "type":"thinboard_circle", "x":TITLE_BAR_POS_X, "y":LABEL_START_Y+LABEL_GAP*12, "width":TITLE_BAR_WIDTH, "height" : 20,
							"children" : ( { "name" : "horizontalbarName12", "type" : "text", "x" : 0, "y" : 0, "text" : "", "all_align" : "center", }, ),
						},

						{
							"name" : "label0", "type" : "thinboard_circle", "x" : LABEL_START_X, "y" : LABEL_START_Y+LABEL_GAP*0, "width" : LABEL_WIDTH, "height" : LABEL_HEIGHT,
							"children" : ( 
								{ "name" : "labelvalue0", "type" : "text", "x" : 0, "y" : 0, "text" : "", "all_align" : "center", },
							),
						},
						{
							"name" : "label1", "type" : "thinboard_circle", "x" : LABEL_START_X, "y" : LABEL_START_Y+LABEL_GAP*1, "width" : LABEL_WIDTH, "height" : LABEL_HEIGHT,
							"children" : ( 
								{ "name" : "labelvalue1", "type" : "text", "x" : 0, "y" : 0, "text" : "", "all_align" : "center", },
							),
						},
						{
							"name" : "label2", "type" : "thinboard_circle", "x" : LABEL_START_X, "y" : LABEL_START_Y+LABEL_GAP*2, "width" : LABEL_WIDTH, "height" : LABEL_HEIGHT,
							"children" : ( 
								{ "name" : "labelvalue2", "type" : "text", "x" : 0, "y" : 0, "text" : "", "all_align" : "center", },
							),
						},
						{
							"name" : "label3", "type" : "thinboard_circle", "x" : LABEL_START_X, "y" : LABEL_START_Y+LABEL_GAP*3, "width" : LABEL_WIDTH, "height" : LABEL_HEIGHT,
							"children" : ( 
								{ "name" : "labelvalue3", "type" : "text", "x" : 0, "y" : 0, "text" : "", "all_align" : "center", },
							),
						},
						{
							"name" : "label4", "type" : "thinboard_circle", "x" : LABEL_START_X, "y" : LABEL_START_Y+LABEL_GAP*4, "width" : LABEL_WIDTH, "height" : LABEL_HEIGHT,
							"children" : ( 
								{ "name" : "labelvalue4", "type" : "text", "x" : 0, "y" : 0, "text" : "", "all_align" : "center", },
							),
						},
						{
							"name" : "label5", "type" : "thinboard_circle", "x" : LABEL_START_X, "y" : LABEL_START_Y+LABEL_GAP*5, "width" : LABEL_WIDTH, "height" : LABEL_HEIGHT,
							"children" : ( 
								{ "name" : "labelvalue5", "type" : "text", "x" : 0, "y" : 0, "text" : "", "all_align" : "center", },
							),
						},
						{
							"name" : "label6", "type" : "thinboard_circle", "x" : LABEL_START_X, "y" : LABEL_START_Y+LABEL_GAP*6, "width" : LABEL_WIDTH, "height" : LABEL_HEIGHT,
							"children" : ( 
								{ "name" : "labelvalue6", "type" : "text", "x" : 0, "y" : 0, "text" : "", "all_align" : "center", },
							),
						},
						{
							"name" : "label7", "type" : "thinboard_circle", "x" : LABEL_START_X, "y" : LABEL_START_Y+LABEL_GAP*7, "width" : LABEL_WIDTH, "height" : LABEL_HEIGHT,
							"children" : ( 
								{ "name" : "labelvalue7", "type" : "text", "x" : 0, "y" : 0, "text" : "", "all_align" : "center", },
							),
						},
						{
							"name" : "label8", "type" : "thinboard_circle", "x" : LABEL_START_X, "y" : LABEL_START_Y+LABEL_GAP*8, "width" : LABEL_WIDTH, "height" : LABEL_HEIGHT,
							"children" : ( 
								{ "name" : "labelvalue8", "type" : "text", "x" : 0, "y" : 0, "text" : "", "all_align" : "center", },
							),
						},
						{
							"name" : "label9", "type" : "thinboard_circle", "x" : LABEL_START_X, "y" : LABEL_START_Y+LABEL_GAP*9, "width" : LABEL_WIDTH, "height" : LABEL_HEIGHT,
							"children" : ( 
								{ "name" : "labelvalue9", "type" : "text", "x" : 0, "y" : 0, "text" : "", "all_align" : "center", },
							),
						},
						{
							"name" : "label10", "type" : "thinboard_circle", "x" : LABEL_START_X, "y" : LABEL_START_Y+LABEL_GAP*10, "width" : LABEL_WIDTH, "height" : LABEL_HEIGHT,
							"children" : ( 
								{ "name" : "labelvalue10", "type" : "text", "x" : 0, "y" : 0, "text" : "", "all_align" : "center", },
							),
						},
						{
							"name" : "label11", "type" : "thinboard_circle", "x" : LABEL_START_X, "y" : LABEL_START_Y+LABEL_GAP*11, "width" : LABEL_WIDTH, "height" : LABEL_HEIGHT,
							"children" : ( 
								{ "name" : "labelvalue11", "type" : "text", "x" : 0, "y" : 0, "text" : "", "all_align" : "center", },
							),
						},
						{
							"name" : "label12", "type" : "thinboard_circle", "x" : LABEL_START_X, "y" : LABEL_START_Y+LABEL_GAP*12, "width" : LABEL_WIDTH, "height" : LABEL_HEIGHT,
							"children" : ( 
								{ "name" : "labelvalue12", "type" : "text", "x" : 0, "y" : 0, "text" : "", "all_align" : "center", },
							),
						},

						{ 
							"name" : "labelname0", "type" : "button",
							"x" : LABEL_NAME_POS_X, "y" : LABEL_START_Y+LABEL_GAP*0, 
							"text" : "", 
							"default_image" : "locale/tr/ui/windows/details_button.tga",
							"over_image" : "locale/tr/ui/windows/details_button.tga",
							"down_image" : "locale/tr/ui/windows/details_button.tga",
						},
						{ 
							"name" : "labelname1", "type" : "button",
							"x" : LABEL_NAME_POS_X, "y" : LABEL_START_Y+LABEL_GAP*1, 
							"text" : "", 
							"default_image" : "locale/tr/ui/windows/details_button.tga",
							"over_image" : "locale/tr/ui/windows/details_button.tga",
							"down_image" : "locale/tr/ui/windows/details_button.tga",
						},
						{ 
							"name" : "labelname2", "type" : "button",
							"x" : LABEL_NAME_POS_X, "y" : LABEL_START_Y+LABEL_GAP*2, 

							"text" : "", 

							"default_image" : "locale/tr/ui/windows/details_button.tga",
							"over_image" : "locale/tr/ui/windows/details_button.tga",
							"down_image" : "locale/tr/ui/windows/details_button.tga",
						},
						{ 
							"name" : "labelname3", "type" : "button",
							"x" : LABEL_NAME_POS_X, "y" : LABEL_START_Y+LABEL_GAP*3, 

							"text" : "", 

							"default_image" : "locale/tr/ui/windows/details_button.tga",
							"over_image" : "locale/tr/ui/windows/details_button.tga",
							"down_image" : "locale/tr/ui/windows/details_button.tga",
						},
						{ 
							"name" : "labelname4", "type" : "button",
							"x" : LABEL_NAME_POS_X, "y" : LABEL_START_Y+LABEL_GAP*4, 

							"text" : "", 

							"default_image" : "locale/tr/ui/windows/details_button.tga",
							"over_image" : "locale/tr/ui/windows/details_button.tga",
							"down_image" : "locale/tr/ui/windows/details_button.tga",
						},
						{
							"name" : "labelname5", "type" : "button",
							"x" : LABEL_NAME_POS_X, "y" : LABEL_START_Y+LABEL_GAP*5, 

							"text" : "", 

							"default_image" : "locale/tr/ui/windows/details_button.tga",
							"over_image" : "locale/tr/ui/windows/details_button.tga",
							"down_image" : "locale/tr/ui/windows/details_button.tga",
						},
						{
							"name" : "labelname6", "type" : "button",
							"x" : LABEL_NAME_POS_X, "y" : LABEL_START_Y+LABEL_GAP*6, 

							"text" : "", 

							"default_image" : "locale/tr/ui/windows/details_button.tga",
							"over_image" : "locale/tr/ui/windows/details_button.tga",
							"down_image" : "locale/tr/ui/windows/details_button.tga",
						},
						{
							"name" : "labelname7", "type" : "button",
							"x" : LABEL_NAME_POS_X, "y" : LABEL_START_Y+LABEL_GAP*7, 

							"text" : "", 

							"default_image" : "locale/tr/ui/windows/details_button.tga",
							"over_image" : "locale/tr/ui/windows/details_button.tga",
							"down_image" : "locale/tr/ui/windows/details_button.tga",
						},
						{
							"name" : "labelname8", "type" : "button",
							"x" : LABEL_NAME_POS_X, "y" : LABEL_START_Y+LABEL_GAP*8, 

							"text" : "", 

							"default_image" : "locale/tr/ui/windows/details_button.tga",
							"over_image" : "locale/tr/ui/windows/details_button.tga",
							"down_image" : "locale/tr/ui/windows/details_button.tga",
						},
						{
							"name" : "labelname9", "type" : "button",
							"x" : LABEL_NAME_POS_X, "y" : LABEL_START_Y+LABEL_GAP*9, 

							"text" : "", 

							"default_image" : "locale/tr/ui/windows/details_button.tga",
							"over_image" : "locale/tr/ui/windows/details_button.tga",
							"down_image" : "locale/tr/ui/windows/details_button.tga",
						},
						{
							"name" : "labelname10", "type" : "button",
							"x" : LABEL_NAME_POS_X, "y" : LABEL_START_Y+LABEL_GAP*10, 

							"text" : "", 

							"default_image" : "locale/tr/ui/windows/details_button.tga",
							"over_image" : "locale/tr/ui/windows/details_button.tga",
							"down_image" : "locale/tr/ui/windows/details_button.tga",
						},
						{
							"name" : "labelname11", "type" : "button",
							"x" : LABEL_NAME_POS_X, "y" : LABEL_START_Y+LABEL_GAP*11, 

							"text" : "", 

							"default_image" : "locale/tr/ui/windows/details_button.tga",
							"over_image" : "locale/tr/ui/windows/details_button.tga",
							"down_image" : "locale/tr/ui/windows/details_button.tga",
						},
						{
							"name" : "labelname12", "type" : "button",
							"x" : LABEL_NAME_POS_X, "y" : LABEL_START_Y+LABEL_GAP*12, 

							"text" : "", 

							"default_image" : "locale/tr/ui/windows/details_button.tga",
							"over_image" : "locale/tr/ui/windows/details_button.tga",
							"down_image" : "locale/tr/ui/windows/details_button.tga",
						},
					),
				},
				{
					"name" : "karakterbonus",
					"type" : "button",

					"x" : 20,
					"y" : 15,
					"height" : 20,
					"width" : 66,
					"text" : "Bonuslar",

					"default_image" : "ibowork/slot_normal.tga",
					"over_image" : "ibowork/slot_hover.tga",
					"down_image" : "ibowork/slot_active.tga",
				},
				{
					"name" : "killbonus",
					"type" : "button",

					"x" : 127,
					"y" : 15,
					"height" : 20,
					"width" : 66,
					"text" : "Ýstatistik",

					"default_image" : "ibowork/slot_normal.tga",
					"over_image" : "ibowork/slot_hover.tga",
					"down_image" : "ibowork/slot_active.tga",
				},
			),
		},
	),
}
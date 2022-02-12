import app
import localeInfo, uiScriptLocale

WINDOW_WIDTH	= 621
WINDOW_HEIGHT	= 480
LOCALE_PATH = "d:/ymir work/ui/privatesearch/"
ROOT = "offlineshop/loading/"

GOLD_COLOR	= 0xFFFEE3AE
try:
	ENABLE_WOLFMAN_CHARACTER = app.ENABLE_WOLFMAN_CHARACTER
except:
	ENABLE_WOLFMAN_CHARACTER = 0

try:
	ENABLE_CHEQUE_SYSTEM = 1
except:
	ENABLE_CHEQUE_SYSTEM = 1




SEARCH_AND_FILTER_BACKGROUND = "offlineshop/searchfilter/base_image_lycan.png"
SAFEBOX_WITHDRAW_BUTTON	= "offlineshop/safebox/withdrawyang_%s.png"

if ENABLE_WOLFMAN_CHARACTER:
	SEARCH_AND_FILTER_BACKGROUND = "offlineshop/searchfilter/base_image_lycan.png"

if ENABLE_CHEQUE_SYSTEM:
	SAFEBOX_WITHDRAW_BUTTON = "offlineshop/safebox/withdrawyang_%s_cheque.png"




if ENABLE_CHEQUE_SYSTEM:
	SAFEBOX_CHILDREN = (
		{
			"name" : "BackGroundSafebox01",
			"type" : "thinboard",

			"x" : 9,
			"y" : 375 - 33 + 12,

			"width" : 398 + 198,
			"height" : 55,
			"children" :
			(
				{
					"name" : "BarItem",
					"type" : "expanded_image",
					"style" : ("attach",),

					"x" : 0,
					"y" : 5,
					"image" : "d:/ymir work/ui/minigame/fish_event/fish_special_slot.sub",
					
					"children" :
					(
						{
							"name" : "BarItemSlot",
							"type" : "image",

							"x" : 7,
							"y" : 7,
							"width" : 32,
							"height" : 32,

							"image" : "icon/item/money.tga",
						},
						{
							"name" : "titleimage1",
							"type" : "image",

							"x" : 44,
							"y" : 3,

							"image" : "d:/ymir work/ui/pet/feed_button/feed_button_default.sub",
							"children" :
							(
								{
									"name" : "yazi1",
									"type" : "text",
									"text" : "Yang",
									"x" : 0, "y" : -1,
									"all_align" : "center",
								},
								{
									"name" : "item1countbar",
									"type" : "slotbar",
									"x" : 1,
									"y" : 20,
									"width" : 115,
									"height" : 18,
									"children" :
									(
										{
											"name" : "ShopSafeboxValuteText",
											"type" : "text",

											"x" : 0,
											"y" : -2,

											"all_align" : "center",
											"text" : "000000",
										},
									),
								},
							),
						},
					),
				},
########################################################################################################################################################
				{
					"name" : "SuItem",
					"type" : "expanded_image",
					"style" : ("attach",),

					"x" : 165,
					"y" : 5,
					"image" : "d:/ymir work/ui/minigame/fish_event/fish_special_slot.sub",
					
					"children" :
					(
						{
							"name" : "SuItemSlot",
							"type" : "image",

							"x" : 7,
							"y" : 7,
							"width" : 32,
							"height" : 32,

							"image" : "icon/item/80020.tga",
						},
						{
							"name" : "titleimage2",
							"type" : "image",

							"x" : 44,
							"y" : 3,

							"image" : "d:/ymir work/ui/pet/feed_button/feed_button_default.sub",
							"children" :
							(
								{
									"name" : "yazi1",
									"type" : "text",
									"text" : "Won",
									"x" : 0, "y" : -1,
									"all_align" : "center",
								},
								{
									"name" : "item2countbar",
									"type" : "slotbar",
									"x" : 1,
									"y" : 20,
									"width" : 115,
									"height" : 18,
									"children" :
									(
										{
											"name" : "ShopSafeboxValuteTextCheque",
											"type" : "text",

											"x" : 0,
											"y" : -2,

											"all_align" : "center",
											"text" : "000000",
										},
									),
								},
							),
						},
					),
				},


				{
					"name" : "ShopSafeboxWithdrawYangButton",
					"type" : "button",

					"x" : 335,
					"y" : 18,
					"text" : "Aktar",
					"fontsize" : "LARGE",

					"default_image" : "yusufwork/stop_switch_hover.png",
					"over_image" : "yusufwork/stop_switch_norm.png",
					"down_image" : "yusufwork/stop_switch_down.png",
				},
			),
		},
		{
			"name" : "GridBackground",
			"type" : "thinboard",

			"x" : 9,
			"y" : 46 - 33,

			"width" : 398 + 198,
			"height" : 337,
		},
	)

else:
	SAFEBOX_CHILDREN = (

		{
			"name" : "ShopSafeboxWithdrawYangButton",
			"type" : "button",

			"x" : 447-205 - 100,
			"y" : 16-4,

			"default_image" :  SAFEBOX_WITHDRAW_BUTTON%"default",
			"over_image" 	:  SAFEBOX_WITHDRAW_BUTTON%"over",
			"down_image" 	:  SAFEBOX_WITHDRAW_BUTTON%"down",
		},

		{
			"name" : "ShopSafeboxValuteText",
			"type" : "text",

			"x" : 468-154 - 100,
			"y" : 22-8,

			"text_horizontal_align" : "center",
			"text" : "000000",
		},
	)






window = {

	"name" : "OfflineshopBoard",
	"style" : ("movable", "float",),

	"x" : SCREEN_WIDTH/2  - WINDOW_WIDTH/2,
	"y" : SCREEN_HEIGHT/2  - WINDOW_HEIGHT/2,

	"width" : WINDOW_WIDTH,
	"height" : WINDOW_HEIGHT,

	"children" :
	(
		{
			"name" : "MainBoard",
			"type" : "board",
			
			"style" : ("attach",),
			
			"x" : 0,
			"y" : 0,
			
			"width" 	: WINDOW_WIDTH,
			"height" 	: WINDOW_HEIGHT,
			
			"children" : 
			(
				{
					"name" : "TitleBar",
					"type" : "titlebar",
					
					"x" : 8,
					"y" : 7,
					
					"width"  : WINDOW_WIDTH - 16,
					"color"  : "red",
					
					"children" :
					(
						{ "name":"TitleName", "type":"text", "x":0, "y":-1, "text":"Offline Shop", "all_align":"center" },
					),
				},


				{
					"name" : "Menu",
					"type" : "thinboard",
					
					"x" : 13,
					"y" : 34,

					"width" : 398 + 50,
					"height" : 27,

					"children":
					(

						{
							"name": "ListOfShopButton",
							"type": "button",

							"x": 90, "y": 7,

							"tooltip_text" : "Pazar Listesi",

							"default_image" : "offlineshop/shopsearchp2p/pazarliste.png",
							"over_image" : "offlineshop/shopsearchp2p/pazarliste2.png",
							"down_image" : "offlineshop/shopsearchp2p/pazarliste3.png",
						},

						{
							"name": "SearchHistoryButton",
							"type": "button",

							"x": 170, "y": 7,

							"tooltip_text" : "Arama Geçmisi",

							"default_image" : "offlineshop/shopsearchp2p/gecmis.png",
							"over_image" : "offlineshop/shopsearchp2p/gecmis2.png",
							"down_image" : "offlineshop/shopsearchp2p/gecmis3.png",
						},

						{
							"name": "MyPatternsButton",
							"type": "button",

							"x": 210, "y": 7,

							"tooltip_text" : "Kayýtlý Aramalar",

							"default_image" : "offlineshop/shopsearchp2p/kaydet.png",
							"over_image" : "offlineshop/shopsearchp2p/kaydet2.png",
							"down_image" : "offlineshop/shopsearchp2p/kaydet3.png",
						},
					),
				},

				# Back Button
				{
					"name": "SearchFilterButton",
					"type": "button",

					"x": 617 - 26 - (45*2) + 13, "y": 24 + 7,

					"tooltip_text" : "Eþya Arama",

					"default_image" : "ibrahimgokalp/offlineshop_22/arama_1.png",
					"over_image" : "ibrahimgokalp/offlineshop_22/arama_2.png",
					"down_image" : "ibrahimgokalp/offlineshop_22/arama_2.png",
					
				},
				
				{
					"name": "MyShopButton",
					"type": "button",

					"x": 617 - 26 - (45*3) + 13, "y": 24 + 7,

					"tooltip_text" : "Pazar",

					"default_image" : "ibrahimgokalp/offlineshop_22/myshop_1.png",
					"over_image" : "ibrahimgokalp/offlineshop_22/myshop_2.png",
					"down_image" : "ibrahimgokalp/offlineshop_22/myshop_2.png",
				},

				{
					"name": "ShopSafeboxButton",
					"type": "button",

					"x": 617 - 26 - 45 + 13, "y": 24 + 7,

					"tooltip_text" : "Depo",

					"default_image" : "ibrahimgokalp/offlineshop_22/depo_1.png",
					"over_image" : "ibrahimgokalp/offlineshop_22/depo_2.png",
					"down_image" : "ibrahimgokalp/offlineshop_22/depo_2.png",
				},

				#MyShop_noShop
				{
					"name" : "MyShopBoardNoShop",
					"type" : "window",
					
                    "width" :  622,  "height" :  567,

                    "x" : 3, "y" : 61,
					
					"children":
					(

						{
							"name" : "BackGroundCreate",
							"type" : "thinboard",

							"x" : 9,
							"y" : 375 - 33,

							"width"     : 398 + 198,
							"height"     : 55,
							"children" :
							(
								{
									"name" : "CreateShopButton",
									"type" : "button",
									
									"x" : 300+80 - 9,
									"y" : 72 - 55,
									"default_image" : "yusufwork/checkbutton.dds",
									"over_image" : "yusufwork/checkbutton_over.dds",
									"down_image" : "yusufwork/checkbutton_down.dds",

									"tooltip_text" : localeInfo.OFFLINESHOP_SCRIPTFILE_CREATE_TEXT,
								},
								{
									"name" : "Day",
									"type" : "text",
									"text" : "Gün : ",
									"x" : 5,
									"y" : 5,
									"fontsize" : "LARGE",
								},
								{
									"name" : "Clock",
									"type" : "text",
									"text" : "Saat : ",
									"x" : 5,
									"y" : 30,
									"fontsize" : "LARGE",
								},
								{
									"name" : "Days_Slotbar",
									"type" : "slotbar",
									"width" : 22,
									"height" : 18,
									"x" : 50+37 -11, "y" : 5,
								},
								{
									"name" : "DaysCountText",
									"type" : "text",
									
									"text" : "0",
									"text_horizontal_align" : "center",
									"x" : 50 + 37, "y" :5,
									"fontsize" : "LARGE",
								},

								{
									"name" : "IncreaseDaysButton",
									"type" : "button",
									

									"x" :50 + 37 + 23,
									"y" : 4,

											"default_image" : "offlineshop/button_scroll_right.tga",
											"over_image"     : "offlineshop/button_scroll_right_hover.tga",
											"down_image"     : "offlineshop/button_scroll_right_active.tga",
								},
								
								
								{
									"name" : "DecreaseDaysButton",
									"type" : "button",
									
									"x" : 50,
									"y" : 4,
									
											"default_image" : "offlineshop/button_scroll_left.tga",
											"over_image"     : "offlineshop/button_scroll_left_hover.tga",
											"down_image"     : "offlineshop/button_scroll_left_active.tga",
								},
								{
									"name" : "Hours_Slotbar",
									"type" : "slotbar",
									"width" : 22,
									"height" : 18,
									"x" : 50+37 -11, "y" : 30,
								},
								{
									"name" : "HoursCountText",
									"type" : "text",
									
									"text" : "0",
									"text_horizontal_align" : "center",
									"x" : 50 + 37, "y" : 30,
									"fontsize" : "LARGE",
								},
								{
									"name" : "IncreaseHoursButton",
									"type" : "button",
									
									"x" : 50 +37 + 23,
									"y" : 29,
									
											"default_image" : "offlineshop/button_scroll_right.tga",
											"over_image"     : "offlineshop/button_scroll_right_hover.tga",
											"down_image"     : "offlineshop/button_scroll_right_active.tga",
								},
								
								
								{
									"name" : "DecreaseHoursButton",
									"type" : "button",
									
									"x" : 50,
									"y" : 29,
									
											"default_image" : "offlineshop/button_scroll_left.tga",
											"over_image"     : "offlineshop/button_scroll_left_hover.tga",
											"down_image"     : "offlineshop/button_scroll_left_active.tga",
								},
								{
									"name" : "Nameslotbar",
									"type" : "slotbar",
									"width" : 200,
									"height" : 20,
									"x" : 160, "y" : 24,
									"children" :
									(
										{
											"name" : "ShopNameInfo",
											"type" : "text",
											"text" : "<- Pazar Ýsmi Belirle -> ",
											"all_align" : "center",
											"x" : 0,
											"y" : -21,
											"fontsize" : "LARGE",
										},
										{
											"name" : "ShopNameInput",
											"type" : "editline",
											
											"width" : 187 , "height" : 15 ,
											
											"input_limit" : 35,
											"x" : 3, "y" : 2,
										},
									),
								},

							),
						},

						{
							"name" : "GridBackground",
							"type" : "thinboard",

							"x" : 9,
							"y" : 46 - 33,

							"width" : 398 + 198,
							"height" : 337,
						},


					),
				},


				#MyShop_Owner
				{
					"name" : "MyShopBoard",
					"type" : "window",
					
					"width" :  622,  "height" :  567,

					"x" : 3, "y" : 61,

					"children":
					(

						{
								"name" : "MyShopShopTitle",
								"type" : "text",

								"x" : 20, "y" : 10 - 28,

								"text" : "title",
								"fontsize" : "LARGE",
								"r" : 1.0,
								"g" : 1.0,
								"b" : 1.0,
								"a" : 1.0,
						},
						{
							"name" : "arkaboard_2",
							"type" : "board",
							"x" : 9,
							"y" : 375 - 33 + 7,
							"width" : 398 + 198,
							"height" : 40,
							"children":
							(
								{
									"name" : "MyShopEditTitleButton",
									"type" : "button",

									"x" : 110, "y" : 8,

									"tooltip_text"	: localeInfo.OFFLINESHOP_EDIT_SHOPNAME_TOOLTIP,
									
									"default_image" : "offlineshop/myshop/editname_default.png",
									"over_image" 	: "offlineshop/myshop/editname_over.png",
									"down_image" 	: "offlineshop/myshop/editname_down.png",
								},

								{
									"name" : "MyShopShopDuration",
									"type" : "text",

									"x" : 29, "y" : 21+17,

									"text" : "99 days",

								},
								{
									"name" : "MyShopCloseButton",
									"type" : "button",

									"x": 465, "y": 19,

									"text" : uiScriptLocale.PRIVATE_SHOP_CLOSE_BUTTON,

									"default_image" : "ibrahimgokalp/dungeoninfo_2022/butonlar/button_1.png",
									"over_image" :    "ibrahimgokalp/dungeoninfo_2022/butonlar/button_2.png",
									"down_image" :    "ibrahimgokalp/dungeoninfo_2022/butonlar/button_2.png",
								},
								{
									"name" : "MyShopShopTotalCount",
									"type" : "text",
									"text" : "ibrahim",
									"x" : 297 , "y" : 21+17,
								},
								# GAUGE
								{
									"name":"Shop_Time_Gauge_Slot",
									"type":"image",
									"x" : 25,
									"y" : 6+10,

									"image" : "ibrahimgokalp/offlineshop_22/timegauge/background.tga",

									"children" :
									(
										{
											"name" : "Shop_Time_Gauge",
											"type" : "ani_image",

											"x" : 4,
											"y" : 0,

											"delay" : 6,

											"images" :
											(
												"ibrahimgokalp/offlineshop_22/timegauge/01.tga",
												"ibrahimgokalp/offlineshop_22/timegauge/02.tga",
												"ibrahimgokalp/offlineshop_22/timegauge/03.tga",
												"ibrahimgokalp/offlineshop_22/timegauge/04.tga",
												"ibrahimgokalp/offlineshop_22/timegauge/05.tga",
												"ibrahimgokalp/offlineshop_22/timegauge/06.tga",
												"ibrahimgokalp/offlineshop_22/timegauge/07.tga",
											),
										},
									),
								},

							),
						},
						{
							"name" : "arkaboard_2",
							"type" : "thinboard",
							"x" : 9,
							"y" : 46 - 33,

							"width" : 398 + 198,
							"height" : 337,
						},
					),
				},

				{
					"name" : "ListOfShop_OpenShop",
					"type" : "window",
					
                    "width" :  622,  "height" :  567,

                    "x" : 3, "y" : 61,
					
					"children":
					(
						{
							"name" : "arkaboard_2",
							"type" : "thinboard",
							"x" : 9,
							"y" : 46 - 33,

							"width" : 398 + 198,
							"height" : 337,
						},

						{
								"name" : "OwnerShopTitle",
								"type" : "text",

								"x" : 20, "y" : 10 - 28,

								"text" : "title",
								"fontsize" : "LARGE",
								"r" : 1.0,
								"g" : 1.0,
								"b" : 1.0,
								"a" : 1.0,
						},
						{
							"name" : "OpenShopBackToListButton",
							"type" : "button",
							
							"x" : 15, "y" : 5 - 33,
							
							"default_image" : "d:/ymir work/ui/public/middle_button_01.sub",
							"over_image" : "d:/ymir work/ui/public/middle_button_02.sub",
							"down_image" : "d:/ymir work/ui/public/middle_button_03.sub",

							"text" : localeInfo.OFFLINESHOP_SCRIPTFILE_CLOSE_SHOP_TEXT,
						},

						{
							"name" : "arkaboard_31",
							"type" : "board",
							"x" : 9,
							"y" : 375 - 33 + 7,
							"width" : 398 + 198,
							"height" : 40,
							"children":
							(
								{
									"name" : "OpenShopShopDuration",
									"type" : "text",

									"x" : 29, "y" : 21+17,

									"text" : "99 days",

								},

								{
									"name" : "OwnerShopPlayerName",
									"type" : "text",
									"text" : "ibrahim",
									"x" : 415 , "y" : 15,
									"fontsize" : "LARGE",
								},

								{
									"name" : "OpenShopShopTitle",
									"type" : "text",
									"text" : "ibrahim",
									"x" : 415 , "y" : 21+17,
									# "fontsize" : "LARGE",
								},

								# GAUGE
								{
									"name":"OwnerShop_Time_Gauge_Slot",
									"type":"image",
									"x" : 25,
									"y" : 6+10,

									"image" : "ibrahimgokalp/offlineshop_22/timegauge/background.tga",
									"children" :
									(
										{
											"name" : "OwnerShop_Time_Gauge",
											"type" : "ani_image",

											"x" : 4,
											"y" : 0,

											"delay" : 6,
											"images" :
											(
												"ibrahimgokalp/offlineshop_22/timegauge/01.tga",
												"ibrahimgokalp/offlineshop_22/timegauge/02.tga",
												"ibrahimgokalp/offlineshop_22/timegauge/03.tga",
												"ibrahimgokalp/offlineshop_22/timegauge/04.tga",
												"ibrahimgokalp/offlineshop_22/timegauge/05.tga",
												"ibrahimgokalp/offlineshop_22/timegauge/06.tga",
												"ibrahimgokalp/offlineshop_22/timegauge/07.tga",
											),
										},
									),
								},
							),
						},
					),
				},

				{
					"name" : "ListOfShop_List",
					"type" : "window",
					
					"width" :  622,  "height" :  544,

					"x" : 3, "y" : 61,

				},


				{
					"name" : "SearchHistoryBoard",
					"type" : "window",
					
					"width" :  622,  "height" :  544,

					"x" : 3, "y" : 61,

				},

				{
					"name" : "MyPatternsBoard",
					"type" : "window",
					
					"width" :  622,  "height" :  544,

					"x" : 3, "y" : 61,

				},

				{
					"name" : "SearchFilterBoard",
					"type" : "window",

					"width" :  WINDOW_WIDTH - 6,  "height" :  WINDOW_HEIGHT - 78,

					"x" : 3, "y" : 78,
					"children":
					(

                        {
                            "name" : "SearchFilterBoardChild07",
                            "type" : "thinboard",

                            "x" : 145 + 15,
                            "y" : 0,

                            "width"     : 398 + 25 + 21,
                            "height"     : 392,
							"children" :
							(
								{
									"name" : "tab01", "type" : "image",
									"x" : -1, "y" : -2,
									"image" : "ibrahimgokalp/offlineshop_22/tab_menu_01.png",
									"children" :
									(
										{ "name" : "ResultNameText1", "type" : "text", "x" : 67, "y" : 6,  "text" : uiScriptLocale.PRIVATESHOPSEARCH_ITEMNAME, },
										{ "name" : "ResultNameText4", "type" : "text", "x" : 260, "y" : 6, "text" : "Yang", },
										{ "name" : "ResultNameText5", "type" : "text", "x" : 339, "y" : 6, "text" : "Won", },
									),
								},
								{
									"name" : "arka",
									"type" : "image",
									"x" : 4,
									"y" : 24,
									"image" : "ibrahimgokalp/offlineshop_22/aramaarka.png",
								},
							),
						},
						{
							"name" : "SearchFilterBoardChild01",
							"type" : "thinboard",

							"x" : 9,
							"y" : 0,

							"width" : 145,
							"height" : 392,
							"children" :
							(
								{
									"name" : "SearchFilterStartSearch",
									"type" : "button",
									
									"x" : 20, "y" : 350,

									"text" : uiScriptLocale.PRIVATESHOPSEARCH_SEARCH,

									"default_image" : "ibrahimgokalp/dungeoninfo_2022/butonlar/button_1.png",
									"over_image" :    "ibrahimgokalp/dungeoninfo_2022/butonlar/button_2.png",
									"down_image" :    "ibrahimgokalp/dungeoninfo_2022/butonlar/button_2.png",
								},
							),
						},
						## ItemName
						{
							"name" : "ItemNameImg",
							"type" : "image",
							"x" : 19,
							"y" : 10,
							"image" : LOCALE_PATH+"private_leftNameImg.sub",
							"children" :
							(
								{ "name" : "ItemNameText", "type" : "text", "text_horizontal_align":"center", "x" : 60, "y" : 5, "text" : uiScriptLocale.PRIVATESHOPSEARCH_ITEMNAME, "color":GOLD_COLOR },
							),
						},
						{
							"name" : "SearchFilterItemNameInputContainer",
							"type" : "image",
							"image" : LOCALE_PATH+"private_leftSlotImg.sub",
							"x" : 18 + 3, "y" : 30,

							"children" :
							(
								{
									"name" : "SearchFilterItemNameInput",
									"type" : "editline",
									"x" : 2,
									"y" : 3,
									"width" : 136,
									"height" : 15,
									"input_limit" : 20,
									"text" : "",
								},
							)
						},
						{
							"name" : "ItemTypeImg",
							"type" : "image",
							"x" : 19,
							"y" : 54,
							"image" : LOCALE_PATH+"private_leftNameImg.sub",
							"children" :
							(
								{ "name" : "ItemTypeName", "type" : "text", "text_horizontal_align":"center", "x" : 60, "y" : 5, "text" : uiScriptLocale.PRIVATESHOPSEARCH_ITEMTYPE, "color":GOLD_COLOR },
							),
						},

						## LevelText
						{
							"name" : "LevelImg",
							"type" : "image",
							"x" : 19,
							"y" : 120,
							"image" : LOCALE_PATH+"private_leftNameImg.sub",
							"children" :
							(
								{ "name" : "LevelText", "type" : "text", "text_horizontal_align":"center", "x" : 60, "y" : 5, "text" : uiScriptLocale.PRIVATESHOPSEARCH_LEVEL, "color":GOLD_COLOR },
							),
						},
						## LevelText2
						{ "name" : "LevelText2", "type" : "text", "x" : 65+9, "y" : 158-15, "text" : "~", "fontsize":"LARGE",},
						## minLevelEditLine
						{
							"name" : "minLevelSlot",
							"type" : "image",
							"x" : 21,
							"y" : 140,
							"image" : LOCALE_PATH+"private_leftSlotHalfImg.sub",

							"children" :
							(
								{
									"name" : "SearchFilterItemLevelStart",
									"type" : "editline",
									"x" : 2,
									"y" : 3,
									"width" : 36,
									"height" : 15,
									"input_limit" : 3,
									"only_number" : 1,
									"text" : "1",
								},
							),
						},
						## maxLevelEditLine
						{
						
							"name" : "maxLevelSlot",
							"type" : "image",
							"x" : 21+78,
							"y" : 140,
							"image" : LOCALE_PATH+"private_leftSlotHalfImg.sub",

							"children" :
							(
								{
									"name" : "SearchFilterItemLevelEnd",
									"type" : "editline",
									"x" : 2,
									"y" : 3,
									"width" : 36,
									"height" : 15,
									"input_limit" : 3,
									"only_number" : 1,
									"text" : "120",
								},
							),
						},
						
						{
							"name" : "HorizontalLine1",
							"type" : "line",

							"x" : 18,
							"y" : 225,
							"width" : 129,
							"height" : 0,
							"color" : 0xff777777,
						},
						{
							"name" : "settingsimage",
							"type" : "image",
							"x" : 23,
							"y" : 202,
							"image" : "ibrahimgokalp/offlineshop_22/settings.png",
						},
						{
							"name" : "text0",
							"type" : "text",
							"x" : 55,
							"y" : 205,
							"text" : "Filtre Ayarlarý",
						},

						{
							"name" : "text1",
							"type" : "text",
							"x" : 37,
							"y" : 250,
							"text" : "Eþya Adý Ýle Arama",
						},

						{
							"name" : "text2",
							"type" : "text",
							"x" : 37,
							"y" : 275,
							"text" : "Eþya Türü Ýle Arama",
						},

						{
							"name" : "text3",
							"type" : "text",
							"x" : 37,
							"y" : 300,
							"text" : "Eþya Seviyesi Ýle Arama",
						},

						{
							"name" : "SearchFilterItemYangMin",
							"type" : "editline",
							
							"width" : 130, "height" : 15,
							
							"input_limit" : len("999999999999999999"),
							"only_number" : 1,
							"x" : 258, "y" : 103,
						},

						{
							"name" : "SearchFilterItemYangMax",
							"type" : "editline",
							
							"width" : 130, "height" : 15,
							
							"input_limit" : len("999999999999999999"),
							"only_number" : 1,
							"x" : 258, "y" : 127,
						},

						{
							"name" : "SearchFilterResetFilterButton",
							"type" : "button",
							
							"x" : 400, "y" : 482,
							
							"default_image" : "d:/ymir work/ui/public/large_button_01.sub",
							"over_image" : "d:/ymir work/ui/public/large_button_02.sub",
							"down_image" : "d:/ymir work/ui/public/large_button_03.sub",

							"text" : localeInfo.OFFLINESHOP_SCRIPTFILE_RESET_FILTER_TEXT,
						},

						{
							"name" : "SearchFilterSavePatternButton",
							"type" : "button",
							
							"x" : 139, "y" : 482,
							
							"default_image" : "d:/ymir work/ui/public/large_button_01.sub",
							"over_image" : "d:/ymir work/ui/public/large_button_02.sub",
							"down_image" : "d:/ymir work/ui/public/large_button_03.sub",

							"text" : localeInfo.OFFLINESHOP_SCRIPTFILE_SAVE_AS_PATTERN_TEXT,
						},

						{
							"name" : "SearchFilterAttributeButton1",
							"type" : "button",
							
							"x" : 406, "y" : 35,
							
							"default_image" : "offlineshop/searchfilter/attribute_default.png",
							"over_image" 	: "offlineshop/searchfilter/attribute_over.png",
							"down_image" 	: "offlineshop/searchfilter/attribute_down.png",
						},

						{
							"name" : "SearchFilterAttributeButton2",
							"type" : "button",
							
							"x" : 406, "y" : 35+22,
							
							"default_image" : "offlineshop/searchfilter/attribute_default.png",
							"over_image" 	: "offlineshop/searchfilter/attribute_over.png",
							"down_image" 	: "offlineshop/searchfilter/attribute_down.png",
						},

						{
							"name" : "SearchFilterAttributeButton3",
							"type" : "button",
							
							"x" : 406, "y" : 35+22*2,
							
							"default_image" : "offlineshop/searchfilter/attribute_default.png",
							"over_image" 	: "offlineshop/searchfilter/attribute_over.png",
							"down_image" 	: "offlineshop/searchfilter/attribute_down.png",
						},

						{
							"name" : "SearchFilterAttributeButton4",
							"type" : "button",
							
							"x" : 406, "y" : 35+22*3,
							
							"default_image" : "offlineshop/searchfilter/attribute_default.png",
							"over_image" 	: "offlineshop/searchfilter/attribute_over.png",
							"down_image" 	: "offlineshop/searchfilter/attribute_down.png",
						},

						{
							"name" : "SearchFilterAttributeButton5",
							"type" : "button",
							
							"x" : 406, "y" : 35+22*4,
							
							"default_image" : "offlineshop/searchfilter/attribute_default.png",
							"over_image" 	: "offlineshop/searchfilter/attribute_over.png",
							"down_image" 	: "offlineshop/searchfilter/attribute_down.png",
						},
						{
							"name" : "ContainerScrollBar",
							"type" : "slimscrollbar",

							"x" : 587,
							"y" : 31,
							"size" : 357,
						},
					),
				},

				{
					"name": "ShopSafeboxPage",
					"type": "window",

					"width" :  622,  "height" :  567,

					"x" : 3, "y" : 61,
					"children":
					(
						SAFEBOX_CHILDREN
					),
				},

				{
					"name": "MyOffersPage",
					"type": "window",

					"width" :  622,  "height" :  544,

					"x" : 3, "y" : 61,

				},

				{
					"name": "MyAuction",
					"type": "window",

					"width" :  622,  "height" :  544,

					"x" : 3, "y" : 61,

					"children":
					(

						{
							"name" : "MyAuction_OwnerName",
							"type" : "text",

							"x" : 235+67, "y" : 100-70 - 33,
							"text_horizontal_align" : "center",
							"text" : " noname ",
						},

						{
							"name" : "MyAuction_Duration",
							"type" : "text",

							"x" : 235+67, "y" : 145-91 - 33,
							"text_horizontal_align" : "center",
							"text" : " noname ",
						},

						{
							"name" : "MyAuction_BestOffer",
							"type" : "text",

							"x" : 235+67, "y" : 197-123 - 33,
							"text_horizontal_align" : "center",
							"text" : " noname ",
						},

						{
							"name": "MyAuction_MinRaise",
							"type": "text",

							"x": 235+67, "y": 243-147 - 33,
							"text_horizontal_align": "center",
							"text": " noname ",
						},
					),
				},

				{
					"name": "OpenAuction",
					"type": "window",

					"width" :  622,  "height" :  544,

					"x" : 3, "y" : 61,

					"children":
					(

						{
							"name": "OpenAuctionBackToListButton",
							"type": "button",

							"x": 15, "y": 5,

							"default_image": "d:/ymir work/ui/public/middle_button_01.sub",
							"over_image": "d:/ymir work/ui/public/middle_button_02.sub",
							"down_image": "d:/ymir work/ui/public/middle_button_03.sub",

							"text": localeInfo.OFFLINESHOP_SCRIPTFILE_CLOSE_AUCTION_TEXT,
						},

						{
							"name": "OpenAuction_OwnerName",
							"type": "text",

							"x" : 235+67, "y" : 100-70,
							"text_horizontal_align": "center",
							"text": " noname ",
						},

						{
							"name": "OpenAuction_Duration",
							"type": "text",

							"x" : 235+67, "y" : 145-91,
							"text_horizontal_align": "center",
							"text": " noname ",
						},

						{
							"name": "OpenAuction_BestOffer",
							"type": "text",

							"x": 235+67, "y": 197-123,
							"text_horizontal_align": "center",
							"text": " noname ",
						},

						{
							"name": "OpenAuction_MinRaise",
							"type": "text",

							"x": 235+67, "y": 243-147,
							"text_horizontal_align": "center",
							"text": " noname ",
						},
					),
				},

				{
					"name": "AuctionList",
					"type": "window",

					"width" :  622,  "height" :  544,

					"x" : 3, "y" : 61,
				},
				{
					"name": "CreateAuction",
					"type": "window",

					"width" :  622,  "height" :  544,

					"x" : 3, "y" : 61,

					"children":
					(
						{
							"name": "CreateAuctionDaysInput",
							"type": "text",

							"width": 23, "height": 17,

							"text_horizontal_align" : "center",
							"text" : "0",
							"x": 299, "y": 181,
						},
						{
							"name": "CreateAuctionStartingPriceInput",
							"type": "editline",

							"width": 122, "height": 15,

							"input_limit": 10,
							"only_number": 1,
							"x": 272, "y": 210,
						},

						{
							"name": "CreateAuctionDecreaseDaysButton",
							"type": "button",

							"x": 325,
							"y": 183,

							"default_image" : "offlineshop/button_scroll_right.tga",
							"over_image"     : "offlineshop/button_scroll_right_hover.tga",
							"down_image"     : "offlineshop/button_scroll_right_active.tga",
						},


						{
							"name" : "CreateAuctionIncreaseDaysButton",
							"type" : "button",

							"x" : 267-2,
							"y" : 183,

							"default_image" : "offlineshop/button_scroll_left.tga",
							"over_image"     : "offlineshop/button_scroll_left_hover.tga",
							"down_image"     : "offlineshop/button_scroll_left_active.tga",
						},

						{
							"name" : "CreateAuctionCreateAuctionButton",
							"type" : "button",

							"x" : 211+98, "y" : 267-33,

							"default_image" : "offlineshop/button.tga",
							"over_image" : "offlineshop/button_hover.tga",
							"down_image" : "offlineshop/button_active.tga",

							"text" : localeInfo.OFFLINESHOP_SCRIPTFILE_CREATE_TEXT,
						}
					),
				},
			),
		},
	),
}














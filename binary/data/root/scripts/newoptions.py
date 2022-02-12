import uiScriptLocale

WINDOW_WIDTH = 461
WINDOW_HEIGHT = 362

LOCALE_PATH = "d:/ymir work/ui/game/shopsearchp2p/"
ROOT_PATH = "scripts/options/"

window = {
	"name" : "NewOptionsWindow",

	"x" : 0,
	"y" : 0,

	"style" : ("movable", "float",),

	"width" : WINDOW_WIDTH,
	"height" : WINDOW_HEIGHT,

	"children" : (
		{
			"name" : "NewOptionsBoard",
			"type" : "dragonboard",
			"style" : ("attach",),

			"x" : 0,
			"y" : 0,

			"width" : WINDOW_WIDTH,
			"height" : WINDOW_HEIGHT,
			"children" : (
				{
					"name" : "TitleBar",
					"type" : "titlebar",
					"style" : ("attach",),

					"x" : 8,
					"y" : 7,

					"width" : WINDOW_WIDTH - 15,
					"color" : "yellow",
					"children" :
					(
						{
							"name" : "TitleName",
							"type" : "text",

							"x" : (WINDOW_WIDTH - 15) / 2,
							"y" : 3,

							"text" : uiScriptLocale.GAME_OPTIONS_TITLE,
							"text_horizontal_align":"center"
						},
					),
				},
				{
					"name" : "BackgroundMain",
					"type" : "image",

					"x" : 10,
					"y" : 30,

					"image" : ROOT_PATH + "background_main.png",
					"children" : (
						{
							"name" : "Button_Page_1",
							"type" : "button",

							"x" : 0,
							"y" : 0,

							"text" : uiScriptLocale.GAME_OPTIONS_SYSTEM,
							"default_image" :  ROOT_PATH + "button_menu_01.png",
							"over_image" 	:  ROOT_PATH + "button_menu_02.png",
							"down_image" 	:  ROOT_PATH + "button_menu_03.png",
							"disable_image" :  ROOT_PATH + "button_menu_03.png",
						},

						{
							"name" : "Button_Page_2",
							"type" : "button",

							"x" : 111,
							"y" : 0,

							"text" : uiScriptLocale.GAME_OPTIONS_INTERFACE,
							"default_image" :  ROOT_PATH + "button_menu_01.png",
							"over_image" 	:  ROOT_PATH + "button_menu_02.png",
							"down_image" 	:  ROOT_PATH + "button_menu_03.png",
							"disable_image" :  ROOT_PATH + "button_menu_03.png",
						},

						{
							"name" : "Button_Page_3",
							"type" : "button",

							"x" : 221.5,
							"y" : 0,

							"text" : uiScriptLocale.GAME_OPTIONS_SOCIAL,
							"default_image" :  ROOT_PATH + "button_menu_01.png",
							"over_image" 	:  ROOT_PATH + "button_menu_02.png",
							"down_image" 	:  ROOT_PATH + "button_menu_03.png",
							"disable_image" :  ROOT_PATH + "button_menu_03.png",
						},

						{
							"name" : "Button_Page_4",
							"type" : "button",

							"x" : 332,
							"y" : 0,

							"text" : uiScriptLocale.GAME_OPTIONS_GAME,
							"default_image" :  ROOT_PATH + "button_menu_01.png",
							"over_image" 	:  ROOT_PATH + "button_menu_02.png",
							"down_image" 	:  ROOT_PATH + "button_menu_03.png",
							"disable_image" :  ROOT_PATH + "button_menu_03.png",
						},

						## Pages
						{
							"name" : "Page_1",
							"type" : "image",

							"x" : 9,
							"y" : 31,

							"image" : ROOT_PATH + "bg_data.png",
							"children" : (
								{
									"name" : "AudioSettings_Text",
									"type" : "text",

									"x" : 0,
									"y" : -130,

									"all_align" : "center",
									"text" : uiScriptLocale.GAME_OPTIONS_AUDIO,
									"color" : 0xffffda0a,
								},

								{
									"name" : "SelectMusicButton",
									"type" : "button",

									"x" : 121.5, "y" : 28,

									"default_image" : ROOT_PATH + "ss_input_norm.png",
									"over_image" 	: ROOT_PATH + "ss_input_hover.png",
									"down_image" 	: ROOT_PATH + "ss_input_down.png",
								},

								{
									"name" : "MusicVolumeDown",
									"type" : "button",

									"x" : 25,
									"y" : 32,

									"default_image" : LOCALE_PATH + "btn_prev_default.dds",
									"over_image" 	: LOCALE_PATH + "btn_prev_hover.dds",
									"down_image" 	: LOCALE_PATH + "btn_prev_down.dds",
								},

								{
									"name" : "MusicVolumeUp",
									"type" : "button",

									"x" : 88,
									"y" : 32,

									"default_image" : LOCALE_PATH + "btn_next_default.dds",
									"over_image" 	: LOCALE_PATH + "btn_next_hover.dds",
									"down_image" 	: LOCALE_PATH + "btn_next_down.dds",
								},


								{
									"name" : "MusicVolume",
									"type" : "slotbar_rb",

									"x" : 43,
									"y" : 31,

									"width" : 38,
									"height" : 17,
									"color_left" : 0xff2A2522,
									"color_right" : 0xff433B38,
									"color_center" : 0xff000000,
									"children" : (
										{
											"name" : "MusicVolume_Data",
											"type" : "text",
											"x" : 0,
											"y" : 0,
											"text" : "0",
											"all_align" : "center",
										},
										{
											"name" : "AudioSettingsVolumeMusic_Text",
											"type" : "text",

											"x" : 0,
											"y" : -17,

											"all_align" : "center",
											"text" : uiScriptLocale.GAME_OPTIONS_MUSIC_VOLUME,
											# "color" : 0xffffda0a,
										},
									),
								},

								{
									"name" : "EffectsVolumeDown",
									"type" : "button",

									"x" : 319,
									"y" : 32,

									"default_image" : LOCALE_PATH + "btn_prev_default.dds",
									"over_image" 	: LOCALE_PATH + "btn_prev_hover.dds",
									"down_image" 	: LOCALE_PATH + "btn_prev_down.dds",
								},

								{
									"name" : "EffectsVolumeUp",
									"type" : "button",

									"x" : 382,
									"y" : 32,

									"default_image" : LOCALE_PATH + "btn_next_default.dds",
									"over_image" 	: LOCALE_PATH + "btn_next_hover.dds",
									"down_image" 	: LOCALE_PATH + "btn_next_down.dds",
								},

								{
									"name" : "EffectsVolume",
									"type" : "slotbar_rb",

									"x" : 337,
									"y" : 31,

									"width" : 38,
									"height" : 17,
									"color_left" : 0xff2A2522,
									"color_right" : 0xff433B38,
									"color_center" : 0xff000000,
									"children" : (
										{
											"name" : "EffectsVolume_Data",
											"type" : "text",
											"x" : 0,
											"y" : 0,
											"text" : "0",
											"all_align" : "center",
										},
										{
											"name" : "AudioSettingsVolumeEffects_Text",
											"type" : "text",

											"x" : 0,
											"y" : -17,

											"all_align" : "center",
											"text" : uiScriptLocale.GAME_OPTIONS_EFFECTS_VOLUME,
										},
									),
								},

								# SEPARATE_LINE_AUDIO
								{
									"name" : "Seperate_Line_Audio",
									"type" : "line",

									"x" : 0,
									"y" : 60,

									"width" : 420,
									"height" : 0,

									"color" : 0xff3d3331,
								},

								{
									"name" : "TextureSettings_Text",
									"type" : "text",

									"x" : 0,
									"y" : -70,

									"all_align" : "center",
									"text" : uiScriptLocale.GAME_OPTIONS_TEXTURE,
									"color" : 0xffffda0a,
								},

								{
									"name" : "SelectCameraButton",
									"type" : "button",

									"x" : 20, "y" : 90,

									"default_image" : ROOT_PATH + "ss_input_norm.png",
									"over_image" 	: ROOT_PATH + "ss_input_hover.png",
									"down_image" 	: ROOT_PATH + "ss_input_down.png",
									"children" : (
										{
											"name" : "TextureSettingsSelectCamera_Text",
											"type" : "text",

											"x" : 0,
											"y" : -19,

											"all_align" : "center",
											"text" : uiScriptLocale.GAME_OPTIONS_CAMERA_DISTANCE,
										},
									)
								},

								{
									"name" : "SelectTimeButton",
									"type" : "button",

									"x" : 20, "y" : 132,

									"default_image" : ROOT_PATH + "ss_input_norm.png",
									"over_image" 	: ROOT_PATH + "ss_input_hover.png",
									"down_image" 	: ROOT_PATH + "ss_input_down.png",
									"children" : (
										{
											"name" : "TextureSettingsSelectTime_Text",
											"type" : "text",

											"x" : 0,
											"y" : -19,

											"all_align" : "center",
											"text" : uiScriptLocale.GAME_OPTIONS_TIME_DAY_NIGHT,
										},
									)
								},
								
								{
									"name" : "SelectOptimizationButton",
									"type" : "button",

									"x" : 223,
									"y" : 90,

									"default_image" : ROOT_PATH + "ss_input_norm.png",
									"over_image" 	: ROOT_PATH + "ss_input_hover.png",
									"down_image" 	: ROOT_PATH + "ss_input_down.png",
									"children" : (
										{
											"name" : "Optimization_Text",
											"type" : "text",

											"x" : 0,
											"y" : -19,

											"all_align" : "center",
											"text" : uiScriptLocale.GAME_OPTIONS_OPTIMIZATION,
										},
									)
								},

								{
									"name" : "SelectShadowButton",
									"type" : "button",

									"x" : 223, "y" : 132,

									"default_image" : ROOT_PATH + "ss_input_norm.png",
									"over_image" 	: ROOT_PATH + "ss_input_hover.png",
									"down_image" 	: ROOT_PATH + "ss_input_down.png",
									"children" : (
										{
											"name" : "TextureSettingsShadow_Text",
											"type" : "text",

											"x" : 0,
											"y" : -19,

											"all_align" : "center",
											"text" : uiScriptLocale.GAME_OPTIONS_SHADOW,
										},
									)
								},

								{
									"name" : "SelectEnvModeButton",
									"type" : "button",

									"x" : 20, "y" : 174,

									"default_image" : ROOT_PATH + "ss_input_norm.png",
									"over_image" 	: ROOT_PATH + "ss_input_hover.png",
									"down_image" 	: ROOT_PATH + "ss_input_down.png",
									"children" : (
										{
											"name" : "TextureSettingsSelectEnv_Text",
											"type" : "text",

											"x" : 0,
											"y" : -19,

											"all_align" : "center",
											"text" : uiScriptLocale.GAME_OPTIONS_ENVIRONMENT,
										},
									)
								},

								{
									"name" : "SelectFogButton",
									"type" : "button",

									"x" : 223, "y" : 174,

									"default_image" : ROOT_PATH + "ss_input_norm.png",
									"over_image" 	: ROOT_PATH + "ss_input_hover.png",
									"down_image" 	: ROOT_PATH + "ss_input_down.png",
									"children" : (
										{
											"name" : "TextureSettingsSelectFog_Text",
											"type" : "text",

											"x" : 0,
											"y" : -19,

											"all_align" : "center",
											"text" : uiScriptLocale.GAME_OPTIONS_FOG,
										},
									)
								},

								{
									"name" : "SelectSnowModeButton",
									"type" : "button",

									"x" : 20, "y" : 216,

									"default_image" : ROOT_PATH + "ss_input_norm.png",
									"over_image" 	: ROOT_PATH + "ss_input_hover.png",
									"down_image" 	: ROOT_PATH + "ss_input_down.png",
									"children" : (
										{
											"name" : "TextureSettingsSelectEnv_Text",
											"type" : "text",

											"x" : 0,
											"y" : -19,

											"all_align" : "center",
											"text" : uiScriptLocale.OPTION_SNOW,
										},
									)
								},

								{
									"name" : "Fov_Range_controller",
									"type" : "sliderbar_new",
									"x" : 109,
									"y" : 256,
									"children" : (
										{
											"name" : "Fov_Text",
											"type" : "text",

											"x" : 0,
											"y" : -17,

											"all_align" : "center",
											"text" : uiScriptLocale.GAME_OPTIONS_PERSPECTIVE,
										},
									)
								},
							)
						},

						{
							"name" : "Page_2",
							"type" : "image",

							"x" : 9,
							"y" : 31,

							"image" : ROOT_PATH + "bg_data.png",
							"children" : (
								{
									"name" : "InterfaceSettings_Text",
									"type" : "text",

									"x" : 0,
									"y" : -130,

									"all_align" : "center",
									"text" : uiScriptLocale.GAME_OPTIONS_NAMES_INFOS,
									"color" : 0xffffda0a,
								},

								# ColorNames
								{
									"name" : "SelectColorNamesButton",
									"type" : "button",

									"x" : 20, "y" : 40,

									"default_image" : ROOT_PATH + "ss_input_norm.png",
									"over_image" 	: ROOT_PATH + "ss_input_hover.png",
									"down_image" 	: ROOT_PATH + "ss_input_down.png",
									"children" : (
										{
											"name" : "NameSettingsSelectColorName_Text",
											"type" : "text",

											"x" : 0,
											"y" : -19,

											"all_align" : "center",
											"text" : uiScriptLocale.GAME_OPTIONS_COLOR_NAMES,
										},
									)
								},

								# VisibleNames
								{
									"name" : "SelectVisibleNamesButton",
									"type" : "button",

									"x" : 223, "y" : 40,

									"default_image" : ROOT_PATH + "ss_input_norm.png",
									"over_image" 	: ROOT_PATH + "ss_input_hover.png",
									"down_image" 	: ROOT_PATH + "ss_input_down.png",
									"children" : (
										{
											"name" : "NameSettingsSelectVisibleNames_Text",
											"type" : "text",

											"x" : 0,
											"y" : -19,

											"all_align" : "center",
											"text" : uiScriptLocale.GAME_OPTIONS_NAME_VISIBILITY,
										},
									)
								},

								# ColorNames
								{
									"name" : "SelectMobLevelsButton",
									"type" : "button",

									"x" : 20, "y" : 82,

									"default_image" : ROOT_PATH + "ss_input_norm.png",
									"over_image" 	: ROOT_PATH + "ss_input_hover.png",
									"down_image" 	: ROOT_PATH + "ss_input_down.png",
									"children" : (
										{
											"name" : "NameSettingsSelectMobLevels_Text",
											"type" : "text",

											"x" : 0,
											"y" : -19,

											"all_align" : "center",
											"text" : uiScriptLocale.GAME_OPTIONS_MONSTER_LEVEL,
										},
									)
								},

								# VisibleNames
								{
									"name" : "SelectAggroMonsterButton",
									"type" : "button",

									"x" : 223, "y" : 82,

									"default_image" : ROOT_PATH + "ss_input_norm.png",
									"over_image" 	: ROOT_PATH + "ss_input_hover.png",
									"down_image" 	: ROOT_PATH + "ss_input_down.png",
									"children" : (
										{
											"name" : "NameSettingsSelectAggroMonster_Text",
											"type" : "text",

											"x" : 0,
											"y" : -19,

											"all_align" : "center",
											"text" : uiScriptLocale.GAME_OPTIONS_MONSTER_AGGRESSIVENESS,
										},
									)
								},

								# VisibleChatLine
								{
									"name" : "SelectChatLineButton",
									"type" : "button",

									"x" : 20, "y" : 124,

									"default_image" : ROOT_PATH + "ss_input_norm.png",
									"over_image" 	: ROOT_PATH + "ss_input_hover.png",
									"down_image" 	: ROOT_PATH + "ss_input_down.png",
									"children" : (
										{
											"name" : "NameSettingsSelectChatLine_Text",
											"type" : "text",

											"x" : 0,
											"y" : -19,

											"all_align" : "center",
											"text" : uiScriptLocale.GAME_OPTIONS_CHAT_VISIBILITY,
										},
									)
								},

								# VisibleAttackDamage
								{
									"name" : "SelectAttackDamageButton",
									"type" : "button",

									"x" : 223, "y" : 124,

									"default_image" : ROOT_PATH + "ss_input_norm.png",
									"over_image" 	: ROOT_PATH + "ss_input_hover.png",
									"down_image" 	: ROOT_PATH + "ss_input_down.png",
									"children" : (
										{
											"name" : "NameSettingsSelectAttackDamage_Text",
											"type" : "text",

											"x" : 0,
											"y" : -19,

											"all_align" : "center",
											"text" : uiScriptLocale.GAME_OPTIONS_ATTACK_INFORMATION,
										},
									)
								},

								# VisibleCountryFlags
								{
									"name" : "SelectCountryFlagsButton",
									"type" : "button",

									"x" : 20, "y" : 166,

									"default_image" : ROOT_PATH + "ss_input_norm.png",
									"over_image" 	: ROOT_PATH + "ss_input_hover.png",
									"down_image" 	: ROOT_PATH + "ss_input_down.png",
									"children" : (
										{
											"name" : "NameSettingsSelectCountryFlags_Text",
											"type" : "text",

											"x" : 0,
											"y" : -19,

											"all_align" : "center",
											"text" : uiScriptLocale.GAME_OPTIONS_COUNTRY_FLAG,
										},
									)
								},

								# FontName
								{
									"name" : "SelectDynamicFontButton",
									"type" : "button",

									"x" : 223, "y" : 166,

									"default_image" : ROOT_PATH + "ss_input_norm.png",
									"over_image" 	: ROOT_PATH + "ss_input_hover.png",
									"down_image" 	: ROOT_PATH + "ss_input_down.png",
									"children" : (
										{
											"name" : "DynamicFont_Text",
											"type" : "text",

											"x" : 0,
											"y" : -19,

											"all_align" : "center",
											"text" : uiScriptLocale.GAME_OPTIONS_FONT,
										},
									)
								},

								# HIDE ELEMENTS
								{
									"name" : "HideElements_Text",
									"type" : "text",

									"x" : 0,
									"y" : 65,

									"all_align" : "center",
									"text" : uiScriptLocale.GAME_OPTIONS_HIDE_ELEMENTS,
									"color" : 0xffffda0a,
								},

								{
									"name" : "hide_button_0",
									"type" : "toggle_button",

									"x" : 20,
									"y" : 220,

									"text" : uiScriptLocale.GAME_OPTIONS_HIDE_MOUNTS,

									"default_image" : ROOT_PATH + "button_menu_01.png",
									"over_image" 	: ROOT_PATH + "button_menu_02.png",
									"down_image" 	: ROOT_PATH + "button_menu_active.png",
								},

								{
									"name" : "hide_button_1",
									"type" : "toggle_button",

									"x" : 155,
									"y" : 220,

									"text" : uiScriptLocale.GAME_OPTIONS_HIDE_PETS,

									"default_image" : ROOT_PATH + "button_menu_01.png",
									"over_image" 	: ROOT_PATH + "button_menu_02.png",
									"down_image" 	: ROOT_PATH + "button_menu_active.png",
								},

								{
									"name" : "hide_button_2",
									"type" : "toggle_button",

									"x" : 290,
									"y" : 220,

									"text" : uiScriptLocale.GAME_OPTIONS_HIDE_SHOPS,

									"default_image" : ROOT_PATH + "button_menu_01.png",
									"over_image" 	: ROOT_PATH + "button_menu_02.png",
									"down_image" 	: ROOT_PATH + "button_menu_active.png",
								},

								{
									"name" : "salestext_range_controller",
									"type" : "sliderbar_new",
									"x" : 109,
									"y" : 263,
									"children" : (
										{
											"name" : "SliderText_Text",
											"type" : "text",

											"x" : 0,
											"y" : -17,

											"all_align" : "center",
											"text" : uiScriptLocale.GAME_OPTIONS_SHOPS_NAME_RANGE,
										},
									)
								},

							),
						},

						{
							"name" : "Page_3",
							"type" : "image",

							"x" : 9,
							"y" : 31,

							"image" : ROOT_PATH + "bg_data.png",
							"children" : (
								{
									"name" : "SocialSettings_Text",
									"type" : "text",

									"x" : 0,
									"y" : -130,

									"all_align" : "center",
									"text" : uiScriptLocale.GAME_OPTIONS_BLOCK,
									"color" : 0xffffda0a,
								},

								#BLOCK_FIRST_LINE
								{
									"name" : "block_button_0",
									"type" : "toggle_button",

									"x" : 20,
									"y" : 35,

									"text" : uiScriptLocale.OPTION_BLOCK_EXCHANGE,

									"default_image" : ROOT_PATH + "button_menu_01.png",
									"over_image" 	: ROOT_PATH + "button_menu_02.png",
									"down_image" 	: ROOT_PATH + "button_menu_active.png",
								},

								{
									"name" : "block_button_1",
									"type" : "toggle_button",

									"x" : 155,
									"y" : 35,

									"text" : uiScriptLocale.OPTION_BLOCK_PARTY,

									"default_image" : ROOT_PATH + "button_menu_01.png",
									"over_image" 	: ROOT_PATH + "button_menu_02.png",
									"down_image" 	: ROOT_PATH + "button_menu_active.png",
								},

								{
									"name" : "block_button_2",
									"type" : "toggle_button",

									"x" : 290,
									"y" : 35,

									"text" : uiScriptLocale.OPTION_BLOCK_GUILD,

									"default_image" : ROOT_PATH + "button_menu_01.png",
									"over_image" 	: ROOT_PATH + "button_menu_02.png",
									"down_image" 	: ROOT_PATH + "button_menu_active.png",
								},

								#BLOCK_SECOND_LINE
								{
									"name" : "block_button_3",
									"type" : "toggle_button",

									"x" : 20,
									"y" : 62,

									"text" : uiScriptLocale.OPTION_BLOCK_WHISPER,

									"default_image" : ROOT_PATH + "button_menu_01.png",
									"over_image" 	: ROOT_PATH + "button_menu_02.png",
									"down_image" 	: ROOT_PATH + "button_menu_active.png",
								},

								{
									"name" : "block_button_4",
									"type" : "toggle_button",

									"x" : 155,
									"y" : 62,

									"text" : uiScriptLocale.OPTION_BLOCK_FRIEND,

									"default_image" : ROOT_PATH + "button_menu_01.png",
									"over_image" 	: ROOT_PATH + "button_menu_02.png",
									"down_image" 	: ROOT_PATH + "button_menu_active.png",
								},

								{
									"name" : "block_button_5",
									"type" : "toggle_button",

									"x" : 290,
									"y" : 62,

									"text" : uiScriptLocale.OPTION_BLOCK_PARTY_REQUEST,

									"default_image" : ROOT_PATH + "button_menu_01.png",
									"over_image" 	: ROOT_PATH + "button_menu_02.png",
									"down_image" 	: ROOT_PATH + "button_menu_active.png",
								},

								{
									"name" : "SocialSettings_Text",
									"type" : "text",

									"x" : 0,
									"y" : -30,

									"all_align" : "center",
									"text" : uiScriptLocale.GAME_OPTIONS_ACCOUNT_SETTINGS,
									"color" : 0xffffda0a,
								},

								{
									"name" : "SelectPvPModeButton",
									"type" : "button",

									"x" : 20, "y" : 150,

									"default_image" : ROOT_PATH + "ss_input_norm.png",
									"over_image" 	: ROOT_PATH + "ss_input_hover.png",
									"down_image" 	: ROOT_PATH + "ss_input_down.png",
									"children" : (
										{
											"name" : "SocialSettingsSelectPvPMode_Text",
											"type" : "text",

											"x" : 0,
											"y" : -19,

											"all_align" : "center",
											"text" : uiScriptLocale.OPTION_PVPMODE,
										},
									)
								},

								{
									"name" : "SelectRenderButton",
									"type" : "button",

									"x" : 223, "y" : 150,

									"default_image" : ROOT_PATH + "ss_input_norm.png",
									"over_image" 	: ROOT_PATH + "ss_input_hover.png",
									"down_image" 	: ROOT_PATH + "ss_input_down.png",
									"children" : (
										{
											"name" : "SelectRenderButton_Text",
											"type" : "text",

											"x" : 0,
											"y" : -19,

											"all_align" : "center",
											"text" : uiScriptLocale.OPTION_PREVIEW,
										},
									)
								},

								{
									"name" : "SelectShowNoticeButton",
									"type" : "button",

									"x" : 121.5, "y" : 150 + 50,

									"default_image" : ROOT_PATH + "ss_input_norm.png",
									"over_image" 	: ROOT_PATH + "ss_input_hover.png",
									"down_image" 	: ROOT_PATH + "ss_input_down.png",
									"children" : (
										{
											"name" : "SelectShowNoticeButton_Text",
											"type" : "text",

											"x" : 0,
											"y" : -19,

											"all_align" : "center",
											"text" : uiScriptLocale.OPTION_NOTICE,
										},
									)
								},

							)
						},

						{
							"name" : "Page_4",
							"type" : "image",

							"x" : 9,
							"y" : 31,

							"image" : ROOT_PATH + "bg_data.png",
							"children" : (
								{
									"name" : "GameSettings_Text",
									"type" : "text",

									"x" : 0,
									"y" : -130,

									"all_align" : "center",
									"text" : uiScriptLocale.GAME_OPTIONS_PICKUP_SETTINGS,
									"color" : 0xffffda0a,
								},

								{
									"name" : "SelectPickupModeButton",
									"type" : "button",

									"x" : 121.5, "y" : 40,

									"default_image" : ROOT_PATH + "ss_input_norm.png",
									"over_image" 	: ROOT_PATH + "ss_input_hover.png",
									"down_image" 	: ROOT_PATH + "ss_input_down.png",
									"children" : (
										{
											"name" : "GameSettingsPickupMode_Text",
											"type" : "text",

											"x" : 0,
											"y" : -19,

											"all_align" : "center",
											"text" : uiScriptLocale.GAME_OPTIONS_PICKUP_MODE,
										},

										{
											"name" : "GameSettingsPickupIgnore_Text",
											"type" : "text",

											"x" : 0,
											"y" : 23,

											"all_align" : "center",
											"text" : uiScriptLocale.GAME_OPTIONS_PICKUP_IGNORE,
										},
									)
								},

								#IGNORE_FIRST_LINE
								{
									"name" : "pickup_ignore_button_0",
									"type" : "toggle_button",

									"x" : 20,
									"y" : 85,

									"text" : uiScriptLocale.GAME_OPTIONS_PICKUP_WEAPONS,

									"default_image" : ROOT_PATH + "button_menu_01.png",
									"over_image" 	: ROOT_PATH + "button_menu_02.png",
									"down_image" 	: ROOT_PATH + "button_menu_active.png",
								},

								{
									"name" : "pickup_ignore_button_1",
									"type" : "toggle_button",

									"x" : 155,
									"y" : 85,

									"text" : uiScriptLocale.GAME_OPTIONS_PICKUP_ARMORS,

									"default_image" : ROOT_PATH + "button_menu_01.png",
									"over_image" 	: ROOT_PATH + "button_menu_02.png",
									"down_image" 	: ROOT_PATH + "button_menu_active.png",
								},

								{
									"name" : "pickup_ignore_button_2",
									"type" : "toggle_button",

									"x" : 290,
									"y" : 85,

									"text" : uiScriptLocale.GAME_OPTIONS_PICKUP_HELMET,

									"default_image" : ROOT_PATH + "button_menu_01.png",
									"over_image" 	: ROOT_PATH + "button_menu_02.png",
									"down_image" 	: ROOT_PATH + "button_menu_active.png",
								},

								#IGNORE_SECOND_LINE
								{
									"name" : "pickup_ignore_button_3",
									"type" : "toggle_button",

									"x" : 20,
									"y" : 85+27,

									"text" : uiScriptLocale.GAME_OPTIONS_PICKUP_SHIELD,

									"default_image" : ROOT_PATH + "button_menu_01.png",
									"over_image" 	: ROOT_PATH + "button_menu_02.png",
									"down_image" 	: ROOT_PATH + "button_menu_active.png",
								},

								{
									"name" : "pickup_ignore_button_4",
									"type" : "toggle_button",

									"x" : 155,
									"y" : 85+27,

									"text" : uiScriptLocale.GAME_OPTIONS_PICKUP_WRISTLET,

									"default_image" : ROOT_PATH + "button_menu_01.png",
									"over_image" 	: ROOT_PATH + "button_menu_02.png",
									"down_image" 	: ROOT_PATH + "button_menu_active.png",
								},

								{
									"name" : "pickup_ignore_button_5",
									"type" : "toggle_button",

									"x" : 290,
									"y" : 85+27,

									"text" : uiScriptLocale.GAME_OPTIONS_PICKUP_FOOTS,

									"default_image" : ROOT_PATH + "button_menu_01.png",
									"over_image" 	: ROOT_PATH + "button_menu_02.png",
									"down_image" 	: ROOT_PATH + "button_menu_active.png",
								},

								#IGNORE_THIRT_LINE
								{
									"name" : "pickup_ignore_button_6",
									"type" : "toggle_button",

									"x" : 20,
									"y" : 85+27*2,

									"text" : uiScriptLocale.GAME_OPTIONS_PICKUP_NECKLACE,

									"default_image" : ROOT_PATH + "button_menu_01.png",
									"over_image" 	: ROOT_PATH + "button_menu_02.png",
									"down_image" 	: ROOT_PATH + "button_menu_active.png",
								},

								{
									"name" : "pickup_ignore_button_7",
									"type" : "toggle_button",

									"x" : 155,
									"y" : 85+27*2,

									"text" : uiScriptLocale.GAME_OPTIONS_PICKUP_EAR,

									"default_image" : ROOT_PATH + "button_menu_01.png",
									"over_image" 	: ROOT_PATH + "button_menu_02.png",
									"down_image" 	: ROOT_PATH + "button_menu_active.png",
								},

								{
									"name" : "pickup_ignore_button_8",
									"type" : "toggle_button",

									"x" : 20,
									"y" : 85+27*3,

									"text" : uiScriptLocale.GAME_OPTIONS_PICKUP_ORE,

									"default_image" : ROOT_PATH + "button_menu_01.png",
									"over_image" 	: ROOT_PATH + "button_menu_02.png",
									"down_image" 	: ROOT_PATH + "button_menu_active.png",
								},

								{
									"name" : "pickup_ignore_button_9",
									"type" : "toggle_button",

									"x" : 155,
									"y" : 85+27*3,

									"text" : uiScriptLocale.GAME_OPTIONS_PICKUP_WINE,

									"default_image" : ROOT_PATH + "button_menu_01.png",
									"over_image" 	: ROOT_PATH + "button_menu_02.png",
									"down_image" 	: ROOT_PATH + "button_menu_active.png",
								},
                                
								{
									"name" : "pickup_ignore_button_10",
									"type" : "toggle_button",

									"x" : 290,
									"y" : 85+27*2,

									"text" : uiScriptLocale.GAME_OPTIONS_PICKUP_OTHER,

									"default_image" : ROOT_PATH + "button_menu_01.png",
									"over_image" 	: ROOT_PATH + "button_menu_02.png",
									"down_image" 	: ROOT_PATH + "button_menu_active.png",
								},

								# HIDE_EFFECTS

								{
									"name" : "HideEffects_Text",
									"type" : "text",

									"x" : 0,
									"y" : 65,

									"all_align" : "center",
									"text" : uiScriptLocale.GAME_OPTIONS_EFFECTS_GENERAL,
									"color" : 0xffffda0a,
								},

								{
									"name" : "hide_effect_button_0",
									"type" : "toggle_button",

									"x" : 20,
									"y" : 220,

									"text" : "General",

									"default_image" : ROOT_PATH + "button_menu_01.png",
									"over_image" 	: ROOT_PATH + "button_menu_02.png",
									"down_image" 	: ROOT_PATH + "button_menu_active.png",
								},

								{
									"name" : "hide_effect_button_1",
									"type" : "toggle_button",

									"x" : 155,
									"y" : 220,

									"text" : uiScriptLocale.GAME_OPTIONS_EFFECTS_SKILLS,

									"default_image" : ROOT_PATH + "button_menu_01.png",
									"over_image" 	: ROOT_PATH + "button_menu_02.png",
									"down_image" 	: ROOT_PATH + "button_menu_active.png",
								},

								{
									"name" : "hide_effect_button_2",
									"type" : "toggle_button",

									"x" : 290,
									"y" : 220,

									"text" : uiScriptLocale.GAME_OPTIONS_EFFECTS_BUFFS,

									"default_image" : ROOT_PATH + "button_menu_01.png",
									"over_image" 	: ROOT_PATH + "button_menu_02.png",
									"down_image" 	: ROOT_PATH + "button_menu_active.png",
								},

							)
						},

					),
				},
			),
		},
	),
}
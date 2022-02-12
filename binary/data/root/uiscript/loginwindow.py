PATH_INTERFACE = "d:/ymir work/login/"
CARACTERES_ID = 16
CARACTERES_PW = 16
CARACTERES_PIN = 4

SPACE = 34

righ_side_panel_x = 548
righ_side_panel_y = -92

window = {
	"name" : "LoginWindow",
	"sytle" : ("movable",),

	"x" : 0,
	"y" : 0,

	"width" : SCREEN_WIDTH,
	"height" : SCREEN_HEIGHT,

	"children" :
	(
		{
			"name" : "background", "type" : "expanded_image", "x" : 0, "y" : 0,
			"x_scale" : float(SCREEN_WIDTH) / 1920.0, "y_scale" : float(SCREEN_HEIGHT) / 1080.0,
			"image" : "background.tga",
			"children" :
			(
				{
					"name" : "background_painel", "type" : "expanded_image", "x" : 0, "y" : 70,
					"horizontal_align" : "center", "vertical_align" : "center",
					"image" : PATH_INTERFACE + "imagens_fundo/painel.tga",
					"children" : 
					(
						
						###############@Main login board
						
						#@Login related (editlines + base text)
						{
							"name" : "login_text", "type" : "text", "x" : 0, "y": 15,
							"horizontal_align" : "center", "text_horizontal_align" : "center",
							"fontsize" : "MEDIUM", "text" : "Giris",
						},
						{
							"name" : "ID_EditLine",
							"type" : "editline",

							"x" : 15,
							"y" : -113,
							
							"width" : 219,
							"height" : 26,
							
							"horizontal_align" : "center",
							"vertical_align" : "center",
							"input_limit" : CARACTERES_ID,
							"enable_codepage" : 0,
							"children" :
							[
								{
									"name" : "username_text", "type" : "text", "x" : -15, "y": -25,
									"horizontal_align" : "center", "text_horizontal_align" : "center",
									"fontsize" : "LARGE", "text" : "Kullanici adi",
								},
							],
						},
						{
							"name" : "Password_EditLine",
							"type" : "editline",

							"x" : 15,
							"y" : -48,

							"width" : 219,
							"height" : 26,
							
							"horizontal_align" : "center",
							"vertical_align" : "center",
							"input_limit" : CARACTERES_PW,
							"secret_flag" : 1,
							"enable_codepage" : 0,
							"children" :
							[
								{
									"name" : "password_text", "type" : "text", "x" : -15, "y": -27,
									"horizontal_align" : "center", "text_horizontal_align" : "center",
									"fontsize" : "LARGE", "text" : "Þifre",
								},
							],
						},
						{
							"name" : "PIN_EditLine",
							"type" : "editline",

							"x" : 15,
							"y" : 25,

							"width" : 219,
							"height" : 26,
							
							"horizontal_align" : "center",
							"vertical_align" : "center",
							"input_limit" : CARACTERES_PIN,
							"secret_flag" : 1,
							"enable_codepage" : 0,
							"children" :
							[
								{
									"name" : "password_text", "type" : "text", "x" : -15, "y": -27,
									"horizontal_align" : "center", "text_horizontal_align" : "center",
									"fontsize" : "LARGE", "text" : "Pin",
								},
							],
						},
						#-#
						{
							"name" : "button_login", "type" : "button", "x" : 0, "y" : 70,
							"horizontal_align" : "center", "vertical_align" : "center",
							"default_image" : PATH_INTERFACE + "painel/button_login_1.tga",
							"over_image" : PATH_INTERFACE + "painel/button_login_2.tga",
							"down_image" : PATH_INTERFACE + "painel/button_login_3.tga",
							"text" : "Giriþ yap",
						},
						{
							"name" : "create_account", "type" : "button", "x" : 0, "y" : 50,
							"horizontal_align" : "center", "vertical_align" : "center",
							# "default_image" : PATH_INTERFACE + "painel/create_acc.tga",
							# "over_image" : PATH_INTERFACE + "painel/create_acc.tga",
							# "down_image" : PATH_INTERFACE + "painel/create_acc.tga",
						},
						{
							"name" : "lost_pw", "type" : "button", "x" : 0, "y" : 70,
							"horizontal_align" : "center", "vertical_align" : "center",
							# "default_image" : PATH_INTERFACE + "painel/loss_pw.tga",
							# "over_image" : PATH_INTERFACE + "painel/loss_pw.tga",
							# "down_image" : PATH_INTERFACE + "painel/loss_pw.tga",
						},
						{
							"name" : "exit", "type" : "button", "x" : 0, "y" : 115,
							"horizontal_align" : "center", "vertical_align" : "center",
							"default_image" : PATH_INTERFACE + "redes_etc/exit_1.tga",
							"over_image" : PATH_INTERFACE + "redes_etc/exit_2.tga",
							"down_image" : PATH_INTERFACE + "redes_etc/exit_3.tga",
							"text" : "Oyundan cik",
						},
						#@Social links
						{
							"name" : "rs_1", "type" : "button", "x" : -50, "y" : 80,
							"horizontal_align" : "center", "vertical_align" : "bottom",
							"default_image" : PATH_INTERFACE + "redes_etc/www_1.tga",
							"over_image" : PATH_INTERFACE + "redes_etc/www_2.tga",
							"down_image" : PATH_INTERFACE + "redes_etc/www_3.tga",
						},
						{
							"name" : "rs_2", "type" : "button", "x" : 0, "y" : 80,
							"horizontal_align" : "center", "vertical_align" : "bottom",
							"default_image" : PATH_INTERFACE + "redes_etc/forum_1.tga",
							"over_image" : PATH_INTERFACE + "redes_etc/forum_2.tga",
							"down_image" : PATH_INTERFACE + "redes_etc/forum_3.tga",
						},
						{
							"name" : "rs_3", "type" : "button", "x" : 50, "y" : 80,
							"horizontal_align" : "center", "vertical_align" : "bottom",
							"default_image" : PATH_INTERFACE + "redes_etc/discord_1.tga",
							"over_image" : PATH_INTERFACE + "redes_etc/discord_2.tga",
							"down_image" : PATH_INTERFACE + "redes_etc/discord_3.tga",
						},
						#######################################
						
						###########@CH PAGE
						{
							"name" : "ch_text", "type" : "text", "x" : -247, "y": 75,
							"horizontal_align" : "center", "text_horizontal_align" : "center",
							"fontsize" : "MEDIUM", "text" : "Kanal",
						},
						##CH BUTTON
						{
							"name" : "button_ch_1", "type" : "radio_button", "x" : 47, "y" : -92,
							"vertical_align" : "center",
							"default_image" : PATH_INTERFACE + "painel/but_ch_1.tga",
							"over_image" : PATH_INTERFACE + "painel/but_ch_2.tga",
							"down_image" : PATH_INTERFACE + "painel/but_ch_3.tga",
							"children" :
							(
								{
									"name" : "ch_name_button_1", "type" : "text", "x" : 42, "y" : -7,
									"text_horizontal_align" : "center", "vertical_align" : "center",
									"text" : "",
								},
								{
									"name" : "ch_state_button_1", "type" : "text", "x" : 126, "y" : -7,
									"text_horizontal_align" : "center", "vertical_align" : "center",
									"text" : "",
								},
								{
									"name" : "ch_1_state", "type" : "image", "x" : 30, "y" : 1,
									"horizontal_align" : "right", "vertical_align" : "center",
									"image" : "d:/ymir work/login/state_button_ch/state_none.tga",
								},
							),
						},
						{
							"name" : "button_ch_2", "type" : "radio_button", "x" : 47, "y" : -92+SPACE,
							"vertical_align" : "center",
							"default_image" : PATH_INTERFACE + "painel/but_ch_1.tga",
							"over_image" : PATH_INTERFACE + "painel/but_ch_2.tga",
							"down_image" : PATH_INTERFACE + "painel/but_ch_3.tga",
							"children" :
							(
								{
									"name" : "ch_name_button_2", "type" : "text", "x" : 42, "y" : -7,
									"text_horizontal_align" : "center", "vertical_align" : "center",
									"text" : "",
								},
								{
									"name" : "ch_state_button_2", "type" : "text", "x" : 126, "y" : -7,
									"text_horizontal_align" : "center", "vertical_align" : "center",
									"text" : "",
								},
								{
									"name" : "ch_2_state", "type" : "image", "x" : 30, "y" : 1,
									"horizontal_align" : "right", "vertical_align" : "center",
									"image" : "d:/ymir work/login/state_button_ch/state_none.tga",
								},
							),
						},
						{
							"name" : "button_ch_3", "type" : "radio_button", "x" : 47, "y" : -92+SPACE+SPACE,
							"vertical_align" : "center",
							"default_image" : PATH_INTERFACE + "painel/but_ch_1.tga",
							"over_image" : PATH_INTERFACE + "painel/but_ch_2.tga",
							"down_image" : PATH_INTERFACE + "painel/but_ch_3.tga",
							"children" :
							(
								{
									"name" : "ch_name_button_3", "type" : "text", "x" : 42, "y" : -7,
									"text_horizontal_align" : "center", "vertical_align" : "center",
									"text" : "",
								},
								{
									"name" : "ch_state_button_3", "type" : "text", "x" : 126, "y" : -7,
									"text_horizontal_align" : "center", "vertical_align" : "center",
									"text" : "",
								},
								{
									"name" : "ch_3_state", "type" : "image", "x" : 30, "y" : 1,
									"horizontal_align" : "right", "vertical_align" : "center",
									"image" : "d:/ymir work/login/state_button_ch/state_none.tga",
								},
							),
						},
						{
							"name" : "button_ch_4", "type" : "radio_button", "x" : 47, "y" : -92+SPACE+SPACE+SPACE,
							"vertical_align" : "center",
							"default_image" : PATH_INTERFACE + "painel/but_ch_1.tga",
							"over_image" : PATH_INTERFACE + "painel/but_ch_2.tga",
							"down_image" : PATH_INTERFACE + "painel/but_ch_3.tga",
							"children" :
							(
								{
									"name" : "ch_name_button_4", "type" : "text", "x" : 42, "y" : -7,
									"text_horizontal_align" : "center", "vertical_align" : "center",
									"text" : "",
								},
								{
									"name" : "ch_state_button_4", "type" : "text", "x" : 126, "y" : -7,
									"text_horizontal_align" : "center", "vertical_align" : "center",
									"text" : "",
								},
								{
									"name" : "ch_4_state", "type" : "image", "x" : 30, "y" : 1,
									"horizontal_align" : "right", "vertical_align" : "center",
									"image" : "d:/ymir work/login/state_button_ch/state_none.tga",
								},
							),
						},
						{
							"name" : "button_ch_5", "type" : "radio_button", "x" : 47, "y" : -92+SPACE+SPACE+SPACE+SPACE,
							"vertical_align" : "center",
							"default_image" : PATH_INTERFACE + "painel/but_ch_1.tga",
							"over_image" : PATH_INTERFACE + "painel/but_ch_2.tga",
							"down_image" : PATH_INTERFACE + "painel/but_ch_3.tga",
							"children" :
							(
								{
									"name" : "ch_name_button_5", "type" : "text", "x" : 42, "y" : -7,
									"text_horizontal_align" : "center", "vertical_align" : "center",
									"text" : "",
								},
								{
									"name" : "ch_state_button_5", "type" : "text", "x" : 126, "y" : -7,
									"text_horizontal_align" : "center", "vertical_align" : "center",
									"text" : "",
								},
								{
									"name" : "ch_5_state", "type" : "image", "x" : 30, "y" : 1,
									"horizontal_align" : "right", "vertical_align" : "center",
									"image" : "d:/ymir work/login/state_button_ch/state_none.tga",
								},
							),
						},
						{
							"name" : "button_ch_6", "type" : "radio_button", "x" : 47, "y" : -92+SPACE+SPACE+SPACE+SPACE+SPACE,
							"vertical_align" : "center",
							"default_image" : PATH_INTERFACE + "painel/but_ch_1.tga",
							"over_image" : PATH_INTERFACE + "painel/but_ch_2.tga",
							"down_image" : PATH_INTERFACE + "painel/but_ch_3.tga",
							"children" :
							(
								{
									"name" : "ch_name_button_6", "type" : "text", "x" : 42, "y" : -7,
									"text_horizontal_align" : "center", "vertical_align" : "center",
									"text" : "",
								},
								{
									"name" : "ch_state_button_6", "type" : "text", "x" : 126, "y" : -7,
									"text_horizontal_align" : "center", "vertical_align" : "center",
									"text" : "",
								},
								{
									"name" : "ch_6_state", "type" : "image", "x" : 30, "y" : 1,
									"horizontal_align" : "right", "vertical_align" : "center",
									"image" : "d:/ymir work/login/state_button_ch/state_none.tga",
								},
							),
						},
						#############
						##########@Save Account
						{
							"name" : "save_acc_text", "type" : "text", "x" : 252, "y": 75,
							"horizontal_align" : "center", "text_horizontal_align" : "center",
							"fontsize" : "MEDIUM", "text" : "Hesap",
						},
						## SAVE BUTTON
						{
							"name" : "save_image_1", "type" : "image", "x" : righ_side_panel_x, "y" : righ_side_panel_y,
							"vertical_align" : "center",
							"image" : PATH_INTERFACE + "save_acc/save_acc_bg_0.tga",
							"children" :
							[
								{
									"name" : "save_image_f1", "type" : "image", "x" : 10, "y" : 1,
									"vertical_align" : "center",
									"image" : PATH_INTERFACE + "save_acc/f_image.tga",
									"children" :
									[
										{
											"name" : "f1_text", "type" : "text", "x" : 12, "y": 1,
											"text_horizontal_align" : "center",
											"fontsize" : "MEDIUM", "text" : "F1",
										},
									],
								},
								{
									"name" : "acc_name_text_1", "type" : "text", "x" : -24, "y": -7,
									"horizontal_align" : "center", "vertical_align" : "center", "text_horizontal_align" : "center",
									"fontsize" : "MEDIUM", "text" : "",
								},
								{
									"name" : "save_button_1", "type" : "button", "x" : 80, "y" : 1,
									"vertical_align" : "center", "horizontal_align" : "right",
									"default_image" : PATH_INTERFACE + "save_acc/save_button_1.tga",
									"over_image" : PATH_INTERFACE + "save_acc/save_button_2.tga",
									"down_image" : PATH_INTERFACE + "save_acc/save_button_3.tga",
								},
								{
									"name" : "delete_account_button_1", "type" : "button", "x" : 40, "y" : 1,
									"vertical_align" : "center", "horizontal_align" : "right",
									"default_image" : PATH_INTERFACE + "save_acc/del_button_1.tga",
									"over_image" : PATH_INTERFACE + "save_acc/del_button_2.tga",
									"down_image" : PATH_INTERFACE + "save_acc/del_button_3.tga",
								},
								{
									"name" : "add_acc_button_1", "type" : "button", "x" : 0, "y" : 1,
									"vertical_align" : "center", "horizontal_align" : "center",
									"default_image" : PATH_INTERFACE + "save_acc/add_acc_1.tga",
									"over_image" : PATH_INTERFACE + "save_acc/add_acc_2.tga",
									"down_image" : PATH_INTERFACE + "save_acc/add_acc_3.tga",
								},
							],
						},
						{
							"name" : "save_image_2", "type" : "image", "x" : righ_side_panel_x, "y" : righ_side_panel_y+SPACE*1,
							"vertical_align" : "center",
							"image" : PATH_INTERFACE + "save_acc/save_acc_bg_0.tga",
							"children" :
							[
								{
									"name" : "save_image_f2", "type" : "image", "x" : 10, "y" : 1,
									"vertical_align" : "center",
									"image" : PATH_INTERFACE + "save_acc/f_image.tga",
									"children" :
									[
										{
											"name" : "f2_text", "type" : "text", "x" : 12, "y": 1,
											"text_horizontal_align" : "center",
											"fontsize" : "MEDIUM", "text" : "F2",
										},
									],
								},
								{
									"name" : "acc_name_text_2", "type" : "text", "x" : -24, "y": -7,
									"horizontal_align" : "center", "vertical_align" : "center", "text_horizontal_align" : "center",
									"fontsize" : "MEDIUM", "text" : "",
								},
								{
									"name" : "save_button_2", "type" : "button", "x" : 80, "y" : 1,
									"vertical_align" : "center", "horizontal_align" : "right",
									"default_image" : PATH_INTERFACE + "save_acc/save_button_1.tga",
									"over_image" : PATH_INTERFACE + "save_acc/save_button_2.tga",
									"down_image" : PATH_INTERFACE + "save_acc/save_button_3.tga",
								},
								{
									"name" : "delete_account_button_2", "type" : "button", "x" : 40, "y" : 1,
									"vertical_align" : "center", "horizontal_align" : "right",
									"default_image" : PATH_INTERFACE + "save_acc/del_button_1.tga",
									"over_image" : PATH_INTERFACE + "save_acc/del_button_2.tga",
									"down_image" : PATH_INTERFACE + "save_acc/del_button_3.tga",
								},
								{
									"name" : "add_acc_button_2", "type" : "button", "x" : 0, "y" : 1,
									"vertical_align" : "center", "horizontal_align" : "center",
									"default_image" : PATH_INTERFACE + "save_acc/add_acc_1.tga",
									"over_image" : PATH_INTERFACE + "save_acc/add_acc_2.tga",
									"down_image" : PATH_INTERFACE + "save_acc/add_acc_3.tga",
								},
							],
						},
						{
							"name" : "save_image_3", "type" : "image", "x" : righ_side_panel_x, "y" : righ_side_panel_y+SPACE*2,
							"vertical_align" : "center",
							"image" : PATH_INTERFACE + "save_acc/save_acc_bg_0.tga",
							"children" :
							[
								{
									"name" : "save_image_f3", "type" : "image", "x" : 10, "y" : 1,
									"vertical_align" : "center",
									"image" : PATH_INTERFACE + "save_acc/f_image.tga",
									"children" :
									[
										{
											"name" : "f3_text", "type" : "text", "x" : 12, "y": 1,
											"text_horizontal_align" : "center",
											"fontsize" : "MEDIUM", "text" : "F3",
										},
									],
								},
								{
									"name" : "acc_name_text_3", "type" : "text", "x" : -24, "y": -7,
									"horizontal_align" : "center", "vertical_align" : "center", "text_horizontal_align" : "center",
									"fontsize" : "MEDIUM", "text" : "",
								},
								{
									"name" : "save_button_3", "type" : "button", "x" : 80, "y" : 1,
									"vertical_align" : "center", "horizontal_align" : "right",
									"default_image" : PATH_INTERFACE + "save_acc/save_button_1.tga",
									"over_image" : PATH_INTERFACE + "save_acc/save_button_2.tga",
									"down_image" : PATH_INTERFACE + "save_acc/save_button_3.tga",
								},
								{
									"name" : "delete_account_button_3", "type" : "button", "x" : 40, "y" : 1,
									"vertical_align" : "center", "horizontal_align" : "right",
									"default_image" : PATH_INTERFACE + "save_acc/del_button_1.tga",
									"over_image" : PATH_INTERFACE + "save_acc/del_button_2.tga",
									"down_image" : PATH_INTERFACE + "save_acc/del_button_3.tga",
								},
								{
									"name" : "add_acc_button_3", "type" : "button", "x" : 0, "y" : 1,
									"vertical_align" : "center", "horizontal_align" : "center",
									"default_image" : PATH_INTERFACE + "save_acc/add_acc_1.tga",
									"over_image" : PATH_INTERFACE + "save_acc/add_acc_2.tga",
									"down_image" : PATH_INTERFACE + "save_acc/add_acc_3.tga",
								},
							],
						},
						{
							"name" : "save_image_4", "type" : "image", "x" : righ_side_panel_x, "y" : righ_side_panel_y+SPACE*3,
							"vertical_align" : "center",
							"image" : PATH_INTERFACE + "save_acc/save_acc_bg_0.tga",
							"children" :
							[
								{
									"name" : "save_image_f4", "type" : "image", "x" : 10, "y" : 1,
									"vertical_align" : "center",
									"image" : PATH_INTERFACE + "save_acc/f_image.tga",
									"children" :
									[
										{
											"name" : "f4_text", "type" : "text", "x" : 12, "y": 1,
											"text_horizontal_align" : "center",
											"fontsize" : "MEDIUM", "text" : "F4",
										},
									],
								},
								{
									"name" : "acc_name_text_4", "type" : "text", "x" : -24, "y": -7,
									"horizontal_align" : "center", "vertical_align" : "center", "text_horizontal_align" : "center",
									"fontsize" : "MEDIUM", "text" : "",
								},
								{
									"name" : "save_button_4", "type" : "button", "x" : 80, "y" : 1,
									"vertical_align" : "center", "horizontal_align" : "right",
									"default_image" : PATH_INTERFACE + "save_acc/save_button_1.tga",
									"over_image" : PATH_INTERFACE + "save_acc/save_button_2.tga",
									"down_image" : PATH_INTERFACE + "save_acc/save_button_3.tga",
								},
								{
									"name" : "delete_account_button_4", "type" : "button", "x" : 40, "y" : 1,
									"vertical_align" : "center", "horizontal_align" : "right",
									"default_image" : PATH_INTERFACE + "save_acc/del_button_1.tga",
									"over_image" : PATH_INTERFACE + "save_acc/del_button_2.tga",
									"down_image" : PATH_INTERFACE + "save_acc/del_button_3.tga",
								},
								{
									"name" : "add_acc_button_4", "type" : "button", "x" : 0, "y" : 1,
									"vertical_align" : "center", "horizontal_align" : "center",
									"default_image" : PATH_INTERFACE + "save_acc/add_acc_1.tga",
									"over_image" : PATH_INTERFACE + "save_acc/add_acc_2.tga",
									"down_image" : PATH_INTERFACE + "save_acc/add_acc_3.tga",
								},
							],
						},
						{
							"name" : "save_image_5", "type" : "image", "x" : righ_side_panel_x, "y" : righ_side_panel_y-1+SPACE*4,
							"vertical_align" : "center",
							"image" : PATH_INTERFACE + "save_acc/save_acc_bg_0.tga",
							"children" :
							[
								{
									"name" : "save_image_f5", "type" : "image", "x" : 10, "y" : 1,
									"vertical_align" : "center",
									"image" : PATH_INTERFACE + "save_acc/f_image.tga",
									"children" :
									[
										{
											"name" : "f5_text", "type" : "text", "x" : 12, "y": 1,
											"text_horizontal_align" : "center",
											"fontsize" : "MEDIUM", "text" : "F5",
										},
									],
								},
								{
									"name" : "acc_name_text_5", "type" : "text", "x" : -24, "y": -7,
									"horizontal_align" : "center", "vertical_align" : "center", "text_horizontal_align" : "center",
									"fontsize" : "MEDIUM", "text" : "",
								},
								{
									"name" : "save_button_5", "type" : "button", "x" : 80, "y" : 1,
									"vertical_align" : "center", "horizontal_align" : "right",
									"default_image" : PATH_INTERFACE + "save_acc/save_button_1.tga",
									"over_image" : PATH_INTERFACE + "save_acc/save_button_2.tga",
									"down_image" : PATH_INTERFACE + "save_acc/save_button_3.tga",
								},
								{
									"name" : "delete_account_button_5", "type" : "button", "x" : 40, "y" : 1,
									"vertical_align" : "center", "horizontal_align" : "right",
									"default_image" : PATH_INTERFACE + "save_acc/del_button_1.tga",
									"over_image" : PATH_INTERFACE + "save_acc/del_button_2.tga",
									"down_image" : PATH_INTERFACE + "save_acc/del_button_3.tga",
								},
								{
									"name" : "add_acc_button_5", "type" : "button", "x" : 0, "y" : 1,
									"vertical_align" : "center", "horizontal_align" : "center",
									"default_image" : PATH_INTERFACE + "save_acc/add_acc_1.tga",
									"over_image" : PATH_INTERFACE + "save_acc/add_acc_2.tga",
									"down_image" : PATH_INTERFACE + "save_acc/add_acc_3.tga",
								},
							],
						},
						{
							"name" : "save_image_6", "type" : "image", "x" : righ_side_panel_x, "y" : righ_side_panel_y-1+SPACE*5,
							"vertical_align" : "center",
							"image" : PATH_INTERFACE + "save_acc/save_acc_bg_0.tga",
							"children" :
							[
								{
									"name" : "save_image_f6", "type" : "image", "x" : 10, "y" : 1,
									"vertical_align" : "center",
									"image" : PATH_INTERFACE + "save_acc/f_image.tga",
									"children" :
									[
										{
											"name" : "f6_text", "type" : "text", "x" : 12, "y": 1,
											"text_horizontal_align" : "center",
											"fontsize" : "MEDIUM", "text" : "F6",
										},
									],
								},
								{
									"name" : "acc_name_text_6", "type" : "text", "x" : -24, "y": -7,
									"horizontal_align" : "center", "vertical_align" : "center", "text_horizontal_align" : "center",
									"fontsize" : "MEDIUM", "text" : "",
								},
								{
									"name" : "save_button_6", "type" : "button", "x" : 80, "y" : 1,
									"vertical_align" : "center", "horizontal_align" : "right",
									"default_image" : PATH_INTERFACE + "save_acc/save_button_1.tga",
									"over_image" : PATH_INTERFACE + "save_acc/save_button_2.tga",
									"down_image" : PATH_INTERFACE + "save_acc/save_button_3.tga",
								},
								{
									"name" : "delete_account_button_6", "type" : "button", "x" : 40, "y" : 1,
									"vertical_align" : "center", "horizontal_align" : "right",
									"default_image" : PATH_INTERFACE + "save_acc/del_button_1.tga",
									"over_image" : PATH_INTERFACE + "save_acc/del_button_2.tga",
									"down_image" : PATH_INTERFACE + "save_acc/del_button_3.tga",
								},
								{
									"name" : "add_acc_button_6", "type" : "button", "x" : 0, "y" : 1,
									"vertical_align" : "center", "horizontal_align" : "center",
									"default_image" : PATH_INTERFACE + "save_acc/add_acc_1.tga",
									"over_image" : PATH_INTERFACE + "save_acc/add_acc_2.tga",
									"down_image" : PATH_INTERFACE + "save_acc/add_acc_3.tga",
								},
							],
						},
						##
					),
				},
			),
		},
		## Check
		# {
			# "name" : "Show",
			# "type" : "thinboard",
			# "x" : SCREEN_WIDTH-165,
			# "y" : 0,
			# "width" : 166,
			# "height" : 90,
			# "children" :
			# (
				# {
					# "name" : "Neu_Text",
					# "type" : "text",
					# "x" : 10,
					# "y" : 3,
					# "vertical_align" : "center",
					# "text_vertical_align" : "center",
					# "text" : "Patch Version: 00001",
				# },
				# {
					# "name" : "Neu_Edit",
					# "type" : "text",
					# "x" : 100,
					# "y" : 42,
					# "width" : 120,
					# "height" : 18,
				# },
				# {
					# "name" : "Client_Text",
					# "type" : "text",
					# "x" : 10,
					# "y" : -23,
					# "vertical_align" : "center",
					# "text_vertical_align" : "center",
					# "text" : "Client Version: 00001",
				# },
				# {
					# "name" : "Client_Edit",
					# "type" : "text",
					# "x" : 100,
					# "y" : 17,
					# "width" : 120,
					# "height" : 28,
				# },
			# ),
		# },
	),
}
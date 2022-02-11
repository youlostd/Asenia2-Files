import uiScriptLocale
ACCOUNT_MANAGER_START_X = 30
ACCOUNT_MANAGER_X = 30
ACCOUNT_MANAGER_Y = 12
LOCALE_PATH = uiScriptLocale.LOGIN_PATH
#Big-List
#SERVER_BOARD_HEIGHT = 180 + 390
#SERVER_LIST_HEIGHT = 171 + 350
#Small list like german
SERVER_BOARD_HEIGHT = 220 + 180
SERVER_LIST_HEIGHT = 171 + 180
SERVER_BOARD_WEIGHT = 375 
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
			'name': 'BackGround',
			'type': 'expanded_image',
			'x': 0,
			'y': 0,
			'image': 'd:/ymir work/ui/intro/pattern/Line_Pattern.tga',
			'x_scale': float(SCREEN_WIDTH) / 800.0,
			'y_scale': float(SCREEN_HEIGHT) / 600.0
		},
		## ConnectBoard
		{
			"name" : "ConnectBoard",
			"type" : "thinboard",
			"x" : (SCREEN_WIDTH - 223) / 2,
			"y" : (SCREEN_HEIGHT - 290),
			"width" : 214,
			"height" : 50,

			"children" :
			(
				{
					'name': 'Channel1Button',
					'type': 'radio_button',
					'x': 10,
					'y': 12,
						'default_image': 'd:/ymir work/ui/public/small_button_01.sub',
					'over_image': 'd:/ymir work/ui/public/small_button_02.sub',
					'down_image': 'd:/ymir work/ui/public/small_button_03.sub',
					'text': 'CH 1'
				},
				{
					'name': 'Channel2Button',
					'type': 'radio_button',
					'x': 60,
					'y': 12,
					'default_image': 'd:/ymir work/ui/public/small_button_01.sub',
					'over_image': 'd:/ymir work/ui/public/small_button_02.sub',
					'down_image': 'd:/ymir work/ui/public/small_button_03.sub',
					'text': 'CH 2'
				},
				{
					'name': 'Channel3Button',
					'type': 'radio_button',
					'x': 110,
					'y': 12,
					'default_image': 'd:/ymir work/ui/public/small_button_01.sub',
					'over_image': 'd:/ymir work/ui/public/small_button_02.sub',
					'down_image': 'd:/ymir work/ui/public/small_button_03.sub',
					'text': 'CH 3'
				},
				{
					'name': 'Channel4Button',
					'type': 'radio_button',
					'x': 160,
					'y': 12,
					'default_image': 'd:/ymir work/ui/public/small_button_01.sub',
					'over_image': 'd:/ymir work/ui/public/small_button_02.sub',
					'down_image': 'd:/ymir work/ui/public/small_button_03.sub',
					'text': 'CH 4'
				},
				{
					'name': 'Channel1Status',
					'type': 'text',
					'x': 30,
					'y': 33,
					'text_horizontal_align': 'center',
					'color': 4294281095L,
					'text': '.....'
				},
				{
					'name': 'Channel2Status',
					'type': 'text',
					'x': 80,
					'y': 33,
					'text_horizontal_align': 'center',
					'color': 4294281095L,
					'text': '.....'
				},
				{
					'name': 'Channel3Status',
					'type': 'text',
					'x': 130,
					'y': 33,
					'text_horizontal_align': 'center',
					'color': 4294281095L,
					'text': '.....'
				},
				{
					'name': 'Channel4Status',
					'type': 'text',
					'x': 180,
					'y': 33,
					'text_horizontal_align': 'center',
					'color': 4294281095L,
					'text': '.....'
				},
			),
		},
		## LoginBoard
		{
			'name': 'LoginBoard',
			'type': 'image',
			'x': (SCREEN_WIDTH - 215) / 2 - 1,
			'y': SCREEN_HEIGHT - 230,
			'image': 'locale/tr/ui/login/login_interface.tga',
			
			'children':
			(
				{
					'name': 'ID_EditLine',
					'type': 'editline',
					'x': 77,
					'y': 18,
					'width': 300,
					'height': 300,
					'input_limit': 16,
					'enable_codepage': 0,
					'r': 1.0,
					'g': 1.0,
					'b': 1.0,
					'a': 1.0
				},
				{
					'name': 'Password_EditLine',
					'type': 'editline',
					'x': 77,
					'y': 45,
					'width': 300,
					'height': 300,
					'input_limit': 16,
					'secret_flag': 1,
					'enable_codepage': 0,
					'r': 1.0,
					'g': 1.0,
					'b': 1.0,
					'a': 1.0
				},
				{	
					'name': 'LoginSaveButton',
					'type': 'button',
					'x': 10,
					'y': 68,
					'default_image': 'd:/ymir work/ui/public/large_button_01.sub',
					'over_image': 'd:/ymir work/ui/public/large_button_02.sub',
					'down_image': 'd:/ymir work/ui/public/large_button_03.sub',
					'text': 'Kaydet'
				},
				{
					'name': 'LoginButton',
					'type': 'button',
					'x': 110,
					'y': 68,
					'default_image': 'd:/ymir work/ui/public/large_button_01.sub',
					'over_image': 'd:/ymir work/ui/public/large_button_02.sub',
					'down_image': 'd:/ymir work/ui/public/large_button_03.sub',
					'text': 'Giriþ'
				},
				{	
					'name': 'AccountDeleteButton',
					'type': 'button',
					'x': 10,
					'y': 90,
					'default_image': 'd:/ymir work/ui/public/large_button_01.sub',
					'over_image': 'd:/ymir work/ui/public/large_button_02.sub',
					'down_image': 'd:/ymir work/ui/public/large_button_03.sub',
					'text': 'Kayýt Sil'
				},
				{	
					'name': 'AccountDeleteAbort',
					'type': 'button',
					'x': 10,
					'y': 90,
					'default_image': 'd:/ymir work/ui/public/large_button_01.sub',
					'over_image': 'd:/ymir work/ui/public/large_button_02.sub',
					'down_image': 'd:/ymir work/ui/public/large_button_03.sub',
					'text': 'Geç'
				},
				{	
					'name': 'LoginExitButton',
					'type': 'button',
					'x': 110,
					'y': 90,
					'default_image': 'd:/ymir work/ui/public/large_button_01.sub',
					'over_image': 'd:/ymir work/ui/public/large_button_02.sub',
					'down_image': 'd:/ymir work/ui/public/large_button_03.sub',
					'text': 'Çýkýþ'
				},
			),
		},
		{
			'name': 'ChannelAccountManager',
			'type': 'thinboard',
			'x': (SCREEN_WIDTH - 415) / 2,
			'y': SCREEN_HEIGHT - 100,
			'width': 415,
			'height': 43,
			'children':
			(
				{
					'name': 'Acc1Login',
					'type': 'button',
					'x': ACCOUNT_MANAGER_START_X,
					'y': ACCOUNT_MANAGER_Y,
					'default_image': 'd:/ymir work/ui/public/large_button_01.sub',
					'over_image': 'd:/ymir work/ui/public/large_button_02.sub',
					'down_image': 'd:/ymir work/ui/public/large_button_03.sub',
					'text': ''
				},
				{
					'name': 'Acc2Login',
					'type': 'button',
					'x': ACCOUNT_MANAGER_X + ACCOUNT_MANAGER_START_X * 3,
					'y': ACCOUNT_MANAGER_Y,
					'default_image': 'd:/ymir work/ui/public/large_button_01.sub',
					'over_image': 'd:/ymir work/ui/public/large_button_02.sub',
					'down_image': 'd:/ymir work/ui/public/large_button_03.sub',
					'text': ''
				},
				{
					'name': 'Acc3Login',
					'type': 'button',
					'x': ACCOUNT_MANAGER_X + ACCOUNT_MANAGER_START_X * 6,
					'y': ACCOUNT_MANAGER_Y,
					'default_image': 'd:/ymir work/ui/public/large_button_01.sub',
					'over_image': 'd:/ymir work/ui/public/large_button_02.sub',
					'down_image': 'd:/ymir work/ui/public/large_button_03.sub',
					'text': ''
				},
				{
					'name': 'Acc4Login',
					'type': 'button',
					'x': ACCOUNT_MANAGER_X + ACCOUNT_MANAGER_START_X * 9,
					'y': ACCOUNT_MANAGER_Y,
					'default_image': 'd:/ymir work/ui/public/large_button_01.sub',
					'over_image': 'd:/ymir work/ui/public/large_button_02.sub',
					'down_image': 'd:/ymir work/ui/public/large_button_03.sub',
					'text': ''
				},
				{	
					'name': 'Warning',
					'type': 'text',
					'x': ACCOUNT_MANAGER_X + ACCOUNT_MANAGER_START_X * 6,
					'y': ACCOUNT_MANAGER_Y * 4,
					'text': 'Hesap bilgileriniz client/ayar/hesap.cfg dosyasýnda açýk (þifrelenmemiþ) biçimde tutulur.',
					'text_horizontal_align': 'center'
				},
			),
		},
	),
}
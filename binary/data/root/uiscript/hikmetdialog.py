import uiScriptLocale
ROOT_PATH = "d:/ymir work/ui/public/"
window = {
	"name" : "GuvenlikDialog",
	"x" : 360,
	"y" : 200,
	"style" : ("movable", "float",),
	"width" : 300,
	"height" : 200,
	"children" :
	(
		{
			"name" : "board",
			"type" : "board",
			"x" : 0,
			"y" : 0,
			"width" : 300,
			"height" : 200,
			"children" :
			(
				{
					"name" : "titlebar",
					"type" : "titlebar",
					"style" : ("attach",),
					"x" : 8,
					"y" : 8,
					"width" : 288,
					"color" : "gray",
					"children" :
					(
						{
							"name" : "TitleName",
							"type" : "text",
							"x" : 288/2,
							"y" : 3,
							"text" : "Güvenli Bilgisayar",
							"text_horizontal_align":"center"
						},
					),
				},
				## ThinBoard
				{
					"name": "SecurtiyInfoBoard",
					"type": "thinboard",
					"x": 15,
					"y": 35,
					"width": 260,
					"height": 60,
					"children": 
					(
						## Thin Info 01
						{
							"name" : "info_01",
							"type" : "text",
							"x" : 8,
							"y" : 5,
							"text" : "Güvenli bilgisayar hesabýnýzýn güvenliði",
							"text_horizontal_align":"left"
						},
						## Thin Info 02
						{
							"name" : "info_02",
							"type" : "text",
							"outline" : 1,
							"x" : 8,
							"y" : 17,
							"text" : "için oluþturulmuþ bir sistemdir.",
							"text_horizontal_align":"left"
						},
						## Thin Info 03
						{
							"name" : "info_03",
							"type" : "text",
							"outline" : 1,
							"x" : 8,
							"y" : 29,
							"text" : "Sadece giriþ yaptýðýnýz bilgisayarý kaydedebilirsiniz",
							"text_horizontal_align":"left"
						},
						## Thin Info 04
						{
							"name" : "info_04",
							"type" : "text",
							"outline" : 1,
							"x" : 8,
							"y" : 41,
							"text" : "ve sadece kaydedilen bilgisayardan giriþ yapabilirsiniz.",
							"text_horizontal_align":"left"
						},
					),
				},
				## Guvenli Pc
				{
					"name" : "guvenli_pc",
					"type" : "text",

					"x" : 110,
					"y" : 100,

					"text" : "Güvenli Bilgisayar ",
				},
				{
					"name" : "guvenliaktif_button",
					"type" : "button",

					"x" : 50,
					"y" : 120,

					"text" : "Aktif Et",

					"default_image" : ROOT_PATH + "large_Button_01.sub",
					"over_image" : ROOT_PATH + "large_Button_02.sub",
					"down_image" : ROOT_PATH + "large_Button_03.sub",
				},
				{
					"name" : "guvenlipasif_button",
					"type" : "button",

					"x" : 170,
					"y" : 120,

					"text" : "Pasif Et",

					"default_image" : ROOT_PATH + "large_Button_01.sub",
					"over_image" : ROOT_PATH + "large_Button_02.sub",
					"down_image" : ROOT_PATH + "large_Button_03.sub",
				},
				{
					"name" : "pcekle_button",
					"type" : "button",

					"x" : 50,
					"y" : 160,

					"text" : "Pc Ekle",

					"default_image" : ROOT_PATH + "large_Button_01.sub",
					"over_image" : ROOT_PATH + "large_Button_02.sub",
					"down_image" : ROOT_PATH + "large_Button_03.sub",
				},
				{
					"name" : "pcsil_button",
					"type" : "button",

					"x" : 170,
					"y" : 160,

					"text" : "Pc Sil",

					"default_image" : ROOT_PATH + "large_Button_01.sub",
					"over_image" : ROOT_PATH + "large_Button_02.sub",
					"down_image" : ROOT_PATH + "large_Button_03.sub",
				},
				## Cancel
				{
					"name" : "cancel_button",
					"type" : "button",
					"x" : 0,
					"y" : 185,
					"text" : uiScriptLocale.CANCEL,
					"horizontal_align" : "center",
					"default_image" : ROOT_PATH + "Middle_Button_01.sub",
					"over_image" : ROOT_PATH + "Middle_Button_02.sub",
					"down_image" : ROOT_PATH + "Middle_Button_03.sub",
				},
			),
		},
	),
}

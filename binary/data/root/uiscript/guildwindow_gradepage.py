import uiScriptLocale
import app

ROOT_DIR = "d:/ymir work/ui/game/guild/guildgradepage/"
GUILD_PATH = uiScriptLocale.GUILD_PATH

window = {
	"name" : "GuildWindow_BoardPage",

	"x" : 8,
	"y" : 30,

	"width" : 360,
	"height" : 298,

	"children" :
	(
				
		## GuildGradeTItle
		{
			"name" : "GuildGradeTItle", "type" : "image", "x" : 3, "y" : 1, "image" : GUILD_PATH+"baslik.sub",
		},
		## GradeNumber
		{
			"name" : "GradeNumber", "type" : "image", "x" : 21, "y" : 5, "image" : ROOT_DIR+"GradeNumber.sub",
		},
		## GradeName
		{
			"name" : "GradeName", "type" : "image", "x" : 76, "y" : 5, "image" : ROOT_DIR+"GradeName.sub",
		},
		## InviteAuthority
		{
			"name" : "InviteAuthority", "type" : "image", "x" : 126, "y" : 5, "image" : ROOT_DIR+"InviteAuthority.sub",
		},
		## DriveOutAuthority
		{
			"name" : "DriveOutAuthority", "type" : "image", "x" : 181-28, "y" : 5, "image" : ROOT_DIR+"DriveOutAuthority.sub",
		},
		## NoticeAuthority
		{
			"name" : "NoticeAuthority", "type" : "image", "x" : 238-44, "y" : 5, "image" : ROOT_DIR+"NoticeAuthority.sub",
		},
		## SkillAuthority
		{
			"name" : "SkillAuthority", "type" : "image", "x" : 295-50, "y" : 5, "image" : ROOT_DIR+"SkillAuthority.sub",
		},
		## GuildWar
		{
			"name" : "GuildWar", "type" : "image", "x" : 295-57+60, "y" : 5, "image" : ROOT_DIR+"GuildWar.sub",
		},
	),
}

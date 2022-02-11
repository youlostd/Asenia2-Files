CC		= g++8
CFLAGS	+= -O2 -ggdb3 -g3 -std=c++2a -pipe -mtune=i386 -fstack-protector -static
TARGET	=/usr/game/share/bin/game

############# cryptopp libs and includes
INCDIR	+= -I../../../Extern/cryptopp
LIBDIR	+= -L../../../Extern/cryptopp/lib
LIBS	+= -lcryptopp

############# devIl libs and includes
INCDIR	+= -I../../../Extern/devil
LIBDIR	+= -L../../../Extern/devil/lib
LIBS	+= -lIL -ljasper -lpng -ltiff -ljbig -lmng -llcms -ljpeg -llzma

############# minilzo libs and includes
INCDIR	+= -I../../../Extern/minilzo
LIBDIR	+= -L../../../Extern/minilzo/lib
LIBS	+= -lminilzo -lmd

############# mysql libs and includes
INCDIR	+= -I../../../Extern/mysql
LIBDIR	+= -L../../../Extern/mysql/lib
LIBS	+= -lmysqlclient -pthread

############# libgame libs and includes
INCDIR	+= -I../../libgame/src
LIBDIR	+= -L../../libgame
LIBS	+= -lgame

############# liblua libs and includes
INCDIR	+= -I../../liblua/src
LIBDIR	+= -L../../liblua
LIBS	+= -llua

############# libpoly libs and includes
INCDIR	+= -I../../libpoly/src
LIBDIR	+= -L../../libpoly
LIBS	+= -lpoly

############# libsql libs and includes
INCDIR	+= -I../../libsql/src
LIBDIR	+= -L../../libsql
LIBS	+= -lsql

############# libthecore libs and includes
INCDIR	+= -I../../libthecore/src
LIBDIR	+= -L../../libthecore
LIBS	+= -lthecore

############# boost includes
INCDIR	+= -I../../../Extern/boost

############# hackshield libs and includes
INCDIR	+= -I../../../Extern/hackshield
LIBDIR	+= -L../../../Extern/hackshield/lib
LIBS	+= -lanticpxsvr

############# gtest libs and includes
INCDIR	+= -I../../../Extern/gtest
LIBDIR	+= -L../../../Extern/gtest/lib
LIBS	+= -lgtest

############# libxtrap includes
INCDIR	+= -I../../../Extern/libxtrap

############# Source files
CPP=item_shop.cpp\
	maintenance.cpp\
	New_PetSystem.cpp\
	questlua_petnew.cpp\
	new_switchbot.cpp\
	offline_shop.cpp\
	offlineshop_manager.cpp\
	offlineshop_config.cpp\
	DragonSoul.cpp\
	BattleArena.cpp\
	FSM.cpp\
	MarkConvert.cpp\
	MarkImage.cpp\
	MarkManager.cpp\
	OXEvent.cpp\
	TrafficProfiler.cpp\
	ani.cpp\
	arena.cpp\
	banword.cpp\
	battle.cpp\
	blend_item.cpp\
	block_country.cpp\
	boss_tracking.cpp\
	buffer_manager.cpp\
	building.cpp\
	castle.cpp\
	char.cpp\
	char_affect.cpp\
	char_battle.cpp\
	char_cards.cpp\
	char_change_empire.cpp\
	char_horse.cpp\
	char_item.cpp\
	char_manager.cpp\
	char_pass.cpp\
	char_quickslot.cpp\
	char_resist.cpp\
	char_skill.cpp\
	char_state.cpp\
	PetSystem.cpp\
	cmd.cpp\
	cmd_emotion.cpp\
	cmd_general.cpp\
	cmd_gm.cpp\
	cmd_oxevent.cpp\
	config.cpp\
	constants.cpp\
	crc32.cpp\
	cuberenewal.cpp\
	db.cpp\
	desc.cpp\
	desc_client.cpp\
	desc_manager.cpp\
	desc_p2p.cpp\
	dev_log.cpp\
	dungeon.cpp\
	empire_text_convert.cpp\
	entity.cpp\
	entity_view.cpp\
	event.cpp\
	event_queue.cpp\
	exchange.cpp\
	file_loader.cpp\
	fishing.cpp\
	gm.cpp\
	guild.cpp\
	guild_manager.cpp\
	guild_war.cpp\
	horse_rider.cpp\
	horsename_manager.cpp\
	input.cpp\
	input_auth.cpp\
	input_db.cpp\
	input_login.cpp\
	input_main.cpp\
	input_p2p.cpp\
	input_teen.cpp\
	input_udp.cpp\
	ip_ban.cpp\
	item.cpp\
	item_addon.cpp\
	item_attribute.cpp\
	item_manager.cpp\
	item_manager_idrange.cpp\
	locale.cpp\
	locale_service.cpp\
	log.cpp\
	login_data.cpp\
	lzo_manager.cpp\
	main.cpp\
	marriage.cpp\
	matrix_card.cpp\
	messenger_manager.cpp\
	mining.cpp\
	mob_manager.cpp\
	monarch.cpp\
	motion.cpp\
	over9refine.cpp\
	p2p.cpp\
	packet_info.cpp\
	party.cpp\
	passpod.cpp\
	pcbang.cpp\
	polymorph.cpp\
	priv_manager.cpp\
	pvp.cpp\
	questevent.cpp\
	questlua.cpp\
	questlua_affect.cpp\
	questlua_arena.cpp\
	questlua_ba.cpp\
	questlua_building.cpp\
	questlua_danceevent.cpp\
	questlua_dungeon.cpp\
	questlua_forked.cpp\
	questlua_game.cpp\
	questlua_global.cpp\
	questlua_guild.cpp\
	questlua_horse.cpp\
	questlua_pet.cpp\
	questlua_item.cpp\
	questlua_marriage.cpp\
	questlua_mgmt.cpp\
	questlua_monarch.cpp\
	questlua_npc.cpp\
	questlua_oxevent.cpp\
	questlua_party.cpp\
	questlua_pc.cpp\
	questlua_quest.cpp\
	questlua_target.cpp\
	questlua_mysql.cpp\
	questmanager.cpp\
	questnpc.cpp\
	questpc.cpp\
	refine.cpp\
	TempleOchao.cpp\
	questlua_TempleOchao.cpp\
	regen.cpp\
	safebox.cpp\
	sectree.cpp\
	sectree_manager.cpp\
	sequence.cpp\
	shop.cpp\
	skill.cpp\
	start_position.cpp\
	target.cpp\
	text_file_loader.cpp\
	trigger.cpp\
	utils.cpp\
	vector.cpp\
	war_map.cpp\
	wedding.cpp\
	xmas_event.cpp\
	panama.cpp\
	threeway_war.cpp\
	title.cpp\
	map_location.cpp\
	auth_brazil.cpp\
	BlueDragon.cpp\
	BlueDragon_Binder.cpp\
	DragonLair.cpp\
	questlua_dragonlair.cpp\
	HackShield.cpp\
	HackShield_Impl.cpp\
	char_hackshield.cpp\
	skill_power.cpp\
	affect.cpp\
	SpeedServer.cpp\
	questlua_speedserver.cpp\
	XTrapManager.cpp\
	auction_manager.cpp\
	FileMonitor_FreeBSD.cpp\
	ClientPackageCryptInfo.cpp\
	cipher.cpp\
	buff_on_attributes.cpp\
	dragon_soul_table.cpp\
	group_text_parse_tree.cpp\
	char_dragonsoul.cpp\
	questlua_dragonsoul.cpp\
	shop_manager.cpp\
	shopEx.cpp\
	item_manager_read_tables.cpp\
	new_offlineshop_manager.cpp\
	new_offlineshop.cpp\
	new_offlineshop_safebox.cpp\
	lwt_buffi.cpp\
###############################################################################
CPPOBJS	= $(CPP:%.cpp=$(OBJDIR)/%.o)

OBJDIR	= obj
$(shell if [ ! -d $(OBJDIR) ]; then mkdir $(OBJDIR); fi)

default: $(TARGET)

clean:
	@find . -type f | xargs -n 5 touch
	@rm -rf $(OBJDIR)
	@rm -rf $(TARGET)

$(OBJDIR)/%.o: %.cpp
	@echo -e "\033[0;32m[Ryzen GAME]\033[0m\033[0;33m Derleniyor ->\033[0m" $<
	@$(CC) $(CFLAGS) $(INCDIR) -c $< -o $@

$(TARGET): $(CPPOBJS)
	@echo -e "\033[0;31m ------------------------------------------------ \033[0m"
	@echo -e "\033[0;31m ----Derleme İşlemi Başarılı Bir Şekilde Tamamlandı.---- \033[0m"
	@echo -e "\033[0;31m ----GAME Dosyanız /usr/game/share/bin Dizinine Gönderildi.---- \033[0m"
	@echo -e "\033[0;31m ------------------------------------------------ \033[0m"
	@$(CC) $(CFLAGS) $(LIBDIR) $(CPPOBJS) $(LIBS) -o $(TARGET)

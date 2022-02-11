CC		= g++8
CFLAGS	+= -O2 -ggdb3 -g3 -std=c++2a -pipe -mtune=i386 -fstack-protector -static
TARGET	=/usr/game/share/bin/db

############# mysql libs and includes
INCDIR	+= -I../../../Extern/mysql
LIBDIR	+= -L../../../Extern/mysql/lib
LIBS	+= -lmysqlclient -pthread

############# libgame libs and includes
INCDIR	+= -I../../libgame/src
LIBDIR	+= -L../../libgame
LIBS	+= -lgame

############# libpoly libs and includes
INCDIR	+= -I../../libpoly/src
LIBDIR	+= -L../../libpoly
LIBS	+= -lpoly

############# libsql libs and includes
INCDIR	+= -I../../libsql/src
LIBDIR	+= -L../../libsql
LIBS	+= -lsql

############# libthecore libs and includes
INCDIR	+= -I/../../libthecore/src
LIBDIR	+= -L../../libthecore
LIBS	+= -lthecore

############# source files
CPP=Config.cpp\
	NetBase.cpp\
	Peer.cpp\
	PeerBase.cpp\
	Main.cpp\
	Lock.cpp\
	DBManager.cpp\
	Cache.cpp\
	LoginData.cpp\
	ClientManager.cpp\
	ClientManagerPlayer.cpp\
	ClientManagerLogin.cpp\
	ClientManagerBoot.cpp\
	ClientManagerParty.cpp\
	ClientManagerGuild.cpp\
	GuildManager.cpp\
	HB.cpp\
	PrivManager.cpp\
	MoneyLog.cpp\
	ItemAwardManager.cpp\
	ClientManagerEventFlag.cpp\
	Marriage.cpp\
	Monarch.cpp\
	BlockCountry.cpp\
	ItemIDRangeManager.cpp\
	ClientManagerHorseName.cpp\
	AuctionManager.cpp\
	ProtoReader.cpp\
	CsvReader.cpp\
	ClientManagerOfflineshop.cpp\
	OfflineshopCache.cpp\

############################################################################
OBJDIR	= .obj

$(shell if [ ! -d $(OBJDIR) ]; then mkdir $(OBJDIR); fi)
OBJS	= $(CPP:%.cpp=$(OBJDIR)/%.o)

default: $(TARGET)

clean:
	@find . -type f | xargs -n 5 touch
	@rm -rf $(OBJDIR)
	@rm -rf $(TARGET)

$(OBJDIR)/%.o: %.cpp
	@echo -e "\033[0;32m[Ryzen DB]\033[0m\033[0;33m Derleniyor ->\033[0m" $<
	@$(CC) $(CFLAGS) $(INCDIR) -c $< -o $@

$(TARGET): $(OBJS)
	@echo -e "\033[0;31m ------------------------------------------------ \033[0m"
	@echo -e "\033[0;31m ----Derleme İşlemi Başarılı Bir Şekilde Tamamlandı.---- \033[0m"
	@echo -e "\033[0;31m ----DB Dosyanız /usr/game/share/bin Dizinine Gönderildi.---- \033[0m"
	@echo -e "\033[0;31m ------------------------------------------------ \033[0m"
	@$(CC) $(CFLAGS) $(LIBDIR) $(OBJS) $(LIBS) -o $(TARGET)

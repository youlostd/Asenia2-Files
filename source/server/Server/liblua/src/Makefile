BIN		= ../liblua.a
CXX		= g++8
CFLAGS	+= -O2 -ggdb3 -g3 -std=c++2a -pipe -mtune=i386 -fstack-protector -static

#####################################################
CPP=lapi.cpp\
	lauxlib.cpp\
	lbaselib.cpp\
	lcode.cpp\
	ldblib.cpp\
	ldebug.cpp\
	ldo.cpp\
	ldump.cpp\
	lfunc.cpp\
	lgc.cpp\
	liolib.cpp\
	llex.cpp\
	lmathlib.cpp\
	lmem.cpp\
	loadlib.cpp\
	lobject.cpp\
	lopcodes.cpp\
	lparser.cpp\
	lstate.cpp\
	lstring.cpp\
	lstrlib.cpp\
	ltable.cpp\
	ltablib.cpp\
	ltests.cpp\
	ltm.cpp\
	lundump.cpp\
	lvm.cpp\
	lzio.cpp\

all: $(BIN)

clean:
	@rm -f *.o

%.o: %.cpp
	@echo -e "\033[0;32m[Ryzen LIBLUA]\033[0m\033[0;33m Derleniyor ->\033[0m" $<
	@$(CXX) $(CFLAGS) -c $^ -o $@

$(BIN): $(CPP:%.cpp=%.o)
	@ar cru $(BIN) $^
	@echo -e "\033[0;31m ------------------------------------------------ \033[0m"
	@echo -e "\033[0;31m ----Derleme İşlemi Başarılı Bir Şekilde Tamamlandı.---- \033[0m"
	@echo -e "\033[0;31m ----LIBLUA.A Dosyanız Bir Alt Dizine Gönderildi.---- \033[0m"
	@echo -e "\033[0;31m ------------------------------------------------ \033[0m"
	@ranlib $(BIN)
	@rm -rf *.o

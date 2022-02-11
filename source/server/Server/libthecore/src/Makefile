BIN		= ../libthecore.a
CXX		= g++8
CFLAGS	+= -O2 -ggdb3 -g3 -std=c++2a -pipe -mtune=i386 -fstack-protector -static

#####################################################
CPP=buffer.cpp\
	des.cpp\
	fdwatch.cpp\
	gost.cpp\
	hangul.cpp\
	heart.cpp\
	kstbl.cpp\
	log.cpp\
	main.cpp\
	memcpy.cpp\
	signal.cpp\
	socket.cpp\
	tea.cpp\
	utils.cpp\

all: $(BIN)

clean:
	@rm -f *.o

%.o: %.cpp
	@echo -e "\033[0;32m[Ryzen LIBTHECORE]\033[0m\033[0;33m Derleniyor ->\033[0m" $<
	@$(CXX) $(CFLAGS) -c $^ -o $@

$(BIN): $(CPP:%.cpp=%.o)
	@ar cru $(BIN) $^
	@echo -e "\033[0;31m ------------------------------------------------ \033[0m"
	@echo -e "\033[0;31m ----Derleme İşlemi Başarılı Bir Şekilde Tamamlandı.---- \033[0m"
	@echo -e "\033[0;31m ----LIBTHECORE.A Dosyanız Bir Alt Dizine Gönderildi.---- \033[0m"
	@echo -e "\033[0;31m ------------------------------------------------ \033[0m"
	@ranlib $(BIN)
	@rm -rf *.o

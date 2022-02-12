import app
import item
import app
import os
if app.ENABLE_KILL_STATISTICS:
	KILL_STATISTICS_DATA = [0, 0, 0, 0,]
if app.ENABLE_CUBE_RENEWAL:
	CUBE_RENEWAL_IS_OPENED = 0
	cube_count_items = {}

if app.ENABLE_CHAT_COLOR_SYSTEM:
	chat_color_page_open = 0
	chat_color = ""

if app.AUTO_SHOUT:
	auto_shout_status = 0
	auto_shout_text = ""
	
if app.ENABLE_ITEM_SHOP_SYSTEM:
	ITEM_DATA = {}
	ITEM_SEARCH_DATA = []
	NEW_INGAME_SHOP=1

if app.ENABLE_MESSENGER_BLOCK:
	MESSENGER_KEY = 0
ACCOUNT_NAME = "NoName"
WOLF_MAN = "ENABLED"
WOLF_WOMEN = "DISABLED"

IsSwitchBotActive = 0

OFFLINE_SHOP = 0
IKARUS_SHOP = 0
WONACULAN = 0
CONFIG_HIDE_COSTUME = 0
CONFIG_HIDE_COSTUME_W = 0
CONFIG_HIDE_COSTUME_H = 0
ENABLE_REFINE_ITEM_DESCRIPTION = 1
########################################################
patron_name1 = "Lusifer"
patron_name2 = "Ta� Ork"
patron_name3 = "Azrail"
patron_name4 = "Barones"
patron_name5 = "Beran-Setaou"
patron_name6 = "Razad�r"
patron_name7 = "Nemere"
patron_name8 = "Nefrit Ejderhas�"
patron_name9 = "Patron Slime"
patron_name10 = "Kalpsiz K�t�k"
patron_name11 = "Paskalya Kabusu"
patron_name12 = "Katil Jaws"
patron_name13 = "Patron Charwiss"
patron_name14 = "Okyanuslar Kral�"
patron_name15 = "Ac�mas�z Korsan �efi"
patron_name16 = "Hidra"
patron_name17 = "Bayku� Sefiri"
patron_name18 = "�eytani Duratus"
patron_name19 = "Kadim Alt�n Ejderha"
patron_name20 = "B�y�l� Mantar Efsanesi"
patron_name21 = "Enfekte �l�mc�l sarma��k"
patron_name22 = "Jotun Thyrm"
patron_name23 = "Dev Kabus"
patron_name24 = "Krali�e Meley"
patron_name25 = "Karanl�k Ruh Ejderhas�"
patron_name26 = "Korku Ejderhas�"
patron_name27 = "Kristal Ejderha"
patron_name28 = "Su alt� Kral�"
patron_name29 = "Firavun Keops"
patron_name30 = "Buz �eytan�"
########################################################
patron_model1 = 1093
patron_model2 = 694 
patron_model3 = 2598
patron_model4 = 2092
patron_model5 = 2493
patron_model6 = 6091
patron_model7 = 6191
patron_model8 = 1997
patron_model9 = 852
patron_model10 = 856
patron_model11 = 853
patron_model12 = 1998
patron_model13 = 854
patron_model14 = 4024
patron_model15 = 988
patron_model16 = 3960
patron_model17 = 719
patron_model18 = 851
patron_model19 = 2000
patron_model20 = 858
patron_model21 = 855
patron_model22 = 6418
patron_model23 = 1996
patron_model24 = 6193
patron_model25 = 850
patron_model26 = 1999
patron_model27 = 857
patron_model28 = 859
patron_model29 = 1371
patron_model30 = 1372
########################################################
if app.ENABLE_TITLE_SYSTEM:
	TITLE_SYSTEM_ITEM_1 = 65001
	TITLE_SYSTEM_ITEM_2 = 65002
	TITLE_SYSTEM_ITEM_3 = 65003
	
ENABLE_SLOT_MARKING_EX = 1
INV_WITH_SPLIT = 0
guistat = 0
EXPKAZAN = 0
direkcan = 0
indexver = 0
sarirenk="|cffFDD017|H|h"
normalrenk="|h|r"
kirmizirenk="|cffff0000|Hemp|h"
yesilrenk="|cff00ff00|H|h"
kapaliyesilrenk="|cff00CC00|Hemp|h"
Premium = 0
dungeontimever = 0
open_security = 0
open_security_button = 0
# option
SPEED_BUTTON=0
# 5Lv Oto Bec Sistemi
zihinsel_oto_bec = 0
bedensel_oto_bec = 0
karabuyu_oto_bec = 0
buyulusilah_oto_bec = 0
yakin_oto_bec = 0
uzak_oto_bec = 0
ejderha_oto_bec = 0
iyilestirme_oto_bec = 0
BOSS_TAKIP_WINDOW = 0
CHEQUE_TO_GOLD_INFO_OPEN = 0
CHEQUE_TO_GOLD_INFO_OPEN_2 = 0
AUTO_PICK_UP = 0
INPUT_IGNORE = 0
IN_GAME_SHOP_ENABLE = 1
depolar = 1
CONSOLE_ENABLE = 0
killgui   = 0
YangDrop = 1
pm_button = ""
eigenmod = 0
LOAD_CURTAIN = 0
ITEM_REMOVE_WINDOW_STATUS = 0
ITEM_REMOVE_WINDOW_STATUS_BUTTON = 0
INPUT_IGNORE = 0
CApiSetHide = 0
SendString = ""
PET_EVOLUTION = 0
PET_LEVEL = 0
PET_MAIN = 0
FEEDWIND = 0
SKILL_PET3 = 0
SKILL_PET2 = 0
SKILL_PET1 = 0
LASTAFFECT_POINT = 0
LASTAFFECT_VALUE = 0
EVOLUTION = 0
PET_TOPLAMA_AC = 0
PET_TOPLAMA_HEPSI = 0
PET_TOPLAMA_KOSTUM = 0
PET_TOPLAMA_BK = 0
PET_TOPLAMA_RUH_TASI = 0
PET_TOPLAMA_75 = 0
PET_TOPLAMA_CELIK = 0
PET_TOPLAMA_TAKI = 0
PET_TOPLAMA_ARTI = 0
PET_TOPLAMA_SANDIK = 0
PET_TOPLAMA_COR = 0
PET_TOPLAMA_COR = 0
PET_TOPLAMA_EVENT = 0
ENABLE_PET_PICKUP = 0
PET_TOPLAMA_AKTIF = 0
PET_TOPLANANLAR = []

DELETE_SELL_OPEN = 0

if app.ENABLE_SEND_TARGET_INFO:
	MONSTER_INFO_DATA = {}
PVPMODE_ENABLE = 1
PVPMODE_TEST_ENABLE = 0
PVPMODE_ACCELKEY_ENABLE = 1
PVPMODE_ACCELKEY_DELAY = 0.5
PVPMODE_PROTECTED_LEVEL = 30

FOG_LEVEL0 = 999999.0
FOG_LEVEL1 = 999999.0
FOG_LEVEL2 = 999999.0
FOG_LEVEL = FOG_LEVEL0
FOG_LEVEL_LIST=[FOG_LEVEL0, FOG_LEVEL1, FOG_LEVEL2]		

CAMERA_MAX_DISTANCE_SHORT = 2500.0
CAMERA_MAX_DISTANCE_LONG = 5050.0
CAMERA_MAX_DISTANCE_LIST=[CAMERA_MAX_DISTANCE_SHORT, CAMERA_MAX_DISTANCE_LONG]
CAMERA_MAX_DISTANCE = CAMERA_MAX_DISTANCE_SHORT

CHRNAME_COLOR_INDEX = 0
MAINTENANCEADMIN_OPEN = 0
AkiraMenu = {0:0,1:0,2:0,3:0,4:0,5:1}

ENVIRONMENT_NIGHT="d:/ymir work/environment/moonlight04.msenv"
ENVIRONMENT_HERO="d:/ymir work/environment/white_sky_1k.msenv"
ibrahim_sky_001="d:/ymir work/environment/cloudymonth_kf.msenv"
ibrahim_sky_002="d:/ymir work/environment/cloudysun_kf.msenv"
ibrahim_sky_003="d:/ymir work/environment/eveningsun_kf.msenv"
ibrahim_sky_004="d:/ymir work/environment/foggysunset_kf.msenv"
ibrahim_sky_005="d:/ymir work/environment/hazysun_kf.msenv"
ibrahim_sky_006="d:/ymir work/environment/overcastday_kf.msenv"
ibrahim_sky_007="d:/ymir work/environment/rainyday_kf.msenv"
ibrahim_sky_008="d:/ymir work/environment/summersun_kf.msenv"
ibrahim_sky_009="d:/ymir work/environment/sunhorizon_kf.msenv"
ibrahim_sky_010="d:/ymir work/environment/sunset_kf.msenv"
ibrahim_aksam="d:/ymir work/environment/moonlight05.msenv"
Night = 0
biyologinfo = {
						# 0	: ["biyolog", "G�n", "Ba�lang�� Saati", Ba�lang�� Dakika, Biti� Saati, "Biti� Dakika","�tem","T�r","Drop","Seviye"],
						0	: ["G�rev Yok", "Yok", "Yok", "Yok"],
						30006	: ["Ork Di�i", "Hareket H�z� +%10", "Yok", "Yok"],
						30047	: ["Lanet Kitab�", "Sald�r� H�z� +%5", "Yok", "Yok"],
						30015	: ["�eytan Hat�ras�", "Savunma +60", "Yok", "Yok"],
						30050	: ["Buz Topu", "Sald�r� De�eri +50", "Yok", "Yok"],
						30165	: ["Zelkova Dal�", "Hareket H�z� +%11", "Hasar Azaltma +%10", "Yok"],
						30166	: ["Tugyis Tabelas�", "Sald�r� H�z� +%6", "Sald�r� De�eri +%10", "Yok"],
						30167	: ["Krm. Hayalet A�a� Dal�", "Karakter Savunmas� +%10", "Yok", "Yok"],
						30168	: ["Liderin Notlar�", "Yar� �nsanlara Kar�� G�� +%8", "Yok", "Yok"],
						30251	: ["Kemg�z M�cevheri", "Max HP +1000", "Savunma +120", "Sald�r� De�eri +50"],
						30252	: ["Bilgelik M�cevheri", "Max HP +1100", "Savunma +140", "Sald�r� De�eri +60"],
						31082	: ["Razador �z�", "Ortalama Zarar +%10", "Yok", "Yok"],
						31089	: ["Nemere �z�", "Beceri Hasar� +%10", "Yok", "Yok"],
						31080	: ["Meley �z�", "Canavarlara kar�� g�� +%40", "Yok", "Yok"],
						31090	: ["Hydra �z�", "Metin ta�lar�na kar�� g�� +%40", "Yok", "Yok"],
						30613	: ["Jotun �z�", "Patronlara Kar�� g�� +%40", "Yok", "Yok"],
						31034	: ["Keops Totemi", "B�y�l�/Yak�n d�v�� sald�r� +%25", "Yok", "Yok"]
						# 31081	: ["Midas�n Kalbi", "Yar� �nsanlara Kar�� G�� +%5", "Yok", "Yok"],
						# 31061	: ["Ent Dal�", "Canavar Savunmas� +%5", "Yok", "Yok"],
						# 31040	: ["Kolezyum �d�l�", "Canavarlara Kar�� G�� +%5", "Yok", "Yok"],
						# 31034	: ["Anglar Totemi", "Canl�l�k-Zeka-G��-�eviklik +12", "Yok", "Yok"]
					}
dungeontimerinfo = {
					# 0	: ["Ba�l�k", "T�r", "Parti/Tek", MinLv, MaxLv, "GrupLimit","Tekrarlama", "Tamamlama S�resi", "Giri�", "G��l�Efsun", "Savunma", "Giri� �temi", "GuiResim","playertime"],
					# 0	: ["Ba�l�k", "T�r", "Parti/Tek", MinLv, MaxLv, "GrupLimit","Tekrarlama", "Tamamlama S�resi", "Giri�", "G��l�Efsun", "Savunma", "Giri� �temi", "GuiResim","playertime"],
					0	: ["�eytan Kulesi", "Zindan", "Bireysel", 55, 120, "Grup yok", "15 Dakika", "30 Dakika", "Hwang Tap�na��", "Karanl�k T�ls�m�", "Karanl�k T�ls�m�", 0, "dungeontimer/seytan.tga", "0",0,patron_model1,patron_name1],
					1	: ["Ork Labirenti", "Zindan", "Bireysel", 70, 120, "Grup yok", "30 Dakika", "30 Dakika", "Ork Labirenti Giri�i", "Karanl�k T�ls�m�", "Karanl�k T�ls�m�", 71130, "dungeontimer/ork.tga", "0",1,patron_model2,patron_name2],
					2	: ["�eytan Katakombu", "Zindan", "Bireysel", 75, 120, "Grup yok", "45 Dakika", "30 Dakika", "Hwang Tap�na��", "Karanl�k T�ls�m�", "Karanl�k T�ls�m�", 30319, "dungeontimer/catacomb.tga", "0",1,patron_model3,patron_name3],
					3	: ["Baronesin �ni", "Zindan", "Bireysel", 90, 120, "Grup yok", "1 Saat", "30 Dakika", "�r�mcek Zindan� 3.Kat", "R�zgar T�ls�m�", "R�zgar T�ls�m�", 30324, "dungeontimer/barones.tga", "0",1,patron_model4,patron_name4],
					4	: ["Kristal Oda", "Zindan", "Tek/Grup", 95, 120, "1-8", "30 Dakika", "30 Dakika", "S�rg�n Ma�aras� 2.Kat", "�im�ek T�ls�m�", "�im�ek T�ls�m�", 30179, "dungeontimer/dragonlair.tga", "0",3,patron_model5,patron_name5],
					5	: ["K�rm�z� Ejderha Kalesi", "Zindan", "Bireysel", 100, 120, "Grup yok", "1 Saat", "30 Dakika", "Doyum Paper", "Ate� T�ls�m�", "Ate� T�ls�m�", 71174, "dungeontimer/razador.tga", "0",1,patron_model6,patron_name6],
					6	: ["Nemerenin G�zetleme Kulesi", "Zindan", "Bireysel", 100, 120, "Grup yok", "1 Saat", "30 Dakika", "Sohan Da��", "Buz T�ls�m�", "Buz T�ls�m�", 71175, "dungeontimer/nemere.tga", "0",1,patron_model7,patron_name7],
					7	: ["Nefrit Ma�aras�", "Zindan", "Bireysel", 105, 120, "Grup Yok", "1.5 Saat", "30 Dakika", "K�z�l Orman", "R�zgar T�ls�m�", "R�zgar T�ls�m�", 31327, "dungeontimer/nefrit.tga", "0",1,patron_model8,patron_name8],
					8	: ["Slime Zindan�", "Zindan", "Bireysel", 110, 120, "Grup Yok", "1.5 Saat", "30 Dakika", "K�z�l Orman", "R�zgar T�ls�m�", "R�zgar T�ls�m�", 31332, "dungeontimer/slime.tga", "0",1,patron_model9,patron_name9],
					9	: ["Sevgililer Bah�esi", "Zindan", "Bireysel", 110, 120, "Grup Yok", "2 Saat", "30 Dakika", "Sevgililer Bah�esi Giri�i", "Toprak T�ls�m�", "Toprak T�ls�m�", 31336, "dungeontimer/sevgili.tga", "0",1,patron_model10,patron_name10],
					10	: ["Paskalya Zindan�", "Zindan", "Bireysel", 110, 120, "Grup Yok", "2 Saat", "30 Dakika", "Paskalya Zindan� Giri�i", "Toprak T�ls�m�", "Toprak T�ls�m�", 31333, "dungeontimer/paskalya.tga", "0",1,patron_model11,patron_name11],
					11	: ["Jaws Zindan�", "Zindan", "Bireysel", 115, 120, "Grup Yok", "2 Saat", "30 Dakika", "Jaws Zindan� Giri�i", "�im�ek T�ls�m�", "�im�ek T�ls�m�", 31328, "dungeontimer/jaws.tga", "0",1,patron_model12,patron_name12],
					12	: ["K�t�phane Zindan�", "Zindan", "Bireysel", 115, 120, "Grup Yok", "2 Saat", "30 Dakika", "K�t�phane Zindan� Giri�i", "Toprak T�ls�m�", "Toprak T�ls�m�", 31334, "dungeontimer/kitap.tga", "0",1,patron_model13,patron_name13],
					13	: ["Kay�p Ada ", "Zindan", "Bireysel", 120, 120, "Grup Yok", "2.5 Saat", "30 Dakika", "Kay�p Ada Sahili ", "Buz T�ls�m�", "Buz T�ls�m�", 30744, "dungeontimer/ada.tga", "0",1,patron_model14,patron_name14],
					14	: ["Derin Okyanus Zindan� ", "Zindan", "Bireysel", 120, 120, "Grup Yok", "2.5 Saat", "30 Dakika", "Derin Okyanus Zindan� Giri�i", "Buz T�ls�m�", "Buz T�ls�m�", 31324, "dungeontimer/derin.tga", "0",1,patron_model15,patron_name15],
					15	: ["Hidra Gemi Savunmas� (R Seviye 1)", "Zindan", "Bireysel", 120, 120, "Grup yok", "3 Saat", "30 Dakika", "Ejderha Ate�i Burnu", "�im�ek T�ls�m�", "�im�ek T�ls�m�", 71180, "dungeontimer/hydra.tga", "0",1,patron_model16,patron_name16],
					16	: ["Bayku� Zindan� (R Seviye 1) ", "Zindan", "Bireysel", 120, 120, "Grup yok", "3 Saat", "30 Dakika", "Bayku� Zindan� Giri�i", "R�zgar T�ls�m�", "R�zgar T�ls�m�", 30713, "dungeontimer/baykus.tga", "0",1,patron_model17,patron_name17],
					17	: ["Duratus Zindan� (R Seviye 3) ", "Zindan", "Bireysel",120, 120, "Grup Yok", "3.5 Saat", "30 Dakika", "Duratus Zindan� Giri�i", "Karanl�k T�ls�m�", "Karanl�k T�ls�m�", 31331, "dungeontimer/duratus.tga", "0",1,patron_model18,patron_name18],
					18	: ["Alt�n Ejderha Zindan� (R Seviye 3) ", "Zindan", "Bireysel", 120, 120, "Grup yok", "3.5 Saat", "30 Dakika", "Alt�n Ejderha Zindan� Giri�i", "Toprak T�ls�m�", "Toprak T�ls�m�", 31330, "dungeontimer/gold.tga", "0",1,patron_model19,patron_name19],
					19	: ["Mantar Zindan� (R Seviye 5) ", "Zindan", "Bireysel",120, 120, "Grup Yok", "4 Saat", "30 Dakika", "Mantar Zindan� Giri�i", "R�zgar T�ls�m�", "R�zgar T�ls�m�", 31338, "dungeontimer/mantar.tga", "0",1,patron_model20,patron_name20],
					20	: ["Enfekte Bah�e (R Seviye 5) ", "Zindan", "Bireysel", 120, 120, "Grup yok", "4 Saat", "30 Dakika", "Enfekte Bah�e Giri�i", "Toprak T�ls�m�", "Toprak T�ls�m�", 31335, "dungeontimer/enfekte.tga", "0",1,patron_model21,patron_name21],
					21	: ["Jotun Thrym (R Seviye 7)", "Zindan", "Bireysel", 120, 120, "Grup", "5 Saat", "30 Dakika", "B�y�l� Orman", "Toprak T�ls�m�", "Toprak T�ls�m�", 71178, "dungeontimer/jotun.tga", "0",1,patron_model22,patron_name22],
					22	: ["Kabus zindan� (R Seviye 7)", "Zindan", "Bireysel", 120, 120, "Grup Yok", "5 Saat", "30 Dakika", "Kabus Zindan� Giri�i", "R�zgar T�ls�m�", "R�zgar T�ls�m�", 31326, "dungeontimer/kabus.tga", "0",1,patron_model23,patron_name23],
					23	: ["Meley'in �ni (R Seviye 10)", "Zindan", "Bireysel", 120, 120, "Grup yok", "6 Saat", "30 Dakika", "Doyum Paper", "Ate� T�ls�m�", "Ate� T�ls�m�", 71179, "dungeontimer/meley.tga", "0",1,patron_model24,patron_name24],
					24	: ["Ruhlar Ma�aras� (R Seviye 10)", "Zindan", "Bireysel", 120, 120, "Grup yok", "6 Saat", "30 Dakika", "Ruhlar Ma�aras� Giri�i", "Karanl�k T�ls�m", "Ate� T�ls�m", 31325, "dungeontimer/ruhlar.tga", "0",1,patron_model25,patron_name25],
					25	: ["Korku Zindan� (R Seviye 10)", "Zindan", "Bireysel", 120, 120, "Grup yok", "6 Saat", "30 Dakika", "Korku Zindan� Giri�i", "Karanl�k T�ls�m", "Karanl�k T�ls�m", 31329, "dungeontimer/korku.tga", "0",1,patron_model26,patron_name26],
					26	: ["Kristal Zindan (R Seviye 10)", "Zindan", "Bireysel", 120, 120, "Grup yok", "6 Saat", "30 Dakika", "Kristal Zindan� Giri�i", "Ate� T�ls�m�", "Ate� T�ls�m�", 31337, "dungeontimer/krsital.tga", "0",1,patron_model27,patron_name27],
					27	: ["Su alt� Zindan� (R Seviye 10)", "Zindan", "Bireysel", 120, 120, "Grup yok", "6 Saat", "30 Dakika", "Su alt� Zindan� Giri�i", "Buz T�ls�m�", "Buz T�ls�m�", 31339, "dungeontimer/water.tga", "0",1,patron_model28,patron_name28],
					28	: ["Firavun KEOPS Piramidi (R Seviye 10)", "Zindan", "Bireysel", 120, 120, "Grup yok", "8 Saat", "30 Dakika", "Antik ��l", "Toprak T�ls�m�", "Toprak T�ls�m�", 31340, "dungeontimer/piramid.tga", "0",1,patron_model29,patron_name29],
					29	: ["Buz Cehennemi Kulesi (R Seviye 10)", "Zindan", "Bireysel", 120, 120, "Grup yok", "9 Saat", "30 Dakika", "Buz Cehennemi Kulesi", "Buz T�ls�m�", "Buz T�ls�m�", 31345, "dungeontimer/buz.tga", "0",1,patron_model30,patron_name30],

					}

petisim = {
					1	: ["Maymun"],
					2	: ["�r�mcek"],
					3	: ["Razador"],
					4	: ["Nemere"],
					5	: ["Mavi Ejderha"],
					6	: ["K�z�l Ejderha"]
					}

petnum = {
					55701	: [34041, 34042],
					55702	: [34045, 34046],
					55703	: [34049, 34050],
					55704	: [34053, 34054],
					55705	: [34036, 34037],
					55706	: [34064, 34065],
					55707	: [34073, 34074],
					55708	: [34075, 34076],
					55709	: [34080, 34081],
					55710	: [34082, 34083]
					}

petskiller = {
					-1	: ["Kullan�lamaz"],
					0	: ["Beceri ��renilmemi�"],
					1	: ["Dayan�kl�l�k (Sava���)"],
					2	: ["Dayan�kl�l�k (Sura)"],
					3	: ["Dayan�kl�l�k (Ninja)"],
					4	: ["Dayan�kl�l�k (�aman)"],
					5	: ["Dayan�kl�l�k (Lycan)"],
					6	: ["Berserker"],
					7	: ["B�y� Bozma"],
					8	: ["H�zland�rma"],
					9	: ["Talim"],
					10	: ["Yenileme"],
					11	: ["Vampir"],
					12	: ["Hayaletler"],
					13	: ["Engel"],
					14	: ["Yans�tma"],
					15	: ["Yang D��me"],
					16	: ["Menzil"],
					17	: ["Yenilmezlik"],
					18	: ["�yile�tirme"],
					19	: ["Canavar Avc�s�"],
					20	: ["Tecr�be"],
					21	: ["Yar� �nsan"]
					}
					
missions_bp = {}
info_missions_bp = {}
rewards_bp = {}
final_rewards = []
size_battle_pass = 0
status_battle_pass = 0
				
#Decoration Shop
decorationshop = ""
decoration_w = 0
#Decoration Shop
# constant
HIGH_PRICE = 500000
MIDDLE_PRICE = 50000
ERROR_METIN_STONE = 28960
SUB2_LOADING_ENABLE = 1
EXPANDED_COMBO_ENABLE = 1
CONVERT_EMPIRE_LANGUAGE_ENABLE = 1
USE_ITEM_WEAPON_TABLE_ATTACK_BONUS = 0
ADD_DEF_BONUS_ENABLE = 1


USE_SKILL_EFFECT_UPGRADE_ENABLE = 1

VIEW_OTHER_EMPIRE_PLAYER_TARGET_BOARD = 1
GUILD_MONEY_PER_GSP = 100
GUILD_WAR_TYPE_SELECT_ENABLE = 1
TWO_HANDED_WEAPON_ATT_SPEED_DECREASE_VALUE = 0

HAIR_COLOR_ENABLE = 1
ARMOR_SPECULAR_ENABLE = 1
WEAPON_SPECULAR_ENABLE = 1
SEQUENCE_PACKET_ENABLE = 1
KEEP_ACCOUNT_CONNETION_ENABLE = 1
MINIMAP_POSITIONINFO_ENABLE = 0
CONVERT_EMPIRE_LANGUAGE_ENABLE = 0
USE_ITEM_WEAPON_TABLE_ATTACK_BONUS = 0
ADD_DEF_BONUS_ENABLE = 0
LOGIN_COUNT_LIMIT_ENABLE = 0
PVPMODE_PROTECTED_LEVEL = 15
TWO_HANDED_WEAPON_ATT_SPEED_DECREASE_VALUE = 10

isItemQuestionDialog = 0
refine_probality_type = 0
if app.ENABLE_REFINE_RENEWAL:
	IS_AUTO_REFINE = False
	IS_AUTO_NPC = False
	AUTO_REFINE_TYPE = 0
	AUTO_REFINE_DATA = {
		"ITEM" : [-1, -1],
		"NPC" : [0, -1, -1, 0]
	}

def GET_ITEM_QUESTION_DIALOG_STATUS():
	global isItemQuestionDialog
	return isItemQuestionDialog

def SET_ITEM_QUESTION_DIALOG_STATUS(flag):
	global isItemQuestionDialog
	isItemQuestionDialog = flag

import app
import net

########################

def SET_DEFAULT_FOG_LEVEL():
	global FOG_LEVEL
	app.SetMinFog(FOG_LEVEL)

def SET_FOG_LEVEL_INDEX(index):
	global FOG_LEVEL
	global FOG_LEVEL_LIST
	try:
		FOG_LEVEL=FOG_LEVEL_LIST[index]
	except IndexError:
		FOG_LEVEL=FOG_LEVEL_LIST[0]
	app.SetMinFog(FOG_LEVEL)

def GET_FOG_LEVEL_INDEX():
	global FOG_LEVEL
	global FOG_LEVEL_LIST
	return FOG_LEVEL_LIST.index(FOG_LEVEL)

########################


## OnMouseWheel
WHEEL_TO_SCROLL_MIN = 5
WHEEL_TO_SCROLL_MAX = 40
WHEEL_TO_SCROLL_DEFAULT = 13
WHEEL_VALUE = WHEEL_TO_SCROLL_DEFAULT

def WHEEL_TO_SCROLL(wheel, custom = None):
	return -wheel * ((custom if custom else WHEEL_VALUE) / WHEEL_TO_SCROLL_MIN)

def WHEEL_TO_SCROLL_SLOW(wheel):
	return -wheel * max(1, WHEEL_VALUE / WHEEL_TO_SCROLL_DEFAULT)

def WHEEL_TO_SCROLL_PX(wheel):
	return -wheel * WHEEL_VALUE



########################


def SET_DEFAULT_CAMERA_MAX_DISTANCE():
	global CAMERA_MAX_DISTANCE
	app.SetCameraMaxDistance(CAMERA_MAX_DISTANCE)

def SET_CAMERA_MAX_DISTANCE_INDEX(index):
	global CAMERA_MAX_DISTANCE
	global CAMERA_MAX_DISTANCE_LIST
	try:
		CAMERA_MAX_DISTANCE=CAMERA_MAX_DISTANCE_LIST[index]
	except:
		CAMERA_MAX_DISTANCE=CAMERA_MAX_DISTANCE_LIST[0]

	app.SetCameraMaxDistance(CAMERA_MAX_DISTANCE)

def GET_CAMERA_MAX_DISTANCE_INDEX():
	global CAMERA_MAX_DISTANCE
	global CAMERA_MAX_DISTANCE_LIST
	return CAMERA_MAX_DISTANCE_LIST.index(CAMERA_MAX_DISTANCE)

########################

import chrmgr
import player
import app

def SET_DEFAULT_CHRNAME_COLOR():
	global CHRNAME_COLOR_INDEX
	chrmgr.SetEmpireNameMode(CHRNAME_COLOR_INDEX)

def SET_CHRNAME_COLOR_INDEX(index):
	global CHRNAME_COLOR_INDEX
	CHRNAME_COLOR_INDEX=index
	chrmgr.SetEmpireNameMode(index)

def GET_CHRNAME_COLOR_INDEX():
	global CHRNAME_COLOR_INDEX
	return CHRNAME_COLOR_INDEX

def SET_VIEW_OTHER_EMPIRE_PLAYER_TARGET_BOARD(index):
	global VIEW_OTHER_EMPIRE_PLAYER_TARGET_BOARD
	VIEW_OTHER_EMPIRE_PLAYER_TARGET_BOARD = index

def GET_VIEW_OTHER_EMPIRE_PLAYER_TARGET_BOARD():
	global VIEW_OTHER_EMPIRE_PLAYER_TARGET_BOARD
	return VIEW_OTHER_EMPIRE_PLAYER_TARGET_BOARD

def SET_DEFAULT_CONVERT_EMPIRE_LANGUAGE_ENABLE():
	global CONVERT_EMPIRE_LANGUAGE_ENABLE
	net.SetEmpireLanguageMode(CONVERT_EMPIRE_LANGUAGE_ENABLE)

def SET_DEFAULT_USE_ITEM_WEAPON_TABLE_ATTACK_BONUS():
	global USE_ITEM_WEAPON_TABLE_ATTACK_BONUS
	player.SetWeaponAttackBonusFlag(USE_ITEM_WEAPON_TABLE_ATTACK_BONUS)

def SET_DEFAULT_USE_SKILL_EFFECT_ENABLE():
	global USE_SKILL_EFFECT_UPGRADE_ENABLE
	app.SetSkillEffectUpgradeEnable(USE_SKILL_EFFECT_UPGRADE_ENABLE)

def SET_TWO_HANDED_WEAPON_ATT_SPEED_DECREASE_VALUE():
	global TWO_HANDED_WEAPON_ATT_SPEED_DECREASE_VALUE
	app.SetTwoHandedWeaponAttSpeedDecreaseValue(TWO_HANDED_WEAPON_ATT_SPEED_DECREASE_VALUE)

########################
import item

ACCESSORY_MATERIAL_LIST = [50623, 50624, 50625, 50626, 50627, 50628, 50629, 50630, 50631, 50632, 50633, 50634, 50635, 50636, 50637, 50638, 50639, 50640, 50661, 50662]
ACCESSORY_MATERIAL_LIST2 = [50641, 50642, 50643, 50644, 50645, 50646, 50647, 50648, 50649, 50650, 50651, 50654, 50655, 50656, 50657, 50658, 50659, 50660, 50663]
#ACCESSORY_MATERIAL_LIST = [50623, 50623, 50624, 50624, 50625, 50625, 50626, 50627, 50628, 50629, 50630, 50631, 50632, 50633, 
#			    50623, 50623, 50624, 50624, ]
JewelAccessoryInfos = [
		# jewel		wrist	neck	ear
		[ 50634,	14420,	16220,	17220 ],	
		[ 50635,	14500,	16500,	17500 ],	
		[ 50636,	14520,	16520,	17520 ],	
		[ 50637,	14540,	16540,	17540 ],	
		[ 50638,	14560,	16560,	17560 ],	
		[ 50639,	14570,	16570,	17570 ],
		[ 50640,	14230,	16230,	17230 ],
		[ 50661,	14580,	16580,	17580 ],
		[ 50635,	24500,	19500,	18500 ],
		[ 50636,	24520,	19520,	18520 ],
		[ 50637,	24540,	19540,	18540 ],
		[ 50638,	24560,	19560,	18560 ],
		[ 50639,	24570,	19570,	18570 ],
		[ 50662,	14240,	16240,	17240 ],

	]
JewelAccessoryInfos2 = [
		# jewel		wrist	neck	ear	
		[ 50640, 14000, 16000, 17000 ], #////tahta
		[ 50641, 14020, 16020, 17020 ], #////bak�r
		[ 50642, 14040, 16040, 17040 ], #////g�m��
		[ 50643, 14100, 16100, 17100 ], #////abanoz
		[ 50644, 14120, 16120, 17120 ], #////inci
		[ 50645, 14200, 16200, 17200 ], #////cennetin
		[ 50646, 14220, 16220, 17220 ], #////ruh
		[ 50647, 14500, 16500, 17500 ], #////yakut
		[ 50648, 14520, 16520, 17520 ], #////grena
		[ 50649, 14540, 16540, 17540 ], #////z�mr�t
		[ 50650, 14560, 16560, 17560 ], #////safir
		[ 50651, 14570, 16570, 17570 ], #////turmalin
		[ 50654, 14140, 16140, 17140 ], #////beyazaltin
		[ 50655, 14230, 16230, 17230 ], #////giyotin
		[ 50656, 14060, 16060, 17060 ], #////altin
		[ 50657, 14080, 16080, 17080 ], #////yesim
		[ 50658, 14160, 16160, 17160 ], #////kristal
		[ 50659, 14180, 16180, 17180 ], #////ametist
		[ 50660, 14580, 16580, 17580 ], #////permalit
		[ 50663, 14240, 16240, 17240 ], #////g�kperma
		[ 50647, 24500, 19500, 18500 ], #////yakut2
		[ 50648, 24520, 19520, 18520 ], #////grena2
		[ 50649, 24540, 19540, 17500 ], #////grena2
		[ 50649, 24560, 19560, 18560 ], #////grena2
		[ 50649, 24570, 19570, 18570 ], #////grena2
	]
def GET_ACCESSORY_MATERIAL_VNUM(vnum, subType):
	ret = vnum
	item_base = (vnum / 10) * 10
	for info in JewelAccessoryInfos:
		if item.ARMOR_WRIST == subType:	
			if info[1] == item_base:
				return info[0]
		elif item.ARMOR_NECK == subType:	
			if info[2] == item_base:
				return info[0]
		elif item.ARMOR_EAR == subType:	
			if info[3] == item_base:
				return info[0]
 
	if vnum >= 16210 and vnum <= 16219:
		return 50625

	if item.ARMOR_WRIST == subType:	
		WRIST_ITEM_VNUM_BASE = 14000
		ret -= WRIST_ITEM_VNUM_BASE
	elif item.ARMOR_NECK == subType:
		NECK_ITEM_VNUM_BASE = 16000
		ret -= NECK_ITEM_VNUM_BASE
	elif item.ARMOR_EAR == subType:
		EAR_ITEM_VNUM_BASE = 17000
		ret -= EAR_ITEM_VNUM_BASE

	type = ret/20

	if type<0 or type>=len(ACCESSORY_MATERIAL_LIST):
		type = (ret-170) / 20
		if type<0 or type>=len(ACCESSORY_MATERIAL_LIST):
			return 0

	return ACCESSORY_MATERIAL_LIST[type]
def GET_ACCESSORY_MATERIAL_VNUM2(vnum, subType):
	ret = vnum
	item_base = (vnum / 10) * 10
	for info in JewelAccessoryInfos2:
		if item.ARMOR_WRIST == subType:	
			if info[1] == item_base:
				return info[0]
		elif item.ARMOR_NECK == subType:	
			if info[2] == item_base:
				return info[0]
		elif item.ARMOR_EAR == subType:	
			if info[3] == item_base:
				return info[0]
 
	if vnum >= 16210 and vnum <= 16219:
		return 50642

	if item.ARMOR_WRIST == subType:	
		WRIST_ITEM_VNUM_BASE = 14000
		ret -= WRIST_ITEM_VNUM_BASE
	elif item.ARMOR_NECK == subType:
		NECK_ITEM_VNUM_BASE = 16000
		ret -= NECK_ITEM_VNUM_BASE
	elif item.ARMOR_EAR == subType:
		EAR_ITEM_VNUM_BASE = 17000
		ret -= EAR_ITEM_VNUM_BASE

	type = ret/20

	if type<0 or type>=len(ACCESSORY_MATERIAL_LIST):
		type = (ret-170) / 20
		if type<0 or type>=len(ACCESSORY_MATERIAL_LIST):
			return 0

	return ACCESSORY_MATERIAL_LIST2[type]

##################################################################
## ���� �߰��� '��Ʈ' ������ Ÿ�԰�, ��Ʈ�� ���Ͽ� ���� ������ ����.. 
## ��Ʈ�� ���Ͻý����� �Ǽ������� �����ϱ� ������, �� �Ǽ����� ���� �ϵ��ڵ�ó�� �̷������� �� ���ۿ� ����..

def GET_BELT_MATERIAL_VNUM(vnum, subType = 0):
	# ����� ��� ��Ʈ���� �ϳ��� ������(#18900)�� ���� ����
	return 18900
def GET_BELT_MATERIAL_VNUM2(vnum, subType = 0):
	# ����� ��� ��Ʈ���� �ϳ��� ������(#18900)�� ���� ����
	return 50652

##################################################################
## �ڵ����� (HP: #72723 ~ #72726, SP: #72727 ~ #72730)

# �ش� vnum�� �ڵ������ΰ�?
def IS_AUTO_POTION(itemVnum):
	return IS_AUTO_POTION_HP(itemVnum) or IS_AUTO_POTION_SP(itemVnum)
	
# �ش� vnum�� HP �ڵ������ΰ�?
def IS_AUTO_POTION_HP(itemVnum):
	if 72723 <= itemVnum and 72726 >= itemVnum:
		return 1
	elif itemVnum >= 76021 and itemVnum <= 76022:		## ���� �� ������ ȭ���� �ູ
		return 1
	elif itemVnum >= 71510 and itemVnum <= 71519:		## ���� �� ������ ȭ���� �ູ
		return 1
	elif itemVnum >= 71523 and itemVnum <= 71527:		## ���� �� ������ ȭ���� �ູ
		return 1
	elif itemVnum >= 71528 and itemVnum <= 71530:		## ���� �� ������ ȭ���� �ູ
		return 1
	elif itemVnum == 79012:
		return 1
	elif itemVnum == 40004:
		return 1
		
	return 0

if app.ENABLE_NEW_PET_SYSTEM:
	def IS_AUTO_POTION_SP(itemVnum):
		if 72727 <= itemVnum and 72730 >= itemVnum:
			return 1
		elif itemVnum >= 76004 and itemVnum <= 76005:
			return 1
		elif itemVnum == 79013:
			return 1
		elif itemVnum >= 55701 and itemVnum <= 55710:
			return 1

		return 0

	def IS_NEW_PET_ITEM(itemVnum):
		if itemVnum == 55701 or 55702 == itemVnum or 55703 == itemVnum or 55704 == itemVnum or 55705 == itemVnum or itemVnum == 55706 or itemVnum == 55707 or itemVnum == 55708 or itemVnum == 55709  or itemVnum == 55710:
			return 1

		if itemVnum == 53251 or itemVnum == 53250:
			return 1

		return 0
else:
	def IS_AUTO_POTION_SP(itemVnum):
		if 72727 <= itemVnum and 72730 >= itemVnum:
			return 1
		elif itemVnum >= 76004 and itemVnum <= 76005:
			return 1
		elif itemVnum == 79013:
			return 1

		return 0

if app.LWT_BUFF_UPDATE:	
	def IS_BUFFI(itemVnum):
		items = [91010, 91011]
		return itemVnum in items

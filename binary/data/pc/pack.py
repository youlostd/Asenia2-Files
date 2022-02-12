import os,sys,shutil
mypath = os.path.dirname(os.path.realpath(__file__))
startm2=0
fu_stop=0
debug=0
while 1:
	print "commands start/stop"
	for packName in str(raw_input("Name of archive: ").lower()).split(" "):
		if packName == "":			pass
		elif packName == "start":	startm2=1
		elif packName == "stop":	startm2=0
		elif packName == "debug":	debug=1
		else:
			pathList = {
				"bgm":"bgm",
				"effect":"d:/ymir work/effect",
				"etc":"d:/ymir work",
				"costume":"d:/ymir work",
				"sv_models":"d:/ymir work",
				"sv_models1":"d:/ymir work",
				"sv_models2":"d:/ymir work",
				"sv_models3":"d:/ymir work",
				"guild":"d:/ymir work/guild",
				"icon":"icon",
				"item":"d:/ymir work/item",
				"locale":"locale",
				"monster":"d:/ymir work/monster",
				"monster2":"d:/ymir work/monster2",
				"npc":"d:/ymir work/npc",
				"npc2":"d:/ymir work/npc2",
				"npc_pet":"d:/ymir work/npc_pet",
				"npc_mount":"d:/ymir work/npc_mount",
				"pc":"d:/ymir work/pc",
				"pc2":"d:/ymir work/pc2",
				"pc3":"d:/ymir work/pc3",
				"property":"property",
				"season1":"season1",
				"season2":"season2",
				"sound":"sound",
				"terrain":"d:/ymir work/terrainmaps",
				"textureset":"textureset",
				"tree":"d:/ymir work/tree",
				"uiloading":"d:/ymir work/uiloading",
				"zone":"d:/ymir work/zone",
			}
			# if packName == "locale_client":
				# fu_stop = 1
				# os.system("cd locale_es & python write2File.pyexec")
			for lang in ('de', 'en', 'ro', 'es', 'tr',):
				if lang == packName:
					for filename in ('item_names.txt', 'mob_names.txt',):
						shutil.copy("locale_tr\\%s\\%s" % (lang,filename), filename)
						fu_stop = 1
					os.system("dump_proto.exe")
					if debug == 0:
						for filename in ('item_names.txt', 'mob_names.txt',):
							os.remove(filename)						
						shutil.move("item_proto","locale_tr\\%s\\item_proto" % lang)
						shutil.move("mob_proto","locale_tr\\%s\\mob_proto" % lang)
			if fu_stop == 1:	fu_stop = 0
			else:
				f = open("bin/make_xml.xml", "w")
				f.write("<ScriptFile>\n")
				f.write("\t<CreateEterPackXml Input=\"" + packName + ":")
				if packName in pathList:
					f.write(pathList[packName] + "/\" ")
				else:
					f.write("\" ")
				f.write("ArchivePath=\"../client/pack/" + packName + "\" ")
				f.write("XmlPath=\"bin/_create.xml\">\n")				
				for ignore_ext in ('db','psd','pyexec','rar','zip',):
					f.write("\t\t<Ignore Pattern=\"[a-zA-z0-9]+.%s\" />\n" % ignore_ext)					
				for ignore_single_file in ('item_names.txt','mob_names.txt',):
					f.write("\t\t<Ignore Pattern=\"%s\" />\n" % ignore_single_file)					
				f.write("\t</CreateEterPackXml>\n")
				f.write("</ScriptFile>\n")
				f.close()
				os.system("\"" + mypath + "\\bin\\FileArchiver.exe\" bin/make_xml.xml")
				print("XML File created.")

				packFile = []
				f = open("bin/_create.xml", "r")
				for line in f:
					if line.find("/" + packName + "/") != -1:
						searchStr = "/" + packName + "/"
						line = line[:line.find(searchStr)] + line[line.find(searchStr) + len(searchStr):]
					if not (packName in pathList):
						searchStr = "ArchivedPath=\"/"
						if line.find(searchStr) != -1:
							line = line[:line.find(searchStr) + len(searchStr) - 1] + line[line.find(searchStr) + len(searchStr):]
					packFile.append(line)
				f.close()
				os.system("\"" + mypath + "\\bin\\FileArchiver.exe\" " + "bin/_create.xml")
				if debug == 0:
					os.remove("bin/make_xml.xml")
					os.remove("bin/_create.xml")
					
				print("Pack Kapatma islemi Tamamlandi")
	if startm2==1:
		os.system("cd \"" + mypath + "\\..\\Binary\\\" & metin2client.exe")
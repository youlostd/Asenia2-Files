import glob, os,sys,shutil
pathList = {
				"bgm":"bgm",
				"effect":"d:/ymir work/effect",
				"etc":"d:/ymir work",
				"costume":"d:/ymir work",
				"guild":"d:/ymir work/guild",
				"icon":"icon",
				"item":"d:/ymir work/item",
				"locale":"locale",
				"maps":"d:/ymir work",
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
def CreateXml(packName, debugmod = 0):
	mypath = os.path.dirname(os.path.realpath(__file__))
	pack_name = str(packName)
	f = open(mypath + "/bin/_create.xml", "w")
	cikti_klasoru = "C:\Users\klasi\OneDrive\Masaüstü\PackYap\Binary\pack"
	default = mypath + "/pack/"

	f.write("<ScriptFile>\n\t<CreateEterPack ArchivePath=\"" + cikti_klasoru + pack_name + "\" BuildKeyfile=\"false\" >\n")
	for root, dirs, files in os.walk(mypath + "/" + pack_name):
		for name in files:
			newstr = root.replace(mypath + "/" + pack_name + "\\", "")
			newstr = newstr.replace(mypath + "/" + pack_name, "")
			newstr = newstr.replace("\\", "/")
			newstr2 = newstr
			if newstr.find("ymir work") != -1:
				newstr2 = "d:/" + newstr
			else:
				if pack_name in pathList:
					newstr2 = pathList[pack_name] + "/" + newstr
					if newstr2[-1] == "/":
						newstr2 = newstr2[:-1]
						# print("%s" % newstr2[:-1])

			if len(newstr) > 0:
				newstr = "/" + newstr
			if len(newstr2) > 0:
				newstr2 = newstr2 + "/"
			if pack_name == "root" and debugmod == 0:
				if newstr2.find("/") == -1 and name.endswith(".py"):
					continue

			f.write("\t\t<File ArchivedPath=\"" + newstr2 + name + "\"><![CDATA[" + pack_name + newstr + "/" + name + "]]></File>\n")
			# print("\t\t<File ArchivedPath=\"" + newstr2 + name + "\"><![CDATA[" + pack_name + newstr + "/" + name + "]]></File>\n")

	f.write("\t</CreateEterPack>\n</ScriptFile>")
	f.close()

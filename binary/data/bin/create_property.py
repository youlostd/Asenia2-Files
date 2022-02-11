import glob
import os

os.system( "PropertyGen.exe ../property" )
os.system( "move property.xml ../root/property.xml" )
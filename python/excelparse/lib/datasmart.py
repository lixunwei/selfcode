import xlrd
from collections import OrderedDict as odict
from .const import Const
import re

class smartdata():
    def __init__(self):
        self.title = odict()
        self.rowlist = []
        self.collist = []

    def titleadd(self, titlename="", titleindex=0):
        titlename = re.sub(r'\(.*?\)',"",titlename)
        titlename = re.sub(r' ',"",titlename)
        if titlename in self.title.keys():
            tilename += "_"+str(titleindex) 
        self.title.update({titlename:titleindex})

    def rowadd(self, rowlist=[]):
        self.rowlist.append(rowlist)

    def coladd(self, collist=[]):
        self.collist.append(collist)
    

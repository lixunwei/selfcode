import sqlite3
import abc
import os
from .const import Const

def isfloat(data):
    try:
        float(data)
    except ValueError:
        return False
    return True
    
def initdbdir(dbname):
    dbpath = os.path.dirname(os.path.abspath(dbname))
    if not os.path.exists(dbpath):
        os.mkdir(dbpath)
    return dbpath
        
class dbtype():
    def __init__(self):
        self.const = Const()
        self.const.db_NULL = 0
        self.const.db_INTEGER = 1
        self.const.db_REAL = 2
        self.const.db_TEXT = 3
        self.const.db_BLOB = 4
        self.tb_TYPE = dict()
        self.tb_TYPE.update({self.const.db_NULL:"NULL"})
        self.tb_TYPE.update({self.const.db_INTEGER:"INSERT"})
        self.tb_TYPE.update({self.const.db_REAL:"REAL"})
        self.tb_TYPE.update({self.const.db_TEXT:"TEXT"})
        self.tb_TYPE.update({self.const.db_BLOB:"BLOB"})

    def gettype(self,datatype):
        return self.tb_TYPE.get(datatype)

    def parsetype(self, collist):
        counttype = {0:0,1:0}#0 is float, 1 is string
        for val in collist:
            if isfloat(val):
                counttype[0] += 1
            elif val:
                counttype[1] += 1
        if counttype[0] > counttype[1]:
            return  self.gettype(self.const.db_REAL)
        return self.gettype(self.const.db_TEXT)

class createtb():
    def __init__(self, smartdata):
        self.smartdata = smartdata
        
    @abc.abstractmethod
    def createdbtable(self):
        pass
    
    @abc.abstractmethod
    def setprimarykey(self, primarykey=None):
        pass
        
class sqlite3createtb(createtb):
    def __init__(self, dbname, tablename, smartdata):
        self.dbpath = initdbdir(dbname)
        self.smartdata = smartdata
        self.tbtitle = dbtype()
        self.primarykey = None
        self.tablename = tablename
        self.conn = sqlite3.connect(dbname)
        self.cursor = self.conn.cursor()
        
    def createdbtable(self):
        tblist = ["{0} {1} {2}".format("ID", "INTEGER","PRIMARY KEY"),]
        for titlename in self.smartdata.title.keys():
            index = self.smartdata.title[titlename]
            titletype = self.tbtitle.parsetype(self.smartdata.collist[index])
            titleattr = " "
            titlecons = " "
            title = "{0} {1} {2} {3}".format(titlename, titletype, titleattr, titlecons)
            tblist.append(title)
        with open(os.path.join(self.dbpath,self.tablename+".sql"),"w+") as wfd:
            wfd.write("CREATE TABLE {0}".format(self.tablename))
            wfd.write("(")
            for index in range(len(tblist)):
                wfd.write(tblist[index])
                if index != len(tblist)-1:
                    wfd.write(",\n")
            wfd.write(");\n")
            
        with open(os.path.join(os.getcwd(),self.tablename+".sql"),"r") as rfd:
            text = rfd.read()
            self.cursor.execute(text)
        self.insertdata()
        self.conn.commit()
        self.conn.close()
        
    def insertdata(self):
        head = "INSERT INTO {0}".format(self.tablename)
        items = str(tuple(self.smartdata.title.keys()))
        with open(os.path.join(self.dbpath, self.tablename+"_debug.txt"),'w') as wfd:
            for val in self.smartdata.rowlist:
                value = str(tuple(val))
                command = "{0} {1} VALUES {2}".format(head, items, value)
                wfd.write(command)
                wfd.write("\n")
                self.cursor.execute(command)
            
    def setprimarykey(self,primarykey=None):
        self.primarykey = primarykey

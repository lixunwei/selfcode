#!/usr/bin/env python3
# encoding:utf-8

from lib.dataexcel import exceldata
from lib.database import sqlite3createtb
import sqlite3

def CreateTB(dbname, excelname, sheetname, tablename,titleindex = 1):
    new = exceldata(excelname)
    sheet = new.open_sheet(sheetname)
    sheet.set_title(titleindex)
    tb = sqlite3createtb(dbname,tablename, sheet)
    tb.createdbtable()
    
if __name__ == '__main__':
    
    CreateTB("./db/IndicatorCarryover.db", 'new.xls', '特设户', 'newspecialuser')
    CreateTB("./db/IndicatorCarryover.db", 'old.xls', '原表', 'origintable')

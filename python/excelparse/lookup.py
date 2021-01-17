#!/usr/bin/env python3
# encoding:utf-8

import os
import sqlite3
import math


def CompareTable(db, tb1, tb2):
    
    tb1conn = sqlite3.connect(db)
    tb1cursor = tb1conn.cursor()
    tb1result = tb1cursor.execute("SELECT ID,指标管理科室,摘要,预算数,支出功能分类,指标结转 from {0} where ID > 1 and 指标管理科室=\"305_教科文科\"".format(tb1))
    
    tb2conn = sqlite3.connect(db)
    tb2cursor = tb2conn.cursor()
    
    for row in tb1result:
        if math.isclose(row[5],0,abs_tol=0.0001):
            continue
        condition = "指标管理科室=\"{0}\" and 摘要=\"{1}\" and  abs(预算数 - {2}) < 0.0001 and 支出功能分类=\"{3}\" and abs(指标结转 - {4}) < 0.0001".format(row[1],row[2],row[3],row[4],row[5])
        tb2result = tb2cursor.execute("SELECT  ID,指标管理科室,摘要,预算数,支出功能分类,指标结转 from {0} where {1}".format(tb2, condition))
        
        retb1conn = sqlite3.connect(db)
        retb1cursor = retb1conn.cursor()
        retb1result = retb1cursor.execute("SELECT  ID,指标管理科室,摘要,预算数,支出功能分类,指标结转 from {0} where {1}".format(tb1, condition))
        
        reitem = retb1result.fetchall()
        originitem = tb2result.fetchall()
        
        if len(reitem) != len(originitem):
            print("newtable = {0} origintable = {1}".format(len(reitem),len(originitem)))
            print(row)
            print(originitem)
            retb1conn.close()
        else:
            retb1conn.close()
            continue

if __name__ == '__main__':

    CompareTable('./db/IndicatorCarryover.db', "newspecialuser", "origintable")
    CompareTable('./db/IndicatorCarryover.db', "origintable", "newspecialuser")

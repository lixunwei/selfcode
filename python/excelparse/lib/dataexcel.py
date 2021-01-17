import xlrd
from .datasmart import smartdata
import re

class excelsheet(smartdata):
    def __init__(self, sheet = None):
        self.sheet = sheet
        super().__init__()

    def col_val(self, col=0, start_rowx=0, end_rowx=None):
        return self.sheet.col_values(col, start_rowx=start_rowx, end_rowx=end_rowx)

    def row_val(self, row=0, start_colx=0, end_colx=None):
        return self.sheet.row_values(row, start_colx=start_colx, end_colx=end_colx)

    def set_title(self, row = 0):
        self.titlerow = row
        self.titlelist = self.row_val(row, start_colx=0)
        for index in range(len(self.titlelist)):
            if self.titlelist[index]:
                self.titleadd(str(self.titlelist[index]), index)
        self.__get_rowdata__()
        self.__get_coldata__()

    def __get_rowdata__(self):
        rowstart = self.titlerow + 1
        rowend = self.sheet.nrows
        for index in range(rowstart, rowend):
            tmplist = self.row_val(index)
            rowlist = [tmplist[i] for i in range(len(tmplist)) if i in self.title.values()]
            rowlist = [re.sub(r' ',"",val) if isinstance(val,str) else val for val in rowlist]
            self.rowadd(rowlist)

    def __get_coldata__(self):
        for index in self.title.values():
            collist = self.col_val(index,self.titlerow)
            self.coladd(collist)
    
class exceldata():
    def __init__(self, file):
        self.file = file
        self.book = self.open_excel_book()

    def open_excel_book(self):
        return xlrd.open_workbook(self.file)

    def open_sheet(self, sheet_name):
        sheet = self.book.sheet_by_name(sheet_name)
        return excelsheet(sheet)

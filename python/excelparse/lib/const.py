import sys

class Const(object):
    def __setattr__(self, key, value):
        if key in self.__dict__:
            raise AttributeError("{0} is a const value, you can't change it".format(key))
        else:
            self.__dict__[key] = value

    def __getattr__(self, key):
        if self.__dict__.has_key(key):
            return self.key
        else:
            return None

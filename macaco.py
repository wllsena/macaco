class BananaFrame:
    import checks_and_error_handling as __ceh
    import integration as __it

    __bf      = None
    __axis0   = 0
    __axis1   = 0
    __columns = {}
    __types   = []

    def __general_return (self, index):
        return lambda : self.__bf.take_column(index)

    def __init__ (self, dic):
        items      = list(dic.items())
        self.__axis0 = len(items[0][1])
        self.__axis1 = len(items)

        self.__ceh.check_empy(self.__axis0, self.__axis1)
        self.__ceh.check_sizes(self.__axis0, items)
        self.__types   = self.__ceh.check_types(items)
        self.__columns = self.__ceh.check_columns(items)

        for name, index in self.__columns.items():
            setattr(self, name, self.__general_return(index))

        self.__bf = self.__it.BananaFrame(self.__axis0, self.__axis1, self.__types)
        for i in range(self.__axis1):
            self.__bf.fill_column(i, items[i][1])


    def query (self, column, key):
        self.__ceh.check_index(self.__axis1, self.__columns[column], key, self.__types)
        return self.__bf.query(self.__columns[column], [key])

    def row (self, index):
        self.__ceh.check_index(self.__axis0, index)
        return self.__bf.take_row(index)

    def slices (self, keys):
        self.__ceh.check_types(keys, False)
        self.__bf.slice(keys)
        self.__axis0 = len(keys)

    def display (self):
        from tabulate import tabulate
        table = [None]*self.__axis0
        for y in range(self.__axis0):
            table[y] = [y] + self.row(y)
        print(tabulate(table, headers=[""] + self.columns, tablefmt="fancy_grid"))

    def __ne__(self, key):
        pass

    def __getitem__(self, key):
        if isinstance(key, list):
            return self.slices(key)
        elif isinstance(key, str):
            return getattr(self, key)()
        elif isinstance(key, int):
            return self.row(key)
        else:
            raise self.__ceh.UnexpectedTypeError(key)

    @property
    def columns (self):
        return list(self.__columns.keys())

    @property
    def size (self):
        return (self.__axis0, self.__axis1)

    @property
    def types (self):
        return [self.__ceh.tint_to_type(t) for t in self.__types]


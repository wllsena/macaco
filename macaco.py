class BananaFrame:
    import checks_and_error_handling as __ceh
    import integration as __it
    import matplotlib.pyplot as __plt
    import tabulate as __tb
    import copy as __cp
    import functools as __ft

    class __serie:
        import tabulate as __tb
        def __init__(self, self_bf, index):
            self.__index   = index
            self.__self_bf = self_bf

        def __eq__(self, key):
            return self.__self_bf.query(self.__index, key)

        def __rshift__(self, key):
            return self.__self_bf.between(self.__index, key[0], key[1])

        def __iter__(self):
            return list.__iter__(self.values)

        def __contains__(self, key):
            return key in self.values

        def __len__(self):
            return self.size

        def __str__ (self):
            return self.display()

        def __repl__ (self):
            return self.display()

        def _repr_html_(self):
            return self.display()

        @property
        def size(self):
            return self.__self_bf.size[0]

        @property
        def name (self):
            return self.__index

        @property
        def values (self):
            return self.__self_bf.take_column(self.__index)

        def display (self, n = 5, l = 10):
            if n < self.size:
                values = self.values[:n] + ['...']
            else:
                values = self.values
            values = [str(value)[:l] for value in values]

            print(self.__tb.tabulate([[self.__index + "(s):"] + values], tablefmt="fancy_grid"))
            print(" macaco.BananaFrame.__serie")
            return ""

    #---

    __bf      = None
    __axis0   = 0
    __axis1   = 0
    __columns = {}
    __types   = []

    #---

    def __init__ (self, dic):
        items        = list(dic.items())
        self.__axis0 = len(items[0][1])
        self.__axis1 = len(items)

        self.__ceh.check_empy(self.__axis0, self.__axis1)
        self.__ceh.check_sizes(self.__axis0, items)
        self.__types   = self.__ceh.check_types(items)
        self.__columns = self.__ceh.check_columns(items)

        for name in self.__columns.keys():
            setattr(self, name, self.__serie(self, name))

        self.__bf = self.__it.BananaFrame(self.__axis0, self.__axis1, self.__types)
        for i in range(self.__axis1):
            self.__bf.fill_column(i, items[i][1])

    def __len__ (self):
        return self.size[1]

    def __str__ (self):
        return self.display()

    def __repl__ (self):
        return self.display()

    def _repr_html_(self):
        return self.display()

    def __iter__(self):
        return list.__iter__([self.column(i) for i in self.column_names])

    def __contains__(self, key):
        return key in self.column_names
    
    def __getitem__(self, key):
        if isinstance(key, int):
            return self.row(key)
        elif isinstance(key, list):
            return self.slices(key)
        elif isinstance(key, str):
            return self.column(key)
        elif isinstance(key, tuple):
            return self.slices(list(self.__ft.reduce(lambda a, b: set(a) & set(b), key)))
        else:
            raise self.__ceh.UnexpectedTypeError(key)

    @property
    def size (self):
        return [self.__axis0, self.__axis1]

    @property
    def column_names (self):
        return list(self.__columns.keys())

    @property
    def types (self):
        return [self.__ceh.tint_to_type(t) for t in self.__types]

    @property
    def indexes(self):
        return [i for i in range(self.__axis0)]

    def display (self, n = 5, l = 10):
        if n < self.__axis0:
            length = n
            table  = [None]*length + [['...']*(self.__axis1 + 1)]
        else:
            length = self.__axis0
            table  = [None]*length

        for y in range(length):
            table[y] = [y] + [str(value)[:l] for value in self.row(y)]

        print(self.__tb.tabulate(table, headers=[""] + self.column_names, tablefmt="fancy_grid"))
        print(" macaco.BananaFrame")
        return ""

    def plot(self, xlabel, ylabel = False, sort = False, marker='o'):
        if ylabel:
            indexY = self.take_column(ylabel)
        else:
            ylabel = "Index"
            indexY = self.indexes
        indexX = self.take_column(xlabel)

        if sort:
            indexX, indexY = zip(*sorted(zip(indexX, indexY), key = lambda x : x[sort]))

        self.__plt.plot(indexX, indexY, marker)
        self.__plt.xlabel(xlabel)
        self.__plt.ylabel(ylabel)
        self.__plt.title(xlabel + " X " + ylabel)
        self.__plt.show()

    def add_columns(self, dic):
        items = list(dic.items())

        self.__ceh.check_sizes(self.__axis0, items)
        types   = self.__ceh.check_types(items)
        columns = self.__ceh.check_columns(items, list(self.__columns.keys()))

        self.__axis1   += len(items)
        self.__types   += types
        self.__columns.update(columns)

        for name in columns.keys():
            setattr(self, name, self.__serie(self, name))

        for i in range(len(types)):
            self.__bf.add_column(types[i], items[i][1])

    def column (self, index):
        self.__ceh.check_in(index, self.__columns.keys())
        return getattr(self, index)

    def row (self, index):
        self.__ceh.check_index(self.__axis0, index)
        return self.__bf.take_row(index)

    def take_column (self, name):
        return self.__bf.take_column(self.__columns[name])

    def query (self, column, key):
        self.__ceh.check_index(self.__axis1, self.__columns[column], self.__types, key)
        return sorted(self.__bf.query(self.__columns[column], key))

    def between (self, column, key_min, key_max):
        self.__ceh.check_index(self.__axis1, self.__columns[column], self.__types, key_min, key_max)
        return sorted(self.__bf.between(self.__columns[column], key_min, key_max))

    def copy (self):
        new_one = self.__class__.__new__(self.__class__)
        for k, v in self.__dict__.items():
            setattr(new_one, k, self.__cp.copy(v))
        return new_one

    def cut (self, keys):
        self.__ceh.check_types(keys, False)
        self.__ceh.check_ins(keys, self.indexes)
        self.__axis0 = len(keys)
        self.__bf.slice(keys)

    def slices (self, keys):
        new_bf = self.copy()
        new_bf.cut(keys)
        return new_bf

#----

def read_csv(path):
    import csv
    reader = list(csv.reader(open(path, mode='r')))

    columns = tuple(reader[0])
    dic = {column:[] for column in columns}

    for row in reader[1:]:
        for i, value in enumerate(row):
            if value.isdigit():
                value = int(value)
            elif value.replace('.','').isdigit():
                value = float(value)
            dic[columns[i]].append(value)

    return BananaFrame(dic)

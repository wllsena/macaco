class UnexpectedTypeError(Exception):
    def __init__(self, value):
        Exception.__init__(self, "Expected types: <class 'list'>, <class 'string'> and <class 'int'>. Given type {} !"
                           .format(type(value)))

class NotInError(Exception):
    def __init__(self, value, context):
        Exception.__init__(self, "{} not in {} !".format(value, context))

class WrongTypeError(Exception):
    def __init__(self, value, one_type = False):
        str_types = one_type if one_type else "<class 'int'>, <class 'float'> and <class 'string'>"
        Exception.__init__(self, "Accepted types: {}. Type of {} is {} !".format(str_types, value, type(value)))

class AxisSizeError(Exception):
    def __init__(self, index, axis):
        Exception.__init__(self, "Axis size: {}, given index {} !".format(axis, index))

class WrongTypeContextError(Exception):
    def __init__(self, first_type, value):
        Exception.__init__(self, "Context Type: {}. Type of {} is {} !".format(first_type, value, type(value)))

class WrongDataSizesError(Exception):
    def __init__(self):
        Exception.__init__(self, "Dictionary data sizes are incompatible !")

class EmpyDicError(Exception):
    def __init__(self):
        Exception.__init__(self, "The dictionary cannot be empty !")

class ColumnNameError(Exception):
    def __init__(self, name):
        Exception.__init__(self, "Name {} appears in more than one column !".format(name))

def check_types(items, from_dic = True):
    if from_dic:
        types = []

        for _, values in items:
            fst_type = type(values[0])
            tint     = type_to_int(fst_type)
            for value in values[1:]:
                if type(value) != fst_type:
                    raise WrongTypeContextError(fst_type, value)
            types.append(tint)

        return types

    else:
        fst_type = type(items[0])
        tint     = type_to_int(fst_type)
        for value in items[1:]:
            if type(value) != fst_type:
                raise WrongTypeContextError(fst_type, value)

def check_sizes(axis0, items):
    for _, values in items:
        if len(values) != axis0:
            raise WrongDataSizesError

def check_empy(axis0, axis1):
    if axis0 == 0 or axis1 == 0:
        raise EmpyDicError

def check_columns(items, prev_keys = []):
    columns = {}
    for i, (names, _) in enumerate(items):
        if names not in list(columns.keys()) + prev_keys:
            columns[names] = i
        else:
            raise ColumnNameError(name)
    return columns

def type_to_int (typ):
    if type(typ) != type:
        typ = type(typ)
    if typ == int:
        return 0
    elif typ == float:
        return 1
    elif typ == str:
        return 2
    else:
        raise WrongTypeError(typ)

def tint_to_type (tint):
    if tint == 0:
        return int
    elif tint == 1:
        return float
    elif tint == 2:
        return str
    else:
        raise WrongTypeError(tint)

def check_index(axis, index, key = False, types=False):
    if not isinstance(index, int):
        raise WrongTypeError(index, int)

    if key:
        if type_to_int(key) != types[index]:
            raise WrongTypeError(key, types[index])
    if index < 0 or axis < index + 1:
        raise AxisSizeError(axis, index)

def check_in(value, values):
    if value not in values:
        raise NotInError(value, values)

def check_ins(values, context):
    if min(values) not in context:
        raise NotInError(min(values), context)
    elif max(values) not in context:
        raise NotInError(max(values), context)

def do_nothing():
    print("do_nothing")

def add(a, b):
    return a+b

def add_sub(a, b):
    return a+b, a-b

def haha_merge_list(a, b):
    print("print input in python", a, b)
    c = list(a)+list(b)+["haha"]
    return c
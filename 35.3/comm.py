#!/usr/bin/python

import random, sys
from optparse import OptionParser

class readline:
    def __init__(self, filename):
        f = open(filename, 'r')
        self.lines = f.read().splitlines()
        f.close()

    def returnlines(self):
        return self.lines

def isSorted(list):
    temp = " "
    for var in list:
        if temp > var:
            return False;
        temp = var
    return True;

def print3(op1, op2, op3, i):
    str=""
    if not op1:
        str+="        "
    if not op2:
        str+="        "
    if not op3:
        str+=i
    if not op3:
        sys.stdout.write(str + "\n")

def print2(op1, op2, op3, i):
    str=""
    if not op1:
        str+="        "
    if not op2:
        str+=i
    if not op3:
        str+="        "
    if not op2:
        sys.stdout.write(str + "\n")

def print1(op1, op2, op3, i):
    str=""
    if not op1:
        str+=i
    if not op2:
        str+="        "
    if not op3:
        str+="        "
    if not op1:
        sys.stdout.write(str + "\n")
        
def main():
    version_msg = "%prog 2.0"
    usage_msg = """%prog [-123] [-u] FILE1 FILE2
    Compares sorted files FILE1 and FILE2 line by line.
    -1 Suppresses column 1
    -2 Suppresses column 2
    -3 Suppresses column 3
    -u Allows it to sort unsorted files
    """

    parser = OptionParser(version=version_msg, usage=usage_msg)
    parser.add_option("-1", action="store_true", dest="op1",
                      help="Suppress column 1 (lines unique to FILE1)",
                      default=False)
    parser.add_option("-2", action="store_true", dest="op2",
                      help="Suppress column 2 (lines unique to FILE2)",
                      default=False)
    parser.add_option("-3", action="store_true", dest="op3",
                      help="Suppress column 3(lines that appear in both files)",
                      default=False)
    parser.add_option("-u", action="store_false", dest="sorted1", default=True,
                      help="Allows it to work on unsorted files.")

    options, args = parser.parse_args(sys.argv[1:])
    if len(args) != 2:
        parser.error("Wrong number of operands")
    input_file1 = args[0]
    input_file2 = args[1]

    list1 = []
    list2 = []

    try:
        if input_file1 != "-":
            file1 = readline(input_file1)
            list1 = file1.returnlines()
        else:
            list1 = sys.stdin.read().splitlines()
        if input_file2 != "-":
            file2 = readline(input_file2)
            list2 = file2.returnlines()
        else:
            list2 = sys.stdin.read().splitlines()
    except IOError as strerror:
        parser.error("I/O error({0}): {1}"+format(strerror))
        
    if options.sorted1:
        if (not isSorted(list1)) or (not isSorted(list2)):
            parser.error("Value error")

    if not options.sorted1:
        for i in list(list1):
            if list2.count(i) > 0:
                list2.remove(i)
                print3(options.op1, options.op2, options.op3, i)
            else:
                print1(options.op1, options.op2, options.op3, i)
        for j in list2:
                print2(options.op1, options.op2, options.op3, j)

    if options.sorted1:
        for i in list(list1):
            for j in list(list2):
                if j < i:
                    print2(options.op1, options.op2, options.op3, j)
                    list2.remove(j)
                elif i == j:
                    print3(options.op1, options.op2, options.op3, i)
                    list1.remove(i)
                    list2.remove(j)
                    break
                elif j > i:
                    print1(options.op1, options.op2, options.op3, i)
                    list1.remove(i)
                    break
        if len(list1) != 0:
            for i in list(list1):
                print1(options.op1, options.op2, options.op3, i)
        if len(list2) != 0:
            for j in list(list2):
                print2(options.op1, options.op2, options.op3, j)

if __name__ == "__main__":
    main()

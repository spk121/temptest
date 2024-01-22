import csv

def is_float(s):
    try:
        float(s)
        return True
    except ValueError:
        return False

def is_int(s):
    try:
        int(s)
        return True
    except ValueError:
        return False

def to_num_if_possible(s):
    if(is_int(s)):
        return int(s)
    elif(is_float(s)):
        return float(s)
    return s

def read_csv_file(filename):
    data = []
    with open(filename, newline='') as csvfile:
        reader = csv.reader(csvfile)
        for row in reader:
            row = [x.strip() for x in row]
            row = [x.replace(" ","_") for x in row]
            row = [to_num_if_possible(x) for x in row]
            data.append(row)
    return data

print(read_csv_file('4000.csv'))


import random
import string

indexes = []

def gen():
    name = ""
    surname = ""
    index = ""
    for i in range(12):
        name += random.choice(string.ascii_uppercase + string.ascii_lowercase)
        surname += random.choice(string.ascii_uppercase + string.ascii_lowercase)
        if i < 7:
            index += random.choice(string.digits)
    return [name, surname, index]

with open('students.txt', 'w') as f:
    for j in range(100000):
        student = gen()
        while student[2] in indexes:
            l=list(student[2])
            random.shuffle(l)
            student[2] = "".join(l)
        indexes.append(student[2])
        print(j)
        f.write(student[0]+" "+student[1]+" "+student[2]+"\n")

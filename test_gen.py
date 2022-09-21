import random

print("Размер стека:")
sz = int(input())

print("Длина теста:")
len = int(input())

print("Колличество различных данных:")
data_delt = int(input())

test = [random.randint(1, data_delt) for i in range(len)]

f = open("ptest", 'w')

f.write(str(sz) + ' ' + str(len) + ' ')

for i in range(len):
    f.write(str(test[i]) + ' ')


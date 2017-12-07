# -*- coding: UTF-8 -*-

import matplotlib.pyplot as plt
import numpy as np
from matplotlib import mlab

# Значения по оси X

X = []

lower_bound = 1
upper_bound = 1000000
i = lower_bound

while(i <= upper_bound):
    X.append(i)
    i += 10000

# Набор значений по оси Y

Y_BRS   = []
Y_CBRS  = []
Y_LSRS  = []
Y_LRS   = []

f_brs = open('log/log_bytes_radix_sort.txt', 'r')
for line in f_brs:
	Y_BRS.append(float(line))

f_brs.close()

f_cbrs = open('log/log_compact_bytes_radix_sort.txt', 'r')
for line in f_cbrs:
	Y_CBRS.append(float(line))

f_cbrs.close()

f_lsrs = open('log/log_lsd_compact_radix_sort.txt', 'r')
for line in f_lsrs:
	Y_LSRS.append(float(line))

f_lsrs.close()

f_lrs = open('log/log_lsd_radix_sort.txt', 'r')
for line in f_lrs:
	Y_LRS.append(float(line))

f_lrs.close()
# Строим диаграмму
# Задаем исходные данные для каждой линии диаграммы, внешний вид линий и маркеров.
# Функция plot() возвращает кортеж ссылок на объекты класса matplotlib.lines.Line2D

line_brs, line_cbrs, line_lsrs, line_lrs = plt.plot(X, Y_BRS, 'b-', X, Y_CBRS, 'r-', X, Y_LSRS, 'g-', X, Y_LRS, 'y-')

# #line_brs = plt.plot(X, Y_BRS, 'b-')
# line_cbrs = plt.plot(X, Y_CBRS, 'r-')


# Задаем интервалы значений по осям X и Y

plt.axis([0.0, 1000000.0, 0.0, 400.0])

# Задаем заголовок диаграммы

plt.title(u'Зависимость времени работы алгоритма от кол-ва эллементов')

# Задаем подписи к осям X и Y

plt.xlabel(u'Кол-во эллементов ')
plt.ylabel(u'Время, ms ')

# Задаем исходные данные для легенды и ее размещение

plt.legend((line_brs, line_cbrs, line_lsrs, line_lrs), (u'Byte-Radix-Sort', u'Compact-Byte-Radix-Sort', u'LSD-Compact-Radix_Sort', u'LSD-Radix_Sort' ), loc = 'best')

# Включаем сетку

plt.grid()

# Сохраняем построенную диаграмму в файл

# Задаем имя файла и его тип

plt.savefig('graphic.png', format = 'png')

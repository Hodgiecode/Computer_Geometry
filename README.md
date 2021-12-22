# Компьютерная геометрия
Репозиторий с задачами по компьютерной геометрии. Преподаватель Сазонов В.В. Осень 2021 год.

## Важно !!!

1. Работа триангуляции не оптимизирована. Трассировка может занимать до минуты

2. Путь к файлу с входными данными нужно исправить вручную

3. Тор нельзя повернуть

4. Трассировщик лучей (v2). Код практически полностью отсюда https://github.com/ssloy/tinyraytracer/blob/master/tinyraytracer.cpp 

#### О - обязательные

1. Триангуляция Делоне. Источник для v1 : https://cp-algorithms.com/geometry/delaunay.html#toc-tgt-5  Версия v2 имеет сложность O(n^4) и взята из книги О'Рурка. Менее громоздкая

2. Алгоритм пересечения объединения, пересечения и разности полигонов (многоугольников) на плоскости. Источник: https://github.com/lukaskett/155ADKG/blob/87dadb6d6c07f7bd1137560cb189de5d2d51cfe2/U4/U4_BooleanOperations/algorithms.cpp                                                          Сравнение результата с библиотекой Clipper.

3. Выпуклая оболочка. Источник: https://www.geeksforgeeks.org/convex-hull-set-1-jarviss-algorithm-or-wrapping/

4. Предикат поворота. Источник: https://habr.com/ru/post/138168/
 
5. Кривые Безье. Полиномы Бернштейна 

6 - 8. Сплайны. Сплайны Лагранжа, кубический и сплаин Эрмита. 

9 - 12. Трассировщик лучей. Сфера. Диффузное освещение. Триангуляции. Прозрачные поверхности.

#### Д - дополнительные

1. Алгоритм построения ЕМОД. 

2. Алгоритм пересечения отрезков Бентли-Оттмана

3-5. Кривые Безье. Разделение кривой Безье на две с сохранением формы. Рациональные кривые Безье. Добавление опорной точки  

6-11. Трассировщик лучей. Конус, цилиндр и тор. Зеркальное отражение. Зеркальные поверхности (только в Трассировщик лучей (v2))

12. Алгоритм Форчуна построения диаграммы Вороного. Оригинал https://github.com/AngusRitossa/Voronoi

#### У - учебные (программы написанные самим Сазоновым В.В. для студентов разных лет, так и программы для изучения возможностей Qt)

1. Все сплаины (Сазонов) (1) (2016)

2. IntersectionApp (2021)

3. Intr (2021)

4. TraceApp (2021)

5. qt_point_work_template (2021) учебный шаблон в котором реализована работа с точками в Qt. 

6. Ближайшая пара точек

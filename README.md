# Постановка задачи

У нас есть 5 фирм – продавцов фруктов, от каждой фирмы мы получаем список того, что она хочет иметь. Список этот – это файл в формате название товара (фрукта), максимальная цена, производитель товара (желаемый), количество (в килограммах). Каждая запись в одну строку, поля разделяются запятыми.

И есть у нас три человека, которым нужны результаты обработки этих файлов – первому в виде списка для производителя 1, второму для другого производителя и третьему нужна информация по всем яблокам. Т.е. этих людей интересует кто и сколько товара заказал.

Нужно написать программу на С/С++, которая будет принимать пять файлов и на выходе давать три файла. Еще если время позволит и будет желание было бы интересно чтобы эта программка получала файлы динамически, т.е. отслеживала бы содержимое каталога с входными файлами, и если они изменились, то обновляла бы и результирующие три файла.

# Реализация

Программа **article-storage.exe** производит отслеживание изменений файлов ***.article** (в кодировке UTF8) в исходном каталоге, адрес которого передается в первом параметре вызова. Предполагается, что в каталоге могут находиться и другие файлы, изменение которых не должно отразиться на работе программы. Программа формирует выходные файлы данных ***.result**, в соответствии с произвольным количеством запросов, которые передаются начиная с третьего параметра вызова.

# Сборка

##### Программа собиралась в windows 7 на Visual Studio 17. (В программе используется стандартный модуль filesystem. C++17 предоставляет возможности для кроссплатформенной работы с файловой системой. Для отслеживания изменений исходного каталога Используется специфичный для windows _ReadDirectoryChangesW_)

## Инструкция по сборке

Программа собирается при помощи cmake следующим образом:

1. mkdir project
2. cd project
2. cmake ..
4. cmake --build . --config Release

В каталоге **project\Release** должны собраться файлы приложения **article-storage** и **article-storage-tests**

# Использование

### ВНИМАНИЕ - программа разработана для работы с входными файлами в кодировке UTF8. Данные файлов в кодировке ANSI будут проигнорированы.

При неправильном использовании программа **article-storage** выводит страницу помощи.

---
### **article-storage** используется следующим образом:

article-storage.exe **sourceDir** **destDir** "(producer|article) = concrete_value" ["(producer|article) = concrete_value" ...]

---

**sourceDir** - указывает на существующую, исходную директорию, в которой программа буде искать **.article** файлы с данными

**destDir** - указывает на целевую директорию, в которую программа размещает файлы **.result** в соответствии порядковому номеру запроса в строке запуска

**"(producer|article) = concrete_value"** - взятый в кавычки запрос, который позволяет выбирать товарные позиции по производителю или названию. Значение (**concrete_value**), по которому производится выборка располагается после знака равенства.







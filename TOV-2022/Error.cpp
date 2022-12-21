#include "Error.h"
#include <cstdlib>
#include <iostream>
namespace Error
{
	ERROR errors[ERROR_MAX_ENTRY] =
	{
		ERROR_ENTRY(0, "Недопустимый код ошибки"),
		ERROR_ENTRY(1, "Системный сбой"),
		ERROR_ENTRY(2, "Ошибка в ключевом слове"), 
		ERROR_ENTRY(3, "Размер таблицы идентификаторов превышает макисмальный"),
		ERROR_ENTRY(4, "Размер таблицы лексем превышает максимальный"),
		ERROR_ENTRY(5, "Идентификатор совпадает с ключевым словом"),
		ERROR_ENTRY(6, "Необходимо указать тип переменной при первом объявлении"),
		ERROR_ENTRY(7, "Неизвестный тип идентификатора"),
		ERROR_ENTRY(8, "Необходимо указать тип параметра при использовании нового идентификатора"),
		ERROR_ENTRY(9, "Индекс n в функции GetEntry (idtable) вне допустимых пределов"),
		ERROR_ENTRY(10, "Индекс n в функции GetEntry (lextable) вне допустимых пределов"),
		ERROR_ENTRY(11, "Неизвестная лексема"),
		ERROR_ENTRY(12, "Превышен лимит литералов: 999"),
		ERROR_ENTRY(13, "Знак '!' может использваться только как часть оператора '!='"),
		ERROR_ENTRY(14, "Идентификатор функции не может быть короче 3 символов"),
		ERROR_ENTRY(15, "Попытка объявить библиотечную функцию в качестве переменной или параметра"),
		ERROR_ENTRY(16, "Размер строки превышает максимальный"),
		ERROR_ENTRY_NODEF(17), 
		ERROR_ENTRY_NODEF(18),
		ERROR_ENTRY_NODEF(19),
		ERROR_ENTRY_NODEF10(20),
		ERROR_ENTRY_NODEF10(30),
		ERROR_ENTRY_NODEF10(40), 
		ERROR_ENTRY(50, "На верхнем уровне должны находиться только функции"),
		ERROR_ENTRY_NODEF(51),
		ERROR_ENTRY(52, "Функция обязана возвращать значение"),
		ERROR_ENTRY(53, "Недопустимая инструкция в теле функции"),
		ERROR_ENTRY(54, "Неверно составленное выражение"),
		ERROR_ENTRY(55, "Неверное арифметическое выражение"),
		ERROR_ENTRY(56, "Неверный список параметров"),
		ERROR_ENTRY(57, "Неверный список аргументов"),
		ERROR_ENTRY(58, "Неверно составленное условное выражение"),
		ERROR_ENTRY(59, "В условном блке могут содержаться только операции присваивания и вывода"),
		ERROR_ENTRY(60, "Неверно составленные блоки else if"),
		ERROR_ENTRY(61, "Условием может являться только сравнение двух выражений"),
		ERROR_ENTRY_NODEF(62),
		ERROR_ENTRY(63, "Неизвестная синтаксическая ошибка"),
		ERROR_ENTRY(64, "Отсутствует подходящее правило, подробности в log-файле"),
		ERROR_ENTRY(65, "Отсутствует подходящая цепочка правил"),
		ERROR_ENTRY_NODEF(66),
		ERROR_ENTRY_NODEF(67),
		ERROR_ENTRY_NODEF(68),
		ERROR_ENTRY_NODEF(69),
		ERROR_ENTRY_NODEF10(70),
		ERROR_ENTRY_NODEF10(80),
		ERROR_ENTRY_NODEF10(90),
		ERROR_ENTRY(100, "Параметр -id должен быть задан"), ERROR_ENTRY(101, "Превышено число параметров"), ERROR_ENTRY_NODEF(102),
		ERROR_ENTRY_NODEF(103), ERROR_ENTRY(104, "Превышена длина входного параметра"), ERROR_ENTRY_NODEF(105),
		ERROR_ENTRY_NODEF(106), ERROR_ENTRY_NODEF(107), ERROR_ENTRY_NODEF(108), ERROR_ENTRY_NODEF(109),
		ERROR_ENTRY(110, "Ошибка при открытии файла с исходным кодом (-in)"),
		ERROR_ENTRY(111, "Недопустимый символ в исходном файле (-in)"),
		ERROR_ENTRY(112, "Ошибка при создании файла протокола (-log)"),
		ERROR_ENTRY(113, "Ошибка при создании выходного файла"),
		ERROR_ENTRY_NODEF(114), 
		ERROR_ENTRY_NODEF(115), 
		ERROR_ENTRY_NODEF(116), ERROR_ENTRY_NODEF(117), ERROR_ENTRY_NODEF(118), ERROR_ENTRY_NODEF(119),
		ERROR_ENTRY_NODEF10(120), ERROR_ENTRY_NODEF10(130), ERROR_ENTRY_NODEF10(140),
		ERROR_ENTRY(150, "Отсутствует точка входа"),
		ERROR_ENTRY(151, "Точек входа больше одной"),
		ERROR_ENTRY(152, "Несоответствие типа при присваивании"),
		ERROR_ENTRY(153, "Несоответствие типа параметра"),
		ERROR_ENTRY(154, "Превышено число параметров"),
		ERROR_ENTRY(155, "Недостаточно параметров"),
		ERROR_ENTRY(156, "Не найдено функции с данным идентификатором"),
		ERROR_ENTRY(157, "Функция должна вызываться с параметрами"),
		ERROR_ENTRY(158, "В операции сравнения не может быть больше одного оператора сравнения"),
		ERROR_ENTRY(159, "В операции сравнения обязан присутствовать оператор сравнения"),
		ERROR_ENTRY(160, "Несоответствие типа в операции сравнения"),
		ERROR_ENTRY(161, "Из арифметических операций к строкам и символам можно применять только сложение"),
		ERROR_ENTRY(162, "Присваивать можно только идентификаторам"),
		ERROR_ENTRY(163, "В инструкции до знака присваивания может стоять только идентификатор"),
		ERROR_ENTRY(164, "В операциях сравнения могут участвовать только числа"),
		ERROR_ENTRY(165, "Несоответствие типа параметра библиотечной функции"),
		ERROR_ENTRY(166, "Несоответствие возвращаемого типа библиотечной функции"),
		ERROR_ENTRY_NODEF(167),
		ERROR_ENTRY_NODEF(168),//Зарезервировано
		ERROR_ENTRY_NODEF(169),//Зарезервировано 
		ERROR_ENTRY_NODEF10(170), ERROR_ENTRY_NODEF10(180), ERROR_ENTRY_NODEF10(190),
		/*ERROR_ENTRY_NODEF100(200), ERROR_ENTRY_NODEF100(300), ERROR_ENTRY_NODEF100(400), ERROR_ENTRY_NODEF100(500),
		ERROR_ENTRY_NODEF100(600), ERROR_ENTRY_NODEF100(700), ERROR_ENTRY_NODEF100(800), ERROR_ENTRY_NODEF100(900)*/
	};
	ERROR geterror(int id)
	{
		if ((id >= ERROR_MAX_ENTRY) || (id <= 0)) return errors[0];
		else return errors[id];
	}
	ERROR geterrorin(int id, int line = -1, int col = -1)
	{
		if ((id >= ERROR_MAX_ENTRY) || (id <= 0)) return errors[0];
		else
		{
			ERROR err;
			err = errors[id];
			err.inext.col = col;
			err.inext.line = line;
			for (int i = 0; errors[id].message[i] != 0; i++)
			{
				err.message[i] = errors[id].message[i];
			}
			return err;
		}
	}
	void ErrorDisplay(ERROR e)
	{
		std::cout << "Error #" << e.id << " " << e.message;
		if (e.inext.col != -1 || e.inext.line != -1) std::cout << " at " << e.inext.col + 1 << " column "
			<< e.inext.line << " line\n";
		std::cout << '\n';
	}
}
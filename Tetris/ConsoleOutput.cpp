#include "ConsoleOutput.h"

// Функция вывода кадра на консоль
void ConsoleOutput::print(std::vector<std::string>& frame)
{
	// Получаем количество строк и столбцов в переданном массиве строк
	uint8_t rows = frame.size();
	uint8_t columns = frame[0].size();

	// Нормализуем количество столбцов с учетом горизонтального коэффициента
	uint8_t normalizedColumns = (frame[0].size() * horizontalRatio);

	// Инициализируем переменную для строки вывода
	std::string out;

	// Проходим по всем строкам кадра
	for (int i = 0; i < rows; ++i) {
		// Проходим по всем нормализованным столбцам
		for (int j = 0; j < normalizedColumns; ++j) {
			// Вычисляем индекс символа для текущего нормализованного столбца
			out += frame[i][(j * columns) / normalizedColumns];
		}
		out += '\n';
	}

	// Добавляем строку футера, если она задана
	out += _row;

	// Выводим сформированную строку на консоль
	std::cout << out;

	// Устанавливаем курсор на переданные координаты
	SetConsoleCursorPosition(_console, _pos);
}

void ConsoleOutput::print(std::string text) {
	// Устанавливаем курсор на переданные координаты
	SetConsoleCursorPosition(_console, _pos);

	// Выводим переданный текст на консоль
	std::cout << text;
}

void ConsoleOutput::setRow(std::string str)
{
	_row = str;
}

void ConsoleOutput::end(HANDLE console, COORD position)
{
	// Устанавливаем курсор на переданные координаты
	SetConsoleCursorPosition(console, position);
}
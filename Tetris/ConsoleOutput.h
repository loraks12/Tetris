#pragma once
#include <iostream>
#include <windows.h>
#include <vector>

using namespace std;

// Объявление класса ConsoleOutput
class ConsoleOutput
{
public:
	// Метод для вывода вектора строк на консоль
	void print(vector<string>& frame);
	// Метод для вывода строки на консоль
	void print(string text);
	// Метод для установки строки, которую нужно вывести на консоль
	void setRow(string str);
	// Метод для окончания вывода на консоль (устанавливает новую позицию курсора)
	void end(HANDLE console, COORD position);

private:
	// Дескриптор консольного окна
	HANDLE _console = GetStdHandle(STD_OUTPUT_HANDLE);
	// Позиция курсора
	COORD _pos = { 0, 0 };

	// Константа, определяющая соотношение ширины знаков в строке
	const int8_t horizontalRatio = 2;

	// Строка, которую нужно вывести на консоль
	string _row;
};
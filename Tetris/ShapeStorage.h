#pragma once
#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;

class ShapeStorage   // объявление класса ShapeStorage
{
public: 
	ShapeStorage();   // конструктор класса ShapeStorage
	vector<string> getFigure(uint8_t id);   // метод возвращает фигуру по заданному идентификатору
	char getFigureSymbol();   // метод возвращает символ, используемый для отображения фигуры
	char getBorderSymbol();   // метод возвращает символ, используемый для отображения границы
	char getFieldSymbol();   // метод возвращает символ, используемый для отображения поля
	char getTrajectorySymbol();   // метод возвращает символ, используемый для отображения траектории движения
private: 
	/*-----------Settings----------*/
	const char _figureSymbol = 219;   // символ, используемый для отображения фигуры
	const char _borderSymbol = '#';   // символ, используемый для отображения границы
	const char _fieldSymbol = ' ';   // символ, используемый для отображения поля
	const char _trajectorySymbol = '|';   // символ, используемый для отображения траектории движения
	/*-----------------------------*/

	unordered_map<uint8_t, vector<string>> figureMap;   // хранилище фигур, используемых в приложении
};
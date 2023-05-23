#pragma once
#include <iostream>
#include "ShapeStorage.h"
#include "ConsoleOutput.h"


// объявление класса FrameBuilder
class FrameBuilder
{
public:
	// конструкторы класса
	FrameBuilder();
	FrameBuilder(std::vector<std::string>& frame);

	// деструктор класса
	~FrameBuilder();

	// методы доступа к фигурам
	std::vector<std::string>& getFigure();
	std::vector<std::string> getFigure(uint8_t figureId, uint8_t state);
	void setFigure(uint8_t x, uint8_t y, uint8_t figureId, uint8_t state);
	void removeFigure(uint8_t x, uint8_t y, uint8_t figureId, uint8_t state);

	// методы для работы с траекториями движения фигур
	void setTrajectory(uint8_t x, int8_t y);
	void removeTrajectory(uint8_t x, uint8_t y);

	// методы доступа к кадру
	std::vector<std::string>& getFrame();
	void setFrame(std::vector<std::string>& frame);
	void setRow(std::string str);
	void showMessage(std::string message);

	// методы для печати фигур на экран
	void print();
	void theEnd();

private:
	// объект для хранения фигур
	ShapeStorage _shape;

	// объект для вывода на консоль
	ConsoleOutput _output;

	// символы для отображения поля и траектории
	char _fieldSymbol;
	char _trajectorySymbol;

	// указатель на текущий кадр и отображаемую фигуру
	std::vector<std::string>* _frame;
	std::vector<std::string> _figure;

	// методы поворота фигуры
	std::vector<std::string>& rotate(std::vector<std::string>& _figureSymbol, uint8_t state);
	std::vector<std::string>& rotate(std::vector<std::string>&& _figureSymbol, uint8_t state);
};
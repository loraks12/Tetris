#include "ShapeStorage.h"
#include <string>

// Конструктор класса ShapeStorage
ShapeStorage::ShapeStorage()
{
    // Инициализация карты фигур
    figureMap[0] = { _figureSymbol + std::string(1,_fieldSymbol), _figureSymbol + std::string(1,_fieldSymbol) , _figureSymbol + std::string("") + _figureSymbol + std::string("") }; //T
    figureMap[1] = { std::string(1,_fieldSymbol) + _figureSymbol, std::string(1,_fieldSymbol) + _figureSymbol, std::string("") + _figureSymbol + std::string("") + _figureSymbol }; // S
    figureMap[2] = { std::string("") + _figureSymbol + std::string("") + _figureSymbol, std::string("") + _figureSymbol + std::string("") + _figureSymbol }; // квадрат
    figureMap[3] = { std::string("") + _figureSymbol, std::string("") + _figureSymbol, std::string("") + _figureSymbol, std::string("") + _figureSymbol }; // I
    figureMap[4] = { std::string("") + _figureSymbol + _figureSymbol + _figureSymbol, std::string(1,_fieldSymbol) + _figureSymbol + std::string(1,_fieldSymbol) }; // Z
    figureMap[5] = { std::string("") + _figureSymbol + _figureSymbol + std::string(1,_fieldSymbol), std::string(1,_fieldSymbol) + _figureSymbol + _figureSymbol }; // L или Г
    figureMap[6] = { std::string(1,_fieldSymbol) + _figureSymbol + _figureSymbol + std::string(""), std::string("") + _figureSymbol + _figureSymbol + std::string(1,_fieldSymbol) }; // L или Г
    //figureMap[7] = { std::string("") + std::string(5,_figureSymbol) }; // Straight line
}

// Получение фигуры по ее идентификатору
std::vector<std::string> ShapeStorage::getFigure(uint8_t id)
{
    return figureMap[id];
}

// Получение символа фигурки
char ShapeStorage::getFigureSymbol()
{
    return _figureSymbol;
}

// Получение символа границы поля
char ShapeStorage::getBorderSymbol()
{
    return _borderSymbol;
}

// Получение символа пустой клетки в поле
char ShapeStorage::getFieldSymbol()
{
    return _fieldSymbol;
}

// Получение символа траектории фигурки
char ShapeStorage::getTrajectorySymbol()
{
    return _trajectorySymbol;
}

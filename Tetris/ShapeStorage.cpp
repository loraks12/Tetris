#include "ShapeStorage.h"
#include <string>



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

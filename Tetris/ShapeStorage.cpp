#include "ShapeStorage.h"
#include <string>

// ����������� ������ ShapeStorage
ShapeStorage::ShapeStorage()
{
    // ������������� ����� �����
    figureMap[0] = { _figureSymbol + std::string(1,_fieldSymbol), _figureSymbol + std::string(1,_fieldSymbol) , _figureSymbol + std::string("") + _figureSymbol + std::string("") }; //T
    figureMap[1] = { std::string(1,_fieldSymbol) + _figureSymbol, std::string(1,_fieldSymbol) + _figureSymbol, std::string("") + _figureSymbol + std::string("") + _figureSymbol }; // S
    figureMap[2] = { std::string("") + _figureSymbol + std::string("") + _figureSymbol, std::string("") + _figureSymbol + std::string("") + _figureSymbol }; // �������
    figureMap[3] = { std::string("") + _figureSymbol, std::string("") + _figureSymbol, std::string("") + _figureSymbol, std::string("") + _figureSymbol }; // I
    figureMap[4] = { std::string("") + _figureSymbol + _figureSymbol + _figureSymbol, std::string(1,_fieldSymbol) + _figureSymbol + std::string(1,_fieldSymbol) }; // Z
    figureMap[5] = { std::string("") + _figureSymbol + _figureSymbol + std::string(1,_fieldSymbol), std::string(1,_fieldSymbol) + _figureSymbol + _figureSymbol }; // L ��� �
    figureMap[6] = { std::string(1,_fieldSymbol) + _figureSymbol + _figureSymbol + std::string(""), std::string("") + _figureSymbol + _figureSymbol + std::string(1,_fieldSymbol) }; // L ��� �
    //figureMap[7] = { std::string("") + std::string(5,_figureSymbol) }; // Straight line
}

// ��������� ������ �� �� ��������������
std::vector<std::string> ShapeStorage::getFigure(uint8_t id)
{
    return figureMap[id];
}

// ��������� ������� �������
char ShapeStorage::getFigureSymbol()
{
    return _figureSymbol;
}

// ��������� ������� ������� ����
char ShapeStorage::getBorderSymbol()
{
    return _borderSymbol;
}

// ��������� ������� ������ ������ � ����
char ShapeStorage::getFieldSymbol()
{
    return _fieldSymbol;
}

// ��������� ������� ���������� �������
char ShapeStorage::getTrajectorySymbol()
{
    return _trajectorySymbol;
}

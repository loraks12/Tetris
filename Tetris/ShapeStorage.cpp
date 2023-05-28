#include "ShapeStorage.h"
#include <string>



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

#pragma once
#include <iostream>
#include <string>
#include "FrameBuilder.h"
#include "ShapeStorage.h"
#include <chrono>
#include <windows.h>

// Класс, отвечающий за игровой процесс

class Gameplay
{
public:
    Gameplay(); // Конструктор
    ~Gameplay(); // Деструктор

    void start(); // Запускает игру
    void update(); // Обновляет игровые объекты на каждом кадре
    bool keyboardCheck(); // Проверяет состояние клавиатуры
    void move(int8_t deltaX = 0, int8_t deltaY = 0); // Двигает текущую фигуру
    void rotate(int8_t deltaState); // Поворачивает текущую фигуру на 90 градусов
    void setNewFigure(uint8_t x, uint8_t y, uint8_t figureId); // Устанавливает новую фигуру на поле

    uint8_t getFigureHeight(); // Возвращает высоту текущей фигуры
    uint8_t getFigureWidth(); // Возвращает ширину текущей фигуры

    void checkLine(); // Проверяет, есть ли заполненные линии на поле
    bool checkWall(int8_t deltaX, int8_t deltaY, int8_t deltaState = 0); // Проверяет, сталкивается ли текущая фигура со стенами или другими фигурами на поле

    std::vector<char> getSymbols(); // Возвращает вектор символов, используемых в игре 

private:
    /*-----------Settings----------*/
    uint8_t _fieldHeight = 25; // Высота игрового поля
    uint8_t _fieldWidth = 12; // Ширина игрового поля

    int16_t _gameSpeed = 750; // Скорость игры

    bool _trajectory = true; // Отображать траекторию текущей фигуры
    /*-----------------------------*/

    // Переменные, отвечающие за время
    std::chrono::time_point<struct std::chrono::system_clock, class std::chrono::duration<__int64, struct std::ratio<1, 10000000>>> timeStart = std::chrono::system_clock::now();
    std::chrono::time_point<struct std::chrono::system_clock, class std::chrono::duration<__int64, struct std::ratio<1, 10000000>>> timeCheckStartRot = std::chrono::system_clock::now();
    std::chrono::time_point<struct std::chrono::system_clock, class std::chrono::duration<__int64, struct std::ratio<1, 10000000>>> timeCheckStartMove = std::chrono::system_clock::now();
    std::chrono::time_point<struct std::chrono::system_clock, class std::chrono::duration<__int64, struct std::ratio<1, 10000000>>> timeCheckStartSpace = std::chrono::system_clock::now();
    std::chrono::time_point<struct std::chrono::system_clock, class std::chrono::duration<__int64, struct std::ratio<1, 10000000>>> timeEnd = std::chrono::system_clock::now();
    std::chrono::time_point<struct std::chrono::system_clock, class std::chrono::duration<__int64, struct std::ratio<1, 10000000>>> timeCheckEndRot = std::chrono::system_clock::now();
    std::chrono::time_point<struct std::chrono::system_clock, class std::chrono::duration<__int64, struct std::ratio<1, 10000000>>> timeCheckEndMove = std::chrono::system_clock::now();
    std::chrono::time_point<struct std::chrono::system_clock, class std::chrono::duration<__int64, struct std::ratio<1, 10000000>>> timeCheckEndSpace = std::chrono::system_clock::now();

    // Объекты для отрисовки игрового поля
    FrameBuilder _frameBuilder;
    ShapeStorage _shapeStorage;

    // Символы для отображения поля и фигур
    char _borderSymbol;
    char _fieldSymbol;
    char _figureSymbol;

    // Фигуры
    std::vector<std::string>* _figure = nullptr;

    // Флаги и счетчики
    bool _isNewFigure = false; // Новая фигура была установлена на поле
    bool _isEnd = false; // Игрок проиграл
    bool _isAction = false; // Действие выполняется (перемещение фигуры, поворот)
    bool _isSpace = false; // Пробел был нажат

    uint64_t _score = 0; // Очки
    uint16_t _scoreAdd = 10; // Количество очков, добавляемых за заполненную линию
    uint16_t _line = 0; // Количество заполненных линий

    // Координаты текущей фигуры
    uint8_t _x = 16;
    uint8_t _y = 1;

    // Идентификатор текущей фигуры и ее состояние
    uint8_t _figureId = 6;
    uint8_t _state = 0;

    // Размеры текущей фигуры
    uint8_t _figureHeight = 0;
    uint8_t _figureWidth = 0;

    const uint16_t _gameSpeedWhenSpace = 20; // Скорость игры при нажатии на пробел
    int16_t _currentSpeed = _gameSpeed; // Текущая скорость игры

    uint16_t _timeForSpace = 1000; // Время, через которое появляется новый фигура после нажатия на пробел
    uint8_t _timeForMove = 100; // Время для перемещения фигуры в стороны
    uint8_t _timeForRotate = 150; // Время для вращения фигуры

    uint8_t map(int8_t value, uint8_t max, uint8_t min); // Функция отображения диапазона значений
    uint8_t clip(int8_t value, uint8_t max, uint8_t min); // Функция ограничения значений
    void cheatLine();
    void scoreUpdate(uint8_t upscale); // Функция для обновления счета игрока
};

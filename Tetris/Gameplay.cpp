#include "Gameplay.h"

Gameplay::Gameplay() {
    _figure = nullptr;
}

Gameplay::~Gameplay()
{
    if (_figure) {
        _figure->clear();
        delete _figure;
    }
}


void Gameplay::start()
{
    _borderSymbol = getSymbols()[0]; // Установка символов границы, поля и фигуры
    _fieldSymbol = getSymbols()[1];
    _figureSymbol = getSymbols()[2];

    std::vector<std::string> frame;

    frame.emplace_back(std::string(_fieldWidth, _borderSymbol)); // Добавление верхней границы поля в кадр
    for (int i = 0; i < _fieldHeight - 2; i++) // Добавление строк поля в кадр
        frame.emplace_back(std::string(1, _borderSymbol) + std::string(_fieldWidth - 2, _fieldSymbol) + std::string(1, _borderSymbol));
    frame.emplace_back(std::string(_fieldWidth, _borderSymbol)); // Добавление нижней границы поля в кадр

    srand(time(0));
    _frameBuilder.setFrame(frame); // Установка созданного кадра в строитель кадра
    update(); // Вызов функции обновления игры
}

void Gameplay::update()
{
    while (!_isEnd) // Цикл игры, пока игра не закончится
    {
        if (!_isNewFigure) { // Если текущая фигура уже упала на дно поля
            _figureId = rand() % 6;
            checkLine(); // Проверка на заполнение целых строк
            try {
                setNewFigure(_frameBuilder.getFrame()[0].size() / 2 - 1, 1, _figureId); // Установка новой фигуры на поле
            }
            catch (int ex) { // Обработка исключений при установке новой фигуры
                switch (ex)
                {
                case 0:
                    _isEnd = true;
                    _frameBuilder.showMessage("This figure is not suitable in 'X' size!"); // Вывод сообщения об ошибке в случае неподходящей ширины фигуры
                    continue;
                    break;
                case 1:
                    _isEnd = true;
                    _frameBuilder.showMessage("This figure is not suitable in 'Y' size!"); // Вывод сообщения об ошибке в случае неподходящей высоты фигуры
                    continue;
                    break;
                default:
                    break;
                }
            }
            if (!checkWall(0, 1)) { // Если новая фигура не может быть установлена на поле
                _isEnd = false;
                _frameBuilder.theEnd(); // Вывод сообщения о конце игры
                break;
            }
        }

        keyboardCheck(); // Проверка нажатых клавиш

        _frameBuilder.setRow("X: " + std::to_string(_x) + " Y: " + std::to_string(_y) + " Width: " + std::to_string(_figureWidth) + " Height: " + std::to_string(_figureHeight) + "  \n" + "Score: " + std::to_string(_score) + "\nLine: " + std::to_string(_line)); // Установка строки информации о текущей фигуре, счете и количестве заполненных строк

        timeEnd = std::chrono::system_clock::now(); // Получение текущего времени

        if (std::chrono::duration_cast<std::chrono::milliseconds>(timeEnd - timeStart).count() > _currentSpeed) { // Проверка времени упавшей фигуры
            if (checkWall(0, 1)) { // Если упавшая фигура не столкнулась с другими фигурами
                move(0, 1); // Сдвиг фигуры на одну строку вниз
            }
            timeStart = std::chrono::system_clock::now(); // Обновление времени упавшей фигуры
        }

        if (_trajectory) // Если включен режим отображения траектории фигуры
            _frameBuilder.setTrajectory(_x, _y); // Отображение траектории фигуры на поле

        _frameBuilder.print(); // Вывод текущего кадра игры

        if (_trajectory) // Если режим отображения траектории включен
            _frameBuilder.removeTrajectory(_x, _y); // Удаление отображения траектории фигуры с поля
    }
}

bool Gameplay::keyboardCheck() {

    _isAction = false;

    timeCheckEndMove = std::chrono::system_clock::now();    // Запись времени окончания движения для проверки времени между движениями

    // Обработка нажатий клавиш для движения фигуры влево, вправо, вверх или вниз
    if (!_isSpace) {
        if (GetAsyncKeyState(VK_RIGHT) && (std::chrono::duration_cast<std::chrono::milliseconds>(timeCheckEndMove - timeCheckStartMove).count() > _timeForMove)) {
            // Проверка возможности перемещения фигуры вправо
            if (checkWall(1, 0)) {
                move(1);
                _isAction = true;
            }
            // Запись времени начала движения после его выполнения
            timeCheckStartMove = std::chrono::system_clock::now();
        }
        if (GetAsyncKeyState(VK_LEFT) && (std::chrono::duration_cast<std::chrono::milliseconds>(timeCheckEndMove - timeCheckStartMove).count() > _timeForMove))
        {
            // Проверка возможности перемещения фигуры влево
            if (checkWall(-1, 0)) {
                move(-1);
                _isAction = true;
            }
            timeCheckStartMove = std::chrono::system_clock::now();
        }
        if (GetAsyncKeyState(VK_UP) && (std::chrono::duration_cast<std::chrono::milliseconds>(timeCheckEndMove - timeCheckStartMove).count() > _timeForMove))
        {
            // Проверка возможности перемещения фигуры вверх
            if (checkWall(0, -1)) {
                move(0, -1);
                _isAction = true;
            }
            timeCheckStartMove = std::chrono::system_clock::now();
        }
        if (GetAsyncKeyState(VK_DOWN) && (std::chrono::duration_cast<std::chrono::milliseconds>(timeCheckEndMove - timeCheckStartMove).count() > _timeForMove))
        {
            // Проверка возможности перемещения фигуры вниз
            if (checkWall(0, 1)) {
                move(0, 1);
                _isAction = true;
            }
            timeCheckStartMove = std::chrono::system_clock::now();
        }

        // Запись времени окончания поворота для проверки времени между ними
        timeCheckEndRot = std::chrono::system_clock::now();
        if (GetAsyncKeyState(65) && (std::chrono::duration_cast<std::chrono::milliseconds>(timeCheckEndRot - timeCheckStartRot).count() > _timeForRotate))
        {
            // Проверка возможности поворота фигуры против часовой стрелки
            if (checkWall(0, 0, -1)) {
                rotate(-1);
                _isAction = true;
            }
            // Запись времени начала поворота после его выполнения
            timeCheckStartRot = std::chrono::system_clock::now();
        }
        if (GetAsyncKeyState(83) && (std::chrono::duration_cast<std::chrono::milliseconds>(timeCheckEndRot - timeCheckStartRot).count() > _timeForRotate))
        {
            // Проверка возможности поворота фигуры по часовой стрелке
            if (checkWall(0, 0, 1)) {
                rotate(1);
                _isAction = true;
            }
            // Запись времени начала поворота после его выполнения
            timeCheckStartRot = std::chrono::system_clock::now();
        }
    }

    // Замеряем время нажатия клавиши "Пробел" или "Ctrl"
    timeCheckEndSpace = std::chrono::system_clock::now();

    // Проверяем, была ли нажата клавиша "Пробел" или "Ctrl", а также проверяем, была ли уже нажата клавиша "Пробел" и прошло ли достаточно времени с последнего нажатия
    if ((GetAsyncKeyState(VK_SPACE) || GetAsyncKeyState(VK_CONTROL)) && !_isSpace && (std::chrono::duration_cast<std::chrono::milliseconds>(timeCheckEndSpace - timeCheckStartSpace).count() > _timeForSpace))
    {
        // Если была нажата клавиша "Ctrl"
        if (GetAsyncKeyState(VK_CONTROL)) {
            move(0, -1);
            cheatLine();
            timeCheckStartSpace = std::chrono::system_clock::now();
            return _isAction;
        }

        _isSpace = true;        // Устанавливаем флаг, что клавиша "Пробел" была нажата

        _currentSpeed = _gameSpeedWhenSpace; //скорость, когда клавиша "Пробел" была нажата

        _isAction = true;

        timeCheckStartSpace = std::chrono::system_clock::now();        // Обновляем время последнего нажатия
    }
    return _isAction;
}

void Gameplay::move(int8_t deltaX, int8_t deltaY)
{
    if (deltaX > 1 && deltaX < -1) // Проверка допустимой величины смещения по х
        return;
    if (deltaY > 1 && deltaY < -1) // Проверка допустимой величины смещения по y
        return;

    _frameBuilder.removeFigure(_x, _y, _figureId, _state); // Удаление фигуры с текущей позиции

    _x += deltaX; // Изменение координаты по х
    _y += deltaY; // Изменение координаты по y

    _frameBuilder.setFigure(_x, _y, _figureId, _state); // Установка фигуры на новую позицию
}

void Gameplay::rotate(int8_t deltaState)
{
    if (deltaState > 1 || deltaState < -1) { // Проверка допустимости величины поворота 
        return;
    }

    if (_x + _figureHeight >= _frameBuilder.getFrame()[0].size()) { // Проверка, чтобы фигура не выходила за границы поля по х
        return;
    }
    if (_y + _figureWidth >= _frameBuilder.getFrame().size()) { // Проверка, чтобы фигура не выходила за границы поля по y
        return;
    }

    _frameBuilder.removeFigure(_x, _y, _figureId, _state); // Удаление фигуры с текущей позиции

    uint8_t temp = _figureHeight;
    _figureHeight = _figureWidth; // Изменение высоты и ширины фигуры при повороте
    _figureWidth = temp;

    if (_state + deltaState > 3) { // Проверка на цикличность состояния фигуры
        _state = 0;
    }
    else if (_state + deltaState < 0) {
        _state = 3;
    }
    else {
        _state += deltaState;
    }

    _frameBuilder.setFigure(_x, _y, _figureId, _state); // Установка фигуры на новую позицию
    *_figure = _frameBuilder.getFigure();
}
// Устанавливает новую фигуру с указанными координатами и идентификатором фигуры
void Gameplay::setNewFigure(uint8_t x, uint8_t y, uint8_t figureId)
{
    _isNewFigure = true; // устанавливаем флаг новой фигуры
    _isSpace = false; // устанавливаем флаг пробела в false
    _currentSpeed = _gameSpeed; // устанавливаем текущую скорость игры

    // если фигура еще не создана, создаем ее
    if (!_figure) {
        _figure = new std::vector<std::string>(_shapeStorage.getFigure(_figureId));
    }
    // иначе перезаписываем фигуру вектором из хранилища фигур
    else {
        *_figure = _shapeStorage.getFigure(_figureId);
    }

    _x = x; // устанавливаем координату по x
    _y = y; // устанавливаем координату по y
    _figureId = figureId; // устанавливаем идентификатор текущей фигуры
    _state = 0;

    _figureHeight = (*_figure).size(); // определяем высоту фигуры
    _figureWidth = (*_figure)[0].size(); // определяем ширину фигуры

    // проверяем, не выходит ли фигура за границы поля
    if (_figureWidth > _frameBuilder.getFrame()[0].size() - 2) {
        throw 0; // выбрасываем исключение, если фигура слишком широкая
    }
    if (_figureHeight > _frameBuilder.getFrame().size() - 2) {
        throw 1; // выбрасываем исключение, если фигура слишком высокая
    }
    if (_figureWidth - 1 + _x > _frameBuilder.getFrame()[0].size() - 2) {
        _x += ((_frameBuilder.getFrame()[0].size() - 2) - (_figureWidth - 1 + _x)); // сдвигаем фигуру влево, чтобы она не выходила за правую границу поля
    }
    if (_figureHeight - 1 + _y > _frameBuilder.getFrame().size() - 2) {
        _y += ((_frameBuilder.getFrame().size() - 2) - (_figureHeight - 1 + _y)); // сдвигаем фигуру вверх, чтобы она не выходила за верхнюю границу поля
    }

    _frameBuilder.setFigure(_x, _y, _figureId, _state); // устанавливаем фигуру в рамку
}

// Возвращает высоту текущей фигуры
uint8_t Gameplay::getFigureHeight()
{
    return _figureHeight;
}

// Возвращает ширину текущей фигуры
uint8_t Gameplay::getFigureWidth()
{
    return _figureWidth;
}

// Возвращает символы для отображения рамки и фигур на поле
std::vector<char> Gameplay::getSymbols()
{
    std::vector<char> symbols = { _shapeStorage.getBorderSymbol(), _shapeStorage.getFieldSymbol(), _shapeStorage.getFigureSymbol() };
    return symbols;
}


void Gameplay::checkLine()
{
    uint8_t upscale = 1; // переменная, хранящая количество удаляемых линий
    bool isFit = true; // флаг, показывающий, подходит ли текущая линия для удаления
    std::vector<std::string> frame = _frameBuilder.getFrame(); // получаем текущее поле
    for (int i = 0; i < frame.size() - 4; ++i) { // проходимся по всем линиям сверху вниз, кроме последних трех строк
        isFit = true; // начинаем проверку с флага "true"
        while (isFit) { // цикл будет выполняться, пока линия удалена
            for (int j = 1; j < frame[0].size() - 1; ++j) { // проходимся по всем символам текущей линии, кроме крайних символов
                if (frame[frame.size() - 2 - i][j] != _shapeStorage.getFigureSymbol()) { // если какой-то символ не совпадает с символом текущей фигуры, прекращаем проверку
                    isFit = false;
                    break;
                }
            }
            if (isFit) { // если все символы равны, удаляем текущую линию и добавляем новую сверху
                for (int k = i; k < frame.size() - 2; k++) {
                    if (frame[frame.size() - 3 - k][1] == _borderSymbol) { // если следующая строка - граница, добавляем новую границу сверху
                        frame[frame.size() - 2 - k] = std::string(1, _borderSymbol) + std::string(_frameBuilder.getFrame()[0].size() - 2, _fieldSymbol) + std::string(1, _borderSymbol);
                    }
                    else { // если следующая строка не граница, копируем ее на текущую позицию
                        frame[frame.size() - 2 - k] = frame[frame.size() - 3 - k];
                    }
                }
                ++upscale; // увеличиваем количество удаленных линий
                ++_line; // увеличиваем общее количество удаленных линий
                _frameBuilder.setFrame(frame); // устанавливаем новое поле
            }
        }
    }
    scoreUpdate(upscale); // обновляем счет
}

void Gameplay::cheatLine() {
    _y++; // сдвигаем текущую фигуру на одну линию вниз
    std::vector<std::string> frame = _frameBuilder.getFrame(); // получаем текущее поле
    for (int k = 0; k < frame.size() - 3; k++) { // копируем все строки поля на одну линию вниз
        frame[frame.size() - 2 - k] = frame[frame.size() - 3 - k];
    }
    ++_line; // увеличиваем количество удаленных линий
    _frameBuilder.setFrame(frame); // устанавливаем новое поле
    scoreUpdate(1); // обновляем счет
}


// Проверка наличия стенок, чтобы фигурка не выходила за пределы поля
bool Gameplay::checkWall(int8_t deltaX, int8_t deltaY, int8_t deltaState)
{
    std::vector<std::string> _frame = _frameBuilder.getFrame();

    // Проверка левой стенки
    if (_x <= 1 && deltaX < 0) {
        return false;
    }
    // Проверка правой стенки
    if (_x + _figureWidth >= _frame[0].size() - 1 && deltaX > 0) {
        return false;
    }
    // Проверка верхней стенки
    if (_y <= 1 && deltaY < 0) {
        return false;
    }
    // Проверка нижней стенки
    if (_y + _figureHeight >= _frame.size() - 1 && deltaY > 0) {
        _isNewFigure = false;
        return false;
    }

    // Проверка наличия препятствий в пути движения фигурки
    if (deltaY != 0) {
        int offsetY = 0;
        for (int i = 0; i < _figureWidth; ++i) {
            while ((*_figure)[_figureHeight - 1 - offsetY][i] == _fieldSymbol && deltaY > 0) {
                ++offsetY;
            }
            if (_frame[_y + _figureHeight - offsetY][_x + i] == _shapeStorage.getFigureSymbol() && deltaY > 0) {
                _isNewFigure = false;
                return false;
            }
            offsetY = 0;
        }
    }
    if (deltaX != 0) {
        int offsetX = 0;
        // Проверка наличия препятствий на пути движения влево
        for (int i = 0; i < _figureHeight; ++i) {
            while ((*_figure)[i][offsetX] == _fieldSymbol && deltaX < 0) {
                ++offsetX;
            }
            if (_frame[_y + i][_x + offsetX - 1] == _shapeStorage.getFigureSymbol() && deltaX < 0) {
                return false;
            }
            offsetX = 0;
        }
        // Проверка наличия препятствий на пути движения вправо
        for (int i = 0; i < _figureHeight; ++i) {
            while ((*_figure)[i][_figureWidth - 1 - offsetX] == _fieldSymbol && deltaX > 0) {
                ++offsetX;
            }
            if (_frame[_y + i][_x + _figureWidth - offsetX] == _shapeStorage.getFigureSymbol() && deltaX > 0) {
                return false;
            }
            offsetX = 0;
        }
    }
    // Проверка на возможность поворота фигурки
    if (deltaState != 0) { // Проверяем, нужен ли поворот фигурки
        if (_figureHeight + _x > _frame[0].size()) { // Проверяем, что точка поворота фигурки не выходит за границы поля
            return false;
        }
        std::vector<std::string> temp = _frameBuilder.getFigure(_figureId, map(_state + deltaState, 3, 0)); // Получаем временную копию фигурки, повернутой на заданный угол
        for (int i = 0; i < temp.size(); ++i) { // Проходимся по полученной фигурке
            for (int j = 0; j < temp[0].size(); ++j) {
                if (temp[i][j] == _fieldSymbol) // Пропускаем пустые клетки
                    continue;
                else if (_frame[_y + i][_x + j] == _fieldSymbol) // Проверяем, находится ли на пути поворота фигурки препятствие
                    continue;

                if (_frame[_y + i][_x + j] != _fieldSymbol) { // Если в текущей клетке на поле находится другая фигурка
                    if (clip(i, (*_figure).size() - 1, 0) != i || clip(j, (*_figure)[0].size() - 1, 0) != j) { // Проверяем, что клетка находится внутри текущей фигурки
                        return false; // Если клетка находится за пределами, то нельзя сделать поворот
                    }
                    else {
                        if ((*_figure)[clip(i, (*_figure).size() - 1, 0)][clip(j, (*_figure)[0].size() - 1, 0)] == _fieldSymbol) { // Проверяем, что внутри текущей фигурки нет другой фигурки в этой клетке
                            return false; // Если в клетке другая фигурка, то нельзя сделать поворот
                        }
                    }
                }
            }
        }
    }

    return true;
}


// Функция, которая переводит значение в диапазоне от min до max
// в зацикленный диапазон, где max и min соседние значения.
uint8_t Gameplay::map(int8_t value, uint8_t max, uint8_t min)
{
    while (value > max) {
        value -= (max - min + 1);
    }
    while (value < min) {
        value += (max - min + 1);
    }
    return value;
}

// Функция, которая ограничивает значение в диапазоне от min до max.
uint8_t Gameplay::clip(int8_t value, uint8_t max, uint8_t min)
{
    if (value > max)
        return max;
    if (value < min)
        return min;
    return value;
}

// Функция, которая обновляет баллы игрока в зависимости от количества линий, которые он убрал.
void Gameplay::scoreUpdate(uint8_t upscale)
{
    _score += _scoreAdd * upscale;
    switch (_line)
    {
        // Увеличиваем скорость игры и баллы при достижении определенного количества линий.
    case 10:
        _gameSpeed -= 50;
        _scoreAdd += 50;
        break;
    case 30:
        _gameSpeed -= 50;
        _scoreAdd += 200;
        break;
    case 50:
        _gameSpeed -= 50;
        _scoreAdd += 500;
        break;
    case 60:
        _gameSpeed -= 50;
        _scoreAdd += 2000;
        break;
    case 70:
        _gameSpeed -= 50;
        _scoreAdd += 10000;
        break;
    case 75:
        _gameSpeed -= 50;
        _scoreAdd += 10000;
        break;
    case 80:
        _gameSpeed -= 100;
        _scoreAdd += 10000;
        break;
    case 85:
        _gameSpeed -= 100;
        _scoreAdd += 10000;
        break;
    case 90:
        _gameSpeed -= 100;
        _scoreAdd += 10000;
        break;
    case 100:
        _gameSpeed -= 110;
        _scoreAdd += 10000;
        break;
    default:
        break;
    }
    _currentSpeed = _gameSpeed; // Обновляем текущую скорость игры.
}
#include "FrameBuilder.h"

FrameBuilder::FrameBuilder()
{
	_frame = nullptr;
	// Получаем символы для поля и траектории из объекта "shape"
	_fieldSymbol = _shape.getFieldSymbol();
	_trajectorySymbol = _shape.getTrajectorySymbol();
}

FrameBuilder::FrameBuilder(std::vector<std::string>& frame)
{
	_frame = &frame;
}

FrameBuilder::~FrameBuilder()
{
	// Если указатель _frame не равен nullptr, то очищаем вектор и удаляем его
	if (_frame) {
		_frame->clear();
		delete _frame;
	}
}

void FrameBuilder::setFigure(uint8_t x, uint8_t y, uint8_t figureId, uint8_t state)
{	
	_figure = _shape.getFigure(figureId);// Получаем фигуру по ее идентификатору
	_figure = rotate(_figure, state);// Поворачиваем фигуру

	// Проверяем, не выходит ли фигура за пределы игрового поля
	if (x + _figure[0].size() > (*_frame)[0].size() || y + _figure.size() > _frame->size() ||
		x < 0 || y < 0 || x >= (*_frame)[0].size() || y >= _frame->size()) {
		return;
	}
	// Проверяем, не является ли переданный идентификатор некорректным
	if (figureId < 0) {
		return;
	}
	bool isCollision = false;

	// Проверяем на столкновение между фигурой и игровым полем
	for (int i = 0; i < _figure.size(); ++i) {
		for (int j = 0; j < _figure[0].size(); ++j) {
			if (_figure[i][j] == _fieldSymbol) {
				continue;
			}
			if ((*_frame)[y + i][x + j] != _fieldSymbol) {
				isCollision = true;
				break;
			}
		}
		if (isCollision)
			break;
	}
	// Если нет столкновений, то помещаем фигуру на игровое поле
	if (!isCollision) {
		for (int i = 0; i < _figure.size(); ++i) {
			for (int j = 0; j < _figure[0].size(); ++j) {
				if (_figure[i][j] == _fieldSymbol) {
					continue;
				}
				(*_frame)[y + i][x + j] = _figure[i][j];
			}
		}
	}
}

// Функция возвращает ссылку на текущее состояние поля игры
std::vector<std::string>& FrameBuilder::getFigure()
{
	return _figure;
}

// Функция возвращает символы фигуры в указанном состоянии и повороте
std::vector<std::string> FrameBuilder::getFigure(uint8_t figureId, uint8_t state)
{
	return rotate(_shape.getFigure(figureId), state);
}

// Функция удаляет фигуру, заданную координатами (x, y), идентификатором и состоянием
void FrameBuilder::removeFigure(uint8_t x, uint8_t y, uint8_t figureId, uint8_t state)
{
	std::vector<std::string> _figureSymbol = _shape.getFigure(figureId);
	_figureSymbol = rotate(_figureSymbol, state);

	// Если фигура выходит за границы поля игры, то ничего не делаем
	if (x + _figureSymbol[0].size() > (*_frame)[0].size() || y + _figureSymbol.size() > _frame->size()) {
		return;
	}
	//координаты находятся за пределами поля игры
	else if (x < 0 || y < 0 || x >= (*_frame)[0].size() || y >= _frame->size()) {
		return;
	}
	//Если идентификатор фигуры некорректен
	else if (figureId < 0) {
		return;
	}

	// Задаем высоту и ширину фигуры
	uint8_t height = _figureSymbol.size();
	uint8_t width = _figureSymbol[0].size();


	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			if (_figureSymbol[i][j] == _fieldSymbol)
				continue;
			else if ((*_frame)[y + i][x + j] == _fieldSymbol)  // Если на поле в данной позиции уже находится символ - пропускаем
				continue;

			if (_figureSymbol[i][j] == (*_frame)[y + i][x + j]) { // Если символ в фигуре равен символу на поле, заменяем его на символ поля
				(*_frame)[y + i][x + j] = _fieldSymbol;
			}
		}
	}
}

void FrameBuilder::setTrajectory(uint8_t x, int8_t y)
{
	// Если фигура оказалась на последней строке рамки, то выходим из метода.
	if (y + _figure.size() == (*_frame).size() - 1)
		return;

	int8_t _y = y;
	for (int i = 0; i < 2; ++i) {
		_y = y;
		// Если нашли препятствие, то продолжаем искать траекторию.

		if ((*_frame)[_figure.size() + _y][x] != _fieldSymbol)
			continue;

		// Данный цикл нужен для нахождения нижней границы пустой области, на которую мы должны нарисовать траекторию.
		while ((*_frame)[_figure.size() + _y--][x] == _fieldSymbol);
		++_y;

		// Данный цикл нужен для изменения символов на игровом поле на символ с траекторией движения фигуры.
		while ((*_frame)[_figure.size() + ++_y][x] == _fieldSymbol) {
			(*_frame)[_figure.size() + _y][x] = _trajectorySymbol;
		}

		// Далее мы переходим к следующей колонке фигуры.
		x += _figure[0].size() - 1;
	}
}

void FrameBuilder::removeTrajectory(uint8_t x, uint8_t y)
{
	if (y + _figure.size() == (*_frame).size() - 1) //координата Y + размер фигуры равна размеру поля минус 1
		return;

	// Координата начала проверки на наличие траектории
	int8_t _y = y;
	for (int i = 0; i < 2; ++i) { // дважды проходим цикл для удаления траектории слева и справа от фигуры
		_y = y;
		if ((*_frame)[_figure.size() + _y][x] != _trajectorySymbol) // если на данной координате нет траектории, то переходим к следующей координате
			continue;
		while ((*_frame)[_figure.size() + _y--][x] == _trajectorySymbol); // ищем начало траектории сверху вниз
		++_y; // корректируем координату Y начала траектории
		while ((*_frame)[_figure.size() + ++_y][x] == _trajectorySymbol) { // проходим траекторию сверху вниз и удаляем её на пути
			(*_frame)[_figure.size() + _y][x] = _fieldSymbol;
		}
		x += _figure[0].size() - 1; // корректируем координату X, чтобы перейти в начало следующей фигуры
	}
}

void FrameBuilder::print()
{
	_output.print(*_frame); //вывод рамки из объекта _frame
}

std::vector<std::string>& FrameBuilder::getFrame()
{
	return *_frame; //ссылка на вектор строк _frame
}

void FrameBuilder::setFrame(std::vector<std::string>& frame)// Функция устанавливает значение вектора строк _frame
{
	// Если _frame уже существует, то меняем его значение, иначе создаем новый вектор
	if (_frame)
		*_frame = frame;
	else
		_frame = new std::vector<std::string>(frame);
}

void FrameBuilder::setRow(std::string str)
{
	_output.setRow(str); //устанавливает значение строки в объекте _output
}

void FrameBuilder::showMessage(std::string message)
{
	_output.print(message); //вывод сообщения на экран с помощью объекта _output
}

// Функция выводит на экран данные в конце работы программы с помощью объекта _output
void FrameBuilder::theEnd()
{
	// Получаем дескриптор консоли и позицию курсора
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD position = { 0, _frame->size() + 1 };
	
	_output.end(console, position);// Выполняем завершающие действия и выводим данные на экран
}
std::vector<std::string>& FrameBuilder::rotate(std::vector<std::string>& _figureSymbol, uint8_t state)
{
	uint8_t height = _figureSymbol.size(); // высота фигуры
	uint8_t width = _figureSymbol[0].size(); // ширина фигуры
	std::vector<std::string> temp; // временный вектор для новой фигуры

	switch (state) // в зависимости от состояния (угла поворота) делаем поворот
	{
	case 0: //ничего не меняем и возвращаем фигуру
		return _figureSymbol;
		break;
	case 1: //поворачиваем на 90 градусов по часовой стрелке
		for (int i = 0; i < width; ++i)
			temp.emplace_back(std::string(height, _fieldSymbol)); // заполняем временную фигуру пустыми символами
		for (int i = 0; i < height; ++i) {
			for (int j = 0; j < width; ++j) {
				temp[j][height - i - 1] = _figureSymbol[i][j]; // делаем поворот
			}
		}
		break;
	case 2: //поворачиваем на 180 градусов
		for (int i = 0; i < height; ++i)
			temp.emplace_back(std::string(width, _fieldSymbol)); // заполняем временную фигуру пустыми символами
		for (int i = 0; i < height; ++i) {
			for (int j = 0; j < width; ++j) {
				temp[height - 1 - i][width - 1 - j] = _figureSymbol[i][j]; // делаем поворот
			}
		}
		break;
	case 3: //поворачиваем на 270 градусов по часовой стрелке
		for (int i = 0; i < width; ++i)
			temp.emplace_back(std::string(height, _fieldSymbol)); // заполняем временную фигуру пустыми символами
		for (int i = 0; i < height; ++i) {
			for (int j = 0; j < width; ++j) {
				temp[width - j - 1][i] = _figureSymbol[i][j]; // делаем поворот
			}
		}
		break;
	}

	_figureSymbol = temp; // сохраняем новую фигуру
	return _figureSymbol; // возвращаем новую фигуру
}

std::vector<std::string>& FrameBuilder::rotate(std::vector<std::string>&& _figureSymbol, uint8_t state)
{
	uint8_t height = _figureSymbol.size(); // Высота фигуры
	uint8_t width = _figureSymbol[0].size(); // Ширина фигуры
	std::vector<std::string> temp; // Временный вектор строк

	switch (state)
	{
	case 0: //возвращаем исходный вектор строк
		return _figureSymbol;
		break;
	case 1: //поворачиваем фигуру на 90 градусов по часовой стрелке
		for (int i = 0; i < width; ++i)
			temp.emplace_back(std::string(height, _fieldSymbol)); // Создаем строки с символами поля во временном векторе
		for (int i = 0; i < height; ++i) { // Перебираем строки фигуры
			for (int j = 0; j < width; ++j) { // Перебираем символы в строке фигуры
				temp[j][height - i - 1] = _figureSymbol[i][j]; // Переносим символы фигуры во временный вектор строк с поворотом
			}
		}
		break;
	case 2: //поворачиваем фигуру на 180 градусов по часовой стрелке
		for (int i = 0; i < height; ++i)
			temp.emplace_back(std::string(width, _fieldSymbol));
		for (int i = 0; i < height; ++i) { 
			for (int j = 0; j < width; ++j) { 
				temp[height - 1 - i][width - 1 - j] = _figureSymbol[i][j];
			}
		}
		break;
	case 3: //поворачиваем фигуру на 270 градусов по часовой стрелке
		for (int i = 0; i < width; ++i)
			temp.emplace_back(std::string(height, _fieldSymbol)); 
		for (int i = 0; i < height; ++i) {
			for (int j = 0; j < width; ++j) {
				temp[width - j - 1][i] = _figureSymbol[i][j];
			}
		}
		break;
	}

	_figureSymbol = temp; // Присваиваем исходному вектору строк значенеи временного вектора
	return _figureSymbol; // Возвращаем ссылку на измененный вектор строк
}

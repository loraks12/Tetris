#pragma once
#include <iostream>
#include <windows.h>
#include <vector>

using namespace std;

// ���������� ������ ConsoleOutput
class ConsoleOutput
{
public:
	// ����� ��� ������ ������� ����� �� �������
	void print(vector<string>& frame);
	// ����� ��� ������ ������ �� �������
	void print(string text);
	// ����� ��� ��������� ������, ������� ����� ������� �� �������
	void setRow(string str);
	// ����� ��� ��������� ������ �� ������� (������������� ����� ������� �������)
	void end(HANDLE console, COORD position);

private:
	// ���������� ����������� ����
	HANDLE _console = GetStdHandle(STD_OUTPUT_HANDLE);
	// ������� �������
	COORD _pos = { 0, 0 };

	// ���������, ������������ ����������� ������ ������ � ������
	const int8_t horizontalRatio = 2;

	// ������, ������� ����� ������� �� �������
	string _row;
};
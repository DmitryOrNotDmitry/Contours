#pragma once
#include <vector>
#include "stdafx.h"


class ColorsPool
{
	std::vector<COLORREF> colors;

	int nextId;

	ColorsPool() 
	{
		colors.push_back(RGB(0, 255, 255));   // Голубой (Cyan)
		colors.push_back(RGB(255, 128, 0));   // Оранжевый
		colors.push_back(RGB(0, 255, 128));   // Мятный
		colors.push_back(RGB(255, 0, 0));     // Красный
		colors.push_back(RGB(0, 0, 255));     // Синий
		colors.push_back(RGB(255, 255, 0));   // Жёлтый
		colors.push_back(RGB(255, 0, 255));   // Пурпурный (Magenta)
		colors.push_back(RGB(128, 0, 255));   // Фиолетовый
		colors.push_back(RGB(255, 0, 128));   // Розовый
		colors.push_back(RGB(128, 255, 0));   // Лаймовый
		colors.push_back(RGB(0, 128, 255));   // Ярко-синий
		colors.push_back(RGB(255, 128, 128)); // Светло-красный
		colors.push_back(RGB(128, 255, 128)); // Светло-зелёный
		colors.push_back(RGB(128, 128, 255)); // Светло-синий
		colors.push_back(RGB(192, 192, 0));   // Оливковый
		colors.push_back(RGB(192, 0, 192));   // Тёмно-розовый
		colors.push_back(RGB(0, 192, 192));   // Бирюзовый
		colors.push_back(RGB(128, 64, 0));    // Коричневый
		colors.push_back(RGB(64, 0, 128));    // Тёмно-фиолетовый
	}

	~ColorsPool() {}

	static ColorsPool instance;

public:
	
	static ColorsPool* getInstance()
	{
		return &instance;
	}

	int getNextColorId()
	{
		int id = nextId++;
		nextId %= colors.size();
		return id;
	}

	COLORREF getColorBy(int id)
	{
		return colors[id];
	}

	void reset()
	{
		nextId = 0;
	}

};

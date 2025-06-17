#pragma once
#include <vector>
#include "stdafx.h"


class ColorsPool
{
	std::vector<COLORREF> colors;

	int nextId;

	ColorsPool() 
	{
		colors.push_back(RGB(0, 255, 255));   // ������� (Cyan)
		colors.push_back(RGB(255, 128, 0));   // ���������
		colors.push_back(RGB(0, 255, 128));   // ������
		colors.push_back(RGB(255, 0, 0));     // �������
		colors.push_back(RGB(0, 0, 255));     // �����
		colors.push_back(RGB(255, 255, 0));   // Ƹ����
		colors.push_back(RGB(255, 0, 255));   // ��������� (Magenta)
		colors.push_back(RGB(128, 0, 255));   // ����������
		colors.push_back(RGB(255, 0, 128));   // �������
		colors.push_back(RGB(128, 255, 0));   // ��������
		colors.push_back(RGB(0, 128, 255));   // ����-�����
		colors.push_back(RGB(255, 128, 128)); // ������-�������
		colors.push_back(RGB(128, 255, 128)); // ������-������
		colors.push_back(RGB(128, 128, 255)); // ������-�����
		colors.push_back(RGB(192, 192, 0));   // ���������
		colors.push_back(RGB(192, 0, 192));   // Ҹ���-�������
		colors.push_back(RGB(0, 192, 192));   // ���������
		colors.push_back(RGB(128, 64, 0));    // ����������
		colors.push_back(RGB(64, 0, 128));    // Ҹ���-����������
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

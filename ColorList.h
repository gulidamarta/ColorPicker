#pragma once
#include <iostream>
#include <vector>

class ColorList {
public:
	void addItem(char *item);
	char *getItem();
	char *getItem(int i);
	void clearList();
	int getSize();
	std::vector<char *> getColorList();
private:
	std::vector<char *> _colorList;
};
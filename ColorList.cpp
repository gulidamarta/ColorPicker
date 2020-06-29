#include <windows.h>
#include "ColorList.h"

#define MAX_AMOUNT_ITEMS_LIST_BOX	25

void ColorList::addItem(char *item) {
	if (getSize() < MAX_AMOUNT_ITEMS_LIST_BOX) {
		_colorList.push_back(item);
	}
}

char *ColorList::getItem(int i) {
	if ((i >= 0) && (i < getSize())) {
		return _colorList[i];
	}
	else
	{
		return nullptr;
	}
}

char *ColorList::getItem() {
	int colorListSize = _colorList.size();
	if (colorListSize > 0) {
		return _colorList[colorListSize - 1];
	}
	else
	{
		return nullptr;
	}
}

void ColorList::clearList() {
	_colorList.clear();
}


std::vector<char *> ColorList::getColorList() {
	return _colorList;
}

int ColorList::getSize() {
	return _colorList.size();
}
#include <windows.h>
#include <iostream>
#include <stdio.h>
#include <gdiplus.h>
#include <atlstr.h>
#include <stdlib.h>
#include <wchar.h>
#include <CommCtrl.h>
#include <sstream>
#include "resource1.h"
#include "custom_window.h"
#include "custom_elements.h"
#include "Color.h"
#include "ColorList.h"


#include <windef.h>

#pragma comment(lib, "comctl32.lib")
#pragma comment (lib,"Gdiplus.lib")

#define PATH_TO_FRANCE_FLAG			L"france_flag.jpg"
#define FILEPATH_LIST_COLORS		"colors.txt"
#define MAX_AMOUNT_ITEMS_LIST_BOX	25
#define ID_LIST						1						
#define ID_BUTTON					2

#define IDM_HELP					1

#define IDM_FILE_SAVE				3
#define IDM_FILE_OPEN				4
#define IDM_FILE_QUIT				5
#define IDM_COLOUR_WHEEL			6
#define IDM_TEXT_TOOLS				7
#define IDM_CLEAR_ALL				8
#define IDM_ZOOM					9
#define IDM_HOTKEY					10
#define IDM_AUTO_COPY				11
#define IDM_STAY_ON_TOP				12
#define IDM_COLOR_ALGORITHM			13
#define IDM_MONOCHROME				14
#define IDM_RGB						15
#define IDM_HSL						16
#define IDM_HUMBROL					25
#define COLOR_PALETE_LENGTH			9
#define IDM_MIX_COLORS				17
#define IDM_MIX_BUTTON				18
#define IDM_SAVE_COLOR_LIST			19
#define IDM_SAVE_COLOR_SCHEME		20
#define COLOR_CODE_LENGTH_FIELD		100
#define IDM_MIX_SAVE_FILE			21
#define IDM_MIX_QUITE				22
#define IDM_MIX_HELP				23
#define IDM_HTML					24

#define XBITMAP						48 
#define YBITMAP						48 
#define BUFFER						MAX_PATH

#define SCALING_STEP				0.3f

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK WndProcMixColor(HWND, UINT, WPARAM, LPARAM);
VOID AddMenus(HWND);
VOID GetPixelColor();
VOID DrawPickerImage(HDC, HWND);
VOID DrawListBox(HWND);
VOID DrawButton(HWND);
VOID DrawTextBox(HWND);
VOID DrawComboBox(HWND);
VOID OutputRgbValues(HDC);
VOID GetDragAndDropedFiles(HWND, WPARAM);
VOID AddItem(HWND, PTSTR, COLORREF);
BOOL CopyColorCodeToClipboard(HWND);
VOID SaveColorsListToFile(char *, ColorList);
VOID SaveColorListOpenDialogFile(HWND, ColorList);
VOID GetColorListFromFile(HWND, char *);
VOID GetColorListOpenDialogFile(HWND);
VOID CreateAHorizontalScrollBar(HWND, int);
VOID OutputColorFormat();

VOID CreateMixColorWindow(HWND);
VOID DrawMixColorRectangles(HWND, HDC);
VOID DrawColorTitlesMixRGB(HDC);
VOID DrawColorTitlesMixHTML(HDC);
VOID DrawColorTitlesMixHSL(HDC);
VOID DrawColorTitlesMixHumbrol(HDC);

VOID DrawColorEditsMix(HWND);
VOID DrawColorButtonsMix(HWND);

VOID OutputMixColorsValuesRGB();
VOID OutputMixColorsValuesHTML();
VOID OutputMixColorValuesHSL();
VOID OutputMixColorValuesHumbrol();

VOID OutputStartColorPortions(char*, char*, char*);
VOID MixColors(int, int, int);
int ConvertStrToInt(char *);
float ConvertStrToFloat(char *str_to_convert);
BOOL UpdateColors();

VOID OutputRGBMixColorCode(HDC);
VOID OutputHTMLMixColorCode(HDC);
VOID OutputHSLMixColorCode(HDC);
VOID OutputHumbrolMixColorCode(HDC);

VOID AddMenusMixWindow(HWND);
VOID UpdateMixWindowToHTML(HWND, HDC);
VOID UpdateMixWindowToHSL(HWND, HDC);
VOID UpdateMixWindowToRGB();
VOID UpdateMixWindowToHumbrol(HWND, HDC);

VOID CreateRadioButtons(HWND);

INT xPosition = 105;
INT yPosition = 5;

INT windowHight = 0;
INT windowWidth = 0;

// default values for pictures 127*189
INT imageHight = 127;
INT imageWidth = 189;

INT mainTransparancy = 255;
INT isInsideBlock = 0;

INT x_coord;
INT y_coord;

INT redColorStart = 10;
INT greenColorStart = 6;
INT blueColorStart = 100;

// Идентификатор копии приложения
HINSTANCE hInst;
// TODO: Make frame for rectangle
HDC hdc_context;
BOOL isInImage = FALSE;
BOOL isSetPos = FALSE;
BOOL isCoordinatesSet = FALSE;
BOOL isPushedButton = FALSE;
static HWND hEdit;
static HWND hWndComboBox;
static HWND hButton;
static HWND hListBox;
static HWND hRedColorScrollBar;
static HWND hGreenColorScrollBar;
static HWND hBlueColorScrollBar;
static HWND hMixColorWindow;

// edit'ы для ввода параметров цветов для смешивания
static HWND hEditMixR_1;
static HWND hEditMixR_2;
static HWND hEditMixR_3;
static HWND hEditMixG_1;
static HWND hEditMixG_2;
static HWND hEditMixG_3;
static HWND hEditMixB_1;
static HWND hEditMixB_2;
static HWND hEditMixB_3;

static HWND hEditMixH_1;
static HWND hEditMixH_2;
static HWND hEditMixH_3;
static HWND hEditMixL_1;
static HWND hEditMixL_2;
static HWND hEditMixL_3;
static HWND hEditMixS_1;
static HWND hEditMixS_2;
static HWND hEditMixS_3;

static HWND hEditHex_1;
static HWND hEditHex_2;
static HWND hEditHex_3;

static HWND hEditHumbrol_1;
static HWND hEditHumbrol_2;
static HWND hEditHumbrol_3;

static HWND hButtonMix;

static HWND hEditMixPortion_1;
static HWND hEditMixPortion_2;
static HWND hEditMixPortion_3;

static HWND hRadioButtonRGB;
static HWND hRadioButtonHSL;

HMENU hColorAlgoritmMenu;
HMENU hColorFormats;


wchar_t PATH_TO_IMAGE_BMP[400];
HBRUSH hbrBkgnd = NULL;
INT amountListBoxItems = 0;
Color color(redColorStart, greenColorStart, blueColorStart);
Color oldColor(redColorStart, greenColorStart, blueColorStart);
Color first_mix_color(0, 141, 75);
Color second_mix_color(120, 108, 94);
Color third_mix_color(165, 98, 69);
Color res_mix_color(0, 0, 0);

ColorList colorList;
ColorList colorMixList;
ColorList colorGeneratedPalette;

std::vector<Color> oldColorVector;
std::vector<Color> colorVector;

std::vector<Color> hslColorVector;
std::vector<Color> hslOldColorVector;

INT colorFormatCode = 0;
INT colorMixColorsFormat = 0;
INT colorPaletteAlgorithm = IDM_MONOCHROME;

BOOL isInit = TRUE;
char help_info[150] = "To copy value press Copy Value button. \nTo pick color press any symbol character. \n";

// mycloud.com - облачный сервер, на котором можно собрать свой сервер (сколько памяти, процессора)

int APIENTRY WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{

	WNDCLASSEX wcex;
	WNDCLASSEX wcex_mix_color;
	// a handle to a window
	HWND hWnd;
	MSG msg;

	// Initialize gdiplus functionality
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex_mix_color.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex_mix_color.style = CS_HREDRAW | CS_VREDRAW;

	wcex.lpfnWndProc = WndProc;
	wcex_mix_color.lpfnWndProc = WndProcMixColor;

	wcex.cbClsExtra = 0;
	wcex_mix_color.cbClsExtra = 0;

	wcex.cbWndExtra = 0;
	wcex_mix_color.cbWndExtra = 0;

	wcex.hInstance = hInstance;
	wcex_mix_color.hInstance = hInstance;

	wcex.hIcon = (HICON)LoadImage(NULL, PATH_TO_MAIN_ICON, IMAGE_ICON, 0, 0, LR_LOADFROMFILE);
	wcex_mix_color.hIcon = (HICON)LoadImage(NULL, PATH_TO_MAIN_ICON, IMAGE_ICON, 0, 0, LR_LOADFROMFILE);

	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex_mix_color.hCursor = LoadCursor(NULL, IDC_ARROW);

	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex_mix_color.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

	wcex.lpszMenuName = NULL;
	wcex_mix_color.lpszMenuName = NULL;

	wcex.lpszClassName = "JustColorPicker";
	wcex_mix_color.lpszClassName = "MixColorClass";

	wcex.hIconSm = wcex.hIcon;
	wcex_mix_color.hIconSm = wcex.hIcon;

	RegisterClassEx(&wcex);
	RegisterClassEx(&wcex_mix_color);

	hWnd = CreateWindow("JustColorPicker", "Just Color Picker",
		WS_OVERLAPPEDWINDOW, START_COORDINATE_X, START_COORDINATE_Y,
		START_WIDTH, START_HIGHT, NULL, NULL, hInstance, NULL);
	
	// Sets the specified window's show state
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	// If wMsgFilterMin and wMsgFilterMax are both zero, GetMessage returns all available messages 
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	Gdiplus::GdiplusShutdown(gdiplusToken);
}

VOID ConvertToFalse() {
	isInImage = FALSE;
}

VOID ConvertToTrue() {
	isInImage = TRUE;
}

VOID OutputColorFormat() {
	switch (colorFormatCode)
	{
	case 0:
		SetWindowText(hEdit, (LPCSTR)color.getStrHtmlColorCode());
		break;
	case 1:
		SetWindowText(hEdit, (LPCSTR)color.getStrRgbColorCode());
		break;
	case 2:
		SetWindowText(hEdit, (LPCSTR)color.getHslColorCode());
		break;
	case 3:
		SetWindowText(hEdit, (LPCSTR)color.getRalCodeStr());
	case 4:
		SetWindowText(hEdit, (LPCSTR)color.getHumbrolCodeStr());
	default:
		break;
	}
}


LRESULT CALLBACK WndProcMixColor(HWND hWnd, UINT message,
	WPARAM wParam, LPARAM lParam) {
	HDC hdc;
	PAINTSTRUCT ps;
	char buff_portion_1[COLOR_CODE_LENGTH_FIELD];
	char buff_portion_2[COLOR_CODE_LENGTH_FIELD];
	char buff_portion_3[COLOR_CODE_LENGTH_FIELD];

	BOOL isCorrect = TRUE;

	char portion_1[5] = "40";
	char portion_2[5] = "30";
	char portion_3[5] = "30";
	int port_1;
	int port_2;
	int port_3;

	switch (message) {
	case WM_CREATE:
		colorMixColorsFormat = IDM_RGB;
		AddMenusMixWindow(hWnd);
		DrawColorEditsMix(hWnd);
		UpdateMixWindowToRGB();
		DrawColorButtonsMix(hWnd);
		MixColors(40, 30, 30);
		OutputStartColorPortions(portion_1, portion_2, portion_3);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		DrawMixColorRectangles(hWnd, hdc);
		switch (colorMixColorsFormat)
		{
		case IDM_RGB:
			UpdateMixWindowToRGB();
			DrawColorTitlesMixRGB(hdc);
			OutputMixColorsValuesRGB();
			OutputRGBMixColorCode(hdc);
			break;
		case IDM_HTML:
			UpdateMixWindowToHTML(hWnd, hdc);
			DrawMixColorRectangles(hWnd, hdc);
			OutputMixColorsValuesHTML();
			OutputHTMLMixColorCode(hdc);
			DrawColorTitlesMixHTML(hdc);
			break;
		case IDM_HSL:
			UpdateMixWindowToHSL(hWnd, hdc);
			DrawMixColorRectangles(hWnd, hdc);
			OutputMixColorValuesHSL();
			OutputHSLMixColorCode(hdc);
			DrawColorTitlesMixHSL(hdc);
			break;
		case IDM_HUMBROL:
			UpdateMixWindowToHumbrol(hWnd, hdc);
			DrawMixColorRectangles(hWnd, hdc);
			OutputMixColorValuesHumbrol();
			OutputHumbrolMixColorCode(hdc);
			DrawColorTitlesMixHumbrol(hdc);
			break;
		default:
			break;
		}
		EndPaint(hWnd, &ps);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDM_MIX_BUTTON:

			isCorrect = UpdateColors();
			GetWindowText(hEditMixPortion_1, buff_portion_1, COLOR_CODE_LENGTH_FIELD);
			GetWindowText(hEditMixPortion_2, buff_portion_2, COLOR_CODE_LENGTH_FIELD);
			GetWindowText(hEditMixPortion_3, buff_portion_3, COLOR_CODE_LENGTH_FIELD);

			port_1 = ConvertStrToInt(buff_portion_1);
			port_2 = ConvertStrToInt(buff_portion_2);
			port_3 = ConvertStrToInt(buff_portion_3);
			
			if ((port_1 >= 0) && (port_2 >= 0) && (port_3 >= 0) && (port_1 + port_2 + port_3 == 100) && (isCorrect)) {
				MixColors(port_1, port_2, port_3);
				SetFocus(hWnd);
				InvalidateRect(hWnd, NULL, TRUE);
				OutputMixColorsValuesRGB();
				colorMixList.clearList();
				switch (colorMixColorsFormat) {
				case IDM_RGB:
					colorMixList.addItem(res_mix_color.getStrRgbColorCode());
					break;
				case IDM_HTML:
					colorMixList.addItem(res_mix_color.getStrHtmlColorCode());
					break;
				case IDM_HSL:
					colorMixList.addItem(res_mix_color.getHslColorCode());
					break;
				case IDM_HUMBROL:
					if (strlen(res_mix_color.getHumbrolCodeStr()) > 0) {
						colorMixList.addItem(res_mix_color.getHumbrolCodeStr());
					}
					else
					{
						colorMixList.addItem(res_mix_color.getStrRgbColorCode());
					}
				}
			}
			else {
				//handle error or message to the user
			}
			break;
		case IDM_MIX_QUITE:
			SendMessage(hWnd, WM_CLOSE, 0, 0);
			break;
		case IDM_MIX_HELP:
			// add descrintion of the functionality
			break;
		case IDM_MIX_SAVE_FILE:
			if (colorMixList.getSize() > 0)
				SaveColorListOpenDialogFile(hWnd, colorMixList);
			break;
		case IDM_RGB:
			colorMixColorsFormat = IDM_RGB;
			CheckMenuItem(hColorFormats, IDM_HTML, MF_UNCHECKED);
			CheckMenuItem(hColorFormats, IDM_HSL, MF_UNCHECKED);
			CheckMenuItem(hColorFormats, IDM_RGB, MF_CHECKED);
			CheckMenuItem(hColorFormats, IDM_HUMBROL, MF_UNCHECKED);
			InvalidateRect(hWnd, NULL, TRUE);
			break;

		case IDM_HTML:
			colorMixColorsFormat = IDM_HTML;
			CheckMenuItem(hColorFormats, IDM_HTML, MF_CHECKED);
			CheckMenuItem(hColorFormats, IDM_RGB, MF_UNCHECKED);
			CheckMenuItem(hColorFormats, IDM_HSL, MF_UNCHECKED);
			CheckMenuItem(hColorFormats, IDM_HUMBROL, MF_UNCHECKED);
			InvalidateRect(hWnd, NULL, TRUE);
			break;

		case IDM_HSL:
			colorMixColorsFormat = IDM_HSL;
			CheckMenuItem(hColorFormats, IDM_HTML, MF_UNCHECKED);
			CheckMenuItem(hColorFormats, IDM_RGB, MF_UNCHECKED);
			CheckMenuItem(hColorFormats, IDM_HSL, MF_CHECKED);
			CheckMenuItem(hColorFormats, IDM_HUMBROL, MF_UNCHECKED);
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case IDM_HUMBROL:
			colorMixColorsFormat = IDM_HUMBROL;
			CheckMenuItem(hColorFormats, IDM_HTML, MF_UNCHECKED);
			CheckMenuItem(hColorFormats, IDM_RGB, MF_UNCHECKED);
			CheckMenuItem(hColorFormats, IDM_HSL, MF_UNCHECKED);
			CheckMenuItem(hColorFormats, IDM_HUMBROL, MF_CHECKED);
			InvalidateRect(hWnd, NULL, TRUE);
		}
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message,
	WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	SCROLLINFO si = { 0 };
	ZeroMemory(&si, sizeof(SCROLLINFO));
	si.cbSize = sizeof(si);
	si.fMask = SIF_ALL;
	HWND message_hwnd;

	int initial_pos = 0;
	int angel = 360 / COLOR_PALETE_LENGTH;
	int start_angel = angel;
	
	switch (message)
	{
	case WM_CREATE:
		
		AddMenus(hWnd);
		DrawListBox(hWnd);
		DrawButton(hWnd);
		DrawComboBox(hWnd);
		DrawTextBox(hWnd);
		CreateAHorizontalScrollBar(hWnd, 15);
		DragAcceptFiles(hWnd, TRUE);
		CreateRadioButtons(hWnd);
		wcsncpy_s(PATH_TO_IMAGE_BMP, PATH_TO_FRANCE_FLAG, sizeof(PATH_TO_FRANCE_FLAG));
		for (int i = 0; i < COLOR_PALETE_LENGTH; i++) {
			oldColorVector.push_back(Color(oldColor.getRandomRedColor(), oldColor.getRandomGreenColor(), color.getRandomBlueColor()));
			hslOldColorVector.push_back(Color(oldColor.getHue() + start_angel, oldColor.getSaturation(), oldColor.getLightness()));
			start_angel += angel;
		}
		break;

	case WM_CHAR:
		GetPixelColor();
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		hdc_context = hdc;
		DrawPickerImage(hdc, hWnd);
		OutputRgbValues(hdc);
		OutputColorFormat();
		SetFocus(hWnd);
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_SETCURSOR:
		POINT _cursor;
		GetCursorPos(&_cursor);
		if ((_cursor.x >= xPosition + START_COORDINATE_X) && (_cursor.x <= xPosition + imageWidth + START_COORDINATE_X) &&
			(_cursor.y >= yPosition + START_COORDINATE_Y + 40) && (_cursor.y <= yPosition + imageHight + START_COORDINATE_Y + 40))
		{
			ConvertToTrue();
			if ((LOWORD(lParam) == HTCLIENT) && (!isSetPos)){
				HINSTANCE hInst;
				hInst = GetModuleHandle(NULL);
				HCURSOR hCursorHelp = SetCursor(LoadCursorFromFile(PATH_TO_PICKER_CURSOR));
				isSetPos = TRUE;
				InvalidateRect(hWnd, NULL, TRUE);
			}
		}
		else
		{
			SetFocus(hWnd);
			if (isInImage) {
				HCURSOR hCursorHelp = SetCursor(LoadCursor(NULL, IDC_ARROW));
				ConvertToFalse();
				isSetPos = FALSE;
				InvalidateRect(hWnd, NULL, TRUE);
			}
		}
		break;
		
	case WM_GETMINMAXINFO:
		MINMAXINFO FAR *lpMinMaxInfo;
		lpMinMaxInfo = (MINMAXINFO FAR *) lParam;
		lpMinMaxInfo->ptMinTrackSize.x = MIN_TRACK_SIZE_X;
		lpMinMaxInfo->ptMinTrackSize.y = MIN_TRACK_SIZE_Y;
		break;
	case WM_DROPFILES:
		GetDragAndDropedFiles(hWnd, wParam);
		InvalidateRect(hWnd, NULL, TRUE);
		break;

	case WM_HSCROLL: {
		message_hwnd = (HWND)lParam;
		if (!GetScrollInfo(message_hwnd, SB_CTL, &si))
			return 1;

		switch (LOWORD(wParam)) {
		case SB_TOP:
			si.nPos = si.nMin;
			break;

		case SB_BOTTOM:
			si.nPos = si.nMax;
			break;

		case SB_THUMBPOSITION:
			si.nPos = si.nTrackPos;
			break;

		case SB_LINELEFT:
			si.nPos -= 1;
			break;

		case SB_LINERIGHT:
			si.nPos += 1;
			break;
		}

		si.cbSize = sizeof(si);
		si.fMask = SIF_POS;
		SetScrollInfo(message_hwnd, SB_CTL, &si, TRUE);

		UpdateWindow(message_hwnd);
		
		if (message_hwnd == hRedColorScrollBar)
			oldColor.setRedColor(si.nPos);

		if (message_hwnd == hGreenColorScrollBar)
			oldColor.setGreenColor(si.nPos);

		if (message_hwnd == hBlueColorScrollBar)
			oldColor.setBlueColor(si.nPos);

		RECT rect;
		rect.left = X_RECT_COLOR + 5;
		rect.top = Y_COPY_VALUE_START_COORD + COPY_VALUE_BUTTON_HIGHT + 10 + 10;
		rect.right = rect.left + windowWidth;
		rect.bottom = rect.top + windowHight;

		InvalidateRect(hWnd, NULL, NULL);
	}
		break;
	case WM_COMMAND:
		
		if ((wParam == BN_PUSHED) || (wParam == BN_CLICKED)) {
				// Отменяем режим перерисовки списка
			SendMessage(hListBox, WM_SETREDRAW, FALSE, 0L);
			SendMessage(hListBox, LB_ADDSTRING, RGB(color.getRedColor(), 
				color.getGreenColor(), color.getBlueColor()),
				(LPARAM)(LPSTR)color.getStrHtmlColorCode());
			colorList.addItem(color.getStrHtmlColorCode());

			// Включаем режим перерисовки списка
			SendMessage(hListBox, WM_SETREDRAW, TRUE, 0L);
			CopyColorCodeToClipboard(hWnd);
			isPushedButton = TRUE;
			SetFocus(hWnd);
			InvalidateRect(hWnd, NULL, TRUE);
		}
		if (HIWORD(wParam) == CBN_SELCHANGE)
		{
			int ItemIndex = SendMessage((HWND)lParam, (UINT)CB_GETCURSEL,
				(WPARAM)0, (LPARAM)0);
			TCHAR  ListItem[256];
			(TCHAR)SendMessage((HWND)lParam, (UINT)CB_GETLBTEXT,
				(WPARAM)ItemIndex, (LPARAM)ListItem);
			colorFormatCode = ItemIndex;
			OutputColorFormat();
			SetFocus(hWnd);
		}
		switch (LOWORD(wParam)) {
		// Color generation schemas
		case IDM_RGB:
			colorGeneratedPalette.clearList();
			colorPaletteAlgorithm = IDM_RGB;
			CheckMenuItem(hColorAlgoritmMenu, IDM_MONOCHROME, MF_UNCHECKED);
			CheckMenuItem(hColorAlgoritmMenu, IDM_HSL, MF_UNCHECKED);
			CheckMenuItem(hColorAlgoritmMenu, IDM_RGB, MF_CHECKED);
			break;

		case IDM_MONOCHROME:
			colorGeneratedPalette.clearList();
			colorPaletteAlgorithm = IDM_MONOCHROME;
			CheckMenuItem(hColorAlgoritmMenu, IDM_MONOCHROME, MF_CHECKED);
			CheckMenuItem(hColorAlgoritmMenu, IDM_RGB, MF_UNCHECKED);
			CheckMenuItem(hColorAlgoritmMenu, IDM_HSL, MF_UNCHECKED);
			break;

		case IDM_HSL:
			colorGeneratedPalette.clearList();
			colorPaletteAlgorithm = IDM_HSL;
			CheckMenuItem(hColorAlgoritmMenu, IDM_MONOCHROME, MF_UNCHECKED);
			CheckMenuItem(hColorAlgoritmMenu, IDM_RGB, MF_UNCHECKED);
			CheckMenuItem(hColorAlgoritmMenu, IDM_HSL, MF_CHECKED);
			break;

		case IDM_HELP:
			MessageBox(hWnd, help_info, "Just Color Picker", MB_ICONINFORMATION);
			break;

		// mix color window
		case IDM_MIX_COLORS:
			CreateMixColorWindow(hWnd);
			break;
		case IDM_SAVE_COLOR_SCHEME:
			if (colorGeneratedPalette.getSize() > 0) {
				SaveColorListOpenDialogFile(hWnd, colorGeneratedPalette);
			}
			break;
		case IDM_SAVE_COLOR_LIST:
			if (colorList.getSize() > 0) {
				SaveColorListOpenDialogFile(hWnd, colorList);
			}
			break;
		case IDM_FILE_OPEN:
			GetColorListOpenDialogFile(hWnd);
			break;
		case IDM_CLEAR_ALL:
			if (colorList.getSize() > 0)
				SendMessage(hListBox, LB_RESETCONTENT, 0, 0);
			break;
		case IDM_FILE_QUIT:
			SendMessage(hWnd, WM_CLOSE, 0, 0);
			break;

		}
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

VOID AddMenusMixWindow(HWND hWnd) {
	HMENU hOptions;
	HMENU hMenubar;
	hMenubar = CreateMenu();
	hOptions = CreateMenu();
	hColorFormats = CreateMenu();

	AppendMenuW(hColorFormats, MF_CHECKED, IDM_RGB, L"&RGB");
	AppendMenuW(hColorFormats, MF_UNCHECKED, IDM_HTML, L"&HTML");
	AppendMenuW(hColorFormats, MF_UNCHECKED, IDM_HSL, L"&HSL");
	AppendMenuW(hColorFormats, MF_UNCHECKED, IDM_HUMBROL, L"&Humbrol");

	AppendMenuW(hOptions, MF_STRING, IDM_MIX_SAVE_FILE, L"&Save");
	AppendMenuW(hOptions, MF_STRING, IDM_MIX_HELP, L"&Help");
	AppendMenuW(hOptions, MF_SEPARATOR, 0, NULL);
	AppendMenuW(hOptions, MF_STRING, IDM_MIX_QUITE, L"&Quit");

	AppendMenuW(hMenubar, MF_POPUP, (UINT_PTR)hOptions, L"&Options");
	AppendMenuW(hMenubar, MF_POPUP, (UINT_PTR)hColorFormats, L"&Color Formats");

	SetMenu(hWnd, hMenubar);
}

VOID AddMenus(HWND hwnd) {

	HMENU hMenubar;
	HMENU hColourListMenu;
	HMENU hToolsMenu;
	HMENU hOptionsMenu;
	HMENU hSaveOptions;

	// for adding some actions and additional pop up menus
	hMenubar = CreateMenu();
	hColourListMenu = CreateMenu();
	hToolsMenu = CreateMenu();
	hOptionsMenu = CreateMenu();
	hColorAlgoritmMenu = CreateMenu();
	hSaveOptions = CreateMenu();

	AppendMenuW(hToolsMenu, MF_STRING, IDM_MIX_COLORS, L"&Mix Colors");
	// TODO 
	//AppendMenuW(hToolsMenu, MF_STRING, IDM_COLOUR_WHEEL, L"&Colour Wheel");
	//AppendMenuW(hToolsMenu, MF_STRING, IDM_TEXT_TOOLS, L"&Text Tools");

	AppendMenuW(hSaveOptions, MF_STRING, IDM_SAVE_COLOR_LIST, L"&Save Color List");
	AppendMenuW(hSaveOptions, MF_STRING, IDM_SAVE_COLOR_SCHEME, L"&Save Color Scheme");

	AppendMenuW(hColourListMenu, MF_STRING, IDM_SAVE_COLOR_LIST, L"&Save");
	AppendMenuW(hColourListMenu, MF_STRING, IDM_FILE_OPEN, L"&Open");
	AppendMenuW(hColourListMenu, MF_STRING, IDM_CLEAR_ALL, L"&Clear All");

	AppendMenuW(hOptionsMenu, MF_POPUP, (UINT_PTR)hSaveOptions, L"&Save");
	AppendMenuW(hOptionsMenu, MF_CHECKED, IDM_AUTO_COPY, L"&Auto Copy");
	//AppendMenuW(hOptionsMenu, MF_STRING, IDM_HOTKEY, L"&Hot Key");
	//AppendMenuW(hOptionsMenu, MF_STRING, IDM_STAY_ON_TOP, L"&Stay On Top");
	AppendMenuW(hOptionsMenu, MF_SEPARATOR, 0, NULL);
	AppendMenuW(hOptionsMenu, MF_STRING, IDM_FILE_QUIT, L"&Quit");

	AppendMenuW(hColorAlgoritmMenu, MF_CHECKED, IDM_MONOCHROME, L"&Monochrome");
	AppendMenuW(hColorAlgoritmMenu, MF_UNCHECKED, IDM_RGB, L"&RGB Algorithm");
	AppendMenuW(hColorAlgoritmMenu, MF_UNCHECKED, IDM_HSL, L"&HSL Algorithm");

	AppendMenuW(hMenubar, MF_POPUP, (UINT_PTR)hOptionsMenu, L"&Options");
	AppendMenuW(hMenubar, MF_POPUP, (UINT_PTR)hToolsMenu, L"&Tools");
	AppendMenuW(hMenubar, MF_POPUP, (UINT_PTR)hColourListMenu, L"&Colour List");
	AppendMenuW(hMenubar, MF_POPUP, (UINT_PTR)hColorAlgoritmMenu, L"&Color Algorithm");
	AppendMenuW(hMenubar, MF_POPUP, IDM_HELP, L"&Help");

	SetMenu(hwnd, hMenubar);
}

VOID GetPixelColor() {
	POINT _cursor;

	GetCursorPos(&_cursor);
	if ((_cursor.x >= xPosition + START_COORDINATE_X) && (_cursor.x <= xPosition + imageWidth + START_COORDINATE_X) &&
	(_cursor.y >= yPosition + START_COORDINATE_Y) && (_cursor.y <= yPosition + imageHight + START_COORDINATE_Y)) {
		x_coord = _cursor.x;
		y_coord = _cursor.y;
		isCoordinatesSet = TRUE;
	}
}

VOID DrawListBox(HWND hWnd) {
	hListBox = CreateWindow("listbox", NULL,
		WS_CHILD | WS_VISIBLE | LBS_STANDARD |
		LBS_WANTKEYBOARDINPUT,
		X_LIST_START_COORD, Y_LIST_START_COORD, LIST_WIDTH, LIST_HIGHT,
		hWnd, (HMENU)ID_LIST, hInst, NULL);
}

VOID DrawButton(HWND hWnd) {
	hButton = CreateWindow("button", "Copy Value",
		WS_CHILD | WS_VISIBLE |
		BS_PUSHBUTTON,
		X_COPY_VALUE_START_COORD, Y_COPY_VALUE_START_COORD, COPY_VALUE_BUTTON_WIDTH, COPY_VALUE_BUTTON_HIGHT,
		hWnd, (HMENU)ID_BUTTON, hInst, NULL);
}

VOID DrawTextBox(HWND hWnd) {
	hEdit = CreateWindow("EDIT", 0, ES_READONLY | WS_BORDER | WS_CHILD | WS_VISIBLE, xPosition,
		Y_COPY_VALUE_START_COORD, X_EDIT_WIDTH, Y_EDIT_HIGHT, hWnd, 0, hInst, 0);
}

VOID DrawComboBox(HWND hWnd) {
	hWndComboBox = CreateWindow(WC_COMBOBOX, 0,
		CBS_DROPDOWNLIST | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,
		X_RECT_COLOR, Y_COPY_VALUE_START_COORD, X_EDIT_WIDTH - 70, Y_EDIT_HIGHT + 200, hWnd, 0, hInst,
		0);

	SendMessage(hWndComboBox, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)TEXT("HTML"));
	SendMessage(hWndComboBox, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)TEXT("RGB"));
	SendMessage(hWndComboBox, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)TEXT("HSL"));
	SendMessage(hWndComboBox, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)TEXT("RAL"));
	SendMessage(hWndComboBox, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)TEXT("Humbrol"));

	SendMessage(hWndComboBox, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
}

void Resize(HWND hWnd, int _width, int _height, bool _fullscreen)
{
	int width = _width;
	int height = _height;
	int fullscreen = _fullscreen;

	RECT rect;
	if (!isInit)
	{
		int x = (GetSystemMetrics(SM_CXSCREEN) - width) / 2;
		int y = (GetSystemMetrics(SM_CYSCREEN) - height) / 2;

		rect.left = x;
		rect.right = x + width;
		rect.top = y;
		rect.bottom = y + height;


		DWORD style = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_SIZEBOX | WS_MAXIMIZEBOX;
		DWORD exStyle = WS_EX_APPWINDOW;

		AdjustWindowRectEx(&rect, style, FALSE, exStyle);

		SetWindowLong(hWnd, GWL_STYLE, style);
		SetWindowLong(hWnd, GWL_EXSTYLE, exStyle);

		SetWindowPos(hWnd, HWND_TOP, rect.left, rect.top,
			rect.right - rect.left, rect.bottom - rect.top,
			SWP_FRAMECHANGED);

		ShowWindow(hWnd, SW_SHOW);
		SetForegroundWindow(hWnd);
		SetFocus(hWnd);
		UpdateWindow(hWnd);
		isInit = TRUE;
	}
	else
	{
		GetClientRect(hWnd, &rect);
	}

	if (!isInit)
		isInit = true;
}

VOID DrawPickerImage(HDC hdc, HWND hWnd) {
	// angel and current_angel для генерации палитры на основе формата hsl
	Color tempColor(color.getRedColor(), color.getGreenColor(), color.getBlueColor());
	int angel = 360 / COLOR_PALETE_LENGTH;
	int current_angel = angel;

	RECT rcClient;
	GetClientRect(hWnd, &rcClient);
	int left = rcClient.left;
	int top = rcClient.top;
	int width = rcClient.right - rcClient.left;
	int hight = rcClient.bottom - rcClient.top;

	HDC hdcMem = CreateCompatibleDC(hdc);
	const int nMemDC = SaveDC(hdcMem);

	HBITMAP hBitmap = CreateCompatibleBitmap(hdc, width, hight);
	SelectObject(hdcMem, hBitmap);

	Gdiplus::Graphics graphics(hdcMem);
	Gdiplus::Bitmap bmp(PATH_TO_IMAGE_BMP);

	int bmp_width = bmp.GetWidth();
	int bmp_height = bmp.GetHeight();

	Gdiplus::SolidBrush solidBrush(Gdiplus::Color(mainTransparancy, color.getRedColor(), 
		color.getGreenColor(), color.getBlueColor()));
	Gdiplus::Color pixelcolor;

	RECT rect;
	if (GetWindowRect(hWnd, &rect))
	{
		windowWidth = rect.right - rect.left;
		windowHight = rect.bottom - rect.top;
	}

	FillRect(hdcMem, &rcClient, (HBRUSH)(COLOR_WINDOW + 1));

	Gdiplus::Pen      pen(Gdiplus::Color(255, 192, 192, 192));
	graphics.DrawLine(&pen, 
		X_RECT_COLOR, 
		Y_COPY_VALUE_START_COORD + COPY_VALUE_BUTTON_HIGHT + 10,
		X_COPY_VALUE_START_COORD + COPY_VALUE_BUTTON_WIDTH , 
		Y_COPY_VALUE_START_COORD + COPY_VALUE_BUTTON_HIGHT + 10);

	graphics.DrawImage(&bmp, xPosition, yPosition);

	// если мышка сейчас не находится внутри области c картинкой, где выбирается цвет
	if (!isCoordinatesSet) {
		isInit = FALSE;
		graphics.FillRectangle(&solidBrush, X_RECT_COLOR, Y_RECT_COLOR, RECT_COLOR_WIDTH, RECT_COLOR_HIGHT);
	}
	// мышка находится внутри области картинки с picker'ом
	else {
		bmp.GetPixel(x_coord - (xPosition + START_COORDINATE_X), y_coord - (yPosition + START_COORDINATE_Y), &pixelcolor);
		Gdiplus::SolidBrush rect_brush(pixelcolor);

		color.setRedColor(pixelcolor.GetRed());
		color.setGreenColor(pixelcolor.GetGreen());
		color.setBlueColor(pixelcolor.GetBlue());

		// если была нажата кнопка "Copy Value", необходимо поменять цвет большого прямоугольника
		if (isPushedButton) {
			graphics.FillRectangle(&rect_brush, X_RECT_COLOR + 5, Y_COPY_VALUE_START_COORD + COPY_VALUE_BUTTON_HIGHT + 10 + 10,
				RECT_COLOR_WIDTH, RECT_COLOR_HIGHT);
		}
		// если она не была нажата, но просто отрисовываем прямоугольник старым цветом
		else {
			Gdiplus::SolidBrush _rect_brush(Gdiplus::Color(oldColor.getRedColor(), oldColor.getGreenColor(), oldColor.getBlueColor()));
			graphics.FillRectangle(&_rect_brush, X_RECT_COLOR + 5, Y_COPY_VALUE_START_COORD + COPY_VALUE_BUTTON_HIGHT + 10 + 10,
				RECT_COLOR_WIDTH, RECT_COLOR_HIGHT);
		}

		graphics.FillRectangle(&rect_brush, X_RECT_COLOR, Y_RECT_COLOR, RECT_COLOR_WIDTH, RECT_COLOR_HIGHT);
	}

	// если была нажата кнопка "Copy Value", необходимо поменять цвет маленького прямоугольника и перерисовать палитру
	if (isPushedButton) {
		colorGeneratedPalette.clearList();
		for (int i = 0; i < COLOR_PALETE_LENGTH; i++) {
			colorVector.push_back(Color(color.getRandomRedColor(), color.getRandomGreenColor(), color.getRandomBlueColor()));
			hslColorVector.push_back(Color(color.getHue() + current_angel, color.getSaturation(), color.getLightness()));
			//hslOldColorVector.push_back(Color(color.getHue() + current_angel, color.getSaturation(), color.getLightness()));
			current_angel += angel;
		}
		graphics.FillRectangle(&solidBrush, X_RECT_COLOR + 5, Y_COPY_VALUE_START_COORD + COPY_VALUE_BUTTON_HIGHT + 10 + 10,
			RECT_COLOR_WIDTH, RECT_COLOR_HIGHT);
		int start_point = 0;
		int x_start_point = 5;
		switch (colorPaletteAlgorithm) {
			case IDM_MONOCHROME: {
				for (int i = 0; i < COLOR_PALETE_LENGTH; i++) {
					Gdiplus::SolidBrush brush_1(Gdiplus::Color((color.getRedColor() + start_point) % 255, (color.getGreenColor() + start_point) % 255,
						(color.getBlueColor() + start_point) % 255));
					tempColor.setRedColor(color.getRedColor() + start_point);
					tempColor.setGreenColor(color.getGreenColor() + start_point);
					tempColor.setBlueColor(color.getBlueColor() + start_point);
					switch (colorFormatCode) {
					case 0:
						colorGeneratedPalette.addItem(tempColor.getStrHtmlColorCode());
						break;
					case 1:
						colorGeneratedPalette.addItem(tempColor.getStrRgbColorCode());
						break;
					case 2:
						colorGeneratedPalette.addItem(tempColor.getHslColorCode());
						break;
					}
					graphics.FillRectangle(&brush_1, X_RECT_COLOR + x_start_point, Y_COPY_VALUE_START_COORD + COPY_VALUE_BUTTON_HIGHT + 200,
						RECT_COLOR_WIDTH / 2 + 2, RECT_COLOR_HIGHT / 2);
					start_point += 10;
					x_start_point += (5 + RECT_COLOR_WIDTH / 2 + 2);
				}

				Gdiplus::LinearGradientBrush linGrBrush(
					Gdiplus::Point(0, 10),
					Gdiplus::Point(475, 10),
					Gdiplus::Color(255, color.getRedColor(), color.getGreenColor(), color.getBlueColor()),
					Gdiplus::Color(255, (color.getRedColor() + start_point - 10) % 255,
					(color.getGreenColor() + start_point - 10) % 255,
						(color.getBlueColor() + start_point - 10) % 255));

				graphics.FillRectangle(&linGrBrush, X_RECT_COLOR + 5, Y_COPY_VALUE_START_COORD + COPY_VALUE_BUTTON_HIGHT + 200 + 5 + (RECT_COLOR_HIGHT / 2),
				463, 30);
			};
			break;
			case IDM_RGB:
			{
				x_start_point = 5;
				for (int i = 0; i < COLOR_PALETE_LENGTH; i++) {
					Gdiplus::SolidBrush brush_1(Gdiplus::Color(colorVector[i].getRedColor(), colorVector[i].getGreenColor(), colorVector[i].getBlueColor()));
					tempColor.setRedColor(colorVector[i].getRedColor());
					tempColor.setGreenColor(colorVector[i].getGreenColor());
					tempColor.setBlueColor(colorVector[i].getBlueColor());
					switch (colorFormatCode) {
					case 0:
						colorGeneratedPalette.addItem(tempColor.getStrHtmlColorCode());
						break;
					case 1:
						colorGeneratedPalette.addItem(tempColor.getStrRgbColorCode());
						break;
					case 2:
						colorGeneratedPalette.addItem(tempColor.getHslColorCode());
						break;
					}
					graphics.FillRectangle(&brush_1, X_RECT_COLOR + x_start_point, Y_COPY_VALUE_START_COORD + COPY_VALUE_BUTTON_HIGHT + 200,
						RECT_COLOR_WIDTH / 2 + 2, RECT_COLOR_HIGHT / 2);
					x_start_point += (5 + RECT_COLOR_WIDTH / 2 + 2);
				}
				Gdiplus::LinearGradientBrush linGrBrush(
					Gdiplus::Point(0, 10),
					Gdiplus::Point(475, 10),
					Gdiplus::Color(255, colorVector[0].getRedColor(), colorVector[0].getGreenColor(), colorVector[0].getBlueColor()),
					Gdiplus::Color(255, colorVector[COLOR_PALETE_LENGTH - 1].getRedColor(),
						colorVector[COLOR_PALETE_LENGTH - 1].getGreenColor(),
						colorVector[COLOR_PALETE_LENGTH - 1].getBlueColor()));

				graphics.FillRectangle(&linGrBrush, X_RECT_COLOR + 5, Y_COPY_VALUE_START_COORD + COPY_VALUE_BUTTON_HIGHT + 200 + 5 + (RECT_COLOR_HIGHT / 2),
					463, 30);
			}
				break;
			case IDM_HSL:
			{
				x_start_point = 5;
				for (int i = 0; i < COLOR_PALETE_LENGTH; i++) {
					Gdiplus::SolidBrush brush_1(Gdiplus::Color(hslColorVector[i].getRedColor(), hslColorVector[i].getGreenColor(), hslColorVector[i].getBlueColor()));
					tempColor.setRedColor(hslColorVector[i].getRedColor());
					tempColor.setGreenColor(hslColorVector[i].getGreenColor());
					tempColor.setBlueColor(hslColorVector[i].getBlueColor());
					switch (colorFormatCode) {
					case 0:
						colorGeneratedPalette.addItem(tempColor.getStrHtmlColorCode());
						break;
					case 1:
						colorGeneratedPalette.addItem(tempColor.getStrRgbColorCode());
						break;
					case 2:
						colorGeneratedPalette.addItem(tempColor.getHslColorCode());
						break;
					}
					graphics.FillRectangle(&brush_1, X_RECT_COLOR + x_start_point, Y_COPY_VALUE_START_COORD + COPY_VALUE_BUTTON_HIGHT + 200,
						RECT_COLOR_WIDTH / 2 + 2, RECT_COLOR_HIGHT / 2);
					x_start_point += (5 + RECT_COLOR_WIDTH / 2 + 2);
				}
				Gdiplus::LinearGradientBrush linGrBrush(
					Gdiplus::Point(0, 10),
					Gdiplus::Point(475, 10),
					Gdiplus::Color(255, hslColorVector[0].getRedColor(), hslColorVector[0].getGreenColor(), hslColorVector[0].getBlueColor()),
					Gdiplus::Color(255, hslColorVector[COLOR_PALETE_LENGTH - 1].getRedColor(),
						hslColorVector[COLOR_PALETE_LENGTH - 1].getGreenColor(),
						hslColorVector[COLOR_PALETE_LENGTH - 1].getBlueColor()));

				graphics.FillRectangle(&linGrBrush, X_RECT_COLOR + 5, Y_COPY_VALUE_START_COORD + COPY_VALUE_BUTTON_HIGHT + 200 + 5 + (RECT_COLOR_HIGHT / 2),
					463, 30);
			}
			break;
		}
		isPushedButton = FALSE;

		current_angel = angel;
		for (int i = 0; i < COLOR_PALETE_LENGTH; i++) {
			oldColorVector[i] = Color(color.getRandomRedColor(), color.getRandomGreenColor(), color.getRandomBlueColor());
			hslOldColorVector[i] = Color(color.getHue() + current_angel, color.getSaturation(), color.getLightness());
			current_angel += angel;
		}

		oldColor.setRedColor(color.getRedColor());
		oldColor.setGreenColor(color.getGreenColor());
		oldColor.setBlueColor(color.getBlueColor());
		
	}
	// цвет маленького прямоугольника и палитра остаются прежними
	else {
		colorGeneratedPalette.clearList();
		Gdiplus::SolidBrush _rect_brush(Gdiplus::Color(oldColor.getRedColor(), oldColor.getGreenColor(), oldColor.getBlueColor()));
		graphics.FillRectangle(&_rect_brush, X_RECT_COLOR + 5, Y_COPY_VALUE_START_COORD + COPY_VALUE_BUTTON_HIGHT + 10 + 10,
			RECT_COLOR_WIDTH, RECT_COLOR_HIGHT);
		int start_point = 0;
		int x_start_point = 5;
		switch (colorPaletteAlgorithm) {
		case IDM_MONOCHROME:
			{
			for (int i = 0; i < COLOR_PALETE_LENGTH; i++) {
				Gdiplus::SolidBrush brush_1(Gdiplus::Color((oldColor.getRedColor() + start_point) % 255, (oldColor.getGreenColor() + start_point) % 255,
					(oldColor.getBlueColor() + start_point) % 255));
				tempColor.setRedColor(oldColor.getRedColor() + start_point);
				tempColor.setGreenColor(oldColor.getGreenColor() + start_point);
				tempColor.setBlueColor(oldColor.getBlueColor() + start_point);
				switch (colorFormatCode) {
				case 0:
					colorGeneratedPalette.addItem(tempColor.getStrHtmlColorCode());
					break;
				case 1:
					colorGeneratedPalette.addItem(tempColor.getStrRgbColorCode());
					break;
				case 2:
					colorGeneratedPalette.addItem(tempColor.getHslColorCode());
					break;
				}
				graphics.FillRectangle(&brush_1, X_RECT_COLOR + x_start_point, Y_COPY_VALUE_START_COORD + COPY_VALUE_BUTTON_HIGHT + 200,
					RECT_COLOR_WIDTH / 2 + 2, RECT_COLOR_HIGHT / 2);
				start_point += 10;
				x_start_point += (5 + RECT_COLOR_WIDTH / 2 + 2);
			}

			Gdiplus::LinearGradientBrush linGrBrush(
				Gdiplus::Point(0, 10),
				Gdiplus::Point(475, 10),
				Gdiplus::Color(255, oldColor.getRedColor(), oldColor.getGreenColor(), oldColor.getBlueColor()),
				Gdiplus::Color(255, (oldColor.getRedColor() + start_point - 10) % 255,
				(oldColor.getGreenColor() + start_point - 10) % 255,
					(oldColor.getBlueColor() + start_point - 10) % 255));

			graphics.FillRectangle(&linGrBrush, X_RECT_COLOR + 5, Y_COPY_VALUE_START_COORD + COPY_VALUE_BUTTON_HIGHT + 200 + 5 + (RECT_COLOR_HIGHT / 2),
				463, 30);
			}
			break;
		case IDM_RGB:
			{
			x_start_point = 5;
			for (int i = 0; i < COLOR_PALETE_LENGTH; i++) {
				Gdiplus::SolidBrush brush_1(Gdiplus::Color(oldColorVector[i].getRedColor(), oldColorVector[i].getGreenColor(), oldColorVector[i].getBlueColor()));
				tempColor.setRedColor(oldColorVector[i].getRedColor());
				tempColor.setGreenColor(oldColorVector[i].getGreenColor());
				tempColor.setBlueColor(oldColorVector[i].getBlueColor());
				switch (colorFormatCode) {
				case 0:
					colorGeneratedPalette.addItem(tempColor.getStrHtmlColorCode());
					break;
				case 1:
					colorGeneratedPalette.addItem(tempColor.getStrRgbColorCode());
					break;
				case 2:
					colorGeneratedPalette.addItem(tempColor.getHslColorCode());
					break;
				}
				graphics.FillRectangle(&brush_1, X_RECT_COLOR + x_start_point, Y_COPY_VALUE_START_COORD + COPY_VALUE_BUTTON_HIGHT + 200,
					RECT_COLOR_WIDTH / 2 + 2, RECT_COLOR_HIGHT / 2);
				x_start_point += (5 + RECT_COLOR_WIDTH / 2 + 2);
			}
			Gdiplus::LinearGradientBrush linGrBrush(
				Gdiplus::Point(0, 10),
				Gdiplus::Point(475, 10),
				Gdiplus::Color(255, oldColorVector[0].getRedColor(), oldColorVector[0].getGreenColor(), oldColorVector[0].getBlueColor()),
				Gdiplus::Color(255, oldColorVector[COLOR_PALETE_LENGTH - 1].getRedColor(),
					oldColorVector[COLOR_PALETE_LENGTH - 1].getGreenColor(),
					oldColorVector[COLOR_PALETE_LENGTH - 1].getBlueColor()));

			graphics.FillRectangle(&linGrBrush, X_RECT_COLOR + 5, Y_COPY_VALUE_START_COORD + COPY_VALUE_BUTTON_HIGHT + 200 + 5 + (RECT_COLOR_HIGHT / 2),
				463, 30);
			}
			break;
		case IDM_HSL:
			{
			x_start_point = 5;
			for (int i = 0; i < COLOR_PALETE_LENGTH; i++) {
				Gdiplus::SolidBrush brush_1(Gdiplus::Color(hslOldColorVector[i].getRedColor(), hslOldColorVector[i].getGreenColor(), hslOldColorVector[i].getBlueColor()));
				tempColor.setRedColor(hslOldColorVector[i].getRedColor());
				tempColor.setGreenColor(hslOldColorVector[i].getGreenColor());
				tempColor.setBlueColor(hslOldColorVector[i].getBlueColor());
				switch (colorFormatCode) {
				case 0:
					colorGeneratedPalette.addItem(tempColor.getStrHtmlColorCode());
					break;
				case 1:
					colorGeneratedPalette.addItem(tempColor.getStrRgbColorCode());
					break;
				case 2:
					colorGeneratedPalette.addItem(tempColor.getHslColorCode());
					break;
				}
				graphics.FillRectangle(&brush_1, X_RECT_COLOR + x_start_point, Y_COPY_VALUE_START_COORD + COPY_VALUE_BUTTON_HIGHT + 200,
					RECT_COLOR_WIDTH / 2 + 2, RECT_COLOR_HIGHT / 2);
				x_start_point += (5 + RECT_COLOR_WIDTH / 2 + 2);
			}
			Gdiplus::LinearGradientBrush linGrBrush(
				Gdiplus::Point(0, 10),
				Gdiplus::Point(475, 10),
				Gdiplus::Color(255, hslOldColorVector[0].getRedColor(), hslOldColorVector[0].getGreenColor(), hslOldColorVector[0].getBlueColor()),
				Gdiplus::Color(255, hslOldColorVector[COLOR_PALETE_LENGTH - 1].getRedColor(),
					hslOldColorVector[COLOR_PALETE_LENGTH - 1].getGreenColor(),
					hslOldColorVector[COLOR_PALETE_LENGTH - 1].getBlueColor()));

			graphics.FillRectangle(&linGrBrush, X_RECT_COLOR + 5, Y_COPY_VALUE_START_COORD + COPY_VALUE_BUTTON_HIGHT + 200 + 5 + (RECT_COLOR_HIGHT / 2),
				463, 30);
			}
			break;
		}
	}

	BitBlt(hdc, left, top, width, hight, hdcMem, left, top, SRCCOPY);
	RestoreDC(hdcMem, nMemDC);
	DeleteObject(hBitmap);
}


VOID OutputRgbValues(HDC hdc) {
	char temp_str_red[10];
	char temp_str_green[10];
	char temp_str_blue[10];

	char res_label[100];
	strcpy_s(res_label, "R: ");
	int radix = 10;
	_itoa_s(color.getRedColor(), temp_str_red, radix);
	strcat_s(res_label, temp_str_red);

	TextOut(hdc, X_RECT_COLOR, Y_RECT_COLOR + RECT_COLOR_HIGHT + 5, TEXT(res_label), strlen(res_label));

	// text for scroll bars
	TextOut(hdc, X_RECT_COLOR + 150 - 50, Y_RECT_COLOR + 250 - 15, oldColor.getRedColorStr(), strlen(oldColor.getRedColorStr()));
	TextOut(hdc, X_RECT_COLOR + 150 - 50, Y_RECT_COLOR + 250 + 10, oldColor.getGreenColorStr(), strlen(oldColor.getGreenColorStr()));
	TextOut(hdc, X_RECT_COLOR + 150 - 50, Y_RECT_COLOR + 250 + 35, oldColor.getBlueColorStr(), strlen(oldColor.getBlueColorStr()));
	
	strncpy_s(res_label, "G: ", sizeof("G: "));
	_itoa_s(color.getGreenColor(), temp_str_green, radix);
	strcat_s(res_label, temp_str_green);

	TextOut(hdc, X_RECT_COLOR, Y_RECT_COLOR + RECT_COLOR_HIGHT + 20, TEXT(res_label), strlen(res_label));

	strncpy_s(res_label, "B: ", sizeof("B: "));
	_itoa_s(color.getBlueColor(), temp_str_blue, radix);
	strcat_s(res_label, temp_str_blue);

	TextOut(hdc, X_RECT_COLOR, Y_RECT_COLOR + RECT_COLOR_HIGHT + 35, TEXT(res_label), strlen(res_label));
}

VOID GetDragAndDropedFiles(HWND hWnd, WPARAM wParam) {
	const int size_buffer = 400;
	char buffer[size_buffer];
	int count_of_dropped_files = 0;
	count_of_dropped_files = DragQueryFile((HDROP)wParam, 0, (LPSTR)buffer, size_buffer);
	int len = strlen(buffer);
	// Converts the path to wide characters
	int needed = MultiByteToWideChar(0, 0, buffer, len + 1, PATH_TO_IMAGE_BMP, len + 1);
	DragFinish((HDROP)wParam);
}

// For OWN ListBox
VOID AddItem(HWND hwnd, PTSTR pstr, COLORREF colorRef)
{
	int lbItem;
	lbItem = SendMessage(hwnd, LB_ADDSTRING, 0, (LPARAM)pstr);
	SendMessage(hwnd, LB_SETITEMDATA, (WPARAM)lbItem, (LPARAM)colorRef);
}

BOOL CopyColorCodeToClipboard(HWND hWnd) {
	const size_t len = strlen(color.getStrHtmlColorCode()) + 1;
	HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, len);
	memcpy(GlobalLock(hMem), color.getStrHtmlColorCode(), len);
	GlobalUnlock(hMem);
	OpenClipboard(0);
	EmptyClipboard();
	SetClipboardData(CF_TEXT, hMem);
	CloseClipboard();
	return TRUE;
}

VOID SaveColorListOpenDialogFile(HWND hWnd, ColorList color_list) {
	char szFile[100];

	OPENFILENAMEA ofn;
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = "All\0*.*\0Text\0*.TXT\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	if (GetOpenFileName(&ofn)) {
		SaveColorsListToFile(szFile, color_list);
	}
	else {
		//MessageBox(hWnd, "Error: file was not choosen or error with opening file.", "Error", 0);
	}
}

VOID SaveColorsListToFile(char *fileName, ColorList color_list) {
	HANDLE hFile;
	BOOL bErrorFlag = FALSE;
	DWORD dwBytesWritten = 0;

	hFile = CreateFile(fileName,	
		GENERIC_WRITE,							
		0,										
		NULL,									
		OPEN_EXISTING,							
		FILE_ATTRIBUTE_NORMAL,					
		NULL);									

	if (hFile == INVALID_HANDLE_VALUE)
	{
		MessageBeep(0);
		_tprintf(TEXT("Terminal failure: Unable to open file \"%s\" for write.\n"), FILEPATH_LIST_COLORS);
		return;
	}

	for (int i = 0; i < color_list.getSize(); i++) {
		bErrorFlag = WriteFile(hFile,color_list.getItem(i),strlen(color_list.getItem(i)),&dwBytesWritten,NULL);							
		if (dwBytesWritten != strlen(color_list.getItem(i))){
			//Handling error
		}
	}
	CloseHandle(hFile);
}

VOID GetColorListOpenDialogFile(HWND hWnd) {
	char szFile[100];
	
	OPENFILENAMEA ofn;
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = "All\0*.*\0Text\0*.TXT\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	if (GetOpenFileName(&ofn)) {
		GetColorListFromFile(hWnd, szFile);
	}
	else {
		MessageBox(hWnd, 
			"Error: file was not choosen or error with opening file.", "Error", 0);
	}
}

VOID GetColorListFromFile(HWND hWnd, char *path) {
	HANDLE hFile;
	char *buff = (char *)calloc(8, sizeof(char));
	char *tempBuff = (char *)calloc(MAX_AMOUNT_ITEMS_LIST_BOX * 8, sizeof(char));
	DWORD numberOfBytesRead = 0;

	hFile = CreateFile(path,					// name of the write
		GENERIC_READ,							// open for reading
		0,										// do not share
		NULL,									// default security
		OPEN_EXISTING,							// open an existing file
		FILE_ATTRIBUTE_NORMAL,					// normal file
		NULL);									// no attr. template

	if (hFile == INVALID_HANDLE_VALUE)
	{
		MessageBox(hWnd, "Terminal failure: Unable to open file for write.", "Error", 0);
		return;
	}

	try {
		ReadFile(hFile, tempBuff, 200, &numberOfBytesRead, NULL);
		if (tempBuff[0] != '#') {
			MessageBox(hWnd, "Data in the file is in incorrect state.", "Error", 1);
			CloseHandle(hFile);
			tempBuff = NULL;
			buff = NULL;
			return;
		}
		buff[0] = tempBuff[0];
		int j = 1;
		for (int i = 1; i < numberOfBytesRead + 1; i++) {
			if (tempBuff[i] != '#')
			{
				buff[j] = tempBuff[i];
				j++;
				if (i == numberOfBytesRead) {
					buff[j] = '\0';
					colorList.addItem(buff);
					SendMessage(hListBox, LB_ADDSTRING, NULL, (LPARAM)(LPSTR)buff);
				}
			}
			else {
				buff[j] = '\0';
				colorList.addItem(buff);
				SendMessage(hListBox, LB_ADDSTRING, NULL, (LPARAM)(LPSTR)buff);
				buff[0] = tempBuff[i];
				j = 1;
			}

		}
	}
	catch (...) {
		MessageBox(hWnd, "Data in file is incorrect.",
			"Error", 0);
	}

	CloseHandle(hFile);
	buff = NULL;
	tempBuff = NULL;
}

VOID CreateAHorizontalScrollBar(HWND hwndParent, int sbHeight)
{
	SCROLLINFO lpcScrollInfo;
	ZeroMemory(&lpcScrollInfo, sizeof(lpcScrollInfo));
	
	lpcScrollInfo.cbSize = sizeof(lpcScrollInfo);
	lpcScrollInfo.nMax = 269;
	lpcScrollInfo.nMin = 0;
	lpcScrollInfo.nPos = oldColor.getRedColor();
	lpcScrollInfo.nTrackPos = 0;

	lpcScrollInfo.nPage = 1;
	lpcScrollInfo.fMask = SIF_ALL;

	hRedColorScrollBar = CreateWindowEx(
		0,								// no extended styles 
		"SCROLLBAR",			// scroll bar control class 
		(PTSTR)NULL,           // no window text 
		WS_CHILD | WS_VISIBLE   // window styles  
		| SBS_HORZ,         // horizontal scroll bar style 
		X_RECT_COLOR + 150,              // horizontal position 
		Y_RECT_COLOR + 250 - sbHeight, // vertical position 
		310,             // width of the scroll bar 
		sbHeight,               // height of the scroll bar
		hwndParent,             // handle to main window 
		(HMENU)NULL,           // no menu 
		hInst,                // instance owning this window 
		NULL             
	);
	SetScrollInfo(hRedColorScrollBar, SB_CTL, &lpcScrollInfo, TRUE);

	lpcScrollInfo.nPos = oldColor.getGreenColor();
	hGreenColorScrollBar = CreateWindowEx(
		0,
		"SCROLLBAR",
		(PTSTR)NULL,
		WS_CHILD | WS_VISIBLE | SBS_HORZ,
		X_RECT_COLOR + 150,
		Y_RECT_COLOR + 250 + 10,
		310,
		sbHeight,
		hwndParent,
		(HMENU)NULL,
		hInst,
		(PVOID)NULL
	);
	SetScrollInfo(hGreenColorScrollBar, SB_CTL, &lpcScrollInfo, TRUE);

	lpcScrollInfo.nPos = oldColor.getBlueColor();
	hBlueColorScrollBar = CreateWindowEx(
		0,
		"SCROLLBAR",
		(PTSTR)NULL,
		WS_CHILD | WS_VISIBLE | SBS_HORZ,
		X_RECT_COLOR + 150,
		Y_RECT_COLOR + 250 + 10 + sbHeight + 10,
		310,
		sbHeight,
		hwndParent,
		(HMENU)NULL,
		hInst,
		(PVOID)NULL
	);
	SetScrollInfo(hBlueColorScrollBar, SB_CTL, &lpcScrollInfo, TRUE);
}

VOID CreateMixColorWindow(HWND hWnd) {
	hMixColorWindow = CreateWindow("MixColorClass", "Mix Colors",
		WS_OVERLAPPEDWINDOW, 0, 0, START_WIDTH + 250, START_HIGHT + 30, NULL, NULL, hInst, NULL);
	ShowWindow(hMixColorWindow, SW_SHOW);
	UpdateWindow(hMixColorWindow);
}

VOID DrawMixColorRectangles(HWND hWnd, HDC hdc) {
	Gdiplus::SolidBrush solidBrushFirstColor(Gdiplus::Color(mainTransparancy, first_mix_color.getRedColor(),
		first_mix_color.getGreenColor(), first_mix_color.getBlueColor()));
	Gdiplus::SolidBrush solidBrushSecondColor(Gdiplus::Color(mainTransparancy, second_mix_color.getRedColor(),
		second_mix_color.getGreenColor(), second_mix_color.getBlueColor()));
	Gdiplus::SolidBrush solidBrushThirdColor(Gdiplus::Color(mainTransparancy, third_mix_color.getRedColor(),
		third_mix_color.getGreenColor(), third_mix_color.getBlueColor()));
	Gdiplus::SolidBrush solidBrushMixColor(Gdiplus::Color(mainTransparancy, res_mix_color.getRedColor(),
		res_mix_color.getGreenColor(), res_mix_color.getBlueColor()));
	RECT rcClient;
	GetClientRect(hWnd, &rcClient);
	int left = rcClient.left;
	int top = rcClient.top;
	int width = rcClient.right - rcClient.left;
	int hight = rcClient.bottom - rcClient.top;

	HDC hdcMem = CreateCompatibleDC(hdc);
	const int nMemDC = SaveDC(hdcMem);

	HBITMAP hBitmap = CreateCompatibleBitmap(hdc, width, hight);
	SelectObject(hdcMem, hBitmap);

	Gdiplus::Graphics graphics(hdcMem);
	FillRect(hdcMem, &rcClient, (HBRUSH)(COLOR_WINDOW + 1));
	graphics.FillRectangle(&solidBrushFirstColor, X_RECT_MIX_COLOR_1, Y_RECT_MIX_COLOR_1, RECT_MIX_COLOR_WIDTH, RECT_MIX_COLOR_HIGHT);
	graphics.FillRectangle(&solidBrushSecondColor, X_RECT_MIX_COLOR_2, Y_RECT_MIX_COLOR_1, RECT_MIX_COLOR_WIDTH, RECT_MIX_COLOR_HIGHT);
	graphics.FillRectangle(&solidBrushThirdColor, X_RECT_MIX_COLOR_3, Y_RECT_MIX_COLOR_1, RECT_MIX_COLOR_WIDTH, RECT_MIX_COLOR_HIGHT);
	graphics.FillRectangle(&solidBrushMixColor, X_RECT_MIX_COLOR_2,
		Y_RECT_MIX_COLOR_1 + RECT_MIX_COLOR_HIGHT + 140 + COPY_VALUE_BUTTON_HIGHT + 15, 
		RECT_MIX_COLOR_WIDTH, RECT_MIX_COLOR_HIGHT);
	BitBlt(hdc, left, top, width, hight, hdcMem, left, top, SRCCOPY);
	RestoreDC(hdcMem, nMemDC);
	DeleteObject(hBitmap);
}

VOID OutputRGBMixColorCode(HDC hdc) {
	char res_label[100];
	strcpy_s(res_label, res_mix_color.getStrRgbColorCode());
	TextOut(hdc, X_RECT_MIX_COLOR_2, Y_RECT_MIX_COLOR_1 + RECT_MIX_COLOR_HIGHT + 140 + COPY_VALUE_BUTTON_HIGHT + 25 + RECT_MIX_COLOR_HIGHT, TEXT(res_label), strlen(res_label));
}

VOID OutputHTMLMixColorCode(HDC hdc) {
	char res_label[100];
	strcpy_s(res_label, res_mix_color.getStrHtmlColorCode());
	TextOut(hdc, X_RECT_MIX_COLOR_2, Y_RECT_MIX_COLOR_1 + RECT_MIX_COLOR_HIGHT + 140 + COPY_VALUE_BUTTON_HIGHT + 25 + RECT_MIX_COLOR_HIGHT, TEXT(res_label), strlen(res_label));
}
VOID OutputHSLMixColorCode(HDC hdc) {
	char res_label[100];
	strcpy_s(res_label, res_mix_color.getHslColorCode());
	TextOut(hdc, X_RECT_MIX_COLOR_2, Y_RECT_MIX_COLOR_1 + RECT_MIX_COLOR_HIGHT + 140 + COPY_VALUE_BUTTON_HIGHT + 25 + RECT_MIX_COLOR_HIGHT, TEXT(res_label), strlen(res_label));
}

VOID OutputHumbrolMixColorCode(HDC hdc) {
	char res_label[100];
	if (strlen(res_mix_color.getHumbrolCodeStr()) > 0)
		strcpy_s(res_label, res_mix_color.getHumbrolCodeStr());
	else
		strcpy_s(res_label, res_mix_color.getStrRgbColorCode());
	TextOut(hdc, X_RECT_MIX_COLOR_2, Y_RECT_MIX_COLOR_1 + RECT_MIX_COLOR_HIGHT + 140 + COPY_VALUE_BUTTON_HIGHT + 25 + RECT_MIX_COLOR_HIGHT, TEXT(res_label), strlen(res_label));
}

VOID UpdateMixWindowToHumbrol(HWND hWnd, HDC hdc) {
	ShowWindow(hEditMixR_1, SW_HIDE);
	ShowWindow(hEditMixR_2, SW_HIDE);
	ShowWindow(hEditMixR_3, SW_HIDE);
	ShowWindow(hEditMixG_1, SW_HIDE);
	ShowWindow(hEditMixG_2, SW_HIDE);
	ShowWindow(hEditMixG_3, SW_HIDE);
	ShowWindow(hEditMixB_1, SW_HIDE);
	ShowWindow(hEditMixB_2, SW_HIDE);
	ShowWindow(hEditMixB_3, SW_HIDE);

	ShowWindow(hEditMixH_1, SW_HIDE);
	ShowWindow(hEditMixH_2, SW_HIDE);
	ShowWindow(hEditMixH_3, SW_HIDE);
	ShowWindow(hEditMixS_1, SW_HIDE);
	ShowWindow(hEditMixS_2, SW_HIDE);
	ShowWindow(hEditMixS_3, SW_HIDE);
	ShowWindow(hEditMixL_1, SW_HIDE);
	ShowWindow(hEditMixL_2, SW_HIDE);
	ShowWindow(hEditMixL_3, SW_HIDE);

	ShowWindow(hEditHumbrol_1, SW_SHOW);
	ShowWindow(hEditHumbrol_2, SW_SHOW);
	ShowWindow(hEditHumbrol_3, SW_SHOW);

	ShowWindow(hEditHex_1, SW_HIDE);
	ShowWindow(hEditHex_2, SW_HIDE);
	ShowWindow(hEditHex_3, SW_HIDE);

	Gdiplus::SolidBrush solidWhiteColor(Gdiplus::Color(mainTransparancy, 255, 255, 255));
	RECT rcClient;
	GetClientRect(hWnd, &rcClient);
	int left = rcClient.left;
	int top = rcClient.top;
	int width = rcClient.right - rcClient.left;
	int hight = rcClient.bottom - rcClient.top;

	HDC hdcMem = CreateCompatibleDC(hdc);
	const int nMemDC = SaveDC(hdcMem);

	HBITMAP hBitmap = CreateCompatibleBitmap(hdc, width, hight);
	SelectObject(hdcMem, hBitmap);
	Gdiplus::Graphics graphics(hdcMem);
	FillRect(hdcMem, &rcClient, (HBRUSH)(COLOR_WINDOW + 1));
	graphics.FillRectangle(&solidWhiteColor, X_RECT_MIX_COLOR_1, Y_RECT_MIX_COLOR_1 + RECT_MIX_COLOR_HIGHT + 8, RECT_MIX_COLOR_WIDTH * 3, RECT_MIX_COLOR_HIGHT);
	BitBlt(hdc, left, top, width, hight, hdcMem, left, top, SRCCOPY);
	RestoreDC(hdcMem, nMemDC);
	DeleteObject(hBitmap);
}

VOID UpdateMixWindowToHTML(HWND hWnd, HDC hdc) {
	ShowWindow(hEditMixR_1, SW_HIDE);
	ShowWindow(hEditMixR_2, SW_HIDE);
	ShowWindow(hEditMixR_3, SW_HIDE);
	ShowWindow(hEditMixG_1, SW_HIDE);
	ShowWindow(hEditMixG_2, SW_HIDE);
	ShowWindow(hEditMixG_3, SW_HIDE);
	ShowWindow(hEditMixB_1, SW_HIDE);
	ShowWindow(hEditMixB_2, SW_HIDE);
	ShowWindow(hEditMixB_3, SW_HIDE);

	ShowWindow(hEditMixH_1, SW_HIDE);
	ShowWindow(hEditMixH_2, SW_HIDE);
	ShowWindow(hEditMixH_3, SW_HIDE);
	ShowWindow(hEditMixS_1, SW_HIDE);
	ShowWindow(hEditMixS_2, SW_HIDE);
	ShowWindow(hEditMixS_3, SW_HIDE);
	ShowWindow(hEditMixL_1, SW_HIDE);
	ShowWindow(hEditMixL_2, SW_HIDE);
	ShowWindow(hEditMixL_3, SW_HIDE);

	ShowWindow(hEditHumbrol_1, SW_HIDE);
	ShowWindow(hEditHumbrol_2, SW_HIDE);
	ShowWindow(hEditHumbrol_3, SW_HIDE);

	ShowWindow(hEditHex_1, SW_SHOW);
	ShowWindow(hEditHex_2, SW_SHOW);
	ShowWindow(hEditHex_3, SW_SHOW);

	Gdiplus::SolidBrush solidWhiteColor(Gdiplus::Color(mainTransparancy, 255, 255, 255));
	RECT rcClient;
	GetClientRect(hWnd, &rcClient);
	int left = rcClient.left;
	int top = rcClient.top;
	int width = rcClient.right - rcClient.left;
	int hight = rcClient.bottom - rcClient.top;

	HDC hdcMem = CreateCompatibleDC(hdc);
	const int nMemDC = SaveDC(hdcMem);

	HBITMAP hBitmap = CreateCompatibleBitmap(hdc, width, hight);
	SelectObject(hdcMem, hBitmap);
	Gdiplus::Graphics graphics(hdcMem);
	FillRect(hdcMem, &rcClient, (HBRUSH)(COLOR_WINDOW + 1));
	graphics.FillRectangle(&solidWhiteColor, X_RECT_MIX_COLOR_1, Y_RECT_MIX_COLOR_1 + RECT_MIX_COLOR_HIGHT + 8, RECT_MIX_COLOR_WIDTH * 3, RECT_MIX_COLOR_HIGHT);
	BitBlt(hdc, left, top, width, hight, hdcMem, left, top, SRCCOPY);
	RestoreDC(hdcMem, nMemDC);
	DeleteObject(hBitmap);
}

VOID UpdateMixWindowToHSL(HWND hWnd, HDC hdc) {
	ShowWindow(hEditMixR_1, SW_HIDE);
	ShowWindow(hEditMixR_2, SW_HIDE);
	ShowWindow(hEditMixR_3, SW_HIDE);
	ShowWindow(hEditMixG_1, SW_HIDE);
	ShowWindow(hEditMixG_2, SW_HIDE);
	ShowWindow(hEditMixG_3, SW_HIDE);
	ShowWindow(hEditMixB_1, SW_HIDE);
	ShowWindow(hEditMixB_2, SW_HIDE);
	ShowWindow(hEditMixB_3, SW_HIDE);

	ShowWindow(hEditHex_1, SW_HIDE);
	ShowWindow(hEditHex_2, SW_HIDE);
	ShowWindow(hEditHex_3, SW_HIDE);

	ShowWindow(hEditMixH_1, SW_SHOW);
	ShowWindow(hEditMixH_2, SW_SHOW);
	ShowWindow(hEditMixH_3, SW_SHOW);
	ShowWindow(hEditMixS_1, SW_SHOW);
	ShowWindow(hEditMixS_2, SW_SHOW);
	ShowWindow(hEditMixS_3, SW_SHOW);
	ShowWindow(hEditMixL_1, SW_SHOW);
	ShowWindow(hEditMixL_2, SW_SHOW);
	ShowWindow(hEditMixL_3, SW_SHOW);

	ShowWindow(hEditHumbrol_1, SW_HIDE);
	ShowWindow(hEditHumbrol_2, SW_HIDE);
	ShowWindow(hEditHumbrol_3, SW_HIDE);

	Gdiplus::SolidBrush solidWhiteColor(Gdiplus::Color(mainTransparancy, 255, 255, 255));
	RECT rcClient;
	GetClientRect(hWnd, &rcClient);
	int left = rcClient.left;
	int top = rcClient.top;
	int width = rcClient.right - rcClient.left;
	int hight = rcClient.bottom - rcClient.top;

	HDC hdcMem = CreateCompatibleDC(hdc);
	const int nMemDC = SaveDC(hdcMem);

	HBITMAP hBitmap = CreateCompatibleBitmap(hdc, width, hight);
	SelectObject(hdcMem, hBitmap);
	Gdiplus::Graphics graphics(hdcMem);
	FillRect(hdcMem, &rcClient, (HBRUSH)(COLOR_WINDOW + 1));
	graphics.FillRectangle(&solidWhiteColor, X_RECT_MIX_COLOR_1, Y_RECT_MIX_COLOR_1 + RECT_MIX_COLOR_HIGHT + 8, RECT_MIX_COLOR_WIDTH * 2, RECT_MIX_COLOR_HIGHT * 3);
	BitBlt(hdc, left, top, width, hight, hdcMem, left, top, SRCCOPY);
	RestoreDC(hdcMem, nMemDC);
	DeleteObject(hBitmap);
}

VOID UpdateMixWindowToRGB() {
	ShowWindow(hEditMixR_1, SW_SHOW);
	ShowWindow(hEditMixR_2, SW_SHOW);
	ShowWindow(hEditMixR_3, SW_SHOW);
	ShowWindow(hEditMixG_1, SW_SHOW);
	ShowWindow(hEditMixG_2, SW_SHOW);
	ShowWindow(hEditMixG_3, SW_SHOW);
	ShowWindow(hEditMixB_1, SW_SHOW);
	ShowWindow(hEditMixB_2, SW_SHOW);
	ShowWindow(hEditMixB_3, SW_SHOW);

	ShowWindow(hEditHex_1, SW_HIDE);
	ShowWindow(hEditHex_2, SW_HIDE);
	ShowWindow(hEditHex_3, SW_HIDE);

	ShowWindow(hEditMixH_1, SW_HIDE);
	ShowWindow(hEditMixH_2, SW_HIDE);
	ShowWindow(hEditMixH_3, SW_HIDE);
	ShowWindow(hEditMixS_1, SW_HIDE);
	ShowWindow(hEditMixS_2, SW_HIDE);
	ShowWindow(hEditMixS_3, SW_HIDE);
	ShowWindow(hEditMixL_1, SW_HIDE);
	ShowWindow(hEditMixL_2, SW_HIDE);
	ShowWindow(hEditMixL_3, SW_HIDE);

	ShowWindow(hEditHumbrol_1, SW_HIDE);
	ShowWindow(hEditHumbrol_2, SW_HIDE);
	ShowWindow(hEditHumbrol_3, SW_HIDE);
}

VOID DrawColorTitlesMixRGB(HDC hdc) {
	char res_label[100];
	strcpy_s(res_label, "R: ");
	TextOut(hdc, X_RECT_MIX_COLOR_1, Y_RECT_MIX_COLOR_1 + RECT_MIX_COLOR_HIGHT + 8, TEXT(res_label), strlen(res_label));
	TextOut(hdc, X_RECT_MIX_COLOR_2, Y_RECT_MIX_COLOR_1 + RECT_MIX_COLOR_HIGHT + 8, TEXT(res_label), strlen(res_label));
	TextOut(hdc, X_RECT_MIX_COLOR_3, Y_RECT_MIX_COLOR_1 + RECT_MIX_COLOR_HIGHT + 8, TEXT(res_label), strlen(res_label));

	strncpy_s(res_label, "G: ", sizeof("G: "));
	TextOut(hdc, X_RECT_MIX_COLOR_1, Y_RECT_MIX_COLOR_1 + RECT_MIX_COLOR_HIGHT + 35, TEXT(res_label), strlen(res_label));
	TextOut(hdc, X_RECT_MIX_COLOR_2, Y_RECT_MIX_COLOR_1 + RECT_MIX_COLOR_HIGHT + 35, TEXT(res_label), strlen(res_label));
	TextOut(hdc, X_RECT_MIX_COLOR_3, Y_RECT_MIX_COLOR_1 + RECT_MIX_COLOR_HIGHT + 35, TEXT(res_label), strlen(res_label));

	strncpy_s(res_label, "B: ", sizeof("B: "));
	TextOut(hdc, X_RECT_MIX_COLOR_1, Y_RECT_MIX_COLOR_1 + RECT_MIX_COLOR_HIGHT + 62, TEXT(res_label), strlen(res_label));
	TextOut(hdc, X_RECT_MIX_COLOR_2, Y_RECT_MIX_COLOR_1 + RECT_MIX_COLOR_HIGHT + 62, TEXT(res_label), strlen(res_label));
	TextOut(hdc, X_RECT_MIX_COLOR_3, Y_RECT_MIX_COLOR_1 + RECT_MIX_COLOR_HIGHT + 62, TEXT(res_label), strlen(res_label));

	strncpy_s(res_label, "Portion (%):", sizeof("Portion (%):"));
	TextOut(hdc, X_RECT_MIX_COLOR_1, Y_RECT_MIX_COLOR_1 + RECT_MIX_COLOR_HIGHT + 90, TEXT(res_label), strlen(res_label));
	TextOut(hdc, X_RECT_MIX_COLOR_2, Y_RECT_MIX_COLOR_1 + RECT_MIX_COLOR_HIGHT + 90, TEXT(res_label), strlen(res_label));
	TextOut(hdc, X_RECT_MIX_COLOR_3, Y_RECT_MIX_COLOR_1 + RECT_MIX_COLOR_HIGHT + 90, TEXT(res_label), strlen(res_label));
}

VOID DrawColorTitlesMixHumbrol(HDC hdc) {
	char res_label[100];
	strcpy_s(res_label, "Humbrol: ");
	TextOut(hdc, X_RECT_MIX_COLOR_1, Y_RECT_MIX_COLOR_1 + RECT_MIX_COLOR_HIGHT + 8, TEXT(res_label), strlen(res_label));
	TextOut(hdc, X_RECT_MIX_COLOR_2, Y_RECT_MIX_COLOR_1 + RECT_MIX_COLOR_HIGHT + 8, TEXT(res_label), strlen(res_label));
	TextOut(hdc, X_RECT_MIX_COLOR_3, Y_RECT_MIX_COLOR_1 + RECT_MIX_COLOR_HIGHT + 8, TEXT(res_label), strlen(res_label));

	strncpy_s(res_label, "Portion (%):", sizeof("Portion (%):"));
	TextOut(hdc, X_RECT_MIX_COLOR_1, Y_RECT_MIX_COLOR_1 + RECT_MIX_COLOR_HIGHT + 90, TEXT(res_label), strlen(res_label));
	TextOut(hdc, X_RECT_MIX_COLOR_2, Y_RECT_MIX_COLOR_1 + RECT_MIX_COLOR_HIGHT + 90, TEXT(res_label), strlen(res_label));
	TextOut(hdc, X_RECT_MIX_COLOR_3, Y_RECT_MIX_COLOR_1 + RECT_MIX_COLOR_HIGHT + 90, TEXT(res_label), strlen(res_label));
}

VOID DrawColorTitlesMixHTML(HDC hdc) {
	char res_label[100];
	strcpy_s(res_label, "HEX: ");
	TextOut(hdc, X_RECT_MIX_COLOR_1, Y_RECT_MIX_COLOR_1 + RECT_MIX_COLOR_HIGHT + 8, TEXT(res_label), strlen(res_label));
	TextOut(hdc, X_RECT_MIX_COLOR_2, Y_RECT_MIX_COLOR_1 + RECT_MIX_COLOR_HIGHT + 8, TEXT(res_label), strlen(res_label));
	TextOut(hdc, X_RECT_MIX_COLOR_3, Y_RECT_MIX_COLOR_1 + RECT_MIX_COLOR_HIGHT + 8, TEXT(res_label), strlen(res_label));

	strncpy_s(res_label, "Portion (%):", sizeof("Portion (%):"));
	TextOut(hdc, X_RECT_MIX_COLOR_1, Y_RECT_MIX_COLOR_1 + RECT_MIX_COLOR_HIGHT + 90, TEXT(res_label), strlen(res_label));
	TextOut(hdc, X_RECT_MIX_COLOR_2, Y_RECT_MIX_COLOR_1 + RECT_MIX_COLOR_HIGHT + 90, TEXT(res_label), strlen(res_label));
	TextOut(hdc, X_RECT_MIX_COLOR_3, Y_RECT_MIX_COLOR_1 + RECT_MIX_COLOR_HIGHT + 90, TEXT(res_label), strlen(res_label));
}

VOID DrawColorTitlesMixHSL(HDC hdc) {
	char res_label[100];
	strcpy_s(res_label, "H: ");
	TextOut(hdc, X_RECT_MIX_COLOR_1, Y_RECT_MIX_COLOR_1 + RECT_MIX_COLOR_HIGHT + 8, TEXT(res_label), strlen(res_label));
	TextOut(hdc, X_RECT_MIX_COLOR_2, Y_RECT_MIX_COLOR_1 + RECT_MIX_COLOR_HIGHT + 8, TEXT(res_label), strlen(res_label));
	TextOut(hdc, X_RECT_MIX_COLOR_3, Y_RECT_MIX_COLOR_1 + RECT_MIX_COLOR_HIGHT + 8, TEXT(res_label), strlen(res_label));

	strncpy_s(res_label, "S: ", sizeof("G: "));
	TextOut(hdc, X_RECT_MIX_COLOR_1, Y_RECT_MIX_COLOR_1 + RECT_MIX_COLOR_HIGHT + 35, TEXT(res_label), strlen(res_label));
	TextOut(hdc, X_RECT_MIX_COLOR_2, Y_RECT_MIX_COLOR_1 + RECT_MIX_COLOR_HIGHT + 35, TEXT(res_label), strlen(res_label));
	TextOut(hdc, X_RECT_MIX_COLOR_3, Y_RECT_MIX_COLOR_1 + RECT_MIX_COLOR_HIGHT + 35, TEXT(res_label), strlen(res_label));

	strncpy_s(res_label, "L: ", sizeof("B: "));
	TextOut(hdc, X_RECT_MIX_COLOR_1, Y_RECT_MIX_COLOR_1 + RECT_MIX_COLOR_HIGHT + 62, TEXT(res_label), strlen(res_label));
	TextOut(hdc, X_RECT_MIX_COLOR_2, Y_RECT_MIX_COLOR_1 + RECT_MIX_COLOR_HIGHT + 62, TEXT(res_label), strlen(res_label));
	TextOut(hdc, X_RECT_MIX_COLOR_3, Y_RECT_MIX_COLOR_1 + RECT_MIX_COLOR_HIGHT + 62, TEXT(res_label), strlen(res_label));

	strncpy_s(res_label, "Portion (%):", sizeof("Portion (%):"));
	TextOut(hdc, X_RECT_MIX_COLOR_1, Y_RECT_MIX_COLOR_1 + RECT_MIX_COLOR_HIGHT + 90, TEXT(res_label), strlen(res_label));
	TextOut(hdc, X_RECT_MIX_COLOR_2, Y_RECT_MIX_COLOR_1 + RECT_MIX_COLOR_HIGHT + 90, TEXT(res_label), strlen(res_label));
	TextOut(hdc, X_RECT_MIX_COLOR_3, Y_RECT_MIX_COLOR_1 + RECT_MIX_COLOR_HIGHT + 90, TEXT(res_label), strlen(res_label));
}


VOID SetEditLimitations() {
	SendMessage(hEditMixR_1, (UINT)EM_SETLIMITTEXT, (WPARAM)3, (LPARAM)0);
	SendMessage(hEditMixR_2, (UINT)EM_SETLIMITTEXT, (WPARAM)3, (LPARAM)0);
	SendMessage(hEditMixR_3, (UINT)EM_SETLIMITTEXT, (WPARAM)3, (LPARAM)0);
	SendMessage(hEditMixG_1, (UINT)EM_SETLIMITTEXT, (WPARAM)3, (LPARAM)0);
	SendMessage(hEditMixG_2, (UINT)EM_SETLIMITTEXT, (WPARAM)3, (LPARAM)0);
	SendMessage(hEditMixG_3, (UINT)EM_SETLIMITTEXT, (WPARAM)3, (LPARAM)0);
	SendMessage(hEditMixB_1, (UINT)EM_SETLIMITTEXT, (WPARAM)3, (LPARAM)0);
	SendMessage(hEditMixB_2, (UINT)EM_SETLIMITTEXT, (WPARAM)3, (LPARAM)0);
	SendMessage(hEditMixB_3, (UINT)EM_SETLIMITTEXT, (WPARAM)3, (LPARAM)0);
	SendMessage(hEditMixPortion_1, (UINT)EM_SETLIMITTEXT, (WPARAM)2, (LPARAM)0);
	SendMessage(hEditMixPortion_2, (UINT)EM_SETLIMITTEXT, (WPARAM)2, (LPARAM)0);
	SendMessage(hEditMixPortion_3, (UINT)EM_SETLIMITTEXT, (WPARAM)2, (LPARAM)0);
	SendMessage(hEditHex_1, (UINT)EM_SETLIMITTEXT, (WPARAM)7, (LPARAM)0);
	SendMessage(hEditHex_2, (UINT)EM_SETLIMITTEXT, (WPARAM)7, (LPARAM)0);
	SendMessage(hEditHex_3, (UINT)EM_SETLIMITTEXT, (WPARAM)7, (LPARAM)0);
	SendMessage(hEditMixH_1, (UINT)EM_SETLIMITTEXT, (WPARAM)6, (LPARAM)0);
	SendMessage(hEditMixH_2, (UINT)EM_SETLIMITTEXT, (WPARAM)6, (LPARAM)0);
	SendMessage(hEditMixH_3, (UINT)EM_SETLIMITTEXT, (WPARAM)6, (LPARAM)0);
	SendMessage(hEditMixS_1, (UINT)EM_SETLIMITTEXT, (WPARAM)6, (LPARAM)0);
	SendMessage(hEditMixS_2, (UINT)EM_SETLIMITTEXT, (WPARAM)6, (LPARAM)0);
	SendMessage(hEditMixS_3, (UINT)EM_SETLIMITTEXT, (WPARAM)6, (LPARAM)0);
	SendMessage(hEditMixL_1, (UINT)EM_SETLIMITTEXT, (WPARAM)6, (LPARAM)0);
	SendMessage(hEditMixL_2, (UINT)EM_SETLIMITTEXT, (WPARAM)6, (LPARAM)0);
	SendMessage(hEditMixL_3, (UINT)EM_SETLIMITTEXT, (WPARAM)6, (LPARAM)0);
	SendMessage(hEditHumbrol_1, (UINT)EM_SETLIMITTEXT, (WPARAM)5, (LPARAM)0);
	SendMessage(hEditHumbrol_2, (UINT)EM_SETLIMITTEXT, (WPARAM)5, (LPARAM)0);
	SendMessage(hEditHumbrol_3, (UINT)EM_SETLIMITTEXT, (WPARAM)5, (LPARAM)0);
}

VOID DrawColorEditsMix(HWND hWnd) {

	hEditMixR_1 = CreateWindow("EDIT", 0, WS_BORDER | WS_CHILD | WS_VISIBLE, X_RECT_MIX_COLOR_1 + 20,
		Y_RECT_MIX_COLOR_1 + RECT_MIX_COLOR_HIGHT + 8, RECT_EDIT_MIX_WIDTH, RECT_EDIT_MIX_HIGHT, hWnd, 0, hInst, 0);
	hEditMixR_2 = CreateWindow("EDIT", 0, WS_BORDER | WS_CHILD | WS_VISIBLE, X_RECT_MIX_COLOR_2 + 20,
		Y_RECT_MIX_COLOR_1 + RECT_MIX_COLOR_HIGHT + 8, RECT_EDIT_MIX_WIDTH, RECT_EDIT_MIX_HIGHT, hWnd, 0, hInst, 0);
	hEditMixR_3 = CreateWindow("EDIT", 0, WS_BORDER | WS_CHILD | WS_VISIBLE, X_RECT_MIX_COLOR_3 + 20,
		Y_RECT_MIX_COLOR_1 + RECT_MIX_COLOR_HIGHT + 8, RECT_EDIT_MIX_WIDTH, RECT_EDIT_MIX_HIGHT, hWnd, 0, hInst, 0);

	hEditMixG_1 = CreateWindow("EDIT", 0, WS_BORDER | WS_CHILD | WS_VISIBLE, X_RECT_MIX_COLOR_1 + 20,
		Y_RECT_MIX_COLOR_1 + RECT_MIX_COLOR_HIGHT + 35, RECT_EDIT_MIX_WIDTH, RECT_EDIT_MIX_HIGHT, hWnd, 0, hInst, 0);
	hEditMixG_2 = CreateWindow("EDIT", 0, WS_BORDER | WS_CHILD | WS_VISIBLE, X_RECT_MIX_COLOR_2 + 20,
		Y_RECT_MIX_COLOR_1 + RECT_MIX_COLOR_HIGHT + 35, RECT_EDIT_MIX_WIDTH, RECT_EDIT_MIX_HIGHT, hWnd, 0, hInst, 0);
	hEditMixG_3 = CreateWindow("EDIT", 0, WS_BORDER | WS_CHILD | WS_VISIBLE, X_RECT_MIX_COLOR_3 + 20,
		Y_RECT_MIX_COLOR_1 + RECT_MIX_COLOR_HIGHT + 35, RECT_EDIT_MIX_WIDTH, RECT_EDIT_MIX_HIGHT, hWnd, 0, hInst, 0);

	hEditMixB_1 = CreateWindow("EDIT", 0, WS_BORDER | WS_CHILD | WS_VISIBLE, X_RECT_MIX_COLOR_1 + 20,
		Y_RECT_MIX_COLOR_1 + RECT_MIX_COLOR_HIGHT + 62, RECT_EDIT_MIX_WIDTH, RECT_EDIT_MIX_HIGHT, hWnd, 0, hInst, 0);
	hEditMixB_2 = CreateWindow("EDIT", 0, WS_BORDER | WS_CHILD | WS_VISIBLE, X_RECT_MIX_COLOR_2 + 20,
		Y_RECT_MIX_COLOR_1 + RECT_MIX_COLOR_HIGHT + 62, RECT_EDIT_MIX_WIDTH, RECT_EDIT_MIX_HIGHT, hWnd, 0, hInst, 0);
	hEditMixB_3 = CreateWindow("EDIT", 0, WS_BORDER | WS_CHILD | WS_VISIBLE, X_RECT_MIX_COLOR_3 + 20,
		Y_RECT_MIX_COLOR_1 + RECT_MIX_COLOR_HIGHT + 62, RECT_EDIT_MIX_WIDTH, RECT_EDIT_MIX_HIGHT, hWnd, 0, hInst, 0);

	hEditMixPortion_1 = CreateWindow("EDIT", 0, WS_BORDER | WS_CHILD | WS_VISIBLE, X_RECT_MIX_COLOR_1,
		Y_RECT_MIX_COLOR_1 + RECT_MIX_COLOR_HIGHT + 110, RECT_EDIT_MIX_WIDTH + 30, RECT_EDIT_MIX_HIGHT, hWnd, 0, hInst, 0);
	hEditMixPortion_2 = CreateWindow("EDIT", 0, WS_BORDER | WS_CHILD | WS_VISIBLE, X_RECT_MIX_COLOR_2,
		Y_RECT_MIX_COLOR_1 + RECT_MIX_COLOR_HIGHT + 110, RECT_EDIT_MIX_WIDTH + 30, RECT_EDIT_MIX_HIGHT, hWnd, 0, hInst, 0);
	hEditMixPortion_3 = CreateWindow("EDIT", 0, WS_BORDER | WS_CHILD | WS_VISIBLE, X_RECT_MIX_COLOR_3,
		Y_RECT_MIX_COLOR_1 + RECT_MIX_COLOR_HIGHT + 110, RECT_EDIT_MIX_WIDTH + 30, RECT_EDIT_MIX_HIGHT, hWnd, 0, hInst, 0);

	hEditMixH_1 = CreateWindow("EDIT", 0, WS_BORDER | WS_CHILD | WS_VISIBLE, X_RECT_MIX_COLOR_1 + 20,
		Y_RECT_MIX_COLOR_1 + RECT_MIX_COLOR_HIGHT + 8, RECT_EDIT_MIX_WIDTH + 30, RECT_EDIT_MIX_HIGHT, hWnd, 0, hInst, 0);
	hEditMixH_2 = CreateWindow("EDIT", 0, WS_BORDER | WS_CHILD | WS_VISIBLE, X_RECT_MIX_COLOR_2 + 20,
		Y_RECT_MIX_COLOR_1 + RECT_MIX_COLOR_HIGHT + 8, RECT_EDIT_MIX_WIDTH + 30, RECT_EDIT_MIX_HIGHT, hWnd, 0, hInst, 0);
	hEditMixH_3 = CreateWindow("EDIT", 0, WS_BORDER | WS_CHILD | WS_VISIBLE, X_RECT_MIX_COLOR_3 + 20,
		Y_RECT_MIX_COLOR_1 + RECT_MIX_COLOR_HIGHT + 8, RECT_EDIT_MIX_WIDTH + 30, RECT_EDIT_MIX_HIGHT, hWnd, 0, hInst, 0);

	hEditMixS_1 = CreateWindow("EDIT", 0, WS_BORDER | WS_CHILD | WS_VISIBLE, X_RECT_MIX_COLOR_1 + 20,
		Y_RECT_MIX_COLOR_1 + RECT_MIX_COLOR_HIGHT + 35, RECT_EDIT_MIX_WIDTH + 30, RECT_EDIT_MIX_HIGHT, hWnd, 0, hInst, 0);
	hEditMixS_2 = CreateWindow("EDIT", 0, WS_BORDER | WS_CHILD | WS_VISIBLE, X_RECT_MIX_COLOR_2 + 20,
		Y_RECT_MIX_COLOR_1 + RECT_MIX_COLOR_HIGHT + 35, RECT_EDIT_MIX_WIDTH + 30, RECT_EDIT_MIX_HIGHT, hWnd, 0, hInst, 0);
	hEditMixS_3 = CreateWindow("EDIT", 0, WS_BORDER | WS_CHILD | WS_VISIBLE, X_RECT_MIX_COLOR_3 + 20,
		Y_RECT_MIX_COLOR_1 + RECT_MIX_COLOR_HIGHT + 35, RECT_EDIT_MIX_WIDTH + 30, RECT_EDIT_MIX_HIGHT, hWnd, 0, hInst, 0);

	hEditMixL_1 = CreateWindow("EDIT", 0, WS_BORDER | WS_CHILD | WS_VISIBLE, X_RECT_MIX_COLOR_1 + 20,
		Y_RECT_MIX_COLOR_1 + RECT_MIX_COLOR_HIGHT + 62, RECT_EDIT_MIX_WIDTH + 30, RECT_EDIT_MIX_HIGHT, hWnd, 0, hInst, 0);
	hEditMixL_2 = CreateWindow("EDIT", 0, WS_BORDER | WS_CHILD | WS_VISIBLE, X_RECT_MIX_COLOR_2 + 20,
		Y_RECT_MIX_COLOR_1 + RECT_MIX_COLOR_HIGHT + 62, RECT_EDIT_MIX_WIDTH + 30, RECT_EDIT_MIX_HIGHT, hWnd, 0, hInst, 0);
	hEditMixL_3 = CreateWindow("EDIT", 0, WS_BORDER | WS_CHILD | WS_VISIBLE, X_RECT_MIX_COLOR_3 + 20,
		Y_RECT_MIX_COLOR_1 + RECT_MIX_COLOR_HIGHT + 62, RECT_EDIT_MIX_WIDTH + 30, RECT_EDIT_MIX_HIGHT, hWnd, 0, hInst, 0);

	hEditHex_1 = CreateWindow("EDIT", 0, WS_BORDER | WS_CHILD | WS_VISIBLE, X_RECT_MIX_COLOR_1 + 40,
		Y_RECT_MIX_COLOR_1 + RECT_MIX_COLOR_HIGHT + 8, RECT_EDIT_MIX_WIDTH + 60, RECT_EDIT_MIX_HIGHT, hWnd, 0, hInst, 0);
	hEditHex_2 = CreateWindow("EDIT", 0, WS_BORDER | WS_CHILD | WS_VISIBLE, X_RECT_MIX_COLOR_2 + 40,
		Y_RECT_MIX_COLOR_1 + RECT_MIX_COLOR_HIGHT + 8, RECT_EDIT_MIX_WIDTH + 60, RECT_EDIT_MIX_HIGHT, hWnd, 0, hInst, 0);
	hEditHex_3 = CreateWindow("EDIT", 0, WS_BORDER | WS_CHILD | WS_VISIBLE, X_RECT_MIX_COLOR_3 + 40,
		Y_RECT_MIX_COLOR_1 + RECT_MIX_COLOR_HIGHT + 8, RECT_EDIT_MIX_WIDTH + 60, RECT_EDIT_MIX_HIGHT, hWnd, 0, hInst, 0);

	hEditHumbrol_1 = CreateWindow("EDIT", 0, WS_BORDER | WS_CHILD | WS_VISIBLE, X_RECT_MIX_COLOR_1 + 60,
		Y_RECT_MIX_COLOR_1 + RECT_MIX_COLOR_HIGHT + 8, RECT_EDIT_MIX_WIDTH + 40, RECT_EDIT_MIX_HIGHT, hWnd, 0, hInst, 0);
	hEditHumbrol_2 = CreateWindow("EDIT", 0, WS_BORDER | WS_CHILD | WS_VISIBLE, X_RECT_MIX_COLOR_2 + 60,
		Y_RECT_MIX_COLOR_1 + RECT_MIX_COLOR_HIGHT + 8, RECT_EDIT_MIX_WIDTH + 40, RECT_EDIT_MIX_HIGHT, hWnd, 0, hInst, 0);
	hEditHumbrol_3 = CreateWindow("EDIT", 0, WS_BORDER | WS_CHILD | WS_VISIBLE, X_RECT_MIX_COLOR_3 + 60,
		Y_RECT_MIX_COLOR_1 + RECT_MIX_COLOR_HIGHT + 8, RECT_EDIT_MIX_WIDTH + 40, RECT_EDIT_MIX_HIGHT, hWnd, 0, hInst, 0);

	SetEditLimitations();
}

VOID DrawColorButtonsMix(HWND hWnd) {
	hButtonMix = CreateWindow("button", "Mix Colors",
		WS_CHILD | WS_VISIBLE |
		BS_PUSHBUTTON,
		X_RECT_MIX_COLOR_1, Y_RECT_MIX_COLOR_1 + RECT_MIX_COLOR_HIGHT + 140, RECT_MIX_COLOR_WIDTH, COPY_VALUE_BUTTON_HIGHT + 5,
		hWnd, (HMENU)IDM_MIX_BUTTON, hInst, NULL);
}


VOID OutputMixColorValuesHumbrol() {
	SetWindowText(hEditHumbrol_1, (LPCSTR)first_mix_color.getHumbrolCodeStr());
	SetWindowText(hEditHumbrol_2, (LPCSTR)second_mix_color.getHumbrolCodeStr());
	SetWindowText(hEditHumbrol_3, (LPCSTR)third_mix_color.getHumbrolCodeStr());
}

VOID OutputMixColorsValuesRGB() {
	SetWindowText(hEditMixR_1, (LPCSTR)first_mix_color.getRedValueStr());
	SetWindowText(hEditMixG_1, (LPCSTR)first_mix_color.getGreenValueStr());
	SetWindowText(hEditMixB_1, (LPCSTR)first_mix_color.getBlueValueStr());
	SetWindowText(hEditMixR_2, (LPCSTR)second_mix_color.getRedValueStr());
	SetWindowText(hEditMixG_2, (LPCSTR)second_mix_color.getGreenValueStr());
	SetWindowText(hEditMixB_2, (LPCSTR)second_mix_color.getBlueValueStr());
	SetWindowText(hEditMixR_3, (LPCSTR)third_mix_color.getRedValueStr());
	SetWindowText(hEditMixG_3, (LPCSTR)third_mix_color.getGreenValueStr());
	SetWindowText(hEditMixB_3, (LPCSTR)third_mix_color.getBlueValueStr());
}

VOID OutputMixColorsValuesHTML() {
	SetWindowText(hEditHex_1, (LPCSTR)first_mix_color.getStrHtmlColorCode());
	SetWindowText(hEditHex_2, (LPCSTR)second_mix_color.getStrHtmlColorCode());
	SetWindowText(hEditHex_3, (LPCSTR)third_mix_color.getStrHtmlColorCode());
}

VOID OutputMixColorValuesHSL() {
	SetWindowText(hEditMixH_1, (LPCSTR)first_mix_color.getHueValueStr());
	SetWindowText(hEditMixH_2, (LPCSTR)second_mix_color.getHueValueStr());
	SetWindowText(hEditMixH_3, (LPCSTR)third_mix_color.getHueValueStr());
	SetWindowText(hEditMixS_1, (LPCSTR)first_mix_color.getSaturationValueStr());
	SetWindowText(hEditMixS_2, (LPCSTR)second_mix_color.getSaturationValueStr());
	SetWindowText(hEditMixS_3, (LPCSTR)third_mix_color.getSaturationValueStr());
	SetWindowText(hEditMixL_1, (LPCSTR)first_mix_color.getLightnessValueStr());
	SetWindowText(hEditMixL_2, (LPCSTR)second_mix_color.getLightnessValueStr());
	SetWindowText(hEditMixL_3, (LPCSTR)third_mix_color.getLightnessValueStr());
}

VOID MixColors(int portion_1, int portion_2, int portion_3) {
	if (portion_1 + portion_2  + portion_3 == 100) {

		int resRed, resGreen, resBlue;
		resRed = (portion_1 * first_mix_color.getRedColor() / 100) + (portion_2 * second_mix_color.getRedColor() / 100) + (portion_3 * third_mix_color.getRedColor() / 100);
		resGreen = (portion_1 * first_mix_color.getGreenColor() / 100) + (portion_2 * second_mix_color.getGreenColor() / 100) + (portion_3 * third_mix_color.getGreenColor() / 100);
		resBlue = (portion_1 * first_mix_color.getBlueColor() / 100) + (portion_2 * second_mix_color.getBlueColor() / 100) + (portion_3 * third_mix_color.getBlueColor() / 100);

		res_mix_color.setRedColor(resRed);
		res_mix_color.setGreenColor(resGreen);
		res_mix_color.setBlueColor(resBlue);
	}
}

VOID OutputStartColorPortions(char *portion_1, char *portion_2, char *portion_3) {
	SetWindowText(hEditMixPortion_1, (LPCSTR)portion_1);
	SetWindowText(hEditMixPortion_2, (LPCSTR)portion_2);
	SetWindowText(hEditMixPortion_3, (LPCSTR)portion_3);
}

int ConvertStrToInt(char *str_to_convert) {
	int v_out;
	try {
		v_out = std::stoi(str_to_convert);
		return v_out;
	}
	catch (std::exception)
	{
		return -1;
	}
}

float ConvertStrToFloat(char *str_to_convert) {
	try {
		float vOut = (float)strtod(str_to_convert, NULL);
		return vOut;
	}
	catch (std::exception) {
		return NULL;
	}

}

BOOL UpdateColors() {
	char red_first_color[COLOR_CODE_LENGTH_FIELD];
	char green_first_color[COLOR_CODE_LENGTH_FIELD];
	char blue_first_color[COLOR_CODE_LENGTH_FIELD];
	char red_second_color[COLOR_CODE_LENGTH_FIELD];
	char green_second_color[COLOR_CODE_LENGTH_FIELD];
	char blue_second_color[COLOR_CODE_LENGTH_FIELD];
	char red_third_color[COLOR_CODE_LENGTH_FIELD];
	char green_third_color[COLOR_CODE_LENGTH_FIELD];
	char blue_third_color[COLOR_CODE_LENGTH_FIELD];

	char hue_first_color[COLOR_CODE_LENGTH_FIELD];
	char hue_second_color[COLOR_CODE_LENGTH_FIELD];
	char hue_third_color[COLOR_CODE_LENGTH_FIELD];
	char saturation_first_color[COLOR_CODE_LENGTH_FIELD];
	char saturation_second_color[COLOR_CODE_LENGTH_FIELD];
	char saturation_third_color[COLOR_CODE_LENGTH_FIELD];
	char lightness_first_color[COLOR_CODE_LENGTH_FIELD];
	char lightness_second_color[COLOR_CODE_LENGTH_FIELD];
	char lightness_third_color[COLOR_CODE_LENGTH_FIELD];

	char hex_first_color[COLOR_CODE_LENGTH_FIELD];
	char hex_second_color[COLOR_CODE_LENGTH_FIELD];
	char hex_third_color[COLOR_CODE_LENGTH_FIELD];

	char humbrol_first_color[COLOR_CODE_LENGTH_FIELD];
	char humbrol_second_color[COLOR_CODE_LENGTH_FIELD];
	char humbrol_third_color[COLOR_CODE_LENGTH_FIELD];

	int red_1, red_2, green_1, green_2, blue_1, blue_2, red_3, green_3, blue_3, humbrol_1, humbrol_2, humbrol_3;
	float hue_1, hue_2, hue_3, saturation_1, saturation_2, saturation_3, lightness_1, lightness_2, lightness_3;

	switch (colorMixColorsFormat) {
	case IDM_RGB:
		GetWindowText(hEditMixR_1, red_first_color, COLOR_CODE_LENGTH_FIELD);
		GetWindowText(hEditMixR_2, red_second_color, COLOR_CODE_LENGTH_FIELD);
		GetWindowText(hEditMixR_3, red_third_color, COLOR_CODE_LENGTH_FIELD);
		GetWindowText(hEditMixG_1, green_first_color, COLOR_CODE_LENGTH_FIELD);
		GetWindowText(hEditMixG_2, green_second_color, COLOR_CODE_LENGTH_FIELD);
		GetWindowText(hEditMixG_3, green_third_color, COLOR_CODE_LENGTH_FIELD);
		GetWindowText(hEditMixB_1, blue_first_color, COLOR_CODE_LENGTH_FIELD);
		GetWindowText(hEditMixB_2, blue_second_color, COLOR_CODE_LENGTH_FIELD);
		GetWindowText(hEditMixB_3, blue_third_color, COLOR_CODE_LENGTH_FIELD);

		red_1 = ConvertStrToInt(red_first_color);
		if ((red_1 > 255) || (red_1 < 0))
			return FALSE;
		red_2 = ConvertStrToInt(red_second_color);
		if ((red_2 < 0) || (red_2 > 255))
			return FALSE;
		red_3 = ConvertStrToInt(red_third_color);
		if ((red_3 < 0) || (red_3 > 255))
			return FALSE;
		green_1 = ConvertStrToInt(green_first_color);
		if ((green_1 < 0) || (green_1 > 255))
			return FALSE;
		green_2 = ConvertStrToInt(green_second_color);
		if ((green_2 < 0) || (green_2 > 255))
			return FALSE;
		green_3 = ConvertStrToInt(green_third_color);
		if ((green_3 < 0) || (green_3 > 255))
			return FALSE;
		blue_1 = ConvertStrToInt(blue_first_color);
		if ((blue_1 < 0) || (blue_1 > 255))
			return FALSE;
		blue_2 = ConvertStrToInt(blue_second_color);
		if ((blue_2 < 0) || (blue_2 > 255))
			return FALSE;
		blue_3 = ConvertStrToInt(blue_third_color);
		if ((blue_3 < 0) || (blue_3 > 255))
			return FALSE;

		first_mix_color.setRedColor(red_1);
		first_mix_color.setGreenColor(green_1);
		first_mix_color.setBlueColor(blue_1);

		second_mix_color.setRedColor(red_2);
		second_mix_color.setGreenColor(green_2);
		second_mix_color.setBlueColor(blue_2);

		third_mix_color.setRedColor(red_3);
		third_mix_color.setGreenColor(green_3);
		third_mix_color.setBlueColor(blue_3);

		return TRUE;
		break;
	case IDM_HTML:
		GetWindowText(hEditHex_1, hex_first_color, COLOR_CODE_LENGTH_FIELD);
		GetWindowText(hEditHex_2, hex_second_color, COLOR_CODE_LENGTH_FIELD);
		GetWindowText(hEditHex_3, hex_third_color, COLOR_CODE_LENGTH_FIELD);

		if (strlen(hex_first_color) == 7 && strlen(hex_second_color) == 7 && strlen(hex_third_color))
		{
			first_mix_color.setHtmlColorCode(hex_first_color);
			second_mix_color.setHtmlColorCode(hex_second_color);
			third_mix_color.setHtmlColorCode(hex_third_color);
		}
		else
		{
			return FALSE;
		}
		return TRUE;
		break;
	case IDM_HSL:
		GetWindowText(hEditMixH_1, hue_first_color, COLOR_CODE_LENGTH_FIELD);
		GetWindowText(hEditMixH_2, hue_second_color, COLOR_CODE_LENGTH_FIELD);
		GetWindowText(hEditMixH_3, hue_third_color, COLOR_CODE_LENGTH_FIELD);
		GetWindowText(hEditMixS_1, saturation_first_color, COLOR_CODE_LENGTH_FIELD);
		GetWindowText(hEditMixS_2, saturation_second_color, COLOR_CODE_LENGTH_FIELD);
		GetWindowText(hEditMixS_3, saturation_third_color, COLOR_CODE_LENGTH_FIELD);
		GetWindowText(hEditMixL_1, lightness_first_color, COLOR_CODE_LENGTH_FIELD);
		GetWindowText(hEditMixL_2, lightness_second_color, COLOR_CODE_LENGTH_FIELD);
		GetWindowText(hEditMixL_3, lightness_third_color, COLOR_CODE_LENGTH_FIELD);

		hue_1 = ConvertStrToFloat(hue_first_color);
		if (hue_1 < 0)
			return FALSE;
		hue_2 = ConvertStrToFloat(hue_second_color);
		if (hue_2 < 0)
			return FALSE;
		hue_3 = ConvertStrToFloat(hue_third_color);
		if (hue_3 < 0)
			return FALSE;
		saturation_1 = ConvertStrToFloat(saturation_first_color);
		if (saturation_1 < 0)
			return FALSE;
		saturation_2 = ConvertStrToFloat(saturation_second_color);
		if (saturation_2 < 0)
			return FALSE;
		saturation_3 = ConvertStrToFloat(saturation_third_color);
		if (saturation_3 < 0)
			return FALSE;
		lightness_1 = ConvertStrToFloat(lightness_first_color);
		if (lightness_1 < 0)
			return FALSE;
		lightness_2 = ConvertStrToFloat(lightness_second_color);
		if (lightness_2 < 0)
			return FALSE;
		lightness_3 = ConvertStrToFloat(lightness_third_color);
		if (lightness_3 < 0)
			return FALSE;

		first_mix_color.setHue(hue_1);
		first_mix_color.setLightness(lightness_1);
		first_mix_color.setSaturation(saturation_1);

		second_mix_color.setHue(hue_2);
		second_mix_color.setLightness(lightness_2);
		second_mix_color.setSaturation(saturation_2);

		third_mix_color.setHue(hue_3);
		third_mix_color.setLightness(lightness_3);
		third_mix_color.setSaturation(saturation_3);

		return TRUE;
		break;
	case IDM_HUMBROL:
		GetWindowText(hEditHumbrol_1, humbrol_first_color, COLOR_CODE_LENGTH_FIELD);
		GetWindowText(hEditHumbrol_2, humbrol_second_color, COLOR_CODE_LENGTH_FIELD);
		GetWindowText(hEditHumbrol_3, humbrol_third_color, COLOR_CODE_LENGTH_FIELD);

		humbrol_1 = ConvertStrToInt(humbrol_first_color);
		if (!first_mix_color.isHumbrolCode(humbrol_1))
			return FALSE;
		humbrol_2 = ConvertStrToInt(humbrol_second_color);
		if (!second_mix_color.isHumbrolCode(humbrol_2))
			return FALSE;
		humbrol_3 = ConvertStrToInt(humbrol_third_color);
		if (!third_mix_color.isHumbrolCode(humbrol_3))
			return FALSE;
		first_mix_color.setHumbrol(humbrol_1);
		second_mix_color.setHumbrol(humbrol_2);
		third_mix_color.setHumbrol(humbrol_3);

		return TRUE;
		break;
	}
	return TRUE;
}

VOID CreateRadioButtons(HWND hWnd) {
	/*
	hRadioButtonRGB = CreateWindow(
		"BUTTON",
		NULL,
		WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON,
		15,
		270,
		15,
		15,
		hWnd,
		NULL,
		hInst,
		NULL
	);
	*/
}
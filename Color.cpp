#include <windows.h>
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <vector>
#include <string>
#include "Color.h"

#define color_difference 15

void initialize_RAL_database();
void initialize_humbrol_database();

struct status { 
	int redColor;
	int greenColor;
	int blueColor;

	status(int red, int green, int blue) {
		redColor = red;
		greenColor = green;
		blueColor = blue;
	}
};

std::map <int, status> database; //user database
std::map <int, status> humbrol_database;

Color::Color(int redColor, int greenColor, int blueColor) {
	_redColor = redColor % 255;
	_greenColor = greenColor % 255;
	_blueColor = blueColor % 255;

	_lightness = _getLightness();
	_saturation = _getSaturation();
	_hue = _getHue();

	initialize_RAL_database();
	initialize_humbrol_database();
}


Color::Color(double hue, double saturation, double lightness) {
	_hue = fmod(hue, 360);
	_saturation = fmod(saturation, 360);
	_lightness = fmod(lightness, 360);

	convertFromHslToRgb();
	initialize_RAL_database();
	initialize_humbrol_database();
}

void Color::convertToXYZ() {
	//sR, sG and sB (Standard RGB) input range = 0 ÷ 255
	//X, Y and Z output refer to a D65/2° standard illuminant.
	double var_R = (_redColor / 255.0);
	double var_G = (_greenColor / 255.0);
	double var_B = (_blueColor / 255.0);

	if (var_R > 0.04045)
		var_R = pow(((var_R + 0.055) / 1.055),2.4);
	else
		var_R = var_R / 12.92;
	
	if (var_G > 0.04045)
		var_G = pow(((var_G + 0.055) / 1.055), 2.4);
	else
		var_G = var_G / 12.92;
	
	if (var_B > 0.04045)
		var_B = pow(((var_B + 0.055) / 1.055), 2.4);
	else
		var_B = var_B / 12.92;

	var_R = var_R * 100;
	var_G = var_G * 100;
	var_B = var_B * 100;

	double X = var_R * 0.4124 + var_G * 0.3576 + var_B * 0.1805;
	double Y = var_R * 0.2126 + var_G * 0.7152 + var_B * 0.0722;
	double Z = var_R * 0.0193 + var_G * 0.1192 + var_B * 0.9505;

	_x_value = X;
	_y_value = Y;
	_z_value = Z;
}

void Color::convertXYZtoLab() {

	float ref_X = 95.047;
	float ref_Y = 100.0;
	float ref_Z = 108.883;
	
	float var_X = (_x_value / ref_X); //ref_X = 95.047
	float var_Y = (_y_value / ref_Y); //ref_Y = 100.0
	float var_Z = (_z_value / ref_Z); //ref_Z = 108.883

	if (var_X > 0.008856)
		var_X = powf(var_X, (1.0f / 3));
	else
		var_X = (7.787 * var_X) + (16.0f / 116);

	if (var_Y > 0.008856)
		var_Y = powf(var_Y, (1.0f / 3));
	else
		var_Y = (7.787 * var_Y) + (16.0f / 116);

	if (var_Z > 0.008856)
		var_Z = powf(var_Z, (1.0f / 3));
	else
		var_Z = (7.787 * var_Z) + (16.0f / 116);

	_CIE_L = (116 * var_Y) - 16;
	_CIE_a = 500 * (var_X - var_Y);
	_CIE_b = 200 * (var_Y - var_Z);
}

void Color::convertLabtoXYZ() {
	float ref_X = 95.047;
	float ref_Y = 100.0;
	float ref_Z = 108.883;
	
	float var_Y = (_CIE_L + 16) / 116;
	float var_X = (_CIE_a / 500) + var_Y;
	float var_Z = var_Y - (_CIE_b / 200);

	if (powf(var_Y, 3.f) > 0.008856)
		var_Y = powf(var_Y, 3.f);
	else
		var_Y = (var_Y - (16 / 116)) / 7.787;

	if (powf(var_X, 3.f) > 0.008856)
		var_X = powf(var_X, 3.f);
	else
		var_X = (var_X - (16 / 116)) / 7.787;

	if (powf(var_Z, 3.f) > 0.008856)
		var_Z = powf(var_Z, 3.f);
	else
		var_Z = (var_Z - (16 / 116)) / 7.787;

	_x_value = ref_X * var_X;     //ref_X =  95.047     Observer= 2°, Illuminant= D65
	_y_value = ref_Y * var_Y;     //ref_Y = 100.000
	_z_value = ref_Z * var_Z;     //ref_Z = 108.883
}

void Color::convertXYZtoRGB(float inX, float inY, float inZ, int * outR, int * outG, int * outB) {
	float var_X = inX / 100;
	float var_Y = inY / 100;
	float var_Z = inZ / 100;

	float var_R = var_X * 3.2406 + (var_Y * -1.5372) + var_Z * (-0.4986);
	float var_G = var_X * (-0.9689) + var_Y * 1.8758 + var_Z * 0.0415;
	float var_B = var_X * 0.0557 + var_Y * (-0.2040) + var_Z * 1.0570;

	if (var_R > 0.0031308)
		var_R = 1.055 * powf(var_R, (1.0f / 2.4)) - 0.055;
	else
		var_R = 12.92 * var_R;

	if (var_G > 0.0031308)
		var_G = 1.055 * powf(var_G, (1.0f / 2.4)) - 0.055;
	else
		var_G = 12.92 * var_G;

	if (var_B > 0.0031308)
		var_B = 1.055 * powf(var_B, (1.0f / 2.4)) - 0.055;
	else
		var_B = 12.92 * var_B;

	*outR = (int)(var_R * 255);
	*outG = (int)(var_G * 255);
	*outB = (int)(var_B * 255);
}

float Color::Lab_color_difference(float inL1, float ina1, float  inb1, float inL2, float ina2, float  inb2) {
	return(sqrt(powf(inL1 - inL2, 2.f) + powf(ina1 - ina2, 2.f) + powf(inb1 - inb2, 2.f)));
}

float Color::get_L() {
	return _CIE_L;
}

float Color::get_a() {
	return _CIE_a;
}

float Color::get_b() {
	return _CIE_b;
}

BOOL Color::isHumbrolCode(int humbrol_temp_code) {
	for (auto i = humbrol_database.begin(); i != humbrol_database.end(); i++) {
		if ((*i).first == humbrol_temp_code) {
			return TRUE;
		}
	}
	return FALSE;
}

void Color::setHumbrol(int humbrol) {
	for (auto i = humbrol_database.begin(); i != humbrol_database.end(); i++) {
		if ((*i).first == humbrol) {
			_humbrol = humbrol;
			_redColor = (*i).second.redColor;
			_greenColor = (*i).second.greenColor;
			_blueColor = (*i).second.blueColor;
			break;
		}
	}
}

void Color::convertFromRgbToHumbrol() {
	for (auto i = humbrol_database.begin(); i != humbrol_database.end(); ++i) {
		if ((_redColor == (*i).second.redColor) && (_greenColor == (*i).second.greenColor) && (_blueColor == (*i).second.blueColor)) {
			_humbrol = (*i).first;

			setRedColor((*i).second.redColor);
			setGreenColor((*i).second.greenColor);
			setBlueColor((*i).second.blueColor);

			break;
		}
		if ((_redColor + color_difference >= (*i).second.redColor) && (_redColor - color_difference <= (*i).second.redColor) &&
			(_greenColor + color_difference >= (*i).second.greenColor) && (_greenColor - color_difference <= (*i).second.greenColor) &&
			(_blueColor + color_difference >= (*i).second.blueColor) && (_blueColor - color_difference <= (*i).second.blueColor)) {
			_humbrol = (*i).first;

			setRedColor((*i).second.redColor);
			setGreenColor((*i).second.greenColor);
			setBlueColor((*i).second.blueColor);

			break;
		}
	}
}

void Color::convertFromRgbToRal() {
	for (auto i = database.begin(); i != database.end(); ++i) {
		if ((_redColor == (*i).second.redColor) && (_greenColor == (*i).second.greenColor) && (_blueColor == (*i).second.blueColor)) {
			_ral = (*i).first;

			setRedColor((*i).second.redColor);
			setGreenColor((*i).second.greenColor);
			setBlueColor((*i).second.blueColor);

			break;
		}
		if ((_redColor + color_difference >= (*i).second.redColor) && (_redColor - color_difference <= (*i).second.redColor) &&
			(_greenColor + color_difference >= (*i).second.greenColor) && (_greenColor - color_difference <= (*i).second.greenColor) &&
			(_blueColor + color_difference >= (*i).second.blueColor) && (_blueColor - color_difference <= (*i).second.blueColor)) {
			_ral = (*i).first;

			setRedColor((*i).second.redColor);
			setGreenColor((*i).second.greenColor);
			setBlueColor((*i).second.blueColor);

			break;
		}
	}
}

void Color::convertFromHslToRgb() {
	double c = (1 - fabs(2 * _lightness - 1)) * _saturation;
	double x = c * (1 - fmod((fabs(_hue / 60)) - 1, 2));
	double m = _lightness - c / 2;

	double r_new, g_new, b_new;

	if ((_hue >= 0) & (_hue < 60)){
		r_new = c;
		g_new = x;
		b_new = 0;
	}

	if ((_hue >= 60) & (_hue < 120)) {
		r_new = x;
		g_new = c;
		b_new = 0;
	}

	if ((_hue >= 120) & (_hue < 180)) {
		r_new = 0;
		g_new = c;
		b_new = x;
	}

	if ((_hue >= 180) & (_hue < 240)) {
		r_new = 0;
		g_new = x;
		b_new = c;
	}

	if ((_hue >= 240) & (_hue < 300)) {
		r_new = x;
		g_new = 0;
		b_new = c;
	}

	if ((_hue >= 300) & (_hue <= 360)) {
		r_new = c;
		g_new = 0;
		b_new = x;
	}

	_redColor = (r_new + m) * 255;
	_greenColor = (g_new + m) * 255;
	_blueColor = (b_new + m) * 255;
}

void Color::setRedColor(int redColor) {
	_redColor = redColor % 255;

	_lightness = _getLightness();
	_saturation = _getSaturation();
	_hue = _getHue();
}

void Color::setGreenColor(int greenColor) {
	_greenColor = greenColor % 255;

	_lightness = _getLightness();
	_saturation = _getSaturation();
	_hue = _getHue();
}

void Color::setBlueColor(int blueColor) {
	_blueColor = blueColor % 255;

	_lightness = _getLightness();
	_saturation = _getSaturation();
	_hue = _getHue();
}

void Color::setHue(double hue) {
	_hue = fmod(hue, 360);
	convertFromHslToRgb();
}

void Color::setLightness(double lightness) {
	_lightness = fmod(lightness, 360);
	convertFromHslToRgb();
}

void Color::setSaturation(double saturation) {
	_saturation = fmod(saturation, 360);
	convertFromHslToRgb();
}

int Color::getRandomRedColor() {
	randomRedColor = (rand() % 255 + _redColor + 1) / 2;
	return randomRedColor;
}

int Color::getRandomGreenColor() {
	randomGreenColor = (rand() % 255 + _greenColor) / 2;
	return randomGreenColor;
}

int Color::getRandomBlueColor() {
	randomBlueColor = (rand() % 255 + _blueColor) / 2;
	return randomBlueColor;
}

int Color::getRedColor() {
	return _redColor;
}

int Color::getGreenColor() {
	return _greenColor;
}

int Color::getBlueColor() {
	return _blueColor;
}

double Color::getHue() {
	return _hue;
}

double Color::getLightness() {
	return _lightness;
}

double Color::getSaturation() {
	return _saturation;
}

char *Color::getRedValueStr() {
	char *result = new char[10];
	char temp_str_red[10];
	char r_color[10];

	int radix = 10;
	_itoa_s(_redColor, r_color, radix);
	strncpy_s(result, strlen(r_color) + 1, r_color, strlen(r_color) + 1);
	return result;
}

char *Color::getGreenValueStr() {
	char *result = new char[10];
	char temp_str_red[10];
	char g_color[10];

	int radix = 10;
	_itoa_s(_greenColor, g_color, radix);
	strncpy_s(result, strlen(g_color) + 1, g_color, strlen(g_color) + 1);
	return result;
}

char *Color::getBlueValueStr() {
	char *result = new char[10];
	char temp_str_red[10];
	char b_color[10];

	int radix = 10;
	_itoa_s(_blueColor, b_color, radix);
	strncpy_s(result, strlen(b_color) + 1, b_color, strlen(b_color) + 1);
	return result;
}

char *Color::getHumbrolCodeStr() {
	convertFromRgbToHumbrol();
	char *result = new char[15];
	char humbrol_code[15];
	char temp_res[15] = "";

	int radix = 10;
	_itoa_s(_humbrol, humbrol_code, radix);
	strcat_s(temp_res, humbrol_code);

	strncpy_s(result, strlen(temp_res) + 1, temp_res, strlen(temp_res) + 1);
	return result;
}

char *Color::getHumbrolStr() {
	convertFromRgbToHumbrol();
	char *result = new char[15];
	char humbrol_code[15];
	char temp_res[15];

	strcpy_s(temp_res, "Humbrol: ");
	int radix = 10;
	_itoa_s(_humbrol, humbrol_code, radix);
	strcat_s(temp_res, humbrol_code);

	strncpy_s(result, strlen(temp_res) + 1, temp_res, strlen(temp_res) + 1);
	return result;
}

char *Color::getRalCodeStr() {
	convertFromRgbToRal();

	char *result = new char[15];
	char ral_code[15];
	char temp_res[15];

	strcpy_s(temp_res, "RAL: ");
	int radix = 10;
	_itoa_s(_ral, ral_code, radix);
	strcat_s(temp_res, ral_code);

	strncpy_s(result, strlen(temp_res) + 1, temp_res, strlen(temp_res) + 1);
	return result;
}

char *Color::getRedColorStr() {
	char *result = new char[10];
	char r_color[10];
	char temp_str_red[10];

	strcpy_s(temp_str_red, "R: ");
	int radix = 10;
	_itoa_s(_redColor, r_color, radix);
	strcat_s(temp_str_red, r_color);

	strncpy_s(result, strlen(temp_str_red) + 1, temp_str_red, strlen(temp_str_red) + 1);
	return result;
}

char *Color::getGreenColorStr() {
	char *result = new char[10];
	char g_color[10];
	char temp_str_green[10];

	strcpy_s(temp_str_green, "G: ");
	int radix = 10;
	_itoa_s(_greenColor, g_color, radix);
	strcat_s(temp_str_green, g_color);

	strncpy_s(result, strlen(temp_str_green) + 1, temp_str_green, strlen(temp_str_green) + 1);
	return result;
}

char *Color::getBlueColorStr() {
	char *result = new char[10];
	char b_color[10];
	char temp_str_blue[10];

	strcpy_s(temp_str_blue, "B: ");
	int radix = 10;
	_itoa_s(_blueColor, b_color, radix);
	strcat_s(temp_str_blue, b_color);

	strncpy_s(result, strlen(temp_str_blue) + 1, temp_str_blue, strlen(temp_str_blue) + 1);
	return result;
}

char *Color::getStrRgbColorCode() {
	char *result = new char[30];
	char rgb_color_code[30];
	char temp_str_red[10];
	char temp_str_green[10];
	char temp_str_blue[10];
	
	strcpy_s(rgb_color_code, "(R: ");

	int radix = 10;
	_itoa_s(_redColor % 255, temp_str_red, radix);

	strcat_s(rgb_color_code, temp_str_red);
	strcat_s(rgb_color_code, ", ");

	_itoa_s(_greenColor % 255, temp_str_green, radix);

	strcat_s(rgb_color_code, "G: ");
	strcat_s(rgb_color_code, temp_str_green);
	strcat_s(rgb_color_code, ", ");

	_itoa_s(_blueColor % 255, temp_str_blue, radix);

	strcat_s(rgb_color_code, "B: ");
	strcat_s(rgb_color_code, temp_str_blue);
	strcat_s(rgb_color_code, ")");

	strncpy_s(result, strlen(rgb_color_code) + 1, rgb_color_code, strlen(rgb_color_code) + 1);

	return result;
}

int Color::ConvertStrToInt(char *str_to_convert) {
	int v_out;
	try {
		v_out = std::stoi(str_to_convert, 0, 16);
		return v_out;
	}
	catch (std::exception const & e)
	{
		return -1;
	}
}

void Color::setHtmlColorCode(char *html_color_code) {
	int red, green, blue;
	char str[3];
	str[2] = '\0';
	str[0] = html_color_code[1];
	str[1] = html_color_code[2];
	red = ConvertStrToInt(str);
	if (red != -1)
		_redColor = red;

	str[0] = html_color_code[3];
	str[1] = html_color_code[4];
	green = ConvertStrToInt(str);
	if (green != -1)
		_greenColor = green;

	str[0] = html_color_code[5];
	str[1] = html_color_code[6];
	blue = ConvertStrToInt(str);
	if (blue != -1)
		_blueColor = blue;
}

char *Color::getStrHtmlColorCode() {
	char *result = new char[8];
	char html_color_code[8];
	char temp_str_red[10];
	char temp_str_green[10];
	char temp_str_blue[10];

	strcpy_s(html_color_code, "#");
	int radix = 16;

	_itoa_s(_redColor, temp_str_red, radix);
	_itoa_s(_greenColor, temp_str_green, radix);
	_itoa_s(_blueColor, temp_str_blue, radix);

	if (strlen(temp_str_red) == 1) {
		strcat_s(html_color_code, "0");
	}
	strcat_s(html_color_code, temp_str_red);
	if (strlen(temp_str_green) == 1) {
		strcat_s(html_color_code, "0");
	}
	strcat_s(html_color_code, temp_str_green);
	if (strlen(temp_str_blue) == 1) {
		strcat_s(html_color_code, "0");
	}
	strcat_s(html_color_code, temp_str_blue);

	strncpy_s(result, strlen(html_color_code) + 1, html_color_code, strlen(html_color_code) + 1);
	return result;
}

double find_max_between_three(double first, double second, double third) {
	if ((first >= second) && (first >= third))
		return first;
	if ((second >= first) && (second >= third))
		return second;
	if ((third >= first) && (third >= second))
		return third;
	return 0;
}

double find_min_between_three(double first, double second, double third) {
	if ((first <= second) && (second <= third))
		return first;
	if ((second <= first) && (first <= third))
		return second;
	if ((third <= first) && (third <= second))
		return third;
	return 0;
}

double Color::_getLightness() {
	double new_r = _redColor / 255.0;
	double new_b = _blueColor / 255.0;
	double new_g = _greenColor / 255.0;

	new_r = round(new_r * 1000.0) / 1000.0;
	new_g = round(new_g * 1000.0) / 1000.0;
	new_b = round(new_b * 1000.0) / 1000.0;

	double c_max, c_min, delta, lightness;
	c_max = find_max_between_three(new_r, new_b, new_g);
	c_min = find_min_between_three(new_r, new_b, new_g);
	delta = c_max - c_min;

	c_max = round(c_max * 1000.0) / 1000.0;
	c_min = round(c_min * 1000.0) / 1000.0;

	lightness = (c_max + c_min) / 2;
	lightness = round(lightness * 100.0) / 100.0;
	return lightness;
}

double Color::_getSaturation() {
	double new_r = _redColor / 255.0;
	double new_b = _blueColor / 255.0;
	double new_g = _greenColor / 255.0;

	new_r = round(new_r * 1000.0) / 1000.0;
	new_g = round(new_g * 1000.0) / 1000.0;
	new_b = round(new_b * 1000.0) / 1000.0;

	double c_max, c_min, delta, lightness, saturation;
	c_max = find_max_between_three(new_r, new_b, new_g);
	c_min = find_min_between_three(new_r, new_b, new_g);
	delta = c_max - c_min;

	c_max = round(c_max * 1000.0) / 1000.0;
	c_min = round(c_min * 1000.0) / 1000.0;

	lightness = (c_max + c_min) / 2;

	// Calculate saturation
	if (delta == 0)
		saturation = 0;
	else
		saturation = delta / (1 - abs((int)(2 * lightness - 1)));

	saturation = round(saturation * 100.0) / 100.0;
	return saturation;
}

double Color::_getHue() {
	double new_r = _redColor / 255.0;
	double new_b = _blueColor / 255.0;
	double new_g = _greenColor / 255.0;

	new_r = round(new_r * 1000.0) / 1000.0;
	new_g = round(new_g * 1000.0) / 1000.0;
	new_b = round(new_b * 1000.0) / 1000.0;

	double c_max, c_min, delta, lightness, saturation, hue;
	c_max = find_max_between_three(new_r, new_b, new_g);
	c_min = find_min_between_three(new_r, new_b, new_g);
	delta = c_max - c_min;

	c_max = round(c_max * 1000.0) / 1000.0;
	c_min = round(c_min * 1000.0) / 1000.0;

	lightness = (c_max + c_min) / 2;

	// Calculate saturation
	if (delta == 0)
		saturation = 0;
	else
		saturation = delta / (1 - abs((int)(2 * lightness - 1)));

	// Calculate hue
	if (delta == 0)
		hue = 0;
	else {
		if (c_max == new_r) {
			hue = 10 * ((new_g - new_b) / delta);
			if (hue < 0)
				hue += 360;
		}
		else {
			if (c_max == new_g)
				hue = 60 * ((new_b - new_r) / delta + 2);
			else {
				if (c_max == new_b)
					hue = 60 * ((new_r - new_g) / delta + 4);
			}
		}
	}

	lightness = round(lightness * 100.0) / 100.0;
	saturation = round(saturation * 100.0) / 100.0;
	hue = round(hue * 100.0) / 100.0;
	return hue;
}

char *Color::getHslColorCode() {
	_hue = _getHue();
	_saturation = _getSaturation();
	_lightness = _getLightness();
	return GetHslStringFormat();
}

char *Color::getHueValueStr() {
	char hsl_color_code[50];
	char *result = new char[50];
	char temp_str_hue[10];
	strcpy_s(hsl_color_code, "");
	_gcvt_s(temp_str_hue, 10, _hue, 4);
	strcat_s(hsl_color_code, temp_str_hue);
	strncpy_s(result, strlen(hsl_color_code) + 1, hsl_color_code, strlen(hsl_color_code) + 1);
	return result;
}

char *Color::getSaturationValueStr() {
	char hsl_color_code[50];
	char *result = new char[50];
	char temp_str_saturation[10];
	strcpy_s(hsl_color_code, "");
	_gcvt_s(temp_str_saturation, 10, _saturation, 4);
	strcat_s(hsl_color_code, temp_str_saturation);
	strncpy_s(result, strlen(hsl_color_code) + 1, hsl_color_code, strlen(hsl_color_code) + 1);
	return result;
}

char *Color::getLightnessValueStr() {
	char hsl_color_code[50];
	char *result = new char[50];
	char temp_str_lightness[10];
	strcpy_s(hsl_color_code, "");
	_gcvt_s(temp_str_lightness, 10, _lightness, 4);
	strcat_s(hsl_color_code, temp_str_lightness);
	strncpy_s(result, strlen(hsl_color_code) + 1, hsl_color_code, strlen(hsl_color_code) + 1);
	return result;
}

char *Color::GetHslStringFormat() {
	char hsl_color_code[50];
	char *result = new char[50];
	
	char temp_str_hue[10];
	char temp_str_saturation[10];
	char temp_str_lightness[10];

	strcpy_s(hsl_color_code, "H: ");

	_gcvt_s(temp_str_hue, 10, _hue, 4);
	strcat_s(hsl_color_code, temp_str_hue);
	strcat_s(hsl_color_code, "°; S: ");

	_gcvt_s(temp_str_saturation, 10, _saturation, 4);
	strcat_s(hsl_color_code, temp_str_saturation);
	strcat_s(hsl_color_code, "%; L: ");

	_gcvt_s(temp_str_lightness, 10, _lightness, 4);
	strcat_s(hsl_color_code, temp_str_lightness);
	strcat_s(hsl_color_code, "%");

	strncpy_s(result, strlen(hsl_color_code) + 1, hsl_color_code, strlen(hsl_color_code) + 1);
	return result;
}

void initialize_humbrol_database() {
	humbrol_database.insert(std::pair<int, status>(2, status(0, 141, 75)));
	humbrol_database.insert(std::pair<int, status>(3, status(0, 71, 25)));
	humbrol_database.insert(std::pair<int, status>(7, status(253, 189, 79)));
	humbrol_database.insert(std::pair<int, status>(9, status(153, 102, 51)));
	humbrol_database.insert(std::pair<int, status>(10, status(62, 58, 57)));
	humbrol_database.insert(std::pair<int, status>(12, status(154, 107, 55)));
	humbrol_database.insert(std::pair<int, status>(14, status(0, 93, 173)));
	humbrol_database.insert(std::pair<int, status>(15, status(0, 51, 104)));
	humbrol_database.insert(std::pair<int, status>(16, status(220, 172, 38)));
	humbrol_database.insert(std::pair<int, status>(18, status(243, 117, 33)));
	humbrol_database.insert(std::pair<int, status>(19, status(225, 48, 38)));
	humbrol_database.insert(std::pair<int, status>(20, status(145, 0, 41)));
	humbrol_database.insert(std::pair<int, status>(22, status(248, 244, 243)));
	humbrol_database.insert(std::pair<int, status>(23, status(202, 232, 220)));
	humbrol_database.insert(std::pair<int, status>(24, status(253, 1919, 78)));
	humbrol_database.insert(std::pair<int, status>(25, status(17, 77, 150)));
	humbrol_database.insert(std::pair<int, status>(26, status(144, 110, 62)));
	humbrol_database.insert(std::pair<int, status>(27, status(82, 93, 87)));
	humbrol_database.insert(std::pair<int, status>(29, status(120, 108, 94)));
	humbrol_database.insert(std::pair<int, status>(30, status(9, 109, 81)));
	humbrol_database.insert(std::pair<int, status>(31, status(85, 105, 106)));
	humbrol_database.insert(std::pair<int, status>(32, status(70, 78, 81)));
	humbrol_database.insert(std::pair<int, status>(34, status(245, 240, 240)));
	humbrol_database.insert(std::pair<int, status>(35, status(247, 245, 245)));
	humbrol_database.insert(std::pair<int, status>(38, status(102, 153, 51)));
	humbrol_database.insert(std::pair<int, status>(40, status(159, 160, 164)));
	humbrol_database.insert(std::pair<int, status>(41, status(243, 237, 185)));
	humbrol_database.insert(std::pair<int, status>(47, status(72, 181, 224)));
	humbrol_database.insert(std::pair<int, status>(48, status(33, 141, 177)));
	humbrol_database.insert(std::pair<int, status>(49, status(245, 242, 242)));
	humbrol_database.insert(std::pair<int, status>(50, status(0, 159, 105)));
	humbrol_database.insert(std::pair<int, status>(51, status(189, 17, 89)));
	humbrol_database.insert(std::pair<int, status>(52, status(0, 109, 174)));
	humbrol_database.insert(std::pair<int, status>(54, status(220, 144, 24)));
	humbrol_database.insert(std::pair<int, status>(55, status(148, 81, 4)));
	humbrol_database.insert(std::pair<int, status>(60, status(186, 19, 29)));
	humbrol_database.insert(std::pair<int, status>(61, status(241, 145, 103)));
	humbrol_database.insert(std::pair<int, status>(62, status(165, 98, 69)));
	humbrol_database.insert(std::pair<int, status>(63, status(205, 153, 95)));
	humbrol_database.insert(std::pair<int, status>(64, status(168, 168, 168)));
	humbrol_database.insert(std::pair<int, status>(65, status(173, 208, 201)));
	humbrol_database.insert(std::pair<int, status>(66, status(49, 69, 70)));
	humbrol_database.insert(std::pair<int, status>(67, status(85, 92, 95)));
	humbrol_database.insert(std::pair<int, status>(68, status(26, 3, 81)));
	humbrol_database.insert(std::pair<int, status>(69, status(254, 220, 0)));
	humbrol_database.insert(std::pair<int, status>(70, status(140, 84, 61)));
	humbrol_database.insert(std::pair<int, status>(71, status(234, 210, 172)));
	humbrol_database.insert(std::pair<int, status>(72, status(156, 143, 127)));
	humbrol_database.insert(std::pair<int, status>(73, status(145, 83, 96)));
	humbrol_database.insert(std::pair<int, status>(74, status(255, 214, 90)));
	humbrol_database.insert(std::pair<int, status>(75, status(78, 97, 93)));
	humbrol_database.insert(std::pair<int, status>(76, status(70, 114, 99)));
	humbrol_database.insert(std::pair<int, status>(77, status(74, 103, 119)));
	humbrol_database.insert(std::pair<int, status>(78, status(46, 90, 67)));
	humbrol_database.insert(std::pair<int, status>(79, status(97, 105, 116)));
	humbrol_database.insert(std::pair<int, status>(80, status(97, 117, 89)));
	humbrol_database.insert(std::pair<int, status>(81, status(213, 190, 86)));
	humbrol_database.insert(std::pair<int, status>(82, status(224, 106, 70)));
	humbrol_database.insert(std::pair<int, status>(83, status(104, 153, 51)));
	humbrol_database.insert(std::pair<int, status>(85, status(6, 6, 7)));
	humbrol_database.insert(std::pair<int, status>(86, status(100, 105, 83)));
	humbrol_database.insert(std::pair<int, status>(87, status(123, 144, 149)));
	humbrol_database.insert(std::pair<int, status>(88, status(77, 114, 70)));
	humbrol_database.insert(std::pair<int, status>(89, status(49, 149, 183)));
	humbrol_database.insert(std::pair<int, status>(90, status(210, 232, 186)));
	humbrol_database.insert(std::pair<int, status>(91, status(0, 19, 8)));
	humbrol_database.insert(std::pair<int, status>(93, status(186, 149, 97)));
	humbrol_database.insert(std::pair<int, status>(94, status(195, 155, 119)));
	humbrol_database.insert(std::pair<int, status>(96, status(89, 119, 147)));
	humbrol_database.insert(std::pair<int, status>(98, status(113, 94, 80)));
	humbrol_database.insert(std::pair<int, status>(99, status(244, 227, 13)));
	humbrol_database.insert(std::pair<int, status>(100, status(192, 86, 72)));
	humbrol_database.insert(std::pair<int, status>(101, status(65, 127, 104)));
	humbrol_database.insert(std::pair<int, status>(102, status(94, 117, 97)));
	humbrol_database.insert(std::pair<int, status>(103, status(231, 206, 140)));
	humbrol_database.insert(std::pair<int, status>(104, status(30, 79, 111)));
	humbrol_database.insert(std::pair<int, status>(106, status(96, 117, 98)));
	humbrol_database.insert(std::pair<int, status>(109, status(79, 130, 173)));
	humbrol_database.insert(std::pair<int, status>(111, status(79, 54, 55)));
	humbrol_database.insert(std::pair<int, status>(112, status(40, 51, 93)));
	humbrol_database.insert(std::pair<int, status>(113, status(165, 113, 102)));
	humbrol_database.insert(std::pair<int, status>(116, status(51, 92, 76)));
	humbrol_database.insert(std::pair<int, status>(117, status(73, 103, 75)));
	humbrol_database.insert(std::pair<int, status>(118, status(164, 129, 109)));
	humbrol_database.insert(std::pair<int, status>(119, status(165, 118, 98)));
	humbrol_database.insert(std::pair<int, status>(121, status(217, 200, 157)));
	humbrol_database.insert(std::pair<int, status>(123, status(103, 114, 120)));
	humbrol_database.insert(std::pair<int, status>(125, status(98, 111, 120)));
	humbrol_database.insert(std::pair<int, status>(126, status(127, 136, 133)));
	humbrol_database.insert(std::pair<int, status>(127, status(156, 182, 195)));
	humbrol_database.insert(std::pair<int, status>(128, status(130, 165, 184)));
	humbrol_database.insert(std::pair<int, status>(129, status(147, 157, 158)));
	humbrol_database.insert(std::pair<int, status>(130, status(247, 240, 240)));
	humbrol_database.insert(std::pair<int, status>(131, status(31, 119, 43)));
	humbrol_database.insert(std::pair<int, status>(132, status(211, 86, 68)));
	humbrol_database.insert(std::pair<int, status>(133, status(130, 0, 36)));
	humbrol_database.insert(std::pair<int, status>(140, status(130, 145, 50)));
	humbrol_database.insert(std::pair<int, status>(142, status(124, 69, 78)));
	humbrol_database.insert(std::pair<int, status>(144, status(95, 118, 136)));
	humbrol_database.insert(std::pair<int, status>(145, status(101, 114, 122)));
	humbrol_database.insert(std::pair<int, status>(147, status(201, 202, 204)));
	humbrol_database.insert(std::pair<int, status>(148, status(255, 216, 157)));
	humbrol_database.insert(std::pair<int, status>(150, status(83, 96, 40)));
	humbrol_database.insert(std::pair<int, status>(151, status(73, 76, 27)));
	humbrol_database.insert(std::pair<int, status>(153, status(178, 16, 27)));
	humbrol_database.insert(std::pair<int, status>(154, status(252, 180, 20)));
	humbrol_database.insert(std::pair<int, status>(155, status(99, 94, 52)));
	humbrol_database.insert(std::pair<int, status>(156, status(120, 121, 123)));
	humbrol_database.insert(std::pair<int, status>(157, status(10, 93, 125)));
	humbrol_database.insert(std::pair<int, status>(159, status(72, 91, 63)));
	humbrol_database.insert(std::pair<int, status>(160, status(95, 51, 26)));
	humbrol_database.insert(std::pair<int, status>(161, status(31, 56, 60)));
	humbrol_database.insert(std::pair<int, status>(163, status(57, 61, 29)));
	humbrol_database.insert(std::pair<int, status>(165, status(150, 157, 163)));
	humbrol_database.insert(std::pair<int, status>(166, status(164, 165, 169)));
	humbrol_database.insert(std::pair<int, status>(167, status(143, 152, 157)));
	humbrol_database.insert(std::pair<int, status>(168, status(186, 165, 102)));
	humbrol_database.insert(std::pair<int, status>(171, status(150, 109, 91)));
	humbrol_database.insert(std::pair<int, status>(172, status(242, 79, 0)));
	humbrol_database.insert(std::pair<int, status>(173, status(44, 37, 36)));
	humbrol_database.insert(std::pair<int, status>(174, status(215, 65, 51)));
	humbrol_database.insert(std::pair<int, status>(176, status(126, 84, 139)));
	humbrol_database.insert(std::pair<int, status>(185, status(230, 13, 46)));
	humbrol_database.insert(std::pair<int, status>(186, status(127, 95, 80)));
	humbrol_database.insert(std::pair<int, status>(187, status(109, 80, 0)));
	humbrol_database.insert(std::pair<int, status>(191, status(219, 228, 235)));
	humbrol_database.insert(std::pair<int, status>(193, status(191, 18, 56)));
	humbrol_database.insert(std::pair<int, status>(195, status(0, 68, 21)));
	humbrol_database.insert(std::pair<int, status>(200, status(237, 165, 169)));
	humbrol_database.insert(std::pair<int, status>(201, status(4, 0, 0)));
	humbrol_database.insert(std::pair<int, status>(208, status(70, 183, 79)));
	humbrol_database.insert(std::pair<int, status>(209, status(238, 39, 44)));
	humbrol_database.insert(std::pair<int, status>(220, status(207, 14, 14)));
	humbrol_database.insert(std::pair<int, status>(222, status(0, 85, 165)));
	humbrol_database.insert(std::pair<int, status>(224, status(106, 105, 100)));
	humbrol_database.insert(std::pair<int, status>(226, status(109, 109, 81)));
	humbrol_database.insert(std::pair<int, status>(237, status(177, 166, 136)));
	humbrol_database.insert(std::pair<int, status>(241, status(66, 68, 55)));
	humbrol_database.insert(std::pair<int, status>(246, status(114, 111, 104)));
	humbrol_database.insert(std::pair<int, status>(248, status(149, 181, 170)));
	humbrol_database.insert(std::pair<int, status>(250, status(185, 141, 112)));
	humbrol_database.insert(std::pair<int, status>(1321, status(240, 14, 14)));
	humbrol_database.insert(std::pair<int, status>(1322, status(241, 78, 35)));
	humbrol_database.insert(std::pair<int, status>(1325, status(95, 145, 72)));
	humbrol_database.insert(std::pair<int, status>(27001, status(204, 200, 200)));
	humbrol_database.insert(std::pair<int, status>(27002, status(208, 213, 219)));
	humbrol_database.insert(std::pair<int, status>(27003, status(175, 192, 202)));
	humbrol_database.insert(std::pair<int, status>(27004, status(44, 46, 24)));
}

void initialize_RAL_database() {
	database.insert(std::pair<int, status>(1000, status(205, 186, 136)));
	database.insert(std::pair<int, status>(1001, status(208, 176, 132)));
	database.insert(std::pair<int, status>(1002, status(210, 170, 109)));
	database.insert(std::pair<int, status>(1003, status(249, 168, 0)));
	database.insert(std::pair<int, status>(1004, status(228, 158, 0)));
	database.insert(std::pair<int, status>(1005, status(203, 142, 0)));
	database.insert(std::pair<int, status>(1006, status(226, 144, 0)));
	database.insert(std::pair<int, status>(1007, status(232, 140, 0)));
	database.insert(std::pair<int, status>(1011, status(175, 128, 79)));
	database.insert(std::pair<int, status>(1012, status(221, 175, 39)));
	database.insert(std::pair<int, status>(1013, status(227, 217, 198)));
	database.insert(std::pair<int, status>(1014, status(221, 196, 154)));
	database.insert(std::pair<int, status>(1015, status(230, 210, 181)));
	database.insert(std::pair<int, status>(1016, status(241, 221, 56)));
	database.insert(std::pair<int, status>(1017, status(246, 169, 80)));
	database.insert(std::pair<int, status>(1018, status(250, 202, 48)));
	database.insert(std::pair<int, status>(1019, status(164, 143, 122)));
	database.insert(std::pair<int, status>(1020, status(160, 143, 101)));
	database.insert(std::pair<int, status>(1021, status(246, 182, 0)));
	database.insert(std::pair<int, status>(1023, status(247, 181, 0)));
	database.insert(std::pair<int, status>(1024, status(186, 143, 76)));
	database.insert(std::pair<int, status>(1026, status(255, 255, 0)));
	database.insert(std::pair<int, status>(1027, status(167, 127, 14)));
	database.insert(std::pair<int, status>(1028, status(255, 155, 0)));
	database.insert(std::pair<int, status>(1032, status(226, 163, 0)));
	database.insert(std::pair<int, status>(1033, status(249, 154, 28)));
	database.insert(std::pair<int, status>(1034, status(235, 156, 82)));
	database.insert(std::pair<int, status>(1035, status(144, 131, 112)));
	database.insert(std::pair<int, status>(1036, status(128, 100, 63)));
	database.insert(std::pair<int, status>(1037, status(240, 146, 0)));
	database.insert(std::pair<int, status>(2000, status(218, 110, 0)));
	database.insert(std::pair<int, status>(2001, status(186, 72, 27)));
	database.insert(std::pair<int, status>(2002, status(191, 57, 34)));
	database.insert(std::pair<int, status>(2003, status(246, 120, 40)));
	database.insert(std::pair<int, status>(2004, status(226, 83, 3)));
	database.insert(std::pair<int, status>(2005, status(255, 77, 6)));
	database.insert(std::pair<int, status>(2007, status(255, 178, 0)));
	database.insert(std::pair<int, status>(2008, status(237, 107, 33)));
	database.insert(std::pair<int, status>(2009, status(222, 83, 7)));
	database.insert(std::pair<int, status>(2010, status(208, 93, 40)));
	database.insert(std::pair<int, status>(2011, status(226, 110, 14)));
	database.insert(std::pair<int, status>(2012, status(213, 101, 77)));
	database.insert(std::pair<int, status>(2013, status(146, 62, 37)));
	database.insert(std::pair<int, status>(3000, status(167, 41, 32)));
	database.insert(std::pair<int, status>(3001, status(155, 36, 35)));
	database.insert(std::pair<int, status>(3002, status(155, 35, 33)));
	database.insert(std::pair<int, status>(3003, status(134, 26, 34)));
	database.insert(std::pair<int, status>(3004, status(107, 28, 35)));
	database.insert(std::pair<int, status>(3005, status(89, 25, 31)));
	database.insert(std::pair<int, status>(3007, status(62, 32, 34)));
	database.insert(std::pair<int, status>(3009, status(109, 52, 45)));
	database.insert(std::pair<int, status>(3011, status(121, 36, 35)));
	database.insert(std::pair<int, status>(3012, status(198, 132, 109)));
	database.insert(std::pair<int, status>(3013, status(151, 46, 37)));
	database.insert(std::pair<int, status>(3014, status(203, 115, 117)));
	database.insert(std::pair<int, status>(3015, status(216, 160, 166)));
	database.insert(std::pair<int, status>(3016, status(166, 61, 47)));
	database.insert(std::pair<int, status>(3017, status(203, 85, 93)));
	database.insert(std::pair<int, status>(3018, status(199, 63, 74)));
	database.insert(std::pair<int, status>(3020, status(187, 30, 16)));
	database.insert(std::pair<int, status>(3022, status(207, 105, 85)));
	database.insert(std::pair<int, status>(3024, status(255, 45, 33)));
	database.insert(std::pair<int, status>(3026, status(255, 42, 27)));
	database.insert(std::pair<int, status>(3027, status(171, 39, 60)));
	database.insert(std::pair<int, status>(3028, status(204, 44, 36)));
	database.insert(std::pair<int, status>(3031, status(166, 52, 55)));
	database.insert(std::pair<int, status>(3032, status(112, 29, 35)));
	database.insert(std::pair<int, status>(3033, status(165, 58, 45)));
	database.insert(std::pair<int, status>(4001, status(129, 97, 131)));
	database.insert(std::pair<int, status>(4002, status(141, 60, 75)));
	database.insert(std::pair<int, status>(4003, status(196, 97, 140)));
	database.insert(std::pair<int, status>(4004, status(101, 30, 56)));
	database.insert(std::pair<int, status>(4005, status(118, 104, 154)));
	database.insert(std::pair<int, status>(4006, status(144, 51, 115)));
	database.insert(std::pair<int, status>(4007, status(71, 36, 60)));
	database.insert(std::pair<int, status>(4008, status(132, 76, 130)));
	database.insert(std::pair<int, status>(4009, status(157, 134, 146)));
	database.insert(std::pair<int, status>(4010, status(188, 64, 119)));
	database.insert(std::pair<int, status>(4011, status(110, 99, 135)));
	database.insert(std::pair<int, status>(4012, status(107, 107, 127)));
	database.insert(std::pair<int, status>(5000, status(49, 79, 111)));
	database.insert(std::pair<int, status>(5001, status(15, 76, 100)));
	database.insert(std::pair<int, status>(5002, status(0, 56, 123)));
	database.insert(std::pair<int, status>(5003, status(31, 56, 85)));
	database.insert(std::pair<int, status>(5004, status(25, 30, 40)));
	database.insert(std::pair<int, status>(5005, status(0, 83, 135)));
	database.insert(std::pair<int, status>(5007, status(55, 107, 140)));
	database.insert(std::pair<int, status>(5008, status(43, 58, 68)));
	database.insert(std::pair<int, status>(5009, status(34, 95, 120)));
	database.insert(std::pair<int, status>(5010, status(0, 79, 124)));
	database.insert(std::pair<int, status>(5012, status(0, 137, 182)));
	database.insert(std::pair<int, status>(5013, status(25, 49, 83)));
	database.insert(std::pair<int, status>(5014, status(99, 125, 150)));
	database.insert(std::pair<int, status>(5015, status(0, 124, 176)));
	database.insert(std::pair<int, status>(5017, status(0, 91, 140)));
	database.insert(std::pair<int, status>(5018, status(5, 139, 140)));
	database.insert(std::pair<int, status>(5019, status(0, 94, 131)));
	database.insert(std::pair<int, status>(5020, status(0, 65, 75)));
	database.insert(std::pair<int, status>(5021, status(0, 117, 119)));
	database.insert(std::pair<int, status>(5022, status(34, 45, 90)));
	database.insert(std::pair<int, status>(5023, status(66, 105, 140)));
	database.insert(std::pair<int, status>(5024, status(96, 147, 172)));
	database.insert(std::pair<int, status>(5025, status(33, 105, 124)));
	database.insert(std::pair<int, status>(5026, status(15, 48, 82)));
	database.insert(std::pair<int, status>(6000, status(60, 116, 96)));
	database.insert(std::pair<int, status>(6001, status(54, 103, 53)));
	database.insert(std::pair<int, status>(6002, status(50, 89, 40)));
	database.insert(std::pair<int, status>(6003, status(80, 83, 60)));
	database.insert(std::pair<int, status>(6004, status(2, 68, 66)));
	database.insert(std::pair<int, status>(6005, status(17, 66, 50)));
	database.insert(std::pair<int, status>(6006, status(60, 57, 46)));
	database.insert(std::pair<int, status>(6007, status(44, 50, 34)));
	database.insert(std::pair<int, status>(6008, status(55, 52, 42)));
	database.insert(std::pair<int, status>(6009, status(39, 53, 42)));
	database.insert(std::pair<int, status>(6010, status(77, 111, 57)));
	database.insert(std::pair<int, status>(6011, status(108, 124, 89)));
	database.insert(std::pair<int, status>(6012, status(48, 61, 58)));
	database.insert(std::pair<int, status>(6013, status(125, 118, 90)));
	database.insert(std::pair<int, status>(6014, status(71, 65, 53)));
	database.insert(std::pair<int, status>(6015, status(61, 61, 54)));
	database.insert(std::pair<int, status>(6016, status(0, 105, 76)));
	database.insert(std::pair<int, status>(6017, status(88, 127, 64)));
	database.insert(std::pair<int, status>(6018, status(97, 153, 59)));
	database.insert(std::pair<int, status>(6019, status(185, 206, 172)));
	database.insert(std::pair<int, status>(6020, status(55, 66, 47)));
	database.insert(std::pair<int, status>(6021, status(138, 153, 119)));
	database.insert(std::pair<int, status>(6022, status(58, 51, 39)));
	database.insert(std::pair<int, status>(6024, status(0, 131, 81)));
	database.insert(std::pair<int, status>(6025, status(94, 110, 59)));
	database.insert(std::pair<int, status>(6026, status(0, 95, 78)));
	database.insert(std::pair<int, status>(6027, status(126, 186, 181)));
	database.insert(std::pair<int, status>(6028, status(49, 84, 66)));
	database.insert(std::pair<int, status>(6029, status(0, 111, 61)));
	database.insert(std::pair<int, status>(6032, status(35, 127, 82)));
	database.insert(std::pair<int, status>(6033, status(70, 135, 127)));
	database.insert(std::pair<int, status>(6034, status(122, 172, 172)));
	database.insert(std::pair<int, status>(6035, status(25, 77, 37)));
	database.insert(std::pair<int, status>(6036, status(4, 87, 75)));
	database.insert(std::pair<int, status>(6038, status(0, 181, 26)));
	database.insert(std::pair<int, status>(7000, status(122, 136, 142)));
	database.insert(std::pair<int, status>(7001, status(140, 150, 157)));
	database.insert(std::pair<int, status>(7002, status(129, 120, 99)));
	database.insert(std::pair<int, status>(7003, status(122, 118, 105)));
	database.insert(std::pair<int, status>(7004, status(155, 155, 155)));
	database.insert(std::pair<int, status>(7005, status(108, 110, 107)));
	database.insert(std::pair<int, status>(7006, status(118, 106, 94)));
	database.insert(std::pair<int, status>(7008, status(116, 94, 61)));
	database.insert(std::pair<int, status>(7009, status(93, 96, 88)));
	database.insert(std::pair<int, status>(7010, status(88, 92, 86)));
	database.insert(std::pair<int, status>(7011, status(82, 89, 93)));
	database.insert(std::pair<int, status>(7012, status(87, 93, 94)));
	database.insert(std::pair<int, status>(7013, status(87, 80, 68)));
	database.insert(std::pair<int, status>(7015, status(79, 83, 88)));
	database.insert(std::pair<int, status>(7016, status(56, 62, 66)));
	database.insert(std::pair<int, status>(7021, status(47, 50, 52)));
	database.insert(std::pair<int, status>(7022, status(76, 74, 68)));
	database.insert(std::pair<int, status>(7023, status(128, 128, 128)));
	database.insert(std::pair<int, status>(7024, status(69, 73, 78)));
	database.insert(std::pair<int, status>(7026, status(55, 67, 69)));
	database.insert(std::pair<int, status>(7030, status(146, 142, 133)));
	database.insert(std::pair<int, status>(7031, status(91, 104, 109)));
	database.insert(std::pair<int, status>(7032, status(181, 176, 161)));
	database.insert(std::pair<int, status>(7033, status(127, 130, 116)));
	database.insert(std::pair<int, status>(7034, status(146, 136, 111)));
	database.insert(std::pair<int, status>(7035, status(197, 199, 196)));
	database.insert(std::pair<int, status>(7036, status(151, 147, 146)));
	database.insert(std::pair<int, status>(7037, status(122, 123, 122)));
	database.insert(std::pair<int, status>(7038, status(176, 176, 169)));
	database.insert(std::pair<int, status>(7039, status(107, 102, 94)));
	database.insert(std::pair<int, status>(7040, status(152, 158, 161)));


	database.insert(std::pair<int, status>(7042, status(142, 146, 145)));
	database.insert(std::pair<int, status>(7043, status(79, 82, 80)));
	database.insert(std::pair<int, status>(7044, status(183, 179, 168)));
	database.insert(std::pair<int, status>(7045, status(141, 146, 149)));
	database.insert(std::pair<int, status>(7046, status(127, 134, 138)));
	database.insert(std::pair<int, status>(7047, status(200, 200, 199)));
	database.insert(std::pair<int, status>(7048, status(129, 123, 115)));
	database.insert(std::pair<int, status>(8000, status(137, 105, 62)));
	database.insert(std::pair<int, status>(8001, status(157, 98, 43)));
	database.insert(std::pair<int, status>(8002, status(121, 77, 62)));
	database.insert(std::pair<int, status>(8003, status(126, 75, 38)));
	database.insert(std::pair<int, status>(8004, status(141, 73, 49)));
	database.insert(std::pair<int, status>(8007, status(112, 69, 42)));
	database.insert(std::pair<int, status>(8008, status(114, 74, 37)));
	database.insert(std::pair<int, status>(8011, status(90, 56, 38)));
	database.insert(std::pair<int, status>(8012, status(102, 51, 43)));
	database.insert(std::pair<int, status>(8014, status(74, 53, 38)));
	database.insert(std::pair<int, status>(8015, status(94, 47, 38)));
	database.insert(std::pair<int, status>(8016, status(76, 43, 32)));
	database.insert(std::pair<int, status>(8017, status(68, 47, 41)));
	database.insert(std::pair<int, status>(8019, status(61, 54, 53)));
	database.insert(std::pair<int, status>(8022, status(26, 23, 24)));
	database.insert(std::pair<int, status>(8023, status(164, 87, 41)));
	database.insert(std::pair<int, status>(8024, status(121, 80, 56)));
	database.insert(std::pair<int, status>(8025, status(117, 88, 71)));
	database.insert(std::pair<int, status>(8028, status(81, 58, 42)));
	database.insert(std::pair<int, status>(8029, status(127, 64, 49)));
	database.insert(std::pair<int, status>(9001, status(233, 224, 210)));
	database.insert(std::pair<int, status>(9002, status(215, 213, 203)));
	database.insert(std::pair<int, status>(9003, status(236, 236, 231)));
	database.insert(std::pair<int, status>(9004, status(43, 43, 44)));
	database.insert(std::pair<int, status>(9005, status(14, 14, 16)));
	database.insert(std::pair<int, status>(9006, status(161, 161, 160)));
	database.insert(std::pair<int, status>(9007, status(135, 133, 129)));
	database.insert(std::pair<int, status>(9010, status(241, 236, 225)));
	database.insert(std::pair<int, status>(9011, status(39, 41, 43)));
	database.insert(std::pair<int, status>(9016, status(241, 240, 234)));
	database.insert(std::pair<int, status>(9017, status(42, 41, 42)));
	database.insert(std::pair<int, status>(9018, status(200, 203, 196)));
	database.insert(std::pair<int, status>(9022, status(133, 133, 131)));
	database.insert(std::pair<int, status>(9023, status(121, 123, 122)));
}


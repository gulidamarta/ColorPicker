#pragma once

class Color {
public:
	Color(int redColor, int greenColor, int blueColor);
	Color(double hue, double saturation, double lightness);

	int randomRedColor;
	int randomGreenColor;
	int randomBlueColor;
	
	void setRedColor(int redColor);
	void setGreenColor(int greenColor);
	void setBlueColor(int blueColor);

	void setHue(double hue);
	void setSaturation(double saturation);
	void setLightness(double lightness);

	void setHumbrol(int humbrol);

	void setHtmlColorCode(char *html_color_code);

	double getHue();
	double getSaturation();
	double getLightness();
	
	float get_L();
	float get_a();
	float get_b();

	int getRedColor();
	int getGreenColor();
	int getBlueColor();

	int getRandomRedColor();
	int getRandomGreenColor();
	int getRandomBlueColor();

	char *getHueValueStr();
	char *getSaturationValueStr();
	char *getLightnessValueStr();

	char *getRedValueStr();
	char *getGreenValueStr();
	char *getBlueValueStr();

	char *getRedColorStr();
	char *getGreenColorStr();
	char *getBlueColorStr();

	char *getStrRgbColorCode();
	char *getStrHtmlColorCode();
	char *getHslColorCode();

	char *getRalCodeStr();
	char *getHumbrolCodeStr();
	char *getHumbrolStr();

	BOOL isHumbrolCode(int humbrol_temp_code);

	void convertToXYZ();
	void convertXYZtoLab();
	void convertLabtoXYZ();
	void convertXYZtoRGB(float inX, float inY, float inZ, int * outR, int * outG, int * outB);
	float Lab_color_difference(float inL1, float ina1, float  inb1, float inL2, float ina2, float  inb2);
private:
	int _redColor;
	int _greenColor;
	int _blueColor;

	double _hue;
	double _saturation;
	double _lightness;

	int _ral;

	float _CIE_L;
	float _CIE_a;
	float _CIE_b;

	float _x_value;
	float _y_value;
	float _z_value;

	int _humbrol;

	double _getHue();
	double _getSaturation();
	double _getLightness();
	void convertFromHslToRgb();
	void convertFromRgbToRal();
	void convertFromRgbToHumbrol();
	char *GetHslStringFormat();
	int ConvertStrToInt(char *str_to_convert);
};

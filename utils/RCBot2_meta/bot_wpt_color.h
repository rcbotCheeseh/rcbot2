#ifndef __BOT_WPT_COLOR_H__
#define __BOT_WPT_COLOR_H__

// gone for the american spelling of 'colour' :)

#define WAYPOINT_ALPHA 200

class WptColor
{
public:
	WptColor()
	{
		memset(this, 0, sizeof(WptColor));
	}

	WptColor(int _r, int _g, int _b, int _a)
	{
		r = _r;
		g = _g;
		b = _b;
		a = _a;
	}

	WptColor(int _r, int _g, int _b)
	{
		r = _r;
		g = _g;
		b = _b;
		a = WAYPOINT_ALPHA;
	}

	void mix(WptColor other)
	{
		float fr = (r - other.r) * 0.5;
		float fg = (g - other.g) * 0.5;
		float fb = (b - other.b) * 0.5;

		if (fr < 0)
			fr = 0;
		if (fg < 0)
			fg = 0;
		if (fb < 0)
			fb = 0;

		r = static_cast<unsigned char>((int)((float)other.r * 0.5)) + static_cast<unsigned char>((int)fr);
		g = static_cast<unsigned char>((int)((float)other.g * 0.5)) + static_cast<unsigned char>((int)fg);
		b = static_cast<unsigned char>((int)((float)other.b * 0.5)) + static_cast<unsigned char>((int)fb);

		//r = (r+other.r)/2;
		//g = (g+other.g)/2;
		//b = (b+other.b)/2;
		//a = (a+other.a)/2;
	}

	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;

	static const WptColor white;
};

#endif
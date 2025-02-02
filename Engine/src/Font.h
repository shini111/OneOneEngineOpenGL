#pragma once
#include <string>
#include <vector>


enum CharEnum {
	SPACEBAR = 0,
	EXCLAMATION = 1,
	DOUBLE_QUOTE = 2,
	HASH = 3,
	DOLLAR = 4,
	PERCENT = 5,
	AMPERSAND = 6,
	SINGLE_QUOTE = 7,
	LEFT_PAREN = 8,
	RIGHT_PAREN = 9,
	ASTERISK = 10,
	PLUS = 11,
	COMMA = 12,
	MINUS = 13,
	DOT = 14,
	SLASH = 15,
	ZERO = 16,
	ONE = 17,
	TWO = 18,
	THREE = 19,
	FOUR = 20,
	FIVE = 21,
	SIX = 22,
	SEVEN = 23,
	EIGHT = 24,
	NINE = 25,
	COLON = 26,
	SEMICOLON = 27,
	LESS_THAN = 28,
	EQUALS = 29,
	GREATER_THAN = 30,
	QUESTION = 31,
	AT = 32,
	A = 33,
	B = 34,
	C = 35,
	D = 36,
	E = 37,
	F = 38,
	G = 39,
	H = 40,
	I = 41,
	J = 42,
	K = 43,
	L = 44,
	M = 45,
	N = 46,
	O = 47,
	P = 48,
	Q = 49,
	R = 50,
	S = 51,
	T = 52,
	U = 53,
	V = 54,
	W = 55,
	X = 56,
	Y = 57,
	Z = 58,
	LEFT_BRACKET = 59,
	BACKSLASH = 60,
	RIGHT_BRACKET = 61,
	CARET = 62,
	UNDERSCORE = 63,
	BACKTICK = 64,
	a = 65,
	b = 66,
	c = 67,
	d = 68,
	e = 69,
	f = 70,
	g = 71,
	h = 72,
	i = 73,
	j = 74,
	k = 75,
	l = 76,
	m = 77,
	n = 78,
	o = 79,
	p = 80,
	q = 81,
	r = 82,
	s = 83,
	t = 84,
	u = 85,
	v = 86,
	w = 87,
	x = 88,
	y = 89,
	z = 90,
	LEFT_CURLY = 91,
	PIPE = 92,
	RIGHT_CURLY = 93,
	UNKNOWN = 94
};


class Font
{
public:
	Font(std::string filepath, int rows = 1, int columns = 1)
		: bitMapPath(filepath), bitMapSize{ rows, columns }
	{
	}

	std::string bitMapPath;

	struct {
		int columns;
		int rows;
	}bitMapSize;

};



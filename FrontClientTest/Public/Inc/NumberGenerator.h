#pragma once

template <class T, class NUMBER = unsigned int, NUMBER BASE = 0, NUMBER STEP = 1> struct NumberGenerator
{
	NUMBER GetNextNumber()
	{
		s_nNextNumber += STEP;
		return s_nNextNumber;
	}
private:
	static NUMBER s_nNextNumber; 
};

template <class T, class NUMBER, NUMBER BASE, NUMBER STEP> NUMBER NumberGenerator<T, NUMBER, BASE, STEP>::s_nNextNumber = BASE;
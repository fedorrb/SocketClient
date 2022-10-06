#ifndef NORMALIZE_H
#define NORMALIZE_H
#include <cstdlib>
#include <string>
#include <list>
#include <fstream>
#include <iostream>
#include <sstream>
#include <locale>
#include <vector>
#include <string>
#include <algorithm>
#include <map>

using namespace std;

//����� ��� ����������� ������ �������
class Normalize
{
public:
	Normalize(void);
	void SplitStringLine(const std::string& str, std::vector<std::string> & cont, int direction, const std::string& delims = "{");
	void AlignCol(std::vector<std::string> & cont);//����������� �������
public:
	~Normalize(void);
};
#endif
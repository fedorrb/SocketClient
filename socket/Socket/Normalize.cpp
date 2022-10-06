//#include "StdAfx.h"
#include "Normalize.h"

Normalize::Normalize(void)
{
}

Normalize::~Normalize(void)
{
}
//������� �������� ���� ������ �� ������� �� ����������
void Normalize::SplitStringLine(const std::string& str, vector<string> & cont, int direction, const std::string& delims)
{
	//��������� �� ������� �'������ ������� ������ � ������� � �������� ������ ������
	vector<std::string>::iterator it;
	std::string tmpStr;
	bool append = false;
	if(direction && cont.size()) {
		it = cont.end();
		--it;
		if((*it)[(*it).length()-1] != '\n') {	//if((*it).find_last_of("\n") == std::string::npos) {
			append = true;
		}
	}
	//��������
    std::size_t current = 0, previous = 0;
	current = str.find(delims, previous);
    while (current != std::string::npos) {
		if(current > 0) {
			if(previous > 0) {
				cont.push_back(str.substr(previous-1, current-(previous-1)) + '\n');
			}
			else
			{
				if(append) { //�'�������
					it = cont.end();
					--it;
					tmpStr = (*it);
					cont.pop_back();
					cont.push_back(tmpStr + str.substr(0, current) + '\n');
				} else {
					cont.push_back(str.substr(0, current) + '\n');
				}
			}
		}
		previous = (current + 1);
		current = str.find(delims, previous);
    }
	if(previous > 0)
		cont.push_back(str.substr(previous-1));
	else
		cont.push_back(str.substr(0));
}

//����������� �������
void Normalize::AlignCol(std::vector<std::string> &cont) {
	std::map<string, int> colWidth; //����: ����� ������� - �������
	colWidth.clear(); 
	size_t pos = 0;
	vector<size_t> positions; //������� �������� ����� � ������
	std::string sub = "\"";
	std::vector<std::string>::iterator itStr; //���� ������ � �������� �������
	std::string str;//����� �������
	std::map<std::string, int>::iterator itMap;
	int newWidth = 0;
	for(itStr = cont.begin(); itStr != cont.end(); ++itStr) {
		//������ ������� ����� � ������
		positions.clear();
		pos = (*itStr).find(sub, 0);
		while(pos != string::npos)
		{
			positions.push_back(pos);
			pos = (*itStr).find(sub,pos+1);
		}
		//���������� ����: ����� ������� - ����������� �������
		if(positions.size() && positions.size() % 4 == 0) {//������ 4 [{"name":"value"}]
			for(int i = 0; i < positions.size(); i+=4) {
				str = (*itStr).substr(positions[i]+1, positions[i+1] - positions[i] - 1);//����� �������
				itMap = colWidth.find(str);
				newWidth = positions[i+3] - positions[i+2] - 1;//������ ������� value
				if (itMap != colWidth.end()) {					
					if(itMap->second < newWidth)
						itMap->second = newWidth;
				} else {
					colWidth.insert(std::make_pair(std::string(str), newWidth));
				}
			}

		}
	}
	//�������� ������ ��� �������
	int currWidth = 0;//������� ������ �������
	int spaces = 0;//������� ������ ��� �����������
	for(itStr = cont.begin(); itStr != cont.end(); ++itStr) {
		//������ ������� ����� � ������
		positions.clear();
		pos = (*itStr).find(sub, 0);
		while(pos != string::npos)
		{
			positions.push_back(pos);
			pos = (*itStr).find(sub,pos+1);
		}
		//�� ���� ������� ������ �� �������
		if(positions.size() && positions.size() % 4 == 0) {//������ 4 [{"name":"value"}]
			for(itMap = colWidth.begin(); itMap != colWidth.end(); ++itMap) {
				pos = (*itStr).find(itMap->first, 0);//������� ������� ������� � ������
				if(pos != string::npos) {
					for(int i = 0; i < positions.size(); i++) {//� ������ � ��������� ����� ������ ������� ������� (�� 1 ������)
						if(positions[i] == pos - 1) {
							currWidth = positions[i+3] - positions[i+2] - 1;//������� ������ �������
							if(itMap->second > currWidth) {
								spaces = itMap->second - currWidth;//������� ������ ��� ���������
								(*itStr).insert(positions[i+3], spaces, ' ');//������ ������
								for(int j = i + 3; j < positions.size(); j++) {
									positions[j] += spaces;//�������� ������� ����� ������
								}
							}
						}
					}
				}
			}
		}
	}
}
/*
//�������������� �������
void Normalize::ModifyLengthCol(std::vector<std::string> &cont)
{
	int amount, cur = 0;
	int sizeMas = cont.size();
	vector<int> blockLengthMas;//������ � ������� �������
	vector<int> strLengthMas;
	blockLengthMas.clear();
	for(cur = 0; cur < sizeMas; cur++)
	{
		strLengthMas.clear();
		GetMasLengthCurStr(cont[cur], strLengthMas);
		if(strLengthMas.size() > 0)
		{
			if(strLengthMas.size() == blockLengthMas.size())
			{
				MergeMas(blockLengthMas, strLengthMas);
				amount++;
			}
			else
			{
				if(blockLengthMas.size())
					ModifyLengthBlockCol(cont, blockLengthMas, cur, amount);
				blockLengthMas.clear();
				MergeMas(blockLengthMas, strLengthMas);
				amount = 1;
			}
		}
		else
		{
			if(blockLengthMas.size() > 1) {
				ModifyLengthBlockCol(cont, blockLengthMas, cur, amount);
			}
			blockLengthMas.clear();
			amount = 0;
		}
	}
}
*/
/*
//��������� ������ ������� � �������� ������
//����� ���������� �� ����� ������ "
void Normalize::GetMasLengthCurStr(std::string str, std::vector<int> &strLengthMas)
{
	std::string delims = "\",\"";
	std::size_t current, previous = 0;
	std::size_t lengthDelim = delims.length();
	int numCol = 0;
	current, previous = 0;
	current = str.find(delims);
	numCol = 0;
	while (current != std::string::npos) {
		if(numCol >= strLengthMas.size())
			strLengthMas.push_back(current - previous);
		else {
			if((current - previous) > strLengthMas[numCol])
				strLengthMas[numCol] = (current - previous);
		}
		previous = (current + lengthDelim);
		current = str.find(delims, previous);
		numCol++;
	}
}
*/
/*
//�� ��������� ������ ������� ������� ������ � �������� ����� ����� �������� ����������
void Normalize::MergeMas(std::vector<int> &blockLengthMas, std::vector<int> &strLengthMas)
{
	int sizeBlockMas = blockLengthMas.size();
	int sizeStrMas = strLengthMas.size();
	if(sizeBlockMas < sizeStrMas)
	{
		blockLengthMas.resize(sizeStrMas, 0);
		sizeBlockMas = blockLengthMas.size();
	}
	for(int i = 0; i < sizeBlockMas; i++)
	{
		if(i >= sizeStrMas)
			break;
		if(blockLengthMas[i] < strLengthMas[i])
			blockLengthMas[i] = strLengthMas[i];
	}
}
*/
/*
//�������� ������ �������
//����:
//cont - ������ ����� (����)
//blockLengthMas - ������ � ������� �������
//cur - ������ (������� ������� � ������� �����)
//amount - ���������� ����� ������� �������������� (������� �� �������������, ������ amount ����������)
void Normalize::ModifyLengthBlockCol(std::vector<std::string> &cont, std::vector<int> &blockLengthMas, int cur, int amount)
{
	std::string delims = "\",\"";
	std::size_t current, previous = 0;//������� � ������
	std::size_t lengthDelim = delims.length();
	int start = cur - amount; //������� � �������
	int delta = 0;
	int numCol = 0;
	if(start >= 0)
	{
		for (int i = start; i < cur; i++)
		{
			current, previous = 0;
			current = cont[i].find(delims);
			numCol = 0;
			while (current != std::string::npos) {
				if(numCol < blockLengthMas.size()) {
					delta = blockLengthMas[numCol] - (current - previous);
					if(delta > 0) {
						cont[i].insert(current, delta, ' ');
						current += delta;
					}
				}
				previous = (current + lengthDelim);
				current = cont[i].find(delims, previous);
				numCol++;
			}
		}
	}
}
*/
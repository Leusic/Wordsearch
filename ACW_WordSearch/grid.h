#pragma once
#include <fstream>
#include <iostream>
#include <typeinfo>
#include <string>
#include <vector>
using namespace::std;

class Grid
{
	friend class WordSearch;
	vector<vector<char>> m_grid;
	vector<string> foundWordsData;
	int gridCellsVisited = 0;
	int wsSize;
	int dictEntriesVisited = 0;

public:

	explicit Grid();
	~Grid();
	void addWords(vector<int>& main, vector<int> &secondary) const;
	void LoadGrid(const char filename[]);
	vector<int> gridPositionCheck(int x, int y, vector<string> &dictionary);
	vector<int> directionCheck(int x, int y, int directionX, int directionY, vector<string> &dictionary, vector<char> &currentChars);
	int dictCheck(vector<char> &currentChars, vector<string> &dictionary);
	vector<int> cellCheck(int x, int y, int directionX, int directionY, string &word, int index, int matches, vector<string> &dictionary, vector<vector<int>> &dictStemWords);
};
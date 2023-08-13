#pragma once
#include <fstream>
#include <iostream>
#include <vector>
#include <functional>
using namespace::std;
#include "grid.h"

class WordSearch {

	friend class Grid;
	const char* puzzleName = "wordsearch_grid.txt";
	const char* dictionaryName = "dictionary.txt";
	vector<string> simpleDict;
	vector<int> foundWords;
	vector<int> unfoundWords;
	string fileName;
	vector<vector<int>> dictStemWords;
	vector<string> foundWordsData;
	int gridCellsVisited = 0;
	int dictEntriesVisited = 0;
	int wsSize;
	Grid wsGrid;

public:

	explicit WordSearch(const char * const filename);
	~WordSearch();
	WordSearch(const WordSearch& a);
	void addWords(vector<int>& main, vector<int> &secondary) const;
	void readSimplePuzzle();
	void readSimpleDictionary();
	void readAdvancedPuzzle();
	void readAdvancedDictionary();
	void solvePuzzleSimple();
	void solvePuzzleAdvanced();
	vector<int> findStems(int startIndex, string &rootWord);
	void writeResults(const double loadTime, const double solveTime) const;

	WordSearch& operator = (const WordSearch& ws) {
		fileName = ws.fileName;
		wsSize = ws.wsSize;
		puzzleName = ws.puzzleName;
		dictionaryName = ws.dictionaryName;
		simpleDict = ws.simpleDict;
		foundWords = ws.foundWords;
		unfoundWords = ws.unfoundWords;
		dictStemWords = ws.dictStemWords;
		foundWordsData = ws.foundWordsData;
		gridCellsVisited = ws.gridCellsVisited;
		dictEntriesVisited = ws.dictEntriesVisited;
		wsGrid = ws.wsGrid;
		return *this;
	}
};


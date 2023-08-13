#include "WordSearch.h"
#include "grid.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <algorithm>
using namespace std;

bool alphaComp(const string &a, const string &b) {
	return a < b;
}

WordSearch::WordSearch(const char* const filename) {
	fileName = filename;
	wsSize = 0;
}

WordSearch::WordSearch(const WordSearch& a) {
	fileName = a.fileName;
	wsSize = a.wsSize;
}

WordSearch::~WordSearch() {
}

//function for combining the contents of two vector<int> objects
void WordSearch::addWords(vector<int> &main, vector<int> &secondary) const {
	for (int i = 0; i < secondary.size(); i++) {
		main.push_back(secondary[i]);
	}
}

void WordSearch::readSimplePuzzle() {
	wsGrid.LoadGrid(puzzleName); 
	wsSize = wsGrid.wsSize;
}

void WordSearch::readSimpleDictionary() {
	ifstream fin(dictionaryName);
	string str;
	while(getline(fin, str)) {
		simpleDict.push_back(str);
	}
	fin.close();
}

void WordSearch::readAdvancedPuzzle() {
	//same as simplePuzzle
	wsGrid.LoadGrid(puzzleName);
	wsSize = wsGrid.wsSize;
}

void WordSearch::readAdvancedDictionary() {
	//reads words into the dictionary vector<string>
	ifstream fin(dictionaryName);
	string str;
	while (getline(fin, str)) {
		simpleDict.push_back(str);
	}
	fin.close();
	//alphabetically sorts the dictionary, estimated dict entry visits stores the estimated amount of dictionary comparisons that will be performed by
	//the sort function based on it having a O(N(log(N)) complexity.
	sort(simpleDict.begin(),simpleDict.end(),alphaComp);
	const int estimatedDictEntryVisits = simpleDict.size() * (log(simpleDict.size()));
	wsGrid.dictEntriesVisited += estimatedDictEntryVisits;
	//goes through each word in the dictionary and measures the amount of matching characters between each word and the word
	//in front of it, if the number of matching characters equals the length of the first word, then all the words which stem
	//off from it are found and stored in dictStemWords at the index of the first word.
	const vector<int> nullStems;
	for (int i = 0; i < simpleDict.size() - 1; i++) {
		int matches = 0;
		for (int z = 0; z < simpleDict[i].length(); z++) {
			wsGrid.dictEntriesVisited++;
			if (simpleDict[i+1].length() >= z) {
				wsGrid.dictEntriesVisited++;
				if (simpleDict[i].at(z) == simpleDict[i + 1].at(z)) {
					matches++;
				}
				else {
					break;
				}
			}
			else {
				break;
			}
		}
		wsGrid.dictEntriesVisited++;
		if (matches == simpleDict[i].length()) {
			vector<int> stemWords = findStems(i + 2, simpleDict[i]);
			stemWords.push_back(i + 1);
			dictStemWords.push_back(stemWords);
		}
		else {
			dictStemWords.push_back(nullStems);
		}
	}
	dictStemWords.push_back(nullStems);
}


//finds the words which stem off from a root word.
vector<int> WordSearch::findStems(int startIndex, string &rootWord) {
	vector<int> stemWords;
	for (startIndex; startIndex < simpleDict.size(); startIndex++) {
		int matches = 0;
		for (int z = 0; z < rootWord.length(); z++) {
			wsGrid.dictEntriesVisited++;
			if (simpleDict[startIndex].length() >= z) {
				wsGrid.dictEntriesVisited++;
				if (simpleDict[startIndex].at(z) == rootWord.at(z)) {
					matches++;
				}
				else {
					return stemWords;
				}
			}
			else {
				return stemWords;
			}
		}
		if (matches == rootWord.length()) {
			stemWords.push_back(startIndex);
		}
		else {
			return stemWords;
		}
	}
	return stemWords;

}

void WordSearch::solvePuzzleSimple() {
	//solves the wordsearch by going in every direction from every grid position and comparing all character sequences
	//against the remaining words in the dictionary.
	for (int y = 0; y < wsSize; y++) {
		for (int x = 0; x < wsSize; x++) {
			addWords(foundWords,wsGrid.gridPositionCheck(x,y,simpleDict));
			if (foundWords.size() > 0) {
			}
		}
	}
	foundWordsData = wsGrid.foundWordsData;
	gridCellsVisited = wsGrid.gridCellsVisited;
	dictEntriesVisited = wsGrid.dictEntriesVisited;
	for (int i = 0; i < simpleDict.size(); i++) {
		dictEntriesVisited++;
		if (simpleDict[i] != " ") {
			unfoundWords.push_back(i);
		}
	}
	//because the dictionary was edited to prevent repeat searching of dictionary entries it now has to be rereead from the file again.
	simpleDict.clear();
	readSimpleDictionary();
}

void WordSearch::solvePuzzleAdvanced() {
	wsGrid.gridCellsVisited = 0;
	wsGrid.dictEntriesVisited = 0;
	//solves the wordsearch by going through each word in the dictionary and then going through every position in the wordsearch grid to find
	//the first letter, upon which it goes in each direction to find the rest of the characters, once a word is found, dictStemWords is checked
	//to see if there are any words stemming off from it which can easily be found.
	for (int d = 0; d < simpleDict.size(); d++) {
		for (int y = 0; y < wsSize; y++) {
			for (int x = 0; x < wsSize; x++) {
				wsGrid.dictEntriesVisited++;
				if (simpleDict[d].length() > 0) {
					wsGrid.dictEntriesVisited++;
					if (wsGrid.m_grid[y][x] == simpleDict[d].at(0)) {
						addWords(foundWords, wsGrid.cellCheck(x, y, 1, 0, simpleDict[d], d, 1, simpleDict, dictStemWords));
						addWords(foundWords, wsGrid.cellCheck(x, y, 1, 1, simpleDict[d], d, 1, simpleDict, dictStemWords));
						addWords(foundWords, wsGrid.cellCheck(x, y, 0, 1, simpleDict[d], d, 1, simpleDict, dictStemWords));
						addWords(foundWords, wsGrid.cellCheck(x, y, -1, 1, simpleDict[d], d, 1, simpleDict, dictStemWords));
						addWords(foundWords, wsGrid.cellCheck(x, y, -1, 0, simpleDict[d], d, 1, simpleDict, dictStemWords));
						addWords(foundWords, wsGrid.cellCheck(x, y, -1, -1, simpleDict[d], d, 1, simpleDict, dictStemWords));
						addWords(foundWords, wsGrid.cellCheck(x, y, 0, -1, simpleDict[d], d, 1, simpleDict, dictStemWords));
						addWords(foundWords, wsGrid.cellCheck(x, y, 1, -1, simpleDict[d], d, 1, simpleDict, dictStemWords));
					}
				}
				else {
					break;
				}
			}
		}
		if (wsGrid.foundWordsData.size() < d) {
			foundWordsData.push_back("");
		}
	}
	foundWordsData = wsGrid.foundWordsData;
	gridCellsVisited = wsGrid.gridCellsVisited;
	dictEntriesVisited = wsGrid.dictEntriesVisited;
	for (int i = 0; i < simpleDict.size(); i++) {
		dictEntriesVisited++;
		if (simpleDict[i] != "") {
			unfoundWords.push_back(i);
		}
	}
	simpleDict.clear();
	readSimpleDictionary();
	sort(simpleDict.begin(), simpleDict.end(), alphaComp);
}

void WordSearch::writeResults(const double loadTime, const double solveTime) const {
	ofstream fout(fileName);
	fout << "NUMBER_OF_WORDS_MATCHED " << foundWords.size() << endl << endl;
	fout << "WORDS_MATCHED_IN_GRID" << endl;
	for (int i = 0; i < foundWords.size(); i++) {
		fout << foundWordsData[i] << " " << simpleDict[foundWords[i]] << endl;
	}
	fout << endl << "WORDS_UNMATCHED_IN_GRID" << endl;
	for (int i = 0; i < unfoundWords.size(); i++) {
		fout << simpleDict[unfoundWords[i]] << endl;
	}
	fout << endl << "NUMBER_OF_GRID_CELLS_VISITED " << gridCellsVisited << endl;
	fout << "NUMBER_OF_DICTIONARY_ENTRIES_VISITED " << dictEntriesVisited << endl;
	fout << "TIME_TO_POPULATE_GRID_STRUCTURE " << loadTime << endl;
	fout << "TIME_TO_SOLVE_PUZZLE " << solveTime << endl;
}
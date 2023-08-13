#include "grid.h"
#include <fstream>
#include <iostream>
#include <typeinfo>
#include <string>
#include <vector>
using namespace::std;

Grid::Grid() {
	wsSize = 0;
}
Grid::~Grid() {

}

//function for adding newly found words to the main found words vector, may or may not be needed.
void Grid::addWords(vector<int>& main, vector<int> &secondary) const {
	for (int i = 0; i < secondary.size(); i++) {
		main.push_back(secondary[i]);
	}
}

void Grid::LoadGrid(const char filename[]) {
	//loads the wordsearch grid by bypassing the size given as first character and then manually finding the size of the first line
	//of the wordsearch by measuring it, then it loads the characters in line by line. It is done this way so that it can process
	//wordsearches of any size.
	ifstream fin(filename);
	fin.ignore(200, '\n'); 
	string firstLine;
	getline(fin, firstLine, '\n');
	firstLine.erase(std::remove(firstLine.begin(), firstLine.end(), ' '), firstLine.end());
	wsSize = firstLine.length();
	//goes to the start of the stream
	fin.seekg(0, fin.beg);
	fin.ignore(200, '\n');
	string line;
	vector<char> lineChars;
	for (int y = 0; y < wsSize; y++) {
		lineChars.clear();
		getline(fin, line);
		line.erase(std::remove(line.begin(), line.end(), ' '), line.end());
		copy(line.begin(), line.end(), back_inserter(lineChars));
		m_grid.push_back(lineChars);
	}
	fin.close();
}

vector<int> Grid::cellCheck(int x, int y, int directionX, int directionY, string &word, const int index, int matches, vector<string> &dictionary, vector<vector<int>> &dictStemWords) {
	//checks if the grid position in a direction matches a given character of a given word from the dictionary, loops until either the character doesn't match or the word
	//has been found, upon which it checks if the current word is a root word and then calls itself to find the possible words that could stem off from it.
	gridCellsVisited++;
	vector<int> foundWords;
	for (matches; matches <= word.length(); matches++) {
		//word has been found
		if (matches >= word.length()) {
			//loop for finding words that stem off.
			for (int i = 0; i < dictStemWords[index].size(); i++) {
				int tempIndex = dictStemWords[index][i];
				dictEntriesVisited++;
				string tempWord = dictionary[tempIndex];
				dictEntriesVisited++;
				vector<int> stemFound = cellCheck(x,y,directionX,directionY,tempWord,tempIndex,matches, dictionary, dictStemWords);
				//if a stemming word is found remove it from the dictionary and from the root words
				if (stemFound.size() > 0) {
					dictStemWords[index].erase(dictStemWords[index].begin() + i);
					dictEntriesVisited++;
					dictionary[tempIndex] = "";
					dictEntriesVisited++;
					addWords(foundWords, stemFound);
				}
			}
			foundWords.push_back(index);
			foundWordsData.push_back(to_string(x) + " " + to_string(y));
			dictEntriesVisited++;
			dictionary[index] = "";
			dictEntriesVisited++;
		}
		//checks for going out of wordsearch bounds and then compares the character found with the character from the word.
		else if (y + (directionY * matches) < wsSize && y + (directionY * matches) >= 0 && x + (directionX * matches) < wsSize && x + (directionX * matches) >= 0){
			if (m_grid[y + (directionY * matches)][x + (directionX * matches)] == word.at(matches)) {
			}
			else {
				break;
			}
		}
		else {
			break;
		}
	}
	return foundWords;


}
//function run on every character in the wordsearch to find any words connected to that character. checks each direction from the given
//position in the grid.
vector<int> Grid::gridPositionCheck(const int x, const int y, vector<string> &dictionary) {
	vector<char> currentChars;
	vector<int> foundWords;
	currentChars.push_back(m_grid[y][x]);
	gridCellsVisited++;
	addWords(foundWords, directionCheck(x, y, 1, 0, dictionary, currentChars)); //right
	currentChars.clear();
	currentChars.push_back(m_grid[y][x]);
	addWords(foundWords, directionCheck(x, y, 1, 1, dictionary, currentChars)); //right down
	currentChars.clear();
	currentChars.push_back(m_grid[y][x]);
	addWords(foundWords, directionCheck(x, y, 0, 1, dictionary, currentChars)); //down
	currentChars.clear();
	currentChars.push_back(m_grid[y][x]);
	addWords(foundWords, directionCheck(x, y, -1, 1, dictionary, currentChars)); //down left
	currentChars.clear();
	currentChars.push_back(m_grid[y][x]);
	addWords(foundWords, directionCheck(x, y, -1, 0, dictionary, currentChars)); //left
	currentChars.clear();
	currentChars.push_back(m_grid[y][x]);
	addWords(foundWords, directionCheck(x, y, -1, -1, dictionary, currentChars)); //up left
	currentChars.clear();
	currentChars.push_back(m_grid[y][x]);
	addWords(foundWords, directionCheck(x, y, 0, -1, dictionary, currentChars)); //up
	currentChars.clear();
	currentChars.push_back(m_grid[y][x]);
	addWords(foundWords, directionCheck(x, y, 1, -1, dictionary, currentChars)); //up right
	currentChars.clear();
	currentChars.push_back(m_grid[y][x]);
	for (int i = 0; i < foundWords.size(); i++) {
		foundWordsData.push_back(to_string(x) + " " + to_string(y));
	}
	return foundWords;
}

//checks every character sequence in one direction with dictCheck() and returns all the words found.
vector<int> Grid::directionCheck(int x, int y,const int directionX,const int directionY, vector<string> &dictionary, vector<char> &currentChars) {
	vector<int> foundWords;
	while (x + directionX < wsSize && y + directionY < wsSize && x + directionX >= 0 && y + directionY >= 0) {
		currentChars.push_back(m_grid[y + directionY][x + directionX]);
		gridCellsVisited++;
		const int dictCheckResult = dictCheck(currentChars, dictionary);
		switch (dictCheckResult){
			case -1:
				break;
			default:
				dictionary[dictCheckResult] = " ";
				dictEntriesVisited++;
				foundWords.push_back(dictCheckResult);
		}
		x += directionX;
		y += directionY;
	}
	return foundWords;
}

//for every character in currentChars and every word in the dictionary, compares the letters and if the same amount of
//letters has matched as there are letters in the word being checked, then return the index of the word that has been found.
int Grid::dictCheck(vector<char> &currentChars, vector<string> &dictionary) {
	for (int i = 0; i < dictionary.size(); i++) {
		int matches = 0;
		for (int z = 0; z < currentChars.size(); z++) {
			try {
				dictEntriesVisited++;
				if (dictionary[i].at(z) == currentChars[z]) {
					matches++;
				}
				else {
					break;
				}
			}
			catch (exception &e){
				break;
			}
		}
		dictEntriesVisited++;
		if (matches == dictionary[i].length()) {
			return i;
		}
	}
	return -1;
}

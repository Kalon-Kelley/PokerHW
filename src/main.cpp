//============================================================================
// Name        : main.cpp
// Author      : Lucas Kelley
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <bits/stdc++.h>
#include <string>
#include "poker.h"
using namespace std;

int main() {
	char command;
	int numPlayers;
	vector<string> names;
	while (true) {
		cout << "Do you wish to play poker or quit (p or q)? ";
		cin >> command;
		if (command == 'q')
			break;
		cout << "How many players are playing? ";
		cin >> numPlayers;
		for (int i = 0; i < numPlayers; i++) {
			string name;
			cout << "Enter player name: ";
			cin >> name;
			names.push_back(name);
		}
		PokerGame *game = new PokerGame;
		game->playGame(names);
		names.clear();
	}
	return 0;
}

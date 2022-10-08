#ifndef POKER_H
#define POKER_H

#include <iostream>
#include <bits/stdc++.h>
#include <string>
#include <algorithm>

using namespace std;

const int N_CARDS_IN_HAND = 5;
const int N_CARDS_IN_DECK = 52;

class Hand;
class HandCategory;

class PokerGame {
public:
	PokerGame();
	void playGame(vector<string> names);
private:
	vector<Hand*> playerHands;
	vector<HandCategory*> handCategories;
};

class HandCategory {
public:
	virtual int testHand(Hand* hand) = 0;
	string toString();
	vector<int> rankVector(Hand* hand);
	vector<int> suitVector(Hand* hand);
	virtual ~HandCategory() = 0;
protected:
	string category;
};

class OnePair : public HandCategory {
public:
	OnePair();
	int testHand(Hand* hand);
};

class TwoPair : public OnePair {
public:
	TwoPair();
	int testHand(Hand* hand);
};

class ThreeOfAKind : public OnePair {
public:
	ThreeOfAKind();
	int testHand(Hand* hand);
};

class Straight : public HandCategory {
public:
	Straight();
	int testHand(Hand* hand);
};

class Flush : public HandCategory {
public:
	Flush();
	int testHand(Hand* hand);
};

class FullHouse : public ThreeOfAKind {
public:
	FullHouse();
	int testHand(Hand* hand);
};

class FourOfAKind : public ThreeOfAKind {
public:
	FourOfAKind();
	int testHand(Hand* hand);
};

class StraightFlush : public Straight {
public:
	StraightFlush();
	int testHand(Hand* hand);
};

class RoyalFlush : public StraightFlush {
public:
	RoyalFlush();
	int testHand(Hand* hand);
};

class Card {
public:
	Card(int suit, int rank);
	int suit;
	int rank;
	string toString();
};

class Deck {
public:
	Deck();
	vector<Card*> cards;
	void shuffle();
	void deal(vector<Hand*>& playerHands);
};

class Hand {
public:
	void orderCards();
	void dealCard(Card* card);
	Hand(string playerName);
	string playerName;
	vector<Card*> cards;
};

#endif

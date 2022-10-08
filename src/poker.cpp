#include "poker.h"
#include <algorithm>
#include <ctime>
#include <cstdlib>

using namespace std;

PokerGame::PokerGame() {
	handCategories.push_back(new RoyalFlush());
	handCategories.push_back(new StraightFlush());
	handCategories.push_back(new FourOfAKind());
	handCategories.push_back(new FullHouse());
	handCategories.push_back(new Flush());
	handCategories.push_back(new Straight());
	handCategories.push_back(new ThreeOfAKind());
	handCategories.push_back(new TwoPair());
	handCategories.push_back(new OnePair());
}

void PokerGame::playGame(vector<string> names) {
	Deck deck;
	deck.shuffle();
	for (string name : names) {
		Hand* hand = new Hand(name);
		playerHands.push_back(hand);
	}
	deck.deal(playerHands);
	Hand* highHand = NULL;
	int highCategory, highRank;
	for (Hand* hand : playerHands) {
		hand->orderCards();
		int i = 9;
		int rank;
		cout << hand->playerName << "'s hand:" << endl;
		for (Card* card : hand->cards) {
			cout << card->toString() << endl;
		}
		for (HandCategory* category : handCategories) {
			rank = category->testHand(hand);
			if (rank != -1) {
				cout << hand->playerName << " has " << category->toString() << endl << endl;
				break;
			}
			i--;
			if (i == 0) {
				cout << hand->playerName << " has nothing" << endl << endl;
				rank = hand->cards[4]->rank;
			}
		}

		if (highHand == NULL || i > highCategory || (i == highCategory && rank > highRank)) {
			highHand = hand;
			highCategory = i; highRank = rank;
		}
	}
	cout << highHand->playerName << " is the winner!" << endl;
}

Card::Card(int suit, int rank) {
	this->suit = suit;
	this->rank = rank;
}

string Card::toString() {
	string final;
	string suit;
	string rank;
	if (this->suit == 0) {
		suit = "clubs";
	} else if (this->suit == 1) {
		suit = "diamonds";
	} else if (this->suit == 2) {
		suit = "hearts";
	} else if (this->suit == 3) {
		suit = "spades";
	}

	switch(this->rank) {
	case 0:
		rank = "2"; break;
	case 1:
		rank = "3"; break;
	case 2:
		rank = "4"; break;
	case 3:
		rank = "5"; break;
	case 4:
		rank = "6"; break;
	case 5:
		rank = "7"; break;
	case 6:
		rank = "8"; break;
	case 7:
		rank = "9"; break;
	case 8:
		rank = "10"; break;
	case 9:
		rank = "jack"; break;
	case 10:
		rank = "queen"; break;
	case 11:
		rank = "king"; break;
	case 12:
		rank = "ace"; break;
	}
	final = rank + " of " + suit;
	return final;
}

Deck::Deck() {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 13; j++) {
			Card* card = new Card(i,j);
			cards.push_back(card);
		}
	}
}

void Deck::shuffle() {
	srand(unsigned(time(0)));
	random_shuffle(cards.begin(), cards.end());
}

void Deck::deal(vector<Hand*>& playerHands){
	for (int i = 0; i < N_CARDS_IN_HAND; i++) {
		for (Hand* playerHand : playerHands) {
			playerHand->dealCard(cards.back());
			cards.pop_back();
		}
	}
}

void Hand::dealCard(Card* card) {
	cards.push_back(card);
}

Hand::Hand(string playerName) {
	this->playerName = playerName;
}

bool sortFunction (Card* i, Card* j) { return (i->rank<j->rank); }

void Hand::orderCards() {
	sort(cards.begin(), cards.end(), sortFunction);
}

HandCategory::~HandCategory(){
}

vector<int> HandCategory::rankVector(Hand* hand) {
	vector<int> ranks;
	for (int i = 0; i < N_CARDS_IN_HAND; i++) {
		ranks.push_back(hand->cards[i]->rank);
	}
	return ranks;
}

vector<int> HandCategory::suitVector(Hand* hand) {
	vector<int> suits;
	for (int i = 0; i < N_CARDS_IN_HAND; i++) {
		suits.push_back(hand->cards[i]->suit);
	}
	return suits;
}

string HandCategory::toString() {
	return category;
}

OnePair::OnePair() {
	category = "1 Pair";
}

int OnePair::testHand(Hand* hand) {
	category = "One Pair";
	for (int i = 0; i < N_CARDS_IN_HAND-1; i++) {
		if (hand->cards[i]->rank == hand->cards[i+1]->rank) {
			return hand->cards[i]->rank;
		}
	}
	return -1;
	// if unique rank vector has 4 unique ranks there is a one pair
}

TwoPair::TwoPair() {
	category = "2 Pair";
}

int TwoPair::testHand(Hand* hand) {
	vector<int> ranks = rankVector(hand);
	vector<int>:: iterator it;
	vector<int> unique(5);
	it = unique_copy(ranks.begin(), ranks.end(), unique.begin());
	unique.resize(distance(unique.begin(), it));
	if (unique.size() == 3) {
		int prevHigh = -1;
		for (int i = 0; i < unique.size(); i++) {
			if (count(ranks.begin(), ranks.end(), unique[i]) == 2) {
				if (unique[i] > prevHigh) {
					prevHigh = unique[i];
				}
			}
		}
		return prevHigh;
	}
	return -1;
}

ThreeOfAKind::ThreeOfAKind() {
	category = "3 of a kind";
}

int ThreeOfAKind::testHand(Hand* hand) {
	vector<int> ranks = rankVector(hand);
	vector<int>:: iterator it;
	vector<int> unique(5);
	it = unique_copy(ranks.begin(), ranks.end(), unique.begin());
	unique.resize(distance(unique.begin(), it));
	if (unique.size() == 3) {
		for (int i = 0; i < unique.size(); i++) {
			if (count(ranks.begin(), ranks.end(), unique[i]) == 3) {
				return unique[i];
			}
		}
	}
	return -1;
}

Straight::Straight() {
	category = "Straight";
}

int Straight::testHand(Hand* hand) {
	for (int i = 0; i < N_CARDS_IN_HAND-1; i++) {
		if (hand->cards[i]->rank+1 != hand->cards[i+1]->rank) {
			return -1;
		}
	}
	return hand->cards[4]->rank;
}

Flush::Flush() {
	category = "Flush";
}

int Flush::testHand(Hand* hand) {
	vector<int> suits = suitVector(hand);
	vector<int>:: iterator it;
	vector<int> unique(4);
	it = unique_copy(suits.begin(), suits.end(), unique.begin());
	unique.resize(distance(unique.begin(), it));
	if (unique.size() == 1) {
		return hand->cards[4]->rank;
	} else {
		return -1;
	}
}

FullHouse::FullHouse() {
	category = "Full House";
}

int FullHouse::testHand(Hand* hand) {
	vector<int> ranks = rankVector(hand);
	vector<int>:: iterator it;
	vector<int> unique(5);
	it = unique_copy(ranks.begin(), ranks.end(), unique.begin());
	unique.resize(distance(unique.begin(), it));
	if (unique.size() == 2 && (count(ranks.begin(),ranks.end(), unique[0]) == 3 || count(ranks.begin(),ranks.end(), unique[0]) == 2)) {
		int nCount = count(ranks.begin(), ranks.end(), unique[0]);
		if (nCount == 3) {
			return unique[0];
		} else {
			return unique[1];
		}
	} else {
		return -1;
	}
}

FourOfAKind::FourOfAKind() {
	category = "4 of a kind";
}

int FourOfAKind::testHand(Hand* hand) {
	vector<int> ranks = rankVector(hand);
	vector<int>:: iterator it;
	vector<int> unique(5);
	it = unique_copy(ranks.begin(), ranks.end(), unique.begin());
	unique.resize(distance(unique.begin(), it));
	if (unique.size() == 2) {
		for (int i = 0; i < unique.size(); i++) {
			int uniqueCount;
			uniqueCount = count(ranks.begin(), ranks.end(), unique[i]);
			if (uniqueCount == 4) {
				return unique[i];
			}
		}
	}
	return -1;
}

StraightFlush::StraightFlush() {
	category = "Straight flush";
}

int StraightFlush::testHand(Hand* hand) {
	for (int i = 0; i < N_CARDS_IN_HAND-1; i++) {
		if (hand->cards[i]->rank+1 != hand->cards[i+1]->rank) {
			return -1;
		}
		if (hand->cards[i+1]->suit != hand->cards[i]->suit) {
			return -1;
		}
	}
	return hand->cards[4]->rank;
}

RoyalFlush::RoyalFlush() {
	category = "Royal flush";
}

int RoyalFlush::testHand(Hand* hand) {
	for (int i = 0; i < N_CARDS_IN_HAND-1; i++) {
		if (hand->cards[i]->rank+1 != hand->cards[i+1]->rank) {
			return -1;
		}
		if (hand->cards[i+1]->suit != hand->cards[i]->suit) {
			return -1;
		}
	}
	if (hand->cards[4]->rank != 12) {
		return -1;
	}
	return hand->cards[4]->rank;
}

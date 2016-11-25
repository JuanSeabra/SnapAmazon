#ifndef REVIEW_H
#define REVIEW_H

#include <iostream>
#include <vector>
#include "product.h"

using namespace std;

class Review{
public:
	Review(string id, string date, double rating, int votes, int helpful);
	void setDate(string date);
	void setId(string id);
	void setRating(double rating);
	void setVotes(int votes);
	void setHelpful(int helpful);
	void setASIN(string ASIN);
	void setId_cliente(string id_cliente);

	string getDate();
	string getId();
	double getRating();
	int getVotes();
	int getHelpful();
	string getASIN();
	string getId_cliente();

private:
	string date;
	string id;
	double rating;
	int votes;
	int helpful;
	string ASIN; // produto
	string id_cliente; // cliente
};

#endif

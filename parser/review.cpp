#include "review.h"

Review::Review(string id, 
		string date, 
		double rating, 
		int votes, 
		int helpful){

	setId(id);
	setDate(date);
	setRating(rating);
	setVotes(votes);
	setHelpful(helpful);
}

void Review::setDate(string date){
	this->date = date;
}

void Review::setId(string id){
	this->id = id;
}

void Review::setRating(double rating){
	this->rating = rating;
}

void Review::setVotes(int votes){
	this->votes = votes;
}

void Review::setHelpful(int helpful){
	this->helpful = helpful;
}

void Review::setASIN(string ASIN){
	this->ASIN = ASIN;
}

void Review::setId_cliente(string id_cliente){
	this->id_cliente = id_cliente;
}

string Review::getDate(){
	return this->date;
}

string Review::getId(){
	return this->id;
}

double Review::getRating(){
	return this->rating;
}

int Review::getVotes(){
	return this->votes;
}

int Review::getHelpful(){
	return this->helpful;
}

string Review::getASIN(){
	return this->ASIN;
}

string Review::getId_cliente(){
	return this->id_cliente;
}

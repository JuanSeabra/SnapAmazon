#include "product.h"

/*
Product::Product(string ASIN, string title, string group, int ranking){
	setASIN(ASIN);
	setTitle(title);
	setGroup(group);
	setSalesRank(ranking);
}

Product::Product(){

}
*/

void Product::setASIN(string ASIN){
	this->ASIN = ASIN;
}

void Product::setTitle(string title){
	this->title = title;
}

void Product::setGroup(string group){
	this->group = group;
}

void Product::setSalesRank(int ranking){
	this->salesRank = ranking;
}

string Product::getASIN(){
	return this->ASIN;
}

string Product::getTitle(){
	return this->title;
}

string Product::getGroup(){
	return this->group;
}

int Product::getSalesRank(){
	return this->salesRank;
}

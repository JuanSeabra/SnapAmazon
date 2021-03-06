#ifndef PRODUCT_H
#define PRODUCT_H

#include <iostream>
#include <vector>

using namespace std;

class Product{
public:
	//Product(string ASIN, string title, string group, int ranking);
	Product();
	void setASIN(string ASIN);
	void setTitle(string title);
	void setGroup(string group);
	void setSalesRank(int ranking);
	void setValid();
	string getASIN();
	string getTitle();
	string getGroup();
	int getSalesRank();
	bool isValid();

private:
	string ASIN;
	string title;
	string group;
	int salesRank;
	bool valid;
};

#endif

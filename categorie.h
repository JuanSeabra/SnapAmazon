#ifndef CATEGORIE_H
#define CATEGORIE_H

#include <iostream>
#include <vector>

using namespace std;

class Categorie{
public:
	Categorie(string name, int id, int super_id);
	void setName(string name);
	void setId(int id);
	void setSuper_id(int super_id);
	string getName();
	int getId();
	int getSuper_id();

private:
	string name;
	int id;
	int super_id;
};

#endif

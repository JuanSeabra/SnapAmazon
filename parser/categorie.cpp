#include "categorie.h"

Categorie::Categorie(string name, int id, int super_id){
	setName(name);
	setId(id);
	setSuper_id(super_id);
}

void Categorie::setName(string name){
	this->name = name;
}

void Categorie::setId(int id){
	this->id = id;
}

void Categorie::setSuper_id(int super_id){
	this->super_id = id;
}

string Categorie::getName(){
	return this->name;
}

int Categorie::getId(){
	return this->id;
}

int Categorie::getSuper_id(){
	return this->super_id;
}

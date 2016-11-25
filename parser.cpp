#include "categorie.h"
#include "review.h"
#include "product.h"
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <boost/algorithm/string.hpp>

using namespace std;
//using namespace boost;

int main(int argc, const char *argv[])
{
	ifstream amazon_meta;
	string linha;
	vector<string> palavras;
	int aux=0, i_aux;
	string title_aux;
	Product produto;
	amazon_meta.open("../amazon-meta.txt"); 
	while (getline(amazon_meta,linha)){
		//cout << linha << endl;
		boost::split(palavras, linha, boost::is_any_of(" []|"));
		for (int i = 0; i < palavras.size(); i++) {
			/***removendo palavras vazias****/
			if(palavras[i].empty()){
				palavras[i].erase();
			} else{

				/*****Pegando ASIN*****/
				if( palavras[i] == "ASIN:"){
					produto.setASIN(palavras[i+1]);
					cout << "ASIN!!!!: " << produto.getASIN() << endl;
				}

				/******Pegando Title*****/

				if( palavras[i] == "title:"){
					title_aux = "";
					for(i_aux = i+1 ; i_aux < palavras.size(); i_aux++){
						if (i_aux == palavras.size()-1) {
							title_aux += (palavras[i_aux]);
						}
						else title_aux += (palavras[i_aux] + " ");
					}
					produto.setTitle(title_aux);
					cout << "TITLE!!!:" <<  produto.getTitle() << endl;
				}

				/******Pegando group******/
				if(palavras[i] == "group:"){
					produto.setGroup(palavras[i+1]);
					cout << "GROUP!!!!: " << produto.getGroup() << endl;;
				}

				/*******Pegando Salesrank****/
				if(palavras[i] == "salesrank:"){
					produto.setSalesRank(stoi(palavras[i+1]));
					cout << "SALESRANK!!!: " << produto.getSalesRank() << endl;
				}
			}
		}
		palavras.clear();
		linha.clear();
		aux++;
		if(aux==20) break;
	}
	return 0;
}

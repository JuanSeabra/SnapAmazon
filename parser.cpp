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

void parse(const char *nome_arquivo){
	ifstream amazon_meta;
	string linha;
	vector<string> palavras;
	int aux=0, i_aux;
	int similar_count;
	string title_aux;
	Product produto;
	amazon_meta.open(nome_arquivo); 
	unordered_multimap<string,string> similares;
	vector<Categorie> categ;

	if (!amazon_meta.is_open()) exit(1);
	while (getline(amazon_meta,linha)){
		//cout << linha << endl;
		boost::split(palavras, linha, boost::is_any_of(" []|"));

		for (int i = 0; i < palavras.size(); i++) {
			//if(palavras[i].empty()) cout << "PALAVRA VAZIA!" << endl; 
			/*****Pegando ASIN*****/
			if( palavras[i] == "ASIN:"){
				produto.setASIN(palavras[i+1]);
				cout << "--------------------------------------------" << endl;
				cout << "ASIN!!!!!: " << produto.getASIN() << endl;
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
				cout << "TITLE!!!!: " <<  produto.getTitle() << endl;
			}

			/******Pegando group******/
			if(palavras[i] == "group:"){
				produto.setGroup(palavras[i+1]);
				cout << "GROUP!!!!: " << produto.getGroup() << endl;;
			}

			/*******Pegando Salesrank****/
			if(palavras[i] == "salesrank:"){
				produto.setSalesRank(stoi(palavras[i+1]));
				cout << "SALESRANK: " << produto.getSalesRank() << endl;
				produto.setValid();
			}

			if(produto.isValid()){
				cout << "INSERE PRODUTO NO BANCO" << endl;
				produto.setValid();
			}

			/*****Pegando similares*******/
			if(palavras[i] == "similar:"){
				cout <<"Numero de similares: "<< palavras[i+1] << endl;
				similar_count = stoi(palavras[i+1]);
				int j = 1;
				while (similar_count) {
					string asin_similar = palavras[i+1+j];
					if (!asin_similar.empty()) {
						cout << "similar " << similar_count << " " << asin_similar << endl;
						pair <string, string> par_ (produto.getASIN(), asin_similar);
						similares.insert(par_);
						similar_count--;
					}
					j++;
				}
			}

			/******Pegando categorias******/
			

		}
		palavras.clear();
		linha.clear();
		aux++;
		if(aux==100) break;
	}
}

int main(int argc, const char *argv[])
{
	if(argc != 2) {
		cout << "uso: ./parse <arquivo>" << endl;
		exit(1);
	}
	cout << "ARQUIVO " << argv[1] << endl;
	parse(argv[1]);
	return 0;
}

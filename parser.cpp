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

void trocaAspas(string &str){
	for(int j = 0; j < str.size(); j++){
		if(str[j] == '\''){
			str.insert(j,"\\");
			j++;
		}
	}

}

void parse(const char *nome_arquivo){
	ifstream amazon_meta;
	string linha;
	vector<string> palavras;
	int aux=0, i_aux;
	int similar_count;
	int categ_count;
	string title_aux;
	Product produto;
	Categorie categoria;
	Review comentario;
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
				//setando asin para comentarios (usado mais tarde)
				comentario.setASIN(palavras[i+1]);
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
				trocaAspas(title_aux);
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
			//TODO ler categorias
			//diferenciar categorias sem nome
			//
			if(palavras[i] == "categories:"){
				categ_count = stoi(palavras[i+1]);
				int super_cat;
				int id_categ;
				for (int i = 0; i < categ_count; i++) {
					getline(amazon_meta, linha);
					super_cat = 0;
					boost::split(palavras, linha, boost::is_any_of("[]|"));
					for (int j = 1; j < palavras.size()-1; j++) {
						if(palavras[j].empty() && j == 1){
							//se a posição 1 é vazia logo a categoria raiz é sem nome
							//string nome_aux = palavras[j];
							//trocaAspas(nome_aux);

							j++;
							while(palavras[j].empty()){
								j++;
							}

							id_categ = stoi(palavras[j]);

							cout << j <<" categoria: -sem nome- id: " << id_categ << " super categ: " << super_cat << endl;

							super_cat = id_categ;
							j++;
						} else if(!palavras[j].empty()) {
							string nome_aux = palavras[j];
							trocaAspas(nome_aux);

							j++;
							while(palavras[j].empty()){
								j++;
							}

							id_categ = stoi(palavras[j]);
							cout << j << " categoria: " << nome_aux << " id: " << id_categ << " super categ: " << super_cat << endl;
							super_cat = id_categ;
							j++;
						}
					}

				}
			}	

			if (palavras[i] == "reviews:"){
				int downloaded;
				//celula 7 recebe o valor de downloaded no split
				downloaded = stoi(palavras[7]);
				cout << "downloaded: " << downloaded << endl;

				for (int i = 0; i < downloaded; i++) {
					getline(amazon_meta, linha);
					boost::split(palavras, linha, boost::is_any_of(" "));
					comentario.setDate(palavras[4]);
					for (int j = 5; j < palavras.size(); j++) {
						if(!palavras[j].empty()){
							if(palavras[j] == "cutomer:"){
								j++;
								while(palavras[j].empty()){
									j++;
								}
								comentario.setId_cliente(palavras[j]);
							}
							if(palavras[j] == "rating:"){
								j++;
								while(palavras[j].empty()){
									j++;
								}
								comentario.setRating(stod(palavras[j]));
							}
							if(palavras[j] == "votes:"){
								j++;
								while(palavras[j].empty()){
									j++;
								}
								comentario.setVotes(stoi(palavras[j]));
							}
							if(palavras[j] == "helpful:"){
								j++;
								while(palavras[j].empty()){
									j++;
								}
								comentario.setHelpful(stoi(palavras[j]));
							}

						}
					}
					cout << "data: " << comentario.getDate() <<
						" cutomer: " << comentario.getId_cliente() <<
						" nota: " << comentario.getRating() <<
						" votos: " << comentario.getVotes() <<
						" ajudou: " << comentario.getHelpful() << endl;
				}
			}

		}
		palavras.clear();
		linha.clear();
		aux++;
		if(aux==400) break;
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

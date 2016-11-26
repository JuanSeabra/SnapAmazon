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
#include <pqxx/pqxx>

using namespace std;
using namespace pqxx;
//using namespace boost;

void trocaAspas(string &str){
	for(int j = 0; j < str.size(); j++){
		if(str[j] == '\''){
			str.insert(j,"\'");
			j++;
		}
	}
}

void removeR(string &str) {
	if (!str.empty() && str[str.size()-1] == '\r')
		str.erase(str.size()-1);
}


void criacao_relacoes(connection& C) {
	string sql;
	work W(C);

	//Relacao produto
	sql = "CREATE TABLE IF NOT EXISTS produto(" \
		"ASIN 		VARCHAR(15)  NOT NULL," \
		"titulo		TEXT NOT NULL," \
		"grupo 		TEXT NOT NULL," \
		"posicao_rank 	INT NOT NULL," \
		"PRIMARY KEY (ASIN) );";
	
	W.exec(sql);	
	cout << "Relacao criada com sucesso: Produto" << endl;

	//Relacao Similar
	sql = "CREATE TABLE IF NOT EXISTS similares(" \
		"ASIN 			VARCHAR(15)  NOT NULL," \
		"ASIN_Similar	VARCHAR(15)  NOT NULL," \
		"PRIMARY KEY (ASIN, ASIN_Similar)," \
		"FOREIGN KEY (ASIN) REFERENCES produto(ASIN)," \
		"FOREIGN KEY (ASIN_Similar) REFERENCES produto(ASIN));";

	W.exec(sql);	
	cout << "Relacao criada com sucesso: Similar" << endl;

	//Relacao Categoria
	sql = "CREATE TABLE IF NOT EXISTS categoria(" \
		"cod 		INT  NOT NULL," \
		"nome		TEXT," \
		"super_categoria INT," \
		"PRIMARY KEY (cod)," \
		"FOREIGN KEY (super_categoria) REFERENCES categoria(cod));";

	W.exec(sql);
	cout << "Relacao criada com sucesso: Categoria" << endl;
	
	//Relacao Produto_Categoria
	sql = "CREATE TABLE IF NOT EXISTS produto_categoria(" \
		"ASIN 	VARCHAR(15)  NOT NULL," \
		"cod	INT  NOT NULL," \
		"PRIMARY KEY (ASIN, cod)," \
		"FOREIGN KEY (ASIN) REFERENCES produto(ASIN)," \
		"FOREIGN KEY (cod) REFERENCES categoria(cod));";

	W.exec(sql);
	cout << "Relacao criada com sucesso: Produto_Categoria" << endl;

	//Relacao Comentario
	sql = "CREATE TABLE IF NOT EXISTS comentario(" \
		"ID 		SERIAL  NOT NULL," \
		"data		DATE NOT NULL," \
		"classif	INT NOT NULL," \
		"votos		INT NOT NULL," \
		"util		INT NOT NULL," \
		"produto	VARCHAR(15) NOT NULL," \
		"cliente	VARCHAR(15) NOT NULL," \
		"PRIMARY KEY (ID)," \
		"FOREIGN KEY (produto) REFERENCES produto(ASIN));";
	W.exec(sql);
	cout << "Relacao criada com sucesso: Comentario" << endl;

	W.commit();
}

void insereProduto(connection& C, Product produto) {
	string sql;
	work W(C);	
	
	sql = "INSERT INTO PRODUTO (ASIN, titulo, grupo, posicao_rank) " \
	"VALUES ('"+ produto.getASIN() +"', '"+ produto.getTitle() +"', '"\
	+ produto.getGroup() +"', " + to_string(produto.getSalesRank())+");";

	W.exec(sql);
	W.commit();
}

void insereCategoria(connection& C, int cod, string nome, int super_cat) {
	string sql;
	work W(C);

	sql = "INSERT INTO categoria (cod, nome, super_categoria) " \
	"VALUES (" + to_string(cod) + ", '" + nome + "', " + to_string(super_cat) + ")" \
	"ON CONFLICT (cod) DO NOTHING;";

	W.exec(sql);
	W.commit();
}

void insereProduto_Categoria(connection& C, string asin, int cat) {
	string sql;
	work W(C);

	sql = "INSERT INTO produto_categoria (ASIN, cod) " \
	"VALUES ('" + asin +"', " + to_string(cat) + ");";

	W.exec(sql);
	W.commit();
}

void insereComentarios(connection &C, Review coment) {
	string sql;
	work W(C);

	sql = "INSERT INTO comentario (data, classif, votos, util, produto, cliente) " \
	"VALUES ('" + coment.getDate() + "', " + to_string(coment.getRating()) + ", " \
	+to_string(coment.getVotes())+ ", " \
	+ to_string(coment.getHelpful()) + ", '" + coment.getASIN() + "', '" + coment.getId_cliente() + "');";
	W.exec(sql);
	W.commit();
}

void parse(string nome_arquivo, connection& C){
	ifstream amazon_meta;
	string linha;
	vector<string> palavras;
	int aux=0, i_aux, contProdutos=0;
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
				removeR(palavras[i+1]);
				produto.setASIN(palavras[i+1]);
				//setando asin para comentarios (usado mais tarde)
				comentario.setASIN(palavras[i+1]);
				//cout << "--------------------------------------------" << endl;
				//cout << "ASIN!!!!!: " << produto.getASIN() << endl;
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
				removeR(title_aux);
				produto.setTitle(title_aux);
				//cout << "TITLE!!!!: " <<  produto.getTitle() << endl;
			}

			/******Pegando group******/
			if(palavras[i] == "group:"){
				removeR(palavras[i+1]);
				produto.setGroup(palavras[i+1]);
				//cout << "GROUP!!!!: " << produto.getGroup() << endl;;
			}

			/*******Pegando Salesrank****/
			if(palavras[i] == "salesrank:"){
				produto.setSalesRank(stoi(palavras[i+1]));
				//cout << "SALESRANK: " << produto.getSalesRank() << endl;
				produto.setValid();
			}

			if(produto.isValid()){
				//cout << "INSERE PRODUTO NO BANCO" << endl;
				insereProduto(C, produto);
				contProdutos++;
				cout << "Numero de produtos: " << contProdutos << endl;
				produto.setValid();
			}

			/*****Pegando similares*******/
			if(palavras[i] == "similar:"){
				//cout <<"Numero de similares: "<< palavras[i+1] << endl;
				similar_count = stoi(palavras[i+1]);
				int j = 1;
				while (similar_count) {
					string asin_similar = palavras[i+1+j];
					if (!asin_similar.empty()) {
						//cout << "similar " << similar_count << " " << asin_similar << endl;
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

							//cout << j <<" categoria: -sem nome- id: " << id_categ << " super categ: " << super_cat << endl;

							
							//cout << j <<" categoria: -sem nome- id: " << palavras[j+1] << " super categ: " << super_cat << endl;

							insereCategoria(C, id_categ, palavras[j], super_cat);
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
							//cout << j << " categoria: " << nome_aux << " id: " << palavras[j+1] << " super categ: " << super_cat << endl;
							insereCategoria(C, id_categ, nome_aux, super_cat);
							super_cat = id_categ;
							j++;
						}
					}
					
					//INSERE PRODUTO_CATEGORIA
					insereProduto_Categoria(C, produto.getASIN(), super_cat);
				}
			}	

			if (palavras[i] == "reviews:"){
				int downloaded;
				//celula 7 recebe o valor de downloaded no split
				downloaded = stoi(palavras[7]);
				//cout << "downloaded: " << downloaded << endl;

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
					comentario.setASIN(produto.getASIN());
					//INSERINDO COMENTARIO
					insereComentarios(C, comentario);
					/*cout << "data: " << comentario.getDate() <<
						" cutomer: " << comentario.getId_cliente() <<
						" nota: " << comentario.getRating() <<
						" votos: " << comentario.getVotes() <<
						" ajudou: " << comentario.getHelpful() << endl;*/
				}
			}

		}
		palavras.clear();
		linha.clear();
		aux++;
		//if(aux==400) break;
	}

	//INSERE SIMILARES
	
}

int main(int argc, const char *argv[]) {
	string sql, endereco, usuario, senha, nome_banco, nome_arquivo;

	if(argc != 6) {
		cout << "uso: ./parser <endereco conexao> <usuario> <senha> <banco> <arquivo>" << endl;
		exit(1);
	}
	else {
		endereco = (string) argv[1];
		usuario = (string) argv[2];
		senha = (string) argv[3];
		nome_banco = (string) argv[4];
		nome_arquivo = (string) argv[5];
	}

	try {
		connection C("dbname=" + nome_banco + " user=" + usuario + " password="+senha+" \
			hostaddr="+endereco+" port=5432");

		if (C.is_open()) {
			cout << "Banco de dados aberto: " << C.dbname() << endl;
		}
		else {
			cout << "Não foi possivel abrir este banco: " << C.dbname() << endl;
			return 1;
		}	

		//Criando statements SQL: criacao de relacoes
		criacao_relacoes(C);	

		//Fazendo o parser
		parse(nome_arquivo, C);

		//Terminando
		C.disconnect();
	}
	catch (const exception &e) {
		cerr << e.what() << endl;
		return 1;
	}
	return 0;
}

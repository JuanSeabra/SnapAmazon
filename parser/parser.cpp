//using namespace boost;

#include "parser.h"

void Parser::trocaAspas(string &str){
	for(int j = 0; j < str.size(); j++){
		if(str[j] == '\''){
			str.insert(j,"\'");
			j++;
		}
	}
}

void Parser::removeR(string &str) {
	if (!str.empty() && str[str.size()-1] == '\r')
		str.erase(str.size()-1);
}

void Parser::removeEspaco(string &str) {
	if (str[str.size()-1] == ' ')
		str.erase(str.size()-1);
}

void Parser::parse(string nome_arquivo, BancoDeDados banco, connection &C){
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
	unordered_multimap<string, int> produto_categoria;

	if (!amazon_meta.is_open()) exit(1);

	cout << "Inserindo..." << endl;
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
				banco.insereProduto(C, produto);
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
				//PARA CADA CATEGORIA
				for (int i = 0; i < categ_count; i++) {
					getline(amazon_meta, linha);
					super_cat = 0;
					
					boost::split(palavras, linha, boost::is_any_of("[]|"));
					work WCategoria(C);
					//OBTENDO SUBCATEGORIAS
					for (int j = 1; j < palavras.size()-1; j++) {
						if(palavras[j].empty() && j == 1){
							//se a posição 1 é vazia logo a categoria raiz é sem nome
							//string nome_aux = palavras[j];
							//trocaAspas(nome_aux);

							j++;
							while(palavras[j].empty()){
								j++;
							}

							try {
								id_categ = stoi(palavras[j]);
							}
							catch (invalid_argument& e) {
								while(palavras[j].empty()){
									j++;
								}
								id_categ = stoi(palavras[j]);
							}							

							//cout << j <<" categoria: -sem nome- id: " << id_categ << " super categ: " << super_cat << endl;

							
							//cout << j <<" categoria: -sem nome- id: " << palavras[j+1] << " super categ: " << super_cat << endl;

							banco.insereCategoria(WCategoria, id_categ, "", super_cat);
							super_cat = id_categ;
							j++;
						} else if(!palavras[j].empty()) {
							string nome_aux = palavras[j];
							trocaAspas(nome_aux);

							j++;
							while(palavras[j].empty()){
								j++;
							}

							try {
								id_categ = stoi(palavras[j]);
							}
							catch (invalid_argument& e) {
								j++;
								while(palavras[j].empty()){
									j++;
								}
								id_categ = stoi(palavras[j]);
							}
							//cout << j << " categoria: " << nome_aux << " id: " << id_categ << " super categ: " << super_cat << endl;
							//cout << j << " categoria: " << nome_aux << " id: " << palavras[j+1] << " super categ: " << super_cat << endl;
							banco.insereCategoria(WCategoria, id_categ, nome_aux, super_cat);
							super_cat = id_categ;
							j++;

						}
					}
					//FAZ COMMIT
					WCategoria.commit();
					//INSERE PRODUTO_CATEGORIA
					pair <string, int> par_ (produto.getASIN(), super_cat);
					produto_categoria.insert(par_);
					//insereProduto_Categoria(C, produto.getASIN(), super_cat);
				}
			}	

			if (palavras[i] == "reviews:"){
				int downloaded;
				//celula 7 recebe o valor de downloaded no split
				downloaded = stoi(palavras[7]);
				//cout << "downloaded: " << downloaded << endl;

				work WComentario(C);
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
					banco.insereComentarios(WComentario, comentario);
					/*cout << "data: " << comentario.getDate() <<
						" cutomer: " << comentario.getId_cliente() <<
						" nota: " << comentario.getRating() <<
						" votos: " << comentario.getVotes() <<
						" ajudou: " << comentario.getHelpful() << endl;*/
				}
				WComentario.commit();
			}

		}
		palavras.clear();
		linha.clear();
	}

	//INSERE PRODUTO CATEGORIA
	work WProduto_Categoria(C);
	unordered_multimap<string,int>::iterator it;
	for (it = produto_categoria.begin(); it != produto_categoria.end(); it++) {
		banco.insereProduto_Categoria(WProduto_Categoria, it->first, it->second);
	}
	WProduto_Categoria.commit();

	//INSERE SIMILARES
	work WSimilares(C);
	unordered_multimap<string,string>::iterator it2;
	for (it2 = similares.begin(); it2 != similares.end(); it2++) {
		if (similares.count(it2->second) != 0) {
			string asin_similar = it2->second;
			removeR(asin_similar);
			banco.insereSimilares(WSimilares, it2->first, asin_similar);
		}		
	}
	WSimilares.commit();

	produto_categoria.clear();
	similares.clear();
	cout << "FIM DA INSERCAO" << endl;
}



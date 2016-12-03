#include "bancoDeDados.h"

/*void removeR(string &str) {
  if (!str.empty() && str[str.size()-1] == '\r')
  str.erase(str.size()-1);
  }*/

BancoDeDados::BancoDeDados(connection& C){
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
		   "ASIN 			VARCHAR(15)  NOT NULL, " \
		   "ASIN_Similar	VARCHAR(15)  NOT NULL, " \
		   "PRIMARY KEY (ASIN, ASIN_Similar), " \
		   "FOREIGN KEY (ASIN) REFERENCES produto(ASIN), " \
		   "FOREIGN KEY (ASIN_Similar) REFERENCES produto(ASIN));";

	W.exec(sql);	
	cout << "Relacao criada com sucesso: Similares" << endl;

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

void BancoDeDados::insereProduto(connection& C, Product produto) {
	string sql;
	work W(C);	

	sql = "INSERT INTO PRODUTO (ASIN, titulo, grupo, posicao_rank) " \
		   "VALUES ('"+ produto.getASIN() +"', '"+ produto.getTitle() +"', '"\
		   + produto.getGroup() +"', " + to_string(produto.getSalesRank())+");";

	W.exec(sql);
	W.commit();
}

void BancoDeDados::insereCategoria(work& W, int cod, string nome, int super_cat) {
	string sql;
	//work W(C);
	string super_categoria;

	if (super_cat == 0)
		super_categoria = "null";
	else super_categoria = to_string(super_cat);

	sql = "INSERT INTO categoria (cod, nome, super_categoria) " \
		   "VALUES (" + to_string(cod) + ", '" + nome + "', " + super_categoria + ")" \
		   "ON CONFLICT (cod) DO NOTHING;";

	W.exec(sql);
	//W.commit();
}

void BancoDeDados::insereProduto_Categoria(work& W, string asin, int cat) {
	string sql;
	//work W(C);

	sql = "INSERT INTO produto_categoria (ASIN, cod) " \
		   "VALUES ('" + asin +"', " + to_string(cat) + ");";

	W.exec(sql);
	//W.commit();
}

void BancoDeDados::insereComentarios(work &W, Review coment) {
	string sql;
	//work W(C);

	sql = "INSERT INTO comentario (data, classif, votos, util, produto, cliente) " \
		   "VALUES ('" + coment.getDate() + "', " + to_string(coment.getRating()) + ", " \
		   +to_string(coment.getVotes())+ ", " \
		   + to_string(coment.getHelpful()) + ", '" + coment.getASIN() + "', '" + coment.getId_cliente() + "');";
	W.exec(sql);
	//W.commit();
}

void BancoDeDados::insereSimilares(work &W, string asin1, string asin2) {
	string sql;
	//removeR(asin2);

	sql = "INSERT INTO similares(ASIN, ASIN_Similar) " \
		   "VALUES ('" + asin1 +"', '" + asin2 + "');";

	W.exec(sql);
}



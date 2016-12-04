#include "bancoDeDados.h"
#include "TextTable.h"

void BancoDeDados::listarASIN_Produto(connection& C, string nome) {
	string sql;
	nontransaction N(C);	
	ofstream tabledat ("listarASIN_Produto.txt");

	sql = "SELECT * FROM produto WHERE titulo LIKE '%"+nome+"%';";

	result R(N.exec(sql));
	TextTable t( '-', '|', '+' );

	//Cabecalho
	t.add( "asin" );
	t.add( "titulo" );
	t.add( "grupo" );
	t.add( "posicao no ranking" );
	t.endOfRow();

	//Preenchendo com informacao
	for (result::const_iterator c = R.begin(); c != R.end(); c++) {
		t.add(c[0].as<string>());
		t.add(c[1].as<string>());
		t.add(c[2].as<string>());
		t.add(to_string(c[3].as<int>()));
		t.endOfRow();
	}

	t.setAlignment( 2, TextTable::Alignment::RIGHT );
	cout << "Produtos pesquisados: " << endl;
	cout << t << endl;
	if (tabledat.is_open()){
		tabledat << t;
		tabledat.close();
	} else cout << "Falhou ao escrever no arquivo" << endl;
}

void BancoDeDados::criarVisoes(connection& C) {
	string sql;
	work W(C);

	sql = "CREATE TEMP VIEW produtosMedia AS SELECT produto AS asin, avg(classif) AS media, count(*) AS num "\
		   "FROM comentario WHERE votos>0 AND util/votos > 0.5 AND classif >= 3 GROUP BY produto;";

	W.exec(sql);

	sql = "CREATE TEMP VIEW categoriasProdutosNome AS SELECT asin, cod, nome from categoria NATURAL JOIN produto_categoria;";
	W.exec(sql);
	W.commit();	
}

void BancoDeDados::listarCincoMaisProdutos(connection& C, string asin) {
	string sql;
	nontransaction N(C);
	ofstream tabledat ("listarCincoMaisProdutos.txt");

	//os 5 mais uteis melhores classificacoes
	sql =  "SELECT * FROM comentario WHERE produto='"+asin+"'"\
			" AND votos>0 ORDER BY (classif, votos, (util/votos)) DESC LIMIT 5;";

	result R(N.exec(sql));
	TextTable t( '-', '|', '+' );

	//Cabecalho
	t.add( "id" );
	t.add( "data" );
	t.add( "avaliacao" );
	t.add( "votos" );
	t.add( "util" );
	t.add( "produto" );
	t.add( "cliente" );
	t.endOfRow();

	//Preenchendo com informacao
	for (result::const_iterator c = R.begin(); c != R.end(); c++) {
		t.add(to_string(c[0].as<int>()));
		t.add(c[1].as<string>());
		t.add(to_string(c[2].as<int>()));
		t.add(to_string(c[3].as<int>()));
		t.add(to_string(c[4].as<int>()));
		t.add(c[5].as<string>());
		t.add(c[6].as<string>());
		t.endOfRow();
	}

	t.setAlignment( 2, TextTable::Alignment::RIGHT );
	cout << "5 comentarios mais uteis e com maior avaliacao: " << endl;
	cout << t << endl;
	if (tabledat.is_open()){
		tabledat << t << endl;
		tabledat.close();
	} else cout << "Falhou ao escrever no arquivo" << endl;


	//os 5 mais uteis menores classificacoes
	sql =  "SELECT * FROM (SELECT * FROM comentario WHERE produto='"+asin+"'"\
			" AND votos>0 ORDER BY (votos,util/votos) DESC ) as uteis ORDER BY classif LIMIT 5;";

	result R2(N.exec(sql));

	TextTable t2( '-', '|', '+' );

	//Cabecalho
	t2.add( "id" );
	t2.add( "data" );
	t2.add( "avaliacao" );
	t2.add( "votos" );
	t2.add( "util" );
	t2.add( "produto" );
	t2.add( "cliente" );
	t2.endOfRow();

	//Preenchendo com informacao
	for (result::const_iterator c = R2.begin(); c != R2.end(); c++) {
		t2.add(to_string(c[0].as<int>()));
		t2.add(c[1].as<string>());
		t2.add(to_string(c[2].as<int>()));
		t2.add(to_string(c[3].as<int>()));
		t2.add(to_string(c[4].as<int>()));
		t2.add(c[5].as<string>());
		t2.add(c[6].as<string>());
		t2.endOfRow();
	}

	t2.setAlignment( 2, TextTable::Alignment::RIGHT );
	cout << "5 comentarios mais uteis e com menor avaliacao: " << endl;
	cout << t2 << endl;
	if (tabledat.is_open()){
		tabledat << t2;
		tabledat.close();
	} else cout << "Falhou ao escrever no arquivo" << endl;

}

void BancoDeDados::listarSimilaresMaioresVendas(connection& C, string asin) {
	string sql;
	nontransaction N(C);	
	ofstream tabledat ("listarSimilaresMaioresVendas.txt");

	sql = "SELECT p2.* FROM produto AS p1, produto AS p2, similares WHERE p1.asin = '"+asin+"' AND p1.asin=similares.asin "\
		   "AND p2.asin = similares.asin_similar AND p2.posicao_rank < p1.posicao_rank AND p2.posicao_rank > 0;";

	result R(N.exec(sql));
	TextTable t( '-', '|', '+' );

	//Cabecalho
	t.add( "asin" );
	t.add( "titulo" );
	t.add( "grupo" );
	t.add( "posicao no ranking" );
	t.endOfRow();

	//Preenchendo com informacao
	for (result::const_iterator c = R.begin(); c != R.end(); c++) {
		t.add(c[0].as<string>());
		t.add(c[1].as<string>());
		t.add(c[2].as<string>());
		t.add(to_string(c[3].as<int>()));
		t.endOfRow();
	}

	t.setAlignment( 2, TextTable::Alignment::RIGHT );
	cout << "Similares com maiores vendas: " << endl;
	cout << t << endl;
	if (tabledat.is_open()){
		tabledat << t;
		tabledat.close();
	} else cout << "Falhou ao escrever no arquivo" << endl;



}

void BancoDeDados::mostrarEvolucaoDasAvaliacoes(connection& C, string asin) {
	string sql;
	nontransaction N(C);
	ofstream tabledat ("mostrarEvolucaoDasAvaliacoes.txt");
	ofstream graph ("graph.dat");


	sql = "SELECT data, AVG(classif) AS media FROM comentario WHERE produto='"+asin+"'"\
		   " GROUP BY data ORDER BY data";

	result R(N.exec(sql));
	TextTable t( '-', '|', '+' );

	//Cabecalho
	t.add( "data" );
	t.add( "media de avaliacao" );
	t.endOfRow();

	//Preenchendo com informacao
	for (result::const_iterator c = R.begin(); c != R.end(); c++) {
		t.add(c[0].as<string>());
		t.add(to_string(c[1].as<float>()));
		t.endOfRow();
		if (graph.is_open()){
			graph << c[0] << " " << c[1] << endl;
		} else cout << "falhou ao gravar arquivo de gráfico" << endl;
	}

	t.setAlignment( 2, TextTable::Alignment::RIGHT );
	cout << "Evolucao diaria de media de avaliacoes por data: " << endl;
	cout << t << endl;
	string command = "gnuplot plot_graph";
	system(command.c_str());

	if (tabledat.is_open()){
		tabledat << t;
		tabledat.close();
	} else cout << "Falhou ao escrever no arquivo" << endl;



}

void BancoDeDados::listarDezLideresPorGrupo(connection& C) {
	string sql;
	nontransaction N(C);	
	ofstream tabledat ("listarDesLideresPorGrupo.txt");

	cout << "Aguarde, obtendo dados" << endl;
	sql = "SELECT asin, titulo, grupo, posicao_rank FROM (SELECT *, ROW_NUMBER() OVER (PARTITION BY grupo ORDER BY posicao_rank) "\
		   "AS Row_ID FROM produto WHERE posicao_rank>0) AS GRUPOS WHERE Row_ID <= 10 ORDER BY grupo;";

	result R(N.exec(sql));
	TextTable t( '-', '|', '+' );

	//Cabecalho
	t.add( "asin" );
	t.add( "titulo" );
	t.add( "grupo" );
	t.add( "posicao no ranking" );
	t.endOfRow();

	//Preenchendo com informacao
	for (result::const_iterator c = R.begin(); c != R.end(); c++) {
		t.add(c[0].as<string>());
		t.add(c[1].as<string>());
		t.add(c[2].as<string>());
		t.add(to_string(c[3].as<int>()));
		t.endOfRow();
	}

	t.setAlignment( 1, TextTable::Alignment::LEFT );
	cout << "Os 10 produtos lideres de venda por grupo: " << endl;
	cout << t << endl;
	if (tabledat.is_open()){
		tabledat << t;
		tabledat.close();
	} else cout << "Falhou ao escrever no arquivo" << endl;

}

void BancoDeDados::listarDezProdutosMaiorMedia(connection& C) {
	string sql;
	nontransaction N(C);	
	ofstream tabledat ("listarDezProdutosMaiorMedia.txt");

	cout << "Aguarde, obtendo dados" << endl;
	sql = "SELECT asin, titulo, media FROM produto, (SELECT produto, avg(classif) AS media FROM comentario WHERE votos>0 AND util/votos > 0.5 "\
		   "AND classif >= 3 GROUP BY produto) AS mm WHERE produto.asin = mm.produto ORDER BY (media) DESC LIMIT 10;";

	result R(N.exec(sql));
	TextTable t( '-', '|', '+' );

	//Cabecalho
	t.add( "asin" );
	t.add( "titulo" );
	t.add( "media avaliacoes uteis positivas" );
	t.endOfRow();

	//Preenchendo com informacao
	for (result::const_iterator c = R.begin(); c != R.end(); c++) {
		t.add(c[0].as<string>());
		t.add(c[1].as<string>());
		t.add(to_string(c[2].as<float>()));
		t.endOfRow();
	}

	t.setAlignment( 2, TextTable::Alignment::RIGHT );
	cout << "Os 10 produtos com a maior media de avaliacoes uteis positivas: " << endl;
	cout << t << endl;
	if (tabledat.is_open()){
		tabledat << t;
		tabledat.close();
	} else cout << "Falhou ao escrever no arquivo" << endl;

}

void BancoDeDados::listarCincoCategMaiorMedia(connection& C) {
	string sql;
	nontransaction N(C);	
	ofstream tabledat ("listarCincoCategMaiorMedia.txt");

	cout << "Aguarde, obtendo dados" << endl;

	sql = "SELECT cod, nome, avg(media) AS media_categ FROM produtosMedia NATURAL JOIN categoriasProdutosNome"\
		   " GROUP BY cod,nome,num ORDER BY (avg(media),num) DESC LIMIT 5; ";

	result R(N.exec(sql));
	TextTable t( '-', '|', '+' );
	//Cabecalho
	t.add( "categoria" );
	t.add( "media avaliacoes uteis positivas" );
	t.endOfRow();

	/*


	//Preenchendo com informacao
	for (result::const_iterator c = R.begin(); c != R.end(); c++) {
	t.add(to_string(c[0].as<int>()));
	t.add(c[1].as<string>());
	t.add(to_string(c[2].as<float>()));
	t.endOfRow();
	}

	t.setAlignment( 2, TextTable::Alignment::RIGHT );
	cout << "AS 5 categorias com a maior media de avaliacoes uteis positivas: " << endl;
	cout << t << endl;*/


	for (result::const_iterator c = R.begin(); c != R.end(); c++) {
		string categoria = "";
		int cod = c[0].as<int>();
		string nome = c[1].as<string>();

		sql = "WITH RECURSIVE super_categ(super, nome, categ) AS (SELECT super_categoria, nome, cod"\
			   " FROM categoria WHERE cod="+to_string(cod)+" UNION SELECT c.super_categoria, c.nome, s.categ FROM categoria"\
			   " AS c, super_categ AS s WHERE s.categ = c.super_categoria OR s.super = c.cod) SELECT super, nome FROM super_categ;";

		result R2(N.exec(sql));

		for (int i = R2.size()-1; i >= 1; i--) {        	
			const result::tuple c2 = R2[i];	
			const result::tuple c3 = R2[i-1];

			string cod2;
			try {
				cod2 = to_string(c3[0].as<int>());	
			}
			catch (const exception &e) {
				cod2 = "";
			}

			string nome2 = c2[1].as<string>();
			categoria += (nome2 + "[" + cod2 + "]" + "|");	        		
		}

		categoria += (nome + "[" + to_string(cod) + "]");
		t.add(categoria);
		t.add(to_string(c[2].as<float>()));
		t.endOfRow();
	}

	t.setAlignment( 1, TextTable::Alignment::LEFT );
	cout << "As 5 categorias com a maior media de avaliacoes uteis positivas: " << endl;
	cout << t << endl;
	if (tabledat.is_open()){
		tabledat << t;
		tabledat.close();
	} else cout << "Falhou ao escrever no arquivo" << endl;

}

void BancoDeDados::listarDezClientesTop(connection& C) {
	string sql;
	nontransaction N(C);	
	ofstream tabledat ("listarDezClientesTop.txt");

	cout << "Aguarde, obtendo dados" << endl;

	sql = "SELECT cliente, qtd, grupo FROM (SELECT *, ROW_NUMBER()	OVER (PARTITION BY grupo ORDER BY qtd DESC) AS numLinhas	FROM (SELECT cliente, COUNT(id) AS qtd, grupo FROM (SELECT id, cliente, asin, grupo FROM produto, comentario WHERE produto.asin = comentario.produto) "\
		   "as clientesGrupo GROUP BY cliente, grupo) AS clientesQtd ) AS clientesTop WHERE numLinhas <= 10;";

	result R(N.exec(sql));
	TextTable t( '-', '|', '+' );

	//Cabecalho
	t.add( "cliente" );
	t.add( "numero comentarios" );
	t.add( "grupo" );
	t.endOfRow();

	//Preenchendo com informacao
	for (result::const_iterator c = R.begin(); c != R.end(); c++) {
		t.add(c[0].as<string>());
		t.add(to_string(c[1].as<int>()));
		t.add(c[2].as<string>());
		t.endOfRow();
	}

	t.setAlignment( 2, TextTable::Alignment::RIGHT );
	cout << "Os 10 clientes que fizeram mais comentarios por grupo: " << endl;
	cout << t << endl;
	if (tabledat.is_open()){
		tabledat << t;
		tabledat.close();
	} else cout << "Falhou ao escrever no arquivo" << endl;

}


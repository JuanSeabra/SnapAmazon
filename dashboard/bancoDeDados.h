#ifndef BANCODEDADOS_H
#define BANCODEDADOS_H

#include <string>
#include <iostream>
#include <pqxx/pqxx>

using namespace std;
using namespace pqxx;

class BancoDeDados {
	public:

		void listarASIN_Produto(connection& C, string nome);
		void listarCincoMaisProdutos(connection& C, string asin);
		void listarSimilaresMaioresVendas(connection& C, string asin);
		void mostrarEvolucaoDasAvaliacoes(connection& C, string asin);
		void listarDezLideresPorGrupo(connection& C);
		void listarDezProdutosMaiorMedia(connection& C);
		void listarCincoCategMaiorMedia(connection& C);
		void listarDezClientesTop(connection& C);
		void criarVisoes(connection& C);

};

#endif

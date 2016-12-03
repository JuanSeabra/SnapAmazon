#ifndef BANCODEDADOS_H
#define BANCODEDADOS_H

#include "categorie.h"
#include "review.h"
#include "product.h"
#include <string>
#include <pqxx/pqxx>

using namespace std;
using namespace pqxx;

class BancoDeDados
{
	public:

		void listarCincoMaisProdutos(connection& C);
		void listarSimilaresMaioresVendas(connection& C);
		void mostrarEvolucaoDasAvaliacoes(connection& C);
		void listarDezLideresPorGrupo(connection& C);
		void listarDezProdutosMaiorMedia(connection& C);
		void listarCincoCategMaiorMedia(connection& C);
		void listarDezClientesTop(connection& C);

};

#endif

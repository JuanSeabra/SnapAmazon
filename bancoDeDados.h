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
		BancoDeDados(connection& C);
		void insereProduto(connection& C, Product produto);
		void insereCategoria(work& W, int cod, string nome, int super_cat);
		void insereProduto_Categoria(work& W, string asin, int cat);
		void insereComentarios(work &W, Review coment);
		void insereSimilares(work &W, string asin1, string asin2);
		
};

#endif

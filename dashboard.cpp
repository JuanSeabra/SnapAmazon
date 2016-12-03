#include "bancoDeDados.h"
#include <stdio.h>
#include <stdlib.h>

using namespace std;

int main(int argc, const char *argv[])
{
	string sql, endereco, usuario, senha, nome_banco;
	int opcao = 0;
	if(argc != 5) {
		cout << "uso: ./parser <endereco conexao> <usuario> <senha> <banco>" << endl;
		exit(1);
	}
	else {
		endereco = (string) argv[1];
		usuario = (string) argv[2];
		senha = (string) argv[3];
		nome_banco = (string) argv[4];
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

		BancoDeDados banco;

		while(opcao != 8){
			cout << "selecione uma opção: " << endl <<
				"1 - listar 5 comentarios mais úteis com maior avaliação e os 5 mais úteis com menos avaliação" << endl <<
				"2 - listar os produtos similares com maiores vendas do que o produto dado" << endl <<
				"3 - mostrar a evolução diária das médias de avaliação ao longo do intervalo de tempo coberto no banco" << endl <<
				"4 - listar os 10 produtos lideres de venda em cada grupo de produtos" << endl <<
				"5 - listar os 10 produtos com maior médias de avaliações úteis positivas por categoria" << endl <<
				"6 - listar as 5 categorias de produto com a maior média de avaliações úteis positivas por produto" << endl <<
				"7 - listar os 10 clientes que mais fizeram comentários por grupo de produto" << endl <<
				"8 - sair" << endl;
			cin >> opcao;

			switch (opcao) {
				case 1:
					//inserir funcao de query
					banco.listarCincoMaisProdutos(C)
					break;
				case 2:
					//inserir funcao de query
					banco.listarSimilaresMaioresVendas(C);
					break;
				case 3:
					//inserir funcao de query
					banco.mostrarEvolucaoDasAvaliacoes(C);
					break;
				case 4:
					//inserir funcao de query
					banco.listarDezLideresPorGrupo(C);
					break;
				case 5:
					//inserir funcao de query
					banco.listarDezProdutosMaiorMedia(C);
					break;
				case 6:
					banco.listarCincoCategMaiorMedia(C);
					break;
				case 7:
					banco.listarDezClientesTop(C);
					break;

			}
		}

	}
	catch (const exception &e) {
		cerr << e.what() << endl;
		return 1;
	}

	return 0;
}

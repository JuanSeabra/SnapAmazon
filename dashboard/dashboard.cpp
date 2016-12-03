#include "bancoDeDados.h"
#include <stdio.h>
#include <stdlib.h>

using namespace std;

int main(int argc, const char *argv[])
{
	string sql, endereco, usuario, senha, nome_banco, asin, nome;
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
		banco.criarVisoes(C);

		while(opcao != 8){
			cout << "selecione uma opção: " << endl <<
				//"0 - Pesquisar um produto pelo nome (contem um nome; para todos os produtos, deixe vazio" << endl <<
				"1 - Listar 5 comentarios mais úteis com maior avaliação e os 5 mais úteis com menos avaliação, dado um produto" << endl <<
				"2 - listar os produtos similares com maiores vendas do que o produto dado" << endl <<
				"3 - mostrar a evolução diária das médias de avaliação ao longo do intervalo de tempo coberto no banco" << endl <<
				"4 - listar os 10 produtos lideres de venda em cada grupo de produtos" << endl <<
				"5 - listar os 10 produtos com maior médias de avaliações úteis positivas" << endl <<
				"6 - listar as 5 categorias de produto com a maior média de avaliações úteis positivas" << endl <<
				"7 - listar os 10 clientes que mais fizeram comentários por grupo de produto" << endl <<
				"8 - sair" << endl;
			cin >> opcao;

			switch (opcao) {
				/*case 0:
					cout << "Digite o nome do produto desejado: " << endl;
					getline(cin,nome);
					//cout << nome << endl;
					banco.listarASIN_Produto(C, nome);
					break;*/
				case 1:
					cout << "Digite o ASIN do produto desejado: " << endl;
					cin >> asin;

					banco.listarCincoMaisProdutos(C, asin);
					break;
				case 2:
					cout << "Digite o ASIN do produto desejado: " << endl;
					cin >> asin;
					banco.listarSimilaresMaioresVendas(C, asin);
					break;
				case 3:
					cout << "Digite o ASIN do produto desejado: " << endl;
					cin >> asin;
					banco.mostrarEvolucaoDasAvaliacoes(C, asin);
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

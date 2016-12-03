#include "parser.h"
#include "bancoDeDados.h"

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
		BancoDeDados banco (C);

		//Fazendo o parser
		Parser P;
		P.parse(nome_arquivo, banco, C);

		//Terminando
		C.disconnect();
	}
	catch (const exception &e) {
		cerr << e.what() << endl;
		return 1;
	}
	return 0;
}

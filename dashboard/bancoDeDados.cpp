#include "bancoDeDados.h"


void BancoDeDados::listarCincoMaisProdutos(connection& C) {
	string sql;
	work W(C);	

	sql =;

	W.exec(sql);
	W.commit();
}

void BancoDeDados::listarSimilaresMaioresVendas(connection& C) {
	string sql;
	work W(C);	

	sql = ;

	W.exec(sql);
	W.commit();
}

void BancoDeDados::mostrarEvolucaoDasAvaliacoes(connection& C) {
	string sql;
	work W(C);	

	sql = ;

	W.exec(sql);
	W.commit();
}

void BancoDeDados::listarDezLideresPorGrupo(connection& C) {
	string sql;
	work W(C);	

	sql = ;

	W.exec(sql);
	W.commit();
}

void BancoDeDados::listarDezProdutosMaiorMedia(connection& C) {
	string sql;
	work W(C);	

	sql = ;

	W.exec(sql);
	W.commit();
}

void BancoDeDados::listarCincoCategMaiorMedia(connection& C) {
	string sql;
	work W(C);	

	sql = ;

	W.exec(sql);
	W.commit();
}

void BancoDeDados::listarDezClientesTop(connection& C) {
	string sql;
	work W(C);	
	
	sql = ;

	W.exec(sql);
	W.commit();
}


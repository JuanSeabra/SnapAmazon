#ifndef PARSER_H
#define PARSER_H

#include "categorie.h"
#include "review.h"
#include "product.h"
#include "bancoDeDados.h"
#include <time.h>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <boost/algorithm/string.hpp>
#include <pqxx/pqxx>

using namespace std;
using namespace pqxx;

class Parser
{
public:
	void trocaAspas(string &str);
	void removeR(string &str);
	void removeEspaco(string &str);
	void parse(string nome_arquivo, BancoDeDados banco, connection &C);
};

#endif

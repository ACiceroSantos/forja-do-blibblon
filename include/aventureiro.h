#ifndef AVENTUREIRO_H
#define AVENTUREIRO_H

#include "pilha.h"
#include <string>

class Aventureiro {
private:
    int id;
    std::string nome;
    pilha build;

public:
    Aventureiro(int id_av = 0, const std::string& nome_av = "");
    int get_id() const;
    std::string get_nome() const;
    pilha& get_build();
};

#endif
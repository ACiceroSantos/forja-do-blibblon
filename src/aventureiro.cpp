#include "aventureiro.h"
#include "pilha.h"

Aventureiro::Aventureiro(int id_av, const std::string& nome_av) 
    : id(id_av), nome(nome_av), build(id_av) {}

int Aventureiro::get_id() const {
    return id;
}

std::string Aventureiro::get_nome() const {
    return nome;
}

pilha& Aventureiro::get_build() {
    return build;
}
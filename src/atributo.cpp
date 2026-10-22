#include "atributo.h"

Atributo::Atributo(int id_atrib, const std::string& nome_atrib) {
    id = id_atrib;
    nome = nome_atrib;
}

int Atributo::get_id() const {
    return id;
}

std::string Atributo::get_nome() const {
    return nome;
}
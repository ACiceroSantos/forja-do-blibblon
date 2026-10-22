#include "item.h"

Item::Item(int id_item, const std::string& nome_item) {
    id = id_item;
    nome = nome_item;
}

int Item::get_id() const {
    return id;
}

std::string Item::get_nome() const {
    return nome;
}
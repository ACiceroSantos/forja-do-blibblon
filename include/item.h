#ifndef ITEM_H
#define ITEM_H

#include <string>

class Item {
private:
    int id;
    std::string nome;

public:
    Item(int id_item = 0, const std::string& nome_item = "");
    int get_id() const;
    std::string get_nome() const;
};

#endif
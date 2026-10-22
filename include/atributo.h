#ifndef ATRIBUTO_H
#define ATRIBUTO_H

#include <string>

class Atributo {
private:
    int id;
    std::string nome;

public:
    Atributo(int id_atrib = 0, const std::string& nome_atrib = "");
    int get_id() const;
    std::string get_nome() const;
};

#endif
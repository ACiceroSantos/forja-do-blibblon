#ifndef FILA_H
#define FILA_H

struct node_fila{
    int id_aventureiro;
    int id_item;
    node_fila* next;
};

class fila{
private:
    node_fila* first;
    node_fila* last;
    int tamanho;

public:
    fila();
    ~fila();

    int adicionar(int id_aventureiro, int id_item); //insere item no fim da fila (comando F) retorna a posicao
    bool remover(int& id_aventureiro, int& id_item); //remove item do inicio (comando Z)
    //metodo bool pela utilidade de ter um retorno se foi possivel remover ou se a fila ja estava vazia

    bool vazia() const;
    int get_tamanho() const;

};

#endif
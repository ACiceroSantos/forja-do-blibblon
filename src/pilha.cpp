#include "pilha.h"
#include <iostream>
using namespace std;


/*
Pilha: "representa a build do aventureiro. 
 Cada aventureiro possui a sua própria pilha de equipamentos, 
 onde os itens são organizados com disciplina LIFO (last in, first out). 
 O item no topo é o equipamento ativo."

Comando Z - Empilhar/Adicionar na Build - Z <k> [k turnos]
 por k turnos Blibblon remove uma encomenda da fila e adiciona 

 "Quando Blibblon atende uma encomenda da fila (comando Z <k>), 
  o item forjado é imediatamente equipado no aventureiro correspondente, 
  indo para o topo de sua pilha (build)."


 "Se o item forjado for de um tipo que o aventureiro já possui equipado no topo da pilha, 
  o item novo substitui o antigo? Não. 
  O item é simplesmente empilhado sobre os anteriores, tornando-se o novo topo."

  Saída:

  encomenda_<turno> <id_aventureiro> <id_item>

Comando D - Desempilhar - D <id_aventureiro> <quantidade>

 "Remove os <quantidade> itens do topo da pilha do aventureiro. 
  Os itens desequipados são descartados (destruídos) e a memória é liberada."

 "Se a quantidade solicitada for maior que o número de itens na pilha, 
  o aventureiro desequipa todos os itens que possui e a pilha fica vazia. 
  Não há erro nessa situação."

  A saída informa a quantidade de itens efetivamente removidos:

 Saída:
  D <id_aventureiro> <quantidade_efetivamente_removida>

Comando LH - Listar Histórico/Build - LH <id_aventureiro>
 
 "Imprime os itens equipados no aventureiro, 
  do topo para a base (do mais recente para o mais antigo)."

 Saída:
  
 LH <id_aventureiro> <item_topo> <item_2> ... <item_base>
 
  "Se a pilha estiver vazia, imprime apenas:"
  LH <id_aventureiro>
*/


//construtor
pilha::pilha(int id_av){
    pilha::id_aventureiro = id_av;
    pilha::top = nullptr;
    tamanho = 0;
}

//destrutor
pilha::~pilha(){
    int id_removido = 0;
    while (!pilha::vazia())
    {
        pilha::desempilhar(id_removido);
    }
}

/*
* empilhar(id_item)
* comando Z<k>
* adiciona o item na pilha da build
* saida: encomenda_<turno> <id_aventureiro> <id_item>  
*/

void pilha::empilhar(int id_item){
    
    //cria um novo node na memoria
    node_pilha* novo_item = new node_pilha;

    novo_item->id_item = id_item;

    //o proximo do novo node passa a ser o antigo topo
    novo_item->next = pilha::top;

    //o novo node passa a aser o topo
    pilha::top = novo_item;
    
    pilha::tamanho++;

}


/*
* desempilhar(quantidade)
* comando D <id_aventureiro> <quantidade>
* saida: D <id_aventureiro> <quantidade_efetivamente_removida>
* retorna a quantidade removida
*/

bool pilha::desempilhar(int &id_item_removido){
    
    // se a pilha estiver vazia, nao ha o que desempilhar
    if (pilha::vazia()) {
        return false;
    }

    // ponteiro auxiliar para o no do topo atual
    node_pilha* node_auxiliar = pilha::top;

    // guarda o id do item que esta sendo removido
    id_item_removido = node_auxiliar->id_item;

    // avanca o topo para o proximo item da pilha
    pilha::top = node_auxiliar->next;

    // libera a memoria do no desequipado
    delete node_auxiliar;

    // decrementa o tamanho total de itens equipados
    pilha::tamanho--;

    return true;
}

node_pilha* pilha::get_top() const {
    return pilha::top;
}

//metodos auxiliares

bool pilha::vazia() const{
    return (pilha::tamanho == 0);
}

int pilha::get_tamanho() const{
    return (pilha::tamanho);
}
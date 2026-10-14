#include "fila.h"

//construtor
fila::fila(){
    first = nullptr;
    last = nullptr;
    tamanho = 0;
}

//destrutor
fila::~fila(){
    int aux_aventureiro;
    int aux_item;
    while(vazia()==false){
        remover(aux_aventureiro, aux_item);
    }
}


/*
 * bool remover(espaco pro id do aventureiro removido, espaco pro id do item removido)
 *
 * Remove o elemento da frente da fila (ordem FIFO/Comando Z), 
 * preenche as variáveis passadas por referência com os dados do nó removido 
 * e desaloca a memória utilizada.
 */

bool fila::remover(int &id_aventureiro,int &id_item){

    //verifica se a fila está vazia
    if(vazia()){
        return false;
        //nao tem oq remover, a fila ja estava vazia
    }

    //guarda endereço do nó da frente em um ponteiro auxiliar
    node_fila* aux_first = fila::first;

    //guarda os ids do nó em variaveis de retorno
    id_aventureiro = aux_first->id_aventureiro;
    id_item = aux_first->id_item;

    //avança o ponteiro first para o próximo nó
    fila::first = aux_first ->next;

    //se esse era o ultimo da fila, o seu next (atual primeiro) era nullptr. 
    //Entao, como o primeiro ja vai ser null, o ultimo tambem tem que ser
    if (fila::first == nullptr)
    {
        fila::last = nullptr;
    }
    
    //Deleta a memória do nó removido, decrementa o tamanho e retorna true.
    delete aux_first;
    fila::tamanho--;
    return true;
}


/*
* adicionar(aventureiro, item)
*
* comando: F <id_aventureiro> <id_item>
*
* registra uma encomenda ao final da fila de espera da forja. 
* o método retorna a posição da encomenda na fila, contada a partir de 1:
* saida = F <posicao>
*
* recebe id_aventureiro e id_item, aloca um novo nó na memoria com os dados,
* coloca ele no final da fila e incrementa o tamanho
*/

int fila::adicionar(int id_aventureiro,int id_item){

    node_fila* nova_encomenda = new node_fila;

    //como a nova encomenda entra no final da fila, a proxima depois dela é nula
    nova_encomenda->next = nullptr;

    nova_encomenda->id_aventureiro = id_aventureiro;
    nova_encomenda->id_item = id_item;
    fila::tamanho++;

    //se fila estiver vazia, a nova encomenda vai ser a primeira e a ultima,
    //caso contrario ela é a ultima e a penultima passa a apontar pra ela
    if (fila::vazia())
    {
        fila::first = nova_encomenda;
        fila::last = nova_encomenda;

    }else
    {
        fila::last->next = nova_encomenda;
        fila::last = nova_encomenda;
    
    }
    //o tamanho da fila é exatamente a posição da nova encomenda
    return fila::tamanho;
}


/*
* vazia()
*
* verifica se a fila está vazia
*/

bool fila::vazia() const{
    return(fila::first == nullptr);
}


/*
* get_tamanho()
*
* metodo getter do tamanho
*/

int fila::get_tamanho() const{
    return(fila::tamanho);
}
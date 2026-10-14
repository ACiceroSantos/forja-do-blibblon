#ifndef PILHA_H
#define PILHA_H

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

struct node_pilha{
    int id_item;
    node_pilha* next;
};

class pilha{
    
    private:
        int id_aventureiro;
        node_pilha* top;
        int tamanho;

    public:

        //definir = 0 como valor padrao evita erros caso seja necessario utilizaro construtor padrao sem parametros
        //instanciando um vetor/matriz de pilhas por exemplo
        pilha(int id_av = 0);
        ~pilha();

        //comando Z<k>
        //adiciona o item na pilha da build
        //saida: encomenda_<turno> <id_aventureiro> <id_item>
        void empilhar(int id_item);

        //comando D <id_aventureiro> <quantidade>
        //saida: D <id_aventureiro> <quantidade_efetivamente_removida>
        //retorna a quantidade removida
        int desempilhar(int quantidade);

        //comando LH <id_aventureiro>
        //saida: LH <id_aventureiro> <item_topo> <item_2> ... <item_base>
        // ou LH <id_aventureiro> se a build estiver vazia
        void listar_build() const;

        bool vazia() const;

        int get_tamanho() const;

};



#endif
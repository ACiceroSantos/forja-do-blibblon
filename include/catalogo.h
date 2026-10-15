/*

"Catálogo: TAD suficientemente geral para representar a matriz esparsa de modificadores
 nas duas representações exigidas, permitindo a troca da representação interna durante a execução, 
 preservando o estado lógico, isto é, sem perda de células. 

Deve oferecer, no mínimo: consulta e atribuição de uma célula, remoção de célula, 
 percurso de uma linha e percurso de uma coluna. 

Deve também expor as operações necessárias ao cálculo das métricas, 
 incluindo o produto interno entre duas linhas esparsas."


* A tabela de modificadores é a matriz modificador [id_item] [id_atributo] em que uma
* célula existe se e somente se o modificador é diferente de zero... Atribuir o valor zero a uma
* célula equivale a removê-la, e a estrutura deve refletir isso."
*
*
* "M <id_item> <id_atributo> <valor>: Define o modificador que o item concede naquele atributo... 
* Se valor for zero, a célula correspondente deixa de existir.
* Se já houver um modificador definido para aquele par, ele é substituído."
*/

#ifndef CATALOGO_H
#define CATALOGO_H

// estrutura de nó para o modo t (vetor de tripletas)
struct node_t {
    int id_item;     // identificador da linha (item)
    int id_atributo; // identificador da coluna (atributo)
    int valor;       // valor do modificador
};

// estrutura de nó para o modo l (lista cruzada ortogonal)
struct node_l { 
    int id_item;           // identificador da linha (item)
    int id_atributo;       // identificador da coluna (atributo)
    int valor;             // valor do modificador

    node_l* next_atributo; // ponteiro para o proximo modificador do mesmo item (mesma linha)
    node_l* next_item;     // ponteiro para o proximo modificador do mesmo atributo (mesma coluna)
};

class catalogo {
private:
    bool modo_atual; // guarda qual representacao (T(0) ou L(1)) esta ativa no momento
    
    int total_itens;      // total de linhas
    int total_atributos;  // total de colunas

    // --- atributos exclusivos do modo T ---
    node_t* elementos_t; // ponteiro para o vetor dinâmico de tripletas
    int tamanho_t;       // quantidade atual de elementos nao nulos armazenados no modo t
    int capacidade_t;    // capacidade maxima alocada no vetor do modo t antes de redimensionar

    // --- atributos exclusivos do modo L ---
    node_l** cabeca_linhas;  // vetor de ponteiros para o primeiro elemento de cada linha (item)
    node_l** cabeca_colunas; // vetor de ponteiros para o primeiro elemento de cada coluna (atributo)

    // --- metodos privados auxiliares do modo t ---

    // verifica se a tripleta 1 deve vir antes da tripleta 2 na ordem do vetor
    bool vem_antes_t(int item1, int atrib1, int item2, int atrib2) const;

    // faz busca binaria no vetor ordenado e retorna a posicao da tripleta ou onde ela deveria ser inserida
    int buscar_posicao_t(int id_item, int id_atributo) const;

    // dobras a capacidade do vetor de tripletas quando ele fica cheio
    void redimensionar_t();

    // insere, altera ou remove um modificador dentro do vetor do modo t
    void definir_modificador_t(int id_item, int id_atributo, int valor);

    // busca e retorna o valor de um modificador no modo t (retorna 0 se nao existir)
    int obter_modificador_t(int id_item, int id_atributo) const;

    // --- metodos privados auxiliares do modo l ---

    // encontra o nó anterior a uma coluna dentro de uma linha especifica no modo l
    node_l* buscar_anterior_linha_l(int id_item, int id_atributo) const;

    // encontra o nó anterior a uma linha dentro de uma coluna especifica no modo l
    node_l* buscar_anterior_coluna_l(int id_item, int id_atributo) const;

    // insere, altera ou remove um nó das listas encadeadas de linha e coluna no modo l
    void definir_modificador_l(int id_item, int id_atributo, int valor);

    // busca e retorna o valor de um modificador no modo l (retorna 0 se nao existir)
    int obter_modificador_l(int id_item, int id_atributo) const;

    // --- metodos privados de migracao e gerenciamento ---

    // libera toda a memoria alocada pelas celulas do modo l
    void limpar_modo_l();

    // libera a memoria alocada pelo vetor do modo t
    void limpar_modo_t();

public:
    // construtor: inicia o catalogo no modo informado (padrao modo l)
    catalogo(modomatriz modo_inicial = modo_l);

    // destrutor: libera toda a memoria alocada do modo ativo
    ~catalogo();

    // atualiza a quantidade de linhas e colunas conforme novos itens e atributos sao cadastrados
    void redimensionar_dimensoes(int novos_itens, int novos_atributos);

    // altera o modo de armazenamento atual (l para t ou t para l) sem perder dados (comando e)
    void alternar_modo(modomatriz novo_modo);

    // insere, altera ou remove um modificador na matriz (comando m)
    void definir_modificador(int id_item, int id_atributo, int valor);

    // consulta o valor de um modificador de um item em um atributo especifico
    int obter_modificador(int id_item, int id_atributo) const;

    // imprime todos os atributos e valores de um item especifico em ordem crescente de atributo (comando li)
    void listar_item(int id_item, const char* nome_item) const;

    // imprime todos os itens e valores de um atributo especifico em ordem crescente de item (comando la)
    void listar_atributo(int id_atributo, const char* nome_atributo) const;

    // calcula o produto interno entre a linha de um item e o vetor de build de um aventureiro (usado na sinergia)
    double produto_interno_build(int id_item, const int* vetor_build) const;

    // calcula a soma de todos os modificadores de um item (usado no poder e equilibrio)
    int somar_modificadores_item(int id_item) const;

    // retorna o modo de armazenamento ativo no momento
    modomatriz get_modo_atual() const;
};

#endif
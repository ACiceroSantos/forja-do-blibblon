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
    node_l** cabeca_linhas;  // vetor de ponteiros para o primeiro elemento / cabeca de cada linha (item)
    node_l** cabeca_colunas; // vetor de ponteiros para o primeiro elemento / cabeca de cada coluna (atributo)

    // --- metodos privados auxiliares do modo t ---

    /*
    Verifica se a celula 1 deve ser ordenada antes da celula 2 no vetor contiguo.
    O vetor do Modo T precisa estar ordenado primeiro por linhas(itens) e depois por colunas (atributos)
    a funcao vem_antes_t é a forma/comparacao padronizada de saber qual celula vem primeiro

    retorna TRUE se a celula 1 vem antes e FALSE se a celula 2 deve vir antes.
    */
    bool vem_antes_t(int item1, int atrib1, int item2, int atrib2) const;

    /*
    int buscar_posicao_t(id_item, id_atributo)

    Faz uma busca binaria no vetor contiguo de tripletas.
    Rretorna o indice exato caso a tripleta exista.
    Caso nao exista, retorna a posicao onde ela deve ser inserida para manter a ordem.
    */
    int buscar_posicao_t(int id_item, int id_atributo) const;

    /*
    dobra o espaco de memoria do vetor dinamico elementos_t.
    o vetor no modo_t tem um limite maximo definido por capacidade_t, quando ele fica cheio
    (tamanho_t == capacidade_t), o programa precisa de mais espaço.
    a funcao redimensionar_t() cria um novo vetor com o dobro do espaço e copia todos os modificadores(nós)
    do vetor antigo para o novo
    */
    void redimensionar_t();

    /*
    essa funcao gerencia a insercao, atualizacao ou remocao de um modificador no vetor contiguo do modo_T
    
    1 - Utiliza a busca binária(buscar_posicao_t) para encontrar o indice alvo
    2 - Se o nó ja existir o valor é atualizado
    2.1 - Caso o valor seja 0, o nó é removido
    2.2 - Se não existir(e o valor for diferente de 0), o nó (modificador) é criado com os elementos
            posteriores sendo empurrados para trás
    */
    void definir_modificador_t(int id_item, int id_atributo, int valor);

    // busca e retorna o valor de um modificador no modo t (retorna 0 se nao existir)
    int obter_modificador_t(int id_item, int id_atributo) const;

    // --- metodos privados auxiliares do modo l ---

    /*
    busca na linha (id_item) o nó anterior àquele que possui o id_atributo procurado.
    como a lista é ordenada pelo id_atributo, a busca para ao encontrar um valor maior ou igual.
    retorna o ponteiro para o nó cujo next é o modificador procurado,
    ou nullptr caso o alvo seja a própria cabeça.
    */
    node_l* buscar_anterior_linha_l(int id_item, int id_atributo) const;


    /*
    busca na coluna (por id_atributo) o nó anterior àquele que possui o id_item procurado.
    como a lista é ordenada pelo id_item, a busca para ao encontrar um valor maior ou igual.
    retorna o ponteiro para o nó anterior, 
    ou nullptr caso o alvo seja a própria cabeça.
    */
    node_l* buscar_anterior_coluna_l(int id_item, int id_atributo) const;


    /*
    funcao responsavel por atualizar, inserir ou remover um nó da Matriz Esparsa do Modo_L.
    cada célula/nó/modificador pertence a duas listas encadeadas simultaneamente,
    a lista da linha next_item
    e a lista da coluna next_atributo

    se o nó já existe e o valor dado != 0, apenas localiza-se o nó e altera o seu valor.
    se o nó já existe e o valor dado == 0, remove-se o nó.

    se o nó NÃO existe e o valor != 0, o nó é criado e alocado na memoria,
    em seguida ele é encadeado em ambas as listas encadeadas
    com o uso da funcao buscar_anterior_linha/coluna_l
    se o nó NÃO existe e o valor == 0, nada é feito.

    */
    void definir_modificador_l(int id_item, int id_atributo, int valor);

    /*
    obtem o valor do modificador para o id_item e id_atributo no modo_l.
    utiliza a busca do nó anterior na linha e trata com seguranca o acesso a cabeca e ponteiros nulos.
    retorna o valor gravado ou 0 caso a celula nao exista.
    */
    int obter_modificador_l(int id_item, int id_atributo) const;

    // --- metodos privados de migracao e gerenciamento ---

    /*
    libera toda a memoria alocada pelas celulas do modo l navegando pelas linhas.
    apos desalocar os nos, ajusta os ponteiros dos vetores de cabeca para nullptr.
    */
    void limpar_modo_l();

    /*
    libera a memoria alocada pelo vetor dinamico do modo t.
    anula o ponteiro elementos_t e reseta o tamanho e a capacidade para zero.
    */
    void limpar_modo_t();

public:
    // construtor: inicia o catalogo no modo informado (padrao modo l)
    catalogo(bool modo_inicial = 1);

    // destrutor: libera toda a memoria alocada do modo ativo
    ~catalogo();

    // atualiza a quantidade de linhas e colunas conforme novos itens e atributos sao cadastrados
    void redimensionar_dimensoes(int novos_itens, int novos_atributos);

    // altera o modo de armazenamento atual (l para t ou t para l) sem perder dados (comando e)
    void alternar_modo(bool novo_modo);

    // insere, altera ou remove um modificador na matriz (comando m)
    void definir_modificador(int id_item, int id_atributo, int valor);

    // consulta o valor de um modificador de um item em um atributo especifico
    int obter_modificador(int id_item, int id_atributo) const;

    // imprime todos os atributos e valores de um item especifico
    void listar_item(int id_item, const std::string& nome_item) const;

    // imprime todos os itens e valores de um atributo especifico
    void listar_atributo(int id_atributo, const std::string& nome_atributo) const;

    // calcula o produto interno entre a linha de um item e o vetor de build de um aventureiro (usado na sinergia)
    double produto_interno_build(int id_item, const int* vetor_build) const;

    // calcula a soma de todos os modificadores de um item (usado no poder e equilibrio)
    int somar_modificadores_item(int id_item) const;

    // retorna o modo de armazenamento ativo no momento
    bool get_modo_atual() const;
};

#endif
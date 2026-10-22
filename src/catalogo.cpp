#include <iostream>
#include "catalogo.h"

using namespace std;

/*
Verifica se a celula 1 deve ser ordenada antes da celula 2 no vetor contiguo.
O vetor do Modo T precisa estar ordenado primeiro por linhas(itens) e depois por colunas (atributos)
a funcao vem_antes_t é a forma/comparacao padronizada de saber qual celula vem primeiro

retorna TRUE se a celula 1 vem antes e FALSE se a celula 2 deve vir antes.
*/

bool catalogo::vem_antes_t(int item1, int atrib1, int item2, int atrib2) const{
    // verifica se o item 1 vem numa linha estritamente anterior ao item 2
    if (item1 < item2){
        return true;
    } else if (item1 == item2){
        // se estiverem na mesma linha, o desempate é pelo atributo (coluna)
        if(atrib1 < atrib2){
            return true;
        } else return false;
    } else return false;
}

/*
int buscar_posicao_t(id_item, id_atributo)

Faz uma busca binaria no vetor contiguo de tripletas.
Rretorna o indice exato caso a tripleta exista.
Caso nao exista, retorna a posicao onde ela deve ser inserida para manter a ordem.
*/

int catalogo::buscar_posicao_t(int id_item, int id_atributo) const{

    node_t* vetor_aux = catalogo::elementos_t;

    //o fim é o ultimo indice valido
    int fim_aux = catalogo::tamanho_t - 1;
    int inicio_aux = 0;
    int meio_aux;

    //caso o elemento seja maior que todos os outros, sua posicao de insercao é o tamanho do vetor
    //sem o '-1'
    int posicao_insercao = catalogo::tamanho_t;

    //o while para quando o inicio ultrapassa o fim (elemento nao existe)
    while (inicio_aux <= fim_aux)
    {
        //calculo do meio
        meio_aux = inicio_aux + (fim_aux - inicio_aux)/2;

        //se encontrar o elemento
        if(vetor_aux[meio_aux].id_item == id_item && vetor_aux[meio_aux].id_atributo == id_atributo){
            return meio_aux;
        }

        //se o que buscamos vem antes
        else if(vem_antes_t(id_item, id_atributo, vetor_aux[meio_aux].id_item, vetor_aux[meio_aux].id_atributo)){

            //como o elemento do meio é vem depois, a posicao de insercao do elemento que buscamos seria aqui,
            //'empurrando' o elemento do meio
            posicao_insercao = meio_aux;

            //o fim agora é o elemento antes do meio
            fim_aux = meio_aux-1;
        }

        //se o que buscamos está depois
        else {
            //o inicio agora é o elemento depois do meio
            inicio_aux = meio_aux+1;
        }

    }

    //se laço acabar e o elemento nao for encontrado, retorna a posicao onde ele deveria ser inserido
    return posicao_insercao;
    
}

/*
dobra o espaco de memoria do vetor dinamico elementos_t.

o vetor no modo_t tem um limite maximo definido por capacidade_t, quando ele fica cheio
(tamanho_t == capacidade_t), o programa precisa de mais espaço.
a funcao redimensionar_t() cria um novo vetor com o dobro do espaço e copia todos os modificadores(nós)
do vetor antigo para o novo
*/

void catalogo::redimensionar_t(){
    if(catalogo::tamanho_t >= catalogo::capacidade_t){

        //cria um novo vetor dinamico com o dobro do espaco (+1 para caso o espaco seja 0)
        node_t* novo_vetor = new node_t[(catalogo::capacidade_t * 2) + 1];

        //copia o vetor de elementos para um vetor auxiliar
        node_t* vetor_aux = catalogo::elementos_t;

        //da ao vetor principal (elementos_t), o espaco reservado pelo novo_vetor
        catalogo::elementos_t = novo_vetor; 

        for (int i = 0; i < catalogo::tamanho_t; i++)
        {
            //copia todos os elementos do vetor auxiliar(com os dados do antigo vetor principal)
            //para o novo vetor principal que agora tem mais espaço
            catalogo::elementos_t[i] = vetor_aux[i];
        }

        //atualiza a informacao sobre a capacidade
        catalogo::capacidade_t = 1 + catalogo::capacidade_t * 2; 

        //deleta o lixo que ficou no vetor auziliar
        delete[] vetor_aux;      
    }
}



/*
essa funcao gerencia a insercao, atualizacao ou remocao de um modificador no vetor contiguo do modo_t

1 - utiliza a busca binaria(buscar_posicao_t) para encontrar o indice alvo
2 - se o nó ja existir o valor é atualizado
2.1 - caso o valor seja 0, o nó é removido
2.2 - se não existir(e o valor for diferente de 0), o nó (modificador) é criado com os elementos
      posteriores sendo empurrados para trás
*/
void catalogo::definir_modificador_t(int id_item, int id_atributo, int valor){
    
    //busca a posicao do modificador
    int posicao = catalogo::buscar_posicao_t(id_item, id_atributo);

    // checagem para confirmar se a celula exata ja existe
    if (posicao < catalogo::tamanho_t && 
        catalogo::elementos_t[posicao].id_item == id_item && 
        catalogo::elementos_t[posicao].id_atributo == id_atributo) {
        
        //se existir e o valor a ser definido é diferente de 0
        if (valor != 0) {
            // atualiza o valor
            catalogo::elementos_t[posicao].valor = valor;
        } 
        
        //se existe e o valor a ser definido é 0 (celula vai ser removida)
        else {
            // valor == 0 o nó deve ser removido e os posteriores puxados para frente
            for (int i = posicao; i < catalogo::tamanho_t - 1; i++) {
                catalogo::elementos_t[i] = catalogo::elementos_t[i + 1];
            }
            catalogo::tamanho_t--;
        }
    } 
    // se o elemento nao existir e valor != 0
    else if (valor != 0) {
        
        // checa e resolve limites fisicos de memoria antes de inserir
        catalogo::redimensionar_t();
    
        // empurra os elementos para tras abrindo espaco na posicao alvo
        for (int i = catalogo::tamanho_t; i > posicao; i--) {
            catalogo::elementos_t[i] = catalogo::elementos_t[i - 1];
        }
        
        // cria o modificador no espaco liberado
        catalogo::elementos_t[posicao].id_item = id_item;
        catalogo::elementos_t[posicao].id_atributo = id_atributo;
        catalogo::elementos_t[posicao].valor = valor;
        
        catalogo::tamanho_t++;
    }
}


int catalogo::obter_modificador_t(int id_item, int id_atributo) const{

    int posicao = catalogo::buscar_posicao_t(id_item, id_atributo);

    /*
    verifica se a posicao nao é invalida (tamanho_t), 
    se o vetor nao é nulo (tamanho_t > posicao >= 0)
    e se ja tem um modificador com tal id e atributo   
    */
    if (posicao < catalogo::tamanho_t &&
        catalogo::elementos_t[posicao].id_item == id_item && 
        catalogo::elementos_t[posicao].id_atributo == id_atributo)
    {
        return catalogo::elementos_t[posicao].valor;
    } else {
        return 0;
    }
}


/*
busca na linha (id_item) o nó anterior àquele que possui o id_atributo procurado.
como a lista é ordenada pelo id_atributo, a busca para ao encontrar um valor maior ou igual.
retorna o ponteiro para o nó cujo next é o modificador procurado,
ou nullptr caso o alvo seja a própria cabeça.
*/
node_l* catalogo::buscar_anterior_linha_l(int id_item, int id_atributo) const{
    
    //acessa a linha correta
    node_l* ponteiro_aux = catalogo::cabeca_linhas[id_item];

    //celula vazia ou a propria cabeca ja e o alvo/passou do alvo
    if(ponteiro_aux == nullptr || ponteiro_aux->id_atributo >= id_atributo){
        return nullptr;
    }

    //caminhar parando no anterior ao alvo
    while (ponteiro_aux->next_atributo != nullptr && 
           ponteiro_aux->next_atributo->id_atributo < id_atributo)
    {
        ponteiro_aux = ponteiro_aux->next_atributo;
    }
    
    //retorna o ponteiro que antecede aquele que procuramos
    return ponteiro_aux;
}


/*
busca na coluna (por id_atributo) o nó anterior àquele que possui o id_item procurado.
como a lista é ordenada pelo id_item, a busca para ao encontrar um valor maior ou igual.
retorna o ponteiro para o nó anterior, 
ou nullptr caso o alvo seja a própria cabeça.
*/
node_l* catalogo::buscar_anterior_coluna_l(int id_item, int id_atributo) const{
    
    //acessa a coluna correta
    node_l* ponteiro_aux = catalogo::cabeca_colunas[id_atributo];

    //celula vazia ou a propria cabeca ja e o alvo/passou do alvo
    if(ponteiro_aux == nullptr || ponteiro_aux->id_item >= id_item){
        return nullptr;
    }

    //caminhar parando no anterior ao alvo
    while (ponteiro_aux->next_item != nullptr && 
           ponteiro_aux->next_item->id_item < id_item)
    {
        ponteiro_aux = ponteiro_aux->next_item;
    }
    
    //retorna o ponteiro que antecede aquele que procuramos
    return ponteiro_aux;
}

/*
obtem o valor do modificador para o id_item e id_atributo no modo_l.
utiliza a busca do nó anterior na linha e trata com seguranca o acesso a cabeca e ponteiros nulos.
retorna o valor gravado ou 0 caso a celula nao exista.
*/
int catalogo::obter_modificador_l(int id_item, int id_atributo) const{

    node_l* node_aux = catalogo::buscar_anterior_linha_l(id_item, id_atributo);
    node_l* node_alvo = nullptr;

    // se o anterior for nulo, a celula alvo e a propria cabeca da linha
    if (node_aux == nullptr) {
        node_alvo = catalogo::cabeca_linhas[id_item];
    } else {
        node_alvo = node_aux->next_atributo;
    }

    // verifica se o no alvo existe e se e exatamente a celula procurada
    if (node_alvo != nullptr && node_alvo->id_atributo == id_atributo) {
        return node_alvo->valor;
    }

    return 0;
}


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

void catalogo::definir_modificador_l(int id_item, int id_atributo, int valor){

    node_l* node_alvo = nullptr;
    
    node_l* node_anterior_linha = buscar_anterior_linha_l(id_item, id_atributo);
    node_l* node_anterior_coluna = buscar_anterior_coluna_l(id_item, id_atributo);

    if (node_anterior_linha == nullptr)
    {
        node_alvo = catalogo::cabeca_linhas[id_item];
    } else {
        node_alvo = node_anterior_linha->next_atributo;
    }
    

    //se o nó existe
    if (node_alvo != nullptr &&
        node_alvo->id_atributo == id_atributo)
    {
        //se valor == 0
        if (valor == 0)
        {
            //remove o nó

            //se a linha anterior for nula, entao o node_alvo é a cabeca da linha
            if (node_anterior_linha == nullptr) {

                //passa a cabeca pro proximo
                catalogo::cabeca_linhas[id_item] = node_alvo->next_atributo;
            } else {

                //se nao for nula desconecta o nó
                node_anterior_linha->next_atributo = node_alvo->next_atributo;
            }

            //desconecta da coluna
            if (node_anterior_coluna == nullptr) {

                catalogo::cabeca_colunas[id_atributo] = node_alvo->next_item;
            } else {

                node_anterior_coluna->next_item = node_alvo->next_item;
            }

            //libera a memoria do nó desvinculado
            delete node_alvo;
        }
        //se valor != 0
        else
        {
            node_alvo->valor = valor;
        }
    } 
    //se o nó nao existe
    else{
        //se valor == 0
        if (valor == 0)
        {
           //nao faz nada
        }
        //se valor != 0 
        else
        {
            //insercao: cria o nó
            node_alvo = new node_l;
            node_alvo->id_atributo = id_atributo;
            node_alvo->id_item = id_item;
            node_alvo->valor = valor;

            // encadeia na linha (horizontal)
            if (node_anterior_linha == nullptr)
            {
                node_alvo->next_atributo = catalogo::cabeca_linhas[id_item];
                catalogo::cabeca_linhas[id_item] = node_alvo;
                
            }else{
                node_alvo->next_atributo = node_anterior_linha->next_atributo;
                node_anterior_linha->next_atributo = node_alvo;
            }
            
            // encadeia na coluna (vertical)
            if (node_anterior_coluna == nullptr)
            {
                node_alvo->next_item = catalogo::cabeca_colunas[id_atributo];
                catalogo::cabeca_colunas[id_atributo] = node_alvo;
            }else{
                node_alvo->next_item = node_anterior_coluna->next_item;
                node_anterior_coluna->next_item = node_alvo;
            }

        }
    }
}




/*
libera toda a memoria alocada pelas celulas do modo l navegando pelas linhas.
apos desalocar os nos, ajusta os ponteiros dos vetores de cabeca para nullptr.
*/
void catalogo::limpar_modo_l(){

    node_l* node_auxiliar = nullptr;
    node_l* next_auxiliar = nullptr;

    // percorre todas as linhas desalocando no por no
    for (int i = 0; i < catalogo::total_itens; i++) {
        node_auxiliar = catalogo::cabeca_linhas[i];

        while (node_auxiliar != nullptr) {
            // guarda o proximo no antes de desalocar o atual
            next_auxiliar = node_auxiliar->next_atributo;
            delete node_auxiliar;
            node_auxiliar = next_auxiliar;
        }

        // reseta a cabeca da linha limpa
        catalogo::cabeca_linhas[i] = nullptr;
    }

    // reseta as cabecas das colunas para evitar ponteiros selvagens
    for (int j = 0; j < catalogo::total_atributos; j++) {
        catalogo::cabeca_colunas[j] = nullptr;
    }
}


/*
libera a memoria alocada pelo vetor dinamico do modo t.
anula o ponteiro elementos_t e reseta o tamanho e a capacidade para zero.
*/
void catalogo::limpar_modo_t(){

    if (catalogo::elementos_t != nullptr)
    {
        delete[] catalogo::elementos_t;
        catalogo::elementos_t = nullptr;
    }
    
    catalogo::tamanho_t = 0;
    catalogo::capacidade_t = 0;
}


/*
construtor da classe catalogo.
inicializa a representacao ativa com o modo informado e reseta os ponteiros
e variaveis de controle de ambos os modos (t e l).
*/
catalogo::catalogo(bool modo_inicial) {

    modo_atual = modo_inicial;
    
    catalogo::total_atributos = 0;
    catalogo::total_itens = 0;

    // inicializacao do modo t
    catalogo::elementos_t = nullptr;
    catalogo::capacidade_t = 0;
    catalogo::tamanho_t = 0;

    // inicializacao do modo l
    catalogo::cabeca_colunas = nullptr;
    catalogo::cabeca_linhas = nullptr;
}

/*
destrutor da classe catalogo.
libera todas as celulas alocadas em ambos os modos e desaloca os vetores
de ponteiros de cabecas de linha e coluna.
*/
catalogo::~catalogo() {
    
    // desaloca os nos armazenados nas estruturas
    catalogo::limpar_modo_l();
    catalogo::limpar_modo_t();

    // libera a memoria dos vetores primarios de ponteiros das cabecas
    delete[] catalogo::cabeca_linhas;
    delete[] catalogo::cabeca_colunas;
}


/*
atualiza o tamanho da matriz (linhas e colunas) conforme novos itens e atributos sao inseridos.
no modo l, realoca os vetores de ponteiros de cabeceira preservando os dados anteriores.
*/
void catalogo::redimensionar_dimensoes(int novos_itens, int novos_atributos) {

    // realocacao do vetor de cabecas de linha (itens) no modo L
    if (novos_itens > catalogo::total_itens) {
        
        // aloca dinamicamente o novo vetor de ponteiros
        node_l** nova_cabeca_linhas = new node_l*[novos_itens];

        // copia as cabecas das linhas ja existentes
        for (int i = 0; i < catalogo::total_itens; i++) {
            nova_cabeca_linhas[i] = catalogo::cabeca_linhas[i];
        }

        // inicializa as posicoes de linha vazias que sobraram com nullptr
        for (int i = catalogo::total_itens; i < novos_itens; i++) {
            nova_cabeca_linhas[i] = nullptr;
        }

        // libera o vetor antigo da memoria
        delete[] catalogo::cabeca_linhas;

        // atualiza o ponteiro do catalogo
        catalogo::cabeca_linhas = nova_cabeca_linhas;
    }

    // realocacao do vetor de cabecas de coluna (atributos) no modo l
    if (novos_atributos > catalogo::total_atributos) {

        // aloca dinamicamente o novo vetor de ponteiros
        node_l** nova_cabeca_colunas = new node_l*[novos_atributos];

        // copia as cabecas das colunas ja existentes
        for (int j = 0; j < catalogo::total_atributos; j++) {
            nova_cabeca_colunas[j] = catalogo::cabeca_colunas[j];
        }

        // inicializa as posicoes de coluna vazias que sobraram com nullptr
        for (int j = catalogo::total_atributos; j < novos_atributos; j++) {
            nova_cabeca_colunas[j] = nullptr;
        }

        // libera o vetor antigo da memoria
        delete[] catalogo::cabeca_colunas;

        // atualiza o ponteiro do catalogo
        catalogo::cabeca_colunas = nova_cabeca_colunas;
    }

    // atualiza os valores de total de itens e atributos
    catalogo::total_itens = novos_itens;
    catalogo::total_atributos = novos_atributos;
}



/*
alterna o modo de armazenamento ativo (modo l <-> modo t) sem perda de dados.
percorre a estrutura atual inserindo os dados no novo modo e limpa a memoria da antiga.
*/
void catalogo::alternar_modo(bool novo_modo) {

    // se o modo solicitado ja for o atual, nenhuma acao e necessária
    if (catalogo::modo_atual == novo_modo) {
        return;
    }

    // alternancia do modo l (1) para o modo t (0)
    if (catalogo::modo_atual == 1 && novo_modo == 0) {
        
        // atualiza o modo atual
        catalogo::modo_atual = 0;

        node_l* node_auxiliar = nullptr;

        // percorre todas as linhas da lista cruzada do modo l
        for (int i = 0; i < catalogo::total_itens; i++) {
            node_auxiliar = catalogo::cabeca_linhas[i];

            // insere cada nó da linha no vetor de tripletas
            while (node_auxiliar != nullptr) {
                catalogo::definir_modificador_t(node_auxiliar->id_item, 
                                                node_auxiliar->id_atributo, 
                                                node_auxiliar->valor);
                node_auxiliar = node_auxiliar->next_atributo;
            }
        }

        // libera a memoria da estrutura antiga do modo l
        catalogo::limpar_modo_l();
    }
    // alternancia do modo t (0) para o modo l (1)
    else if (catalogo::modo_atual == 0 && novo_modo == 1) {

        // atualiza o modo antes de fazer a mudanca
        catalogo::modo_atual = 1;

        // percorre o vetor contiguo e insere cada tripleta na matriz esparsa
        for (int k = 0; k < catalogo::tamanho_t; k++) {
            catalogo::definir_modificador_l(catalogo::elementos_t[k].id_item, 
                                            catalogo::elementos_t[k].id_atributo, 
                                            catalogo::elementos_t[k].valor);
        }

        // libera a memoria do vetor do modo t
        catalogo::limpar_modo_t();
    }
}


/*
insere, altera ou remove um modificador na matriz (comando m).
redireciona a operacao para a funcao interna do modo ativo no momento.
*/
void catalogo::definir_modificador(int id_item, int id_atributo, int valor) {

    // para o modo t (0)
    if (catalogo::modo_atual == 0) {
        catalogo::definir_modificador_t(id_item, id_atributo, valor);
    }
    // para o modo l (1)
    else {
        catalogo::definir_modificador_l(id_item, id_atributo, valor);
    }
}


/*
consulta e retorna o valor de um modificador para um determinado item e atributo.
redireciona a consulta para a funcao especifica do modo ativo (t ou l).
retorna 0 caso o modificador nao esteja definido na matriz esparsa.
*/
int catalogo::obter_modificador(int id_item, int id_atributo) const {

    // roteamento de consulta para o modo t (0)
    if (catalogo::modo_atual == 0) {
        return catalogo::obter_modificador_t(id_item, id_atributo);
    }
    // roteamento de consulta para o modo l (1)
    else {
        return catalogo::obter_modificador_l(id_item, id_atributo);
    }
}


/*
imprime todos os atributos modificados por um item em ordem crescente de id_atributo (comando li).
se o item nao modificar nenhum atributo, imprime VAZIO conforme a especificacao.
*/
void catalogo::listar_item(int id_item, const std::string& nome_item) const {
    cout << "LI " << nome_item;

    bool possui_modificadores = false;
    int contador = 1; // CORREÇÃO: contador para atributo_1, atributo_2...

    // consulta via modo t
    if (catalogo::modo_atual == 0) {
        for (int k = 0; k < catalogo::tamanho_t; k++) {
            if (catalogo::elementos_t[k].id_item == id_item) {
                cout << " atributo_" << contador << " " << catalogo::elementos_t[k].id_atributo 
                     << " " << catalogo::elementos_t[k].valor;
                contador++;
                possui_modificadores = true;
            }
        }
    } 
    // consulta via modo l
    else {
        node_l* node_auxiliar = catalogo::cabeca_linhas[id_item];
        while (node_auxiliar != nullptr) {
            cout << " atributo_" << contador << " " << node_auxiliar->id_atributo 
                 << " " << node_auxiliar->valor;
            contador++;
            possui_modificadores = true;
            node_auxiliar = node_auxiliar->next_atributo;
        }
    }

    if (!possui_modificadores) {
        cout << " VAZIO";
    }

    cout << endl;
}


/*
imprime todos os itens que modificam um atributo especifico em ordem crescente de id_item (comando la).
se nenhum item modificar o atributo, imprime VAZIO conforme a especificacao.
*/
void catalogo::listar_atributo(int id_atributo, const std::string& nome_atributo) const {
    cout << "LA " << nome_atributo;

    bool possui_modificadores = false;
    int contador = 1; // CORREÇÃO: contador para item_1, item_2...

    // consulta via modo t
    if (catalogo::modo_atual == 0) {
        for (int k = 0; k < catalogo::tamanho_t; k++) {
            if (catalogo::elementos_t[k].id_atributo == id_atributo) {
                cout << " item_" << contador << " " << catalogo::elementos_t[k].id_item 
                     << " " << catalogo::elementos_t[k].valor;
                contador++;
                possui_modificadores = true;
            }
        }
    } 
    // consulta via modo l
    else {
        node_l* node_auxiliar = catalogo::cabeca_colunas[id_atributo];
        while (node_auxiliar != nullptr) {
            cout << " item_" << contador << " " << node_auxiliar->id_item 
                 << " " << node_auxiliar->valor;
            contador++;
            possui_modificadores = true;
            node_auxiliar = node_auxiliar->next_item;
        }
    }

    if (!possui_modificadores) {
        cout << " VAZIO";
    }

    cout << endl;
}

/*
calcula o produto interno entre a linha do item e a build do aventureiro.
usado para medir a sinergia na recomendacao.
multiplica e soma os modificadores do item com os da build.
*/
double catalogo::produto_interno_build(int id_item, const int* vetor_build) const {
    double produto_interno = 0.0;

    if (vetor_build == nullptr) {
        return 0.0;
    }

    // calculo via modo t
    if (catalogo::modo_atual == 0) {
        for (int k = 0; k < catalogo::tamanho_t; k++) {
            if (catalogo::elementos_t[k].id_item == id_item) {
                int atrib = catalogo::elementos_t[k].id_atributo;
                produto_interno += static_cast<double>(catalogo::elementos_t[k].valor) * vetor_build[atrib];
            }
        }
    } 
    // calculo via modo l
    else {
        node_l* node_auxiliar = catalogo::cabeca_linhas[id_item];
        while (node_auxiliar != nullptr) {
            int atrib = node_auxiliar->id_atributo;
            produto_interno += static_cast<double>(node_auxiliar->valor) * vetor_build[atrib];
            node_auxiliar = node_auxiliar->next_atributo;
        }
    }

    return produto_interno;
}


/*
soma todos os modificadores nao nulos concedidos por um item.
usado para calcular o poder e a media do equilibrio.
percorre a linha acumulando o valor de cada atributo.
*/
int catalogo::somar_modificadores_item(int id_item) const {
    int soma_modificadores = 0;

    // calculo via modo t
    if (catalogo::modo_atual == 0) {
        for (int k = 0; k < catalogo::tamanho_t; k++) {
            if (catalogo::elementos_t[k].id_item == id_item) {
                soma_modificadores += catalogo::elementos_t[k].valor;
            }
        }
    } 
    // calculo via modo l
    else {
        node_l* node_auxiliar = catalogo::cabeca_linhas[id_item];
        while (node_auxiliar != nullptr) {
            soma_modificadores += node_auxiliar->valor;
            node_auxiliar = node_auxiliar->next_atributo;
        }
    }

    return soma_modificadores;
}
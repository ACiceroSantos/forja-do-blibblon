#include "catalogo.h"
#include <iostream>
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
    
    // 1. acessa a coluna correta
    node_l* ponteiro_aux = catalogo::cabeca_colunas[id_atributo];

    // 2. e 3. celula vazia ou a propria cabeca ja e o alvo/passou do alvo
    if(ponteiro_aux == nullptr || ponteiro_aux->id_item >= id_item){
        return nullptr;
    }

    // 4. caminhar parando no anterior ao alvo
    while (ponteiro_aux->next_item != nullptr && 
           ponteiro_aux->next_item->id_item < id_item)
    {
        ponteiro_aux = ponteiro_aux->next_item;
    }
    
    // 5. retorna o ponteiro que antecede aquele que procuramos
    return ponteiro_aux;
}


/*
responde uma consulta direta do modo L, 
dizendo qual e o valor do modificador para uma determinada combinação de item e atributo


*/
void catalogo::definir_modificador_l(int id_item, int id_atributo, int valor){

    node_l* ponteiro_aux = catalogo::buscar_anterior_coluna_l(id_item, id_atributo);
    //se a celula nao existe retorna 0
    if ()
    {
        /* code */
    }
    

}
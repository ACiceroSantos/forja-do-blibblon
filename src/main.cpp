#include <iostream>
#include <string>
#include <cmath>

#include "catalogo.h"
#include "fila.h"
#include "pilha.h"
#include "atributo.h"
#include "item.h"
#include "aventureiro.h"

using namespace std;

/*
vetores dinamicos primarios para o gerenciamento das entidades cadastradas no sistema.
as variaveis de total armazenam a quantidade atual de entidades, enquanto as variaveis
de capacidade armazenam o tamanho limite alocado na memoria heap.
*/
static Atributo** lista_atributos = nullptr;
static int total_atributos = 0;
static int cap_atributos = 0;

static Item** lista_itens = nullptr;
static int total_itens = 0;
static int cap_itens = 0;

static Aventureiro** lista_aventureiros = nullptr;
static int total_aventureiros = 0;
static int cap_aventureiros = 0;

/*
adiciona um novo atributo ao vetor dinamico do sistema.
caso a quantidade atingir a capacidade maxima alocada, o vetor e redimensionado
dobrando de tamanho para garantir insercoes eficientes.
*/
void adicionar_atributo(const string& nome) {
    // verifica a necessidade de expandir a capacidade do vetor na memoria
    if (total_atributos >= cap_atributos) {
        int nova_cap = cap_atributos == 0 ? 10 : cap_atributos * 2;
        Atributo** novo_vetor = new Atributo*[nova_cap];
        
        // preserva os atributos cadastrados anteriormente
        for (int i = 0; i < total_atributos; i++) {
            novo_vetor[i] = lista_atributos[i];
        }
        
        delete[] lista_atributos;
        lista_atributos = novo_vetor;
        cap_atributos = nova_cap;
    }
    
    // instancia o novo atributo com identificador incremental automatico
    lista_atributos[total_atributos] = new Atributo(total_atributos, nome);
    total_atributos++;
}

/*
adiciona um novo item ao cadastro global do sistema.
aloca dinamicamente a estrutura e realiza a expansao de capacidade quando necessario.
*/
void adicionar_item(const string& nome) {
    // checa o limite fisico do vetor de itens
    if (total_itens >= cap_itens) {
        int nova_cap = cap_itens == 0 ? 10 : cap_itens * 2;
        Item** novo_vetor = new Item*[nova_cap];
        
        // copia os ponteiros dos itens já existentes
        for (int i = 0; i < total_itens; i++) {
            novo_vetor[i] = lista_itens[i];
        }
        
        delete[] lista_itens;
        lista_itens = novo_vetor;
        cap_itens = nova_cap;
    }
    
    // atribui o identificador unico baseado na posicao corrente
    lista_itens[total_itens] = new Item(total_itens, nome);
    total_itens++;
}

/*
cadastra um novo aventureiro no sistema.
instancia a estrutura individual contendo a pilha de build exclusiva daquele aventureiro.
*/
void adicionar_aventureiro(const string& nome) {
    // verifica o limite de aventureiros alocados
    if (total_aventureiros >= cap_aventureiros) {
        int nova_cap = cap_aventureiros == 0 ? 10 : cap_aventureiros * 2;
        Aventureiro** novo_vetor = new Aventureiro*[nova_cap];
        
        // transfere as referencias de aventureiros anteriores
        for (int i = 0; i < total_aventureiros; i++) {
            novo_vetor[i] = lista_aventureiros[i];
        }
        
        delete[] lista_aventureiros;
        lista_aventureiros = novo_vetor;
        cap_aventureiros = nova_cap;
    }
    
    // cria o aventureiro associando seu identificador
    lista_aventureiros[total_aventureiros] = new Aventureiro(total_aventureiros, nome);
    total_aventureiros++;
}

/*
calcula o vetor denso acumulado da build do aventureiro.
percorre todos os itens equipados na pilha do aventureiro do topo ate a base,
consultando a matriz do catalogo e somando os modificadores de cada atributo.
*/
int* calcular_vetor_build(Aventureiro* av, const catalogo& cat) {
    // aloca o vetor denso do tamanho exato do total de atributos
    int* vetor = new int[total_atributos];
    for (int j = 0; j < total_atributos; j++) {
        vetor[j] = 0;
    }

    // obtém o no do topo da pilha de equipamentos do aventureiro
    node_pilha* no_atual = av->get_build().get_top();
    
    // itera por cada item equipado acumulando os modificadores
    while (no_atual != nullptr) {
        for (int a = 0; a < total_atributos; a++) {
            vetor[a] += cat.obter_modificador(no_atual->id_item, a);
        }
        no_atual = no_atual->next;
    }
    
    return vetor;
}

/*
estrutura auxiliar para a ordenacao dos itens na consulta de recomendacao (q).
armazena o identificador do item e a pontuacao obtida.
*/
struct ItemPontuado {
    int id;
    double pontos;
};

/*
ordena os itens por pontuacao decrescente para a recomendacao q.
em caso de empate exato nas pontuacoes, aplica o desempate obrigatorio
pelo identificador do item em ordem crescente.
*/
void ordenar_itens(ItemPontuado* arr, int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            bool trocar = false;
            
            // pontuacao maior antecede pontuacao menor
            if (arr[j].pontos < arr[j + 1].pontos) {
                trocar = true;
            } 
            // criterio de desempate pelo menor id em caso de pontuacoes iguais
            else if (arr[j].pontos == arr[j + 1].pontos) {
                if (arr[j].id > arr[j + 1].id) {
                    trocar = true;
                }
            }
            
            // realiza a troca de posicoes
            if (trocar) {
                ItemPontuado temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

/*
processador principal do sistema.
inicia o catalogo de modificadores, a fila global de encomendas da forja
e entra no laço de leitura e processamento de comandos da entrada.
*/
int main() {
    // inicializa o catalogo com o modo l (1) como padrao inicial
    catalogo cat(1); 
    fila fila_encomendas;

    string comando;
    // laço contínuo para leitura de comandos ate o final do arquivo de entrada
    while (cin >> comando) {

        /*
        comando e: alteracao de modo de armazenamento (e <modo>).
        alterna a matriz entre lista cruzada (l) e vetor de tripletas (t)
        sem perda de dados cadastrados.
        */
        if (comando == "E") {
            char modo;
            cin >> modo;
            bool novo_modo = (modo == 'L' || modo == 'l') ? 1 : 0;
            cat.alternar_modo(novo_modo);
            cout << "E " << modo << endl;
        }

        /*
        comando a: cadastro de atributo (a <nome>).
        registra um novo atributo no sistema e redimensiona a matriz de modificadores.
        */
        else if (comando == "A") {
            string nome;
            cin >> nome;
            adicionar_atributo(nome);
            cat.redimensionar_dimensoes(total_itens, total_atributos);
            cout << "A " << (total_atributos - 1) << endl;
        }

        /*
        comando i: cadastro de item (i <nome>).
        registra um novo item no catálogo e expande as dimensoes da matriz.
        */
        else if (comando == "I") {
            string nome;
            cin >> nome;
            adicionar_item(nome);
            cat.redimensionar_dimensoes(total_itens, total_atributos);
            cout << "I " << (total_itens - 1) << endl;
        }

        /*
        comando m: definicao de modificador (m <id_item> <id_atributo> <valor>).
        atribui um valor para a celula da matriz. se o valor for zero, a celula e removida.
        */
        else if (comando == "M") {
            int id_item, id_atrib, valor;
            cin >> id_item >> id_atrib >> valor;
            cat.definir_modificador(id_item, id_atrib, valor);
            cout << "M " << lista_itens[id_item]->get_nome() << " "
                 << lista_atributos[id_atrib]->get_nome() << " " << valor << endl;
        }

        /*
        comando h: cadastro de aventureiro (h <nome>).
        cria um novo aventureiro no sistema com uma pilha de build inicial vazia.
        */
        else if (comando == "H") {
            string nome;
            cin >> nome;
            adicionar_aventureiro(nome);
            cout << "H " << (total_aventureiros - 1) << endl;
        }

        /*
        comando f: registro de encomenda (f <id_aventureiro> <id_item>).
        insere uma encomenda no final da fila global compartilhada da forja.
        */
        else if (comando == "F") {
            int id_av, id_item;
            cin >> id_av >> id_item;
            int pos = fila_encomendas.adicionar(id_av, id_item);
            cout << "F " << pos << endl;
        }

        /*
        comando z: execucao de turnos de trabalho (z <k>).
        retira k encomendas do inicio da fila na ordem de chegada (fifo)
        e equipa os itens forjados no topo da pilha de build de cada aventureiro.
        */
        else if (comando == "Z") {
            int k;
            cin >> k;
            
            // vetores temporarios para guardar o historico de encomendas atendidas neste turno
            int* forjados_av = new int[k];
            int* forjados_item = new int[k];
            int atendidose = 0;

            int id_av, id_item;
            // processa ate k encomendas ou ate a fila esvaziar
            while (atendidose < k && fila_encomendas.remover(id_av, id_item)) {
                lista_aventureiros[id_av]->get_build().empilhar(id_item);
                forjados_av[atendidose] = id_av;
                forjados_item[atendidose] = id_item;
                atendidose++;
            }

            // imprime a quantidade de encomendas efetivamente forjadas
            cout << "Z " << atendidose << endl;
            if (atendidose > 0) {
                for (int i = 0; i < atendidose; i++) {
                    cout << "encomenda_" << (i + 1) << " " << forjados_av[i] << " " << forjados_item[i];
                    if (i < atendidose - 1) cout << " ";
                }
                cout << endl;
            }

            // libera a memoria dos vetores temporarios da execucao do turno
            delete[] forjados_av;
            delete[] forjados_item;
        }

        /*
        comando d: desequipar item (d <id_aventureiro>).
        remove a ultima peça equipada da pilha do aventureiro (lifo).
        */
        else if (comando == "D") {
            int id_av;
            cin >> id_av;
            int id_item_removido = -1;
            bool desequipou = lista_aventureiros[id_av]->get_build().desempilhar(id_item_removido);

            // imprime o nome do item removido ou vazio caso a build estivesse limpa
            if (desequipou) {
                cout << "D " << lista_aventureiros[id_av]->get_nome() << " "
                     << lista_itens[id_item_removido]->get_nome() << endl;
            } else {
                cout << "D " << lista_aventureiros[id_av]->get_nome() << " VAZIO" << endl;
            }
        }

        // comando li
        else if (comando == "LI") {
            int id_item;
            cin >> id_item;
            cat.listar_item(id_item, lista_itens[id_item]->get_nome());
        }

        // comando la
        else if (comando == "LA") {
            int id_atrib;
            cin >> id_atrib;
            cat.listar_atributo(id_atrib, lista_atributos[id_atrib]->get_nome());
        }

        /*
        comando lh: consulta de build do aventureiro (lh <id_aventureiro>).
        imprime o vetor acumulado somado de todos os equipamentos do aventureiro.
        */
        else if (comando == "LH") {
            int id_av;
            cin >> id_av;
            Aventureiro* av = lista_aventureiros[id_av];
            
            cout << "LH " << av->get_nome();

            // verifica se o aventureiro possui itens equipados
            if (av->get_build().vazia()) {
                cout << " VAZIO" << endl;
            } else {
                // calcula a soma dos modificadores da build
                int* v_build = calcular_vetor_build(av, cat);
                bool imprimiu = false;
                
                int contador = 1; // contador sequencial para a saída
                // exibe apenas os atributos cujo valor final e diferente de zero
                for (int a = 0; a < total_atributos; a++) {
                    if (v_build[a] != 0) {
                        cout << " atributo_" << contador << " " << a << " " << v_build[a];
                        contador++;
                        imprimiu = true;
                    }
                }

                if (!imprimiu) {
                    cout << " VAZIO";
                }
                cout << endl;
                
                // libera o vetor acumulado calculado
                delete[] v_build;
            }
        }

        /*
        comando q: consulta de recomendacao de itens.
        calcula e combina as pontuacoes das metricas especificadas (poder, equilibrio, sinergia),
        descarta itens ja equipados e exibe os n itens de maior recomendacao.
        */
        else if (comando == "Q") {
            int id_av, n, m;
            cin >> id_av >> n >> m;

            // alocacao dos vetores para as metricas e pesos informados na consulta
            string* metricas = new string[m];
            double* pesos = new double[m];
            for (int j = 0; j < m; j++) {
                cin >> metricas[j] >> pesos[j];
            }

            // obtem o vetor denso da build do aventureiro para o calculo da sinergia
            int* v_build = calcular_vetor_build(lista_aventureiros[id_av], cat);

            // inicializa a estrutura de pontuacao global acumulada para todos os itens
            ItemPontuado* pontuacao_final = new ItemPontuado[total_itens];
            for (int i = 0; i < total_itens; i++) {
                pontuacao_final[i].id = i;
                pontuacao_final[i].pontos = 0.0;
            }

            // processa cada uma das m metricas solicitadas na consulta
            for (int j = 0; j < m; j++) {
                ItemPontuado* valores_m = new ItemPontuado[total_itens];

                // calcula o valor bruto de cada item na metrica atual
                for (int i = 0; i < total_itens; i++) {
                    valores_m[i].id = i;
                    
                    // calculo da metrica de poder (soma simples dos modificadores)
                    if (metricas[j] == "PODER") {
                        valores_m[i].pontos = cat.somar_modificadores_item(i);
                    } 
                    // calculo da metrica de equilibrio (desvio populacional dos atributos)
                    else if (metricas[j] == "EQUILIBRIO") {
                        if (total_atributos == 0) {
                            valores_m[i].pontos = 1.0;
                        } else {
                            double pod = cat.somar_modificadores_item(i);
                            // CORRECAO: Forcar divisao por double
                            double mi = pod / static_cast<double>(total_atributos);
                            double soma_quad = 0.0;
                            
                            for (int a = 0; a < total_atributos; a++) {
                                double v = cat.obter_modificador(i, a);
                                soma_quad += (v - mi) * (v - mi);
                            }
                            
                            double sigma = sqrt(soma_quad / static_cast<double>(total_atributos));
                            valores_m[i].pontos = 1.0 / (1.0 + sigma);
                        }
                    } 
                    // calculo da metrica de sinergia (similaridade cosseno com a build)
                    else if (metricas[j] == "SINERGIA") {
                        double prod = cat.produto_interno_build(i, v_build);
                        double norma_item = 0.0, norma_build = 0.0;
                        
                        for (int a = 0; a < total_atributos; a++) {
                            double v = cat.obter_modificador(i, a);
                            // CORRECAO: Cast para double para evitar overflow de int
                            norma_item += v * v;
                            norma_build += static_cast<double>(v_build[a]) * v_build[a]; 
                        }
                        norma_item = sqrt(norma_item);
                        norma_build = sqrt(norma_build);

                        if (norma_item == 0.0 || norma_build == 0.0) {
                            valores_m[i].pontos = 0.0;
                        } else {
                            valores_m[i].pontos = prod / (norma_item * norma_build);
                        }
                    }
                    
                    // realiza o arredondamento obrigatorio para 6 casas decimais antes de ordenar
                    valores_m[i].pontos = round(valores_m[i].pontos * 1000000.0) / 1000000.0;
                }

                // ordena os itens de acordo com a metrica atual
                ordenar_itens(valores_m, total_itens);

                // atribui os pontos por posicao de ranking: (n - rank) multiplicado pelo peso da metrica
                for (int rank = 0; rank < total_itens; rank++) {
                    int id_item = valores_m[rank].id;
                    pontuacao_final[id_item].pontos += (total_itens - rank) * pesos[j];
                }

                // libera a estrutura temporaria da metrica
                delete[] valores_m;
            }

            // mapeia os itens atualmente equipados pelo aventureiro para descarta-los da recomendacao
            bool* equipado = new bool[total_itens];
            for (int i = 0; i < total_itens; i++) {
                equipado[i] = false;
            }

            // marca os ids dos itens na pilha de build do aventureiro
            node_pilha* no_p = lista_aventureiros[id_av]->get_build().get_top();
            while (no_p != nullptr) {
                equipado[no_p->id_item] = true;
                no_p = no_p->next;
            }

            // ordena o resultado combinado final das pontuacoes de todos os itens
            ordenar_itens(pontuacao_final, total_itens);

            // imprime no maximo n itens recomendados que nao estejam equipados
            int impressos = 0;
            for (int i = 0; i < total_itens && impressos < n; i++) {
                int id_i = pontuacao_final[i].id;
                
                // verifica se o item nao esta equipado no aventureiro
                if (!equipado[id_i]) {
                    impressos++;
                    cout << "Q " << impressos << " " << id_i << " ";
                    cout.precision(2);
                    cout << fixed << pontuacao_final[i].pontos << endl;
                }
            }

            // desaloca toda a memoria dinamica temporaria utilizada durante a consulta q
            delete[] metricas;
            delete[] pesos;
            delete[] v_build;
            delete[] pontuacao_final;
            delete[] equipado;
        }
    }

    // desaloca os vetores dinâmicos primarios e todas as entidades instanciadas antes de encerrar
    for (int i = 0; i < total_atributos; i++) {
        delete lista_atributos[i];
    }
    delete[] lista_atributos;

    for (int i = 0; i < total_itens; i++) {
        delete lista_itens[i];
    }
    delete[] lista_itens;

    for (int i = 0; i < total_aventureiros; i++) {
        delete lista_aventureiros[i];
    }
    delete[] lista_aventureiros;

    return 0;
}
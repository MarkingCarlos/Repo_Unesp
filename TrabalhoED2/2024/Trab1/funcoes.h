#define TAMANHO_PLACA 8
#define TAMANHO_MODELO 20
#define TAMANHO_MARCA 20
#define TAMANHO_CATEGORIA 15
#define TAMANHO_STATUS 16
#define ORDEM 5
#define MAXCHAVES ORDEM-1

typedef struct {
char placa[TAMANHO_PLACA];
char modelo[TAMANHO_MODELO];
char marca[TAMANHO_MARCA];
int ano;
char categoria[TAMANHO_CATEGORIA];
int quilometragem;
char status[TAMANHO_STATUS];
} Veiculo;

typedef struct no
{
    int IsFolha;
    int RRN;
    int numeroChaves;
    char Placa[MAXCHAVES][TAMANHO_PLACA];
    int RRNFilhos[MAXCHAVES + 1];
    struct no *filhos[ORDEM];
}no;

typedef struct ArvoreB
{
    no *raiz;
}ArvoreB;

no* criarNo();
void INSERIR(no* arvore, char *placa);
ArvoreB* CriarArvore();

ArvoreB* Dividir(ArvoreB* ArvoreB, char *placa);

void LerArquivo();
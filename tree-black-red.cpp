#include <stdio.h>
#include <stdlib.h>

#define RED   1
#define BLACK 0

typedef struct arvoreRB {
  int info;
  int cor;
  struct arvoreRB *esq;
  struct arvoreRB *dir;
} ArvoreRB;

typedef struct NO* ArvLLRB;
// Acessando a cor de um nó
int eh_no_vermelho(ArvoreRB * no){
  if(!no) return BLACK;
  return(no->cor == RED);
}

//trocando a cor de um nó
void trocaCor (struct ArvoreRB no) {
	no->cor = !no->cor
	if(no->esq != NULL)
		no->esq->cor = !no->esq->cor;
	if(no->dir != NULL)
		no->dir->cor = !no->dir->cor;
}

//Rotação esquerda
struct ArvoreRB* rotacionaEsquerda (struct ArvoreRB* A) {
	struct ArvoreRB* B = A->dir;
	A->dir = B->esq;
	B->esq = A;
	B->cor = A->cor;
	A->cor = RED;
	return B;
}

//Rotação direita
struct ArvoreRB* rotacionaDireita (struct ArvoreRB* A) {
	struct ArvoreRB* B = A->esq;
	A->esq = B->dir;
	B->dir = A;
	B->cor = A->cor;
	A->cor = RED;
	return B;
}

//Mover nó vermelho para a esquerda
struct ArvoreRB* move2EsqRED (struct ArvoreRB* H) {
	trocaCor(H);
	if (cor(H->dir->esq) == RED) {
		H->dir = rotacionaDireita(H->dir);
		H = rotacionaEsquerda(H);
		trocaCor(H);
	}
	return H;
}

//Mover nó vermelho para a direita
struct ArvoreRB* move2DirRED (struct ArvoreRB* H) {
	trocaCor(H);
	if (cor(H->esq->esq) == RED) {
		H = rotacionaDireita(H);
		trocaCor(H);
	}
	return H;
}

//Arrumar balanceamento
struct ArvoreRB* balancear(struct ArvoreRB* H) {
	//nó vermelho é sempre filho esquerda
	if(cor(H->dir) == RED)
		H = rotacionaEsquerda(H);
	//ambos filhos vermelhos
	if(H->cor != NULL && cor(H->dir) == RED && cor(H->esq->esq) == RED)
		H = rotacionaDireita(H);
	//dois filhos vermelhos, troca cor
	if(cor(H->esq) == RED && cor(H->dir) == RED)
		trocaCor(H);
	return H;
}

//inicio da árvore 
ArvLLRB* cria_ArvLLRB(){
    ArvLLRB* raiz = (ArvLLRB*) malloc(sizeof(ArvLLRB));
    if(raiz != NULL){
        *raiz = NULL;
    }
    return raiz;
}

//inserir na árvore
int insere_ArvLLRB (ArvoreRB* raiz, int valor) {
	int resp;
	*raiz = insereNO(*raiz, valor, &resp);
	if((*raiz) != NULL)
		(*raiz)->cor = BLACK;
	return resp;
}

//Insere no
struct ArvoreRB* insereNO(struct ArvoreRB* H, int valor, int *resp) {
	if(H == NULL) {
		struct ArvoreRB* novo;
		novo = (struct ArvoreRB*)malloc(sizeof(struct NO));
		if(novo == NULL) {
			*resp = 0;
			return NULL;
		}
		novo->info = valor;
		novo->cor = RED;
		novo->dir = NULL;
		novo->esq = NULL;
		*resp = 1;
		return novo;
	}
	if(valor == H->info)
		*resp = 0;
	else {
		if(valor < H->info)
			H->esq = intereNO(H->esq, valor, resp);
		else
			H->dir = insereNO(H->dir, valor, resp);
	}
	if(cor(H->dir) == RED && cor(H->esq) == BLACK)
		H = rotacionaEsquerda(H);
	if(cor(H->esq) == RED && cor(H->esq->esq) == RED)
		H = rotacionaDireita(H);
	if(cor(H->esq) == RED && cor(H->dir) == RED)
		trocaCor(H);
	return H;
}

//Remover menor
struct ArvoreRB* removerMenor (struct ArvoreRB* H) {
	if(H->esq == NULL) {
		free(H);
		return NULL;
	}
	if(cor(H->esq) == BLACK && cor(H->esq->esq) == BLACK)
		H = move2EsqRED(H);
	
	H->esq = removerMenor(H->esq);
	return balancear(H);
}

struct ArvoreRB* procuraMenor (struct ArvoreRB* atual) {
	struct ArvoreRB* no1 = atual;
	struct ArvoreRB* no2 = atual->esq;
	while(no2 != NULL) {
		no1 = no2;
		no2 = no2->esq;
	}
	return no1;
}

//Remove arvore
int remove_ArvLLRB(ArvoreRB* raiz, int valor) {
	if(consulta_ArvLLRB(raiz, valor)) {
		struct ArvoreRB* h = *raiz;
		*raiz = remove_NO(h, valor);
		if(*raiz != NULL)
			(*raiz)->cor = BLACK;
		return 1;
	} else
		return 0;
}

//remover no
struct ArvoreRB* remove-NO(struct ArvoreRB* H, int valor) {
	if(valor < H->info) {
		if(cor(H->esq) == BLACK && cor(H->esq->esq) == BLACK)
			H = move2EsqRED(H);
		H->esq = remove_NO(H->esq, valor);
	} else {
		if(cor(H->esq) == RED)
			H = rotacionaDireita(H);
		if(valor == H->info && (H->dir == NULL)) {
			free(H);
			return NULL;
		}
		if(cor(H->dir) == BLACK && cor(H->dir->esq) == BLACK)
			H = move2DirRED(H);
		if(valor == H->info) {
			struct ArvoreRB* x = procuraMenor(H->dir);
			H->info = x->info;
			H->dir = removeMenor(H->dir);
		} else
			H->dir = remove_NO(H->dir, valor);
	}
	return balancear(H);
}

int buscar (ArvoreRB *a, int v) {
  if (a == NULL) { return 0; } /*Nao achou*/
  else if (v < a->info) {
    return buscar (a->esq, v);
  }
  else if (v > a->info) {
    return buscar (a->dir, v);
  }
  else { return 1; } /*Achou*/
}

void in_order(ArvoreRB *a){
  if(!a)
    return;
  in_order(a->esq);
  printf("%d ",a->info);
  in_order(a->dir);
}

void print(ArvoreRB * a,int spaces){
  int i;
  for(i=0;i<spaces;i++) printf(" ");
  if(!a){
    printf("//\n");
    return;
  }

  printf("%d\n", a->info);
  print(a->esq,spaces+2);
  print(a->dir,spaces+2);
}

int main(){
  ArvoreRB * a;

}


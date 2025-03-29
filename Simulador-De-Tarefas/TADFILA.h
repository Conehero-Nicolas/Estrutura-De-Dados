#define MAXFILAC 7

struct TpTarefa {
	char Tipo[20], NomeTarefa[70], Responsavel[30], DataInicio[12];
	int TempoConclusao, EntraFila;
};


struct TpFilaC
{
	int fim;
	TpTarefa FILAC[MAXFILAC];
};

void Inicializar(TpFilaC &Reg);
void Inserir(TpFilaC &Reg, TpTarefa Elem);
TpTarefa Retirar(TpFilaC &Reg);
bool Cheia(int Qtde);
bool Vazia(int Qtde);
TpTarefa ElementoFim(TpFilaC Reg);
TpTarefa ElementoInicio(TpFilaC Reg);
void Exibir(TpFilaC Reg);

void Inicializar(TpFilaC &Reg)
{
	Reg.fim = -1;
}

void Inserir(TpFilaC &Reg, TpTarefa Elem)
{	
	int i;

    Reg.FILAC[++Reg.fim] = Elem;
    i = Reg.fim;
    while (i > 0 && Reg.FILAC[i - 1].Tipo[0] > Reg.FILAC[i].Tipo[0]) {
        Reg.FILAC[i] = Reg.FILAC[i - 1];
        Reg.FILAC[i - 1] = Elem;
        i--;
    }
}

TpTarefa Retirar(TpFilaC &Reg)
{
	TpTarefa aux = Reg.FILAC[0];
	int i = 0;
	
	for (i=0; i < Reg.fim; i++) {
		Reg.FILAC[i] = Reg.FILAC[i + 1];
	}
	Reg.fim--;
	
	return aux;
}

TpTarefa ElementoFim(TpFilaC Reg)
{
	return Reg.FILAC[Reg.fim];
}

TpTarefa ElementoInicio(TpFilaC Reg)
{
	return Reg.FILAC[0];
}

char Cheia(TpFilaC Reg)
{
	return Reg.fim == MAXFILAC - 1;
}

char Vazia(TpFilaC Reg)
{
	return Reg.fim == -1;
}

void Exibir(TpFilaC Reg)
{
	int linha = 5;
	TpTarefa aux;
	while(!Vazia(Reg)){
		aux = Retirar(Reg);
		gotoxy(85,linha++);
		printf("Nome: %s", aux.NomeTarefa);
		gotoxy(85,linha++);
		printf("Tipo: %s", aux.Tipo);
		gotoxy(85,linha++);
		printf("Tempo de Conclusao: %d", aux.TempoConclusao);
		gotoxy(85,linha++);
		printf("Data de Inicio: %s", aux.DataInicio);
		linha++;
	}
}

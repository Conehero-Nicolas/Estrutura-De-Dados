#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <windows.h>
#include <conio.h>
#include <time.h>
#include <iostream>
#include <locale.h>
#include "meuconio.h"
#include "TadTarefas.h"
#define MAXDEVS 5

struct TpDev {
	TpTarefa Tarefa;
	int status = 1, TempoTarefa = 0;
};

struct TpCritico {
	int feito=0, total=0, tempoEspera = 0;
};
struct TpImportante {
	int feito=0, total=0, tempoEspera = 0;
};
struct TpMelhoria {
	int feito=0, total=0, tempoEspera = 0;
};

TpTarefa lerArquivo(FILE *arqTarefa, int qtdTempo) {
	TpTarefa reg;
    fscanf(arqTarefa,"%[^,],%d,%[^,],%[^,],%s\n", &reg.Tipo, &reg.TempoConclusao, &reg.NomeTarefa, &reg.Responsavel, &reg.DataInicio);
    reg.EntraFila = qtdTempo;
    return reg;
}

int tempoAleatorio() {
	return rand() % 10;
}

void tempoEspera(TpDev Devs[], int contDev, int qtdTempo, TpCritico &Cri,TpImportante &Imp, TpMelhoria &Mel) {
	if(Devs[contDev].Tarefa.Tipo[0] == 'C') {
		Cri.tempoEspera += Devs[contDev].Tarefa.EntraFila - qtdTempo;
	}
	else{
		if(Devs[contDev].Tarefa.Tipo[0] == 'I')
			Imp.tempoEspera += Devs[contDev].Tarefa.EntraFila - qtdTempo;
		else
			Mel.tempoEspera += Devs[contDev].Tarefa.EntraFila - qtdTempo;
	}
}

void tempoEsperaFila(TpFilaC Tarefas, TpCritico &Cri,TpImportante &Imp, TpMelhoria &Mel) {
	TpTarefa aux;
	while(!Vazia(Tarefas)) {
		aux = Retirar(Tarefas);
		
		if(aux.Tipo[0] == 'C') {
			Cri.tempoEspera += aux.EntraFila;
		}
		else{
			if(aux.Tipo[0] == 'I')
				Imp.tempoEspera += aux.EntraFila;
			else
				Mel.tempoEspera += aux.EntraFila;
		}
	}
}

void adicionaFila(FILE *arqTarefa, TpFilaC &Tarefas, TpCritico &Cri, TpImportante &Imp, TpMelhoria &Mel, int qtdTempo) {
	TpTarefa novaTarefa;
	novaTarefa = lerArquivo(arqTarefa, qtdTempo);
	Inserir(Tarefas, novaTarefa);
	
	if(novaTarefa.Tipo[0] == 'C')
		Cri.total++;
	else{
		if(novaTarefa.Tipo[0] == 'I')
			Imp.total++;
		else
			Mel.total++;
	}
}

void atribuiTarefa(TpFilaC &Tarefas, TpDev Devs[], int qtdDev, int qtdTempo, TpCritico &Cri,TpImportante &Imp, TpMelhoria &Mel) {
	int contDev = 0;
	while(!Vazia(Tarefas) && contDev < qtdDev) {
		if(Devs[contDev].status == 1) {
			Devs[contDev].Tarefa = Retirar(Tarefas);
			Devs[contDev].status = 0;
			Devs[contDev].TempoTarefa = 0;
			tempoEspera(Devs, contDev, qtdTempo, Cri, Imp, Mel);
		}
		contDev++;
	}
}

void verificaTarefa(TpDev Devs[], int qtdDev, TpFilaC Tarefas, TpCritico &Cri,TpImportante &Imp, TpMelhoria &Mel) {
	int contDev = 0;
	
	while(contDev < qtdDev) {
		if(Devs[contDev].status == 0 && Devs[contDev].Tarefa.TempoConclusao == Devs[contDev].TempoTarefa) {
			Devs[contDev].status = 1;
			if(Devs[contDev].Tarefa.Tipo[0] == 'C') {
				Cri.feito++;
			}
			else{
				if(Devs[contDev].Tarefa.Tipo[0] == 'I')
					Imp.feito++;
				else
					Mel.feito++;
			}
		}
		contDev++;
	}
}

int acabouTarefas(TpDev Devs[], TpFilaC Tarefas, FILE *arqTarefa, int qtdDev) {
	int vazio = 1, contDev = 0, devVazio = 0;
	
	if(Vazia(Tarefas) && feof(arqTarefa)) {
		while (contDev < qtdDev) {
	        if (Devs[contDev].status == 1)
	        	devVazio ++;
	        contDev++;
	    }
	}
	
	if(devVazio == qtdDev) 
		vazio = 0;
		
	return vazio;
}

void contaTempoTarefas(TpDev Devs[], int qtdDev) {
	int contDev = 0;
	while(contDev < qtdDev) {
		if(Devs[contDev].status == 0) {
			Devs[contDev].TempoTarefa++;
		}
		contDev++;
	}
}

void desenharMoldura(int x1, int y1, int x2, int y2) {
    int i;

    char cantoSupEsq = 201, cantoSupDir = 187;
    char cantoInfEsq = 200, cantoInfDir = 188;
    char horizontal = 205, vertical = 186;
    
	clrscr();
	textcolor(15); 
    textbackground(0);
    gotoxy(x1, y1);
    printf("%c", cantoSupEsq);
    for (i = x1 + 1; i < x2; i++) {
        printf("%c", horizontal);
    }
    printf("%c", cantoSupDir);

    for (i = y1 + 1; i < y2; i++) {
        gotoxy(x1, i);
        printf("%c", vertical);
        gotoxy(x2, i);
        printf("%c", vertical);
    }

    gotoxy(x1, y2);
    printf("%c", cantoInfEsq);
    for (i = x1 + 1; i < x2; i++) {
        printf("%c", horizontal);
    }
    printf("%c", cantoInfDir);
    gotoxy(10,5);
}

void desenharMoldura2(int x1, int y1, int x2, int y2) {
    int i;

    char cantoSupEsq = 201, cantoSupDir = 187;
    char cantoInfEsq = 200, cantoInfDir = 188;
    char horizontal = 205, vertical = 186;

	textcolor(15); 
    textbackground(0);
    gotoxy(x1, y1);
    printf("%c", cantoSupEsq);
    for (i = x1 + 1; i < x2; i++) {
        printf("%c", horizontal);
    }
    printf("%c", cantoSupDir);

    for (i = y1 + 1; i < y2; i++) {
        gotoxy(x1, i);
        printf("%c", vertical);
        gotoxy(x2, i);
        printf("%c", vertical);
    }

    gotoxy(x1, y2);
    printf("%c", cantoInfEsq);
    for (i = x1 + 1; i < x2; i++) {
        printf("%c", horizontal);
    }
    printf("%c", cantoInfDir);
    gotoxy(10,5);
}

void exibeDevs(TpDev Devs[], int qtdDev, int x1, int y1, int x2, int y2) {
    int contDev = 0, linha;
    linha = y1 + 2;

    desenharMoldura2(x1, y1, x2, y2);
	gotoxy(32, 1);
    printf("DESENVOLVEDORES");
    while (contDev < qtdDev) {
        if (Devs[contDev].status == 0) { 
            
            gotoxy(x1 + 2, linha++);
            printf("  Tarefa: %s", Devs[contDev].Tarefa.NomeTarefa);
        
            gotoxy(x1 + 2, linha++);
            printf("  Tipo: %s", Devs[contDev].Tarefa.Tipo);
        
            gotoxy(x1 + 2, linha++);
            printf("  Responsavel: %s", Devs[contDev].Tarefa.Responsavel);
        
            gotoxy(x1 + 2, linha++);
            printf("  Data Inicio: %s", Devs[contDev].Tarefa.DataInicio);
        
            gotoxy(x1 + 2, linha++);
            printf("  Tempo Conclusao: %d min", Devs[contDev].Tarefa.TempoConclusao);
        
            gotoxy(x1 + 2, linha++);
            printf("  Tempo Tarefa: %d min", Devs[contDev].TempoTarefa);
        	
        	linha++;
        }
        contDev++;
    }
}

void exibeFila(TpFilaC Tarefas, int x1, int y1, int x2, int y2) {
	desenharMoldura2(x1, y1, x2, y2);
	gotoxy(108,1);
	printf("FILA DE TAREFAS");
	if(!Vazia(Tarefas))
		Exibir(Tarefas);
}

void Relatorio(TpCritico Cri,TpImportante Imp,TpMelhoria Mel){
	
	int x1 = 5, y1 = 3, x2 = 110, y2 = 30;
	desenharMoldura(x1, y1, x2, y2);
	gotoxy(8,5);
	textcolor(8);
	printf("Tipo Critico");
	gotoxy(8,7);
	textcolor(4);
	printf("Pendente: %d", Cri.total - Cri.feito);
	gotoxy(8,8);
	textcolor(2);
	printf("Feito: %d", Cri.feito);
	gotoxy(8,9);
	textcolor(15);	
	printf("Tempo Medio de Espera: %.2f minutos", (float)Cri.tempoEspera / Cri.total);
	
	gotoxy(8,11);
	textcolor(8);
	printf("----------------------------------------------------------------------------------------------------");
	
	gotoxy(8,13);
	printf("Tipo Importante");
	gotoxy(8,15);
	textcolor(4);
	printf("Pendente: %d", Imp.total - Imp.feito);
	gotoxy(8,16);
	textcolor(2);
	printf("Feito: %d", Imp.feito);
	gotoxy(8,17);
	textcolor(15);
	printf("Tempo Medio de Espera: %.2f minutos", (float)Imp.tempoEspera / Imp.total);
	
	gotoxy(8,19);
	textcolor(8);
	printf("----------------------------------------------------------------------------------------------------");
	
	gotoxy(8,21);
	printf("Tipo Melhoria");
	gotoxy(8,23);
	textcolor(4);
	printf("Pendente: %d", Mel.total - Mel.feito);
	gotoxy(8,24);
	textcolor(2);
	printf("Feito: %d", Mel.feito);
	gotoxy(8,25);
	textcolor(15);
	printf("Tempo Medio de Espera: %.2f minutos", (float)Mel.tempoEspera / Mel.total);
	
}

void simulacao(int qtdDev, int qtdTempo) {
	FILE *arqTarefa = fopen("ArquivoTarefa.txt", "r");
	TpTarefa novaTarefa, aux;
	TpFilaC Tarefas;
	TpDev Devs[qtdDev];
	TpCritico Cri;
	TpImportante Imp;
	TpMelhoria Mel;
	int novaTarefaTempo = tempoAleatorio(), x1 = 5, y1 = 3, x2 = 110, y2 = 30, contDev = 0;
	Inicializar(Tarefas);
	
	while(qtdTempo > 0 && acabouTarefas(Devs, Tarefas, arqTarefa, qtdDev)) {
		
		if(novaTarefaTempo <= 0) {
			novaTarefaTempo = tempoAleatorio();
			if(!feof(arqTarefa) && !Cheia(Tarefas)) {
				adicionaFila(arqTarefa, Tarefas, Cri, Imp, Mel, qtdTempo);
			}
		}
		
		clrscr();
		
		contaTempoTarefas(Devs, qtdDev);
		
		exibeDevs(Devs, qtdDev, 5, 3, 75, 40);
		exibeFila(Tarefas, 80, 3, 150, 40);
		
		atribuiTarefa(Tarefas, Devs, qtdDev, qtdTempo, Cri, Imp, Mel);
		verificaTarefa(Devs, qtdDev, Tarefas, Cri, Imp, Mel);
		
		novaTarefaTempo--;
		qtdTempo --;
		
		Sleep(1000);
	}
	
	tempoEsperaFila(Tarefas, Cri, Imp, Mel);
	Relatorio(Cri,Imp,Mel);
	fclose(arqTarefa);
	getch();
}

void limpar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int main() {
    int x1 = 5, y1 = 3, x2 = 110, y2 = 30, qtdDev, qtdTempo, resultado;
    
	srand(time(NULL));
    
    desenharMoldura(x1, y1, x2, y2);
	
	printf("Digite ENTER para iniciar a simulacao: ");
	
    if(getch() == 13) {
	    do {
	        clrscr();
	        desenharMoldura(x1, y1, x2, y2);
	        gotoxy(10, 5);
	        printf("Quantos devs para essa simulacao? ");
	
	        resultado = scanf("%d", &qtdDev);
	        if (resultado != 1 || qtdDev <= 0 || qtdDev > MAXDEVS) { //verifica se é possivel ler o numero inteiro e se é positivo
			  	gotoxy(10, 8);
	            printf("Entrada invalida! Digite um numero inteiro entre 1 e %d.", MAXDEVS);
	            limpar_buffer(); //caso a entrada seja invalida
	            getch();
	        }
	
	    } while (resultado != 1 || qtdDev <= 0 || qtdDev > MAXDEVS);
	
	    do {
	        clrscr();
	        desenharMoldura(x1, y1, x2, y2);
	        gotoxy(10, 5);
	        printf("Por quanto tempo sera feita a simulacao? ");
	
	        resultado = scanf("%d", &qtdTempo);
	        if (resultado != 1 || qtdTempo <= 0) {
	        	gotoxy(10, 8);
	            printf("Entrada invalida! Digite um numero inteiro positivo.");
	            limpar_buffer();
	            getch();
	        }
	
	    } while (resultado != 1 || qtdTempo <= 0);
	    
		simulacao(qtdDev, qtdTempo);
	}
	
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <locale.h>
#define max 4
#define delay 1
#define cor1 "\033[31m"
#define cor2 "\033[36m"
#define cor3 "\033[33m"
#define reset "\033[0m \e[m"

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#define sleep(seconds) Sleep((seconds) * 1000)
#elif defined(__linux__) || defined(__APPLE__)
#include <unistd.h>
#else
#error "Sistema operacional não suportado!"
#endif

typedef enum play
{
    ROCK,
    PAPER,
    SCISSOR
} Play;

typedef enum result
{
    VICTORY,
    DRAW,
    DEFEAT
} Result;

// Struct do jogador com nome e pontuação
typedef struct player
{
    char name[max];
    int score;
} Player;

typedef struct node
{
    int round;
    Result result;
    struct node *next;
} Node;

typedef struct match
{
    Node *head;
    Player player;
    int victories;
    int draws;
    int defeats;
    int rounds;
} Match;

void initialize(FILE *file, Match *match);
void logoDisplay(FILE *file);
void commands();
void convert(char objeto[], int opcao);
void roundResult(Match *match, Result result);
void draft(char jogada[], Match *match);
void moveResult(char objeto[], char robo[], Match *match);
void tempo(double duracao);
void score(Match *match, double duracao);
void finish(Match *match);

int main()
{
    FILE *logo;
    Match match;
    initialize(logo, &match);
    setlocale(LC_ALL, "utf-8");
    srand(time(NULL));
    int option;
    char move[8];
    time_t start, end;
    displayLogo(logo);
    printf("Pressione qualquer tecla para iniciar o jogo!\n");
    sleep(delay);
    getchar();
    printf("Iniciando");
    for (int i = 0; i < 3; i++)
    {
        printf(" .");
        sleep(delay);
    }
    printf("\n");
    time(&start);
    while (true)
    {
        commands();
        scanf("%d", &option);
        if (option == 0)
            break;
        else if (option != 1 && option != 2 && option != 3)
            continue;
        convert(move, option);
        draft(move, &match);
    }
    time(&end);
    double duracao = difftime(end, start);
    score(&match, duracao);
    fclose(logo);
    return 0;
}

void initialize(FILE *file, Match *match)
{
    file = fopen("logo.txt", "r");
    if (file == NULL)
    {
        printf("Erro na leitura do <logo.txt>\n");
        exit(1);
    }
    match->head = NULL;
    match->rounds = 0;
    match->victories = 0;
    match->draws = 0;
    match->defeats = 0;
    printf("Digite um nome de 3 digitos: ");
    fgets(match->player.name, max, stdin);
}

void displayLogo(FILE *file)
{
    char row[255];
    for (int i = 0; i != 12; i++)
    {
        fgets(row, 255, file);
        printf("%s", row);
        sleep(delay);
    }
}

void commands()
{
    printf("Escolha uma das opções abaixo!\n");
    printf("\t[0] - Sair\t\n");
    sleep(delay);
    printf("\t[1] - Pedra\t\n");
    sleep(delay);
    printf("\t[2] - Papel\t\n");
    sleep(delay);
    printf("\t[3] - Tesoura\t\n");
    sleep(delay);
}

void convert(char vector[], int option)
{
    switch (option)
    {
    case 1:
        strcpy(vector, "pedra");
        break;
    case 2:
        strcpy(vector, "papel");
        break;
    case 3:
        strcpy(vector, "tesoura");
        break;
    }
}

void roundResult(Match *match, Result result)
{
    Node *novo = (Node *)malloc(sizeof(Node));
    if (novo == NULL)
        return;
    novo->result = result;
    novo->round = ++(match->rounds);
    novo->next = NULL;
    if (result == VICTORY)
        match->victories++;
    else if (result == DRAW)
        match->draws++;
    else if (result == DEFEAT)
        match->defeats++;
    if (match->head == NULL)
    {
        match->head = novo;
    }
    else
    {
        Node *temp = match->head;
        while (temp->next != NULL)
            temp = temp->next;
        temp->next = novo;
    }
}

void draft(char jogada[], Match *match)
{
    int opcao = rand() % 3 + 1;
    char robo[8];
    convert(robo, opcao);
    printf("Você escolheu %s\n", jogada);
    printf("Oponente escolheu %s\n", robo);
    moveResult(jogada, robo, match);
}

void moveResult(char objeto[], char robo[], Match *match)
{
    if ((strcmp(objeto, "pedra") == 0 && strcmp(robo, "tesoura") == 0) || (strcmp(objeto, "papel") == 0 && strcmp(robo, "pedra") == 0) || (strcmp(objeto, "tesoura") == 0 && strcmp(robo, "papel") == 0))
    {
        printf("O resultado é " cor2 "vitória!" reset "\n");
        roundResult(match, VICTORY);
    }
    else if (strcmp(objeto, robo) == 0)
    {
        printf("O resultado é " cor3 " empate!" reset "\n");
        roundResult(match, DRAW);
    }
    else
    {
        printf("O resultado é " cor1 "derrota!" reset "\n");
        roundResult(match, DEFEAT);
    }
}

void tempo(double duracao)
{
    int horas, minutos, segundos;

    horas = (int)(duracao / 3600);
    duracao = (int)duracao % 3600;

    minutos = (int)(duracao / 60);
    segundos = (int)duracao % 60;

    printf("Duração: %dh %dmin %ds\n", horas, minutos, segundos);
}

void score(Match *match, double duracao)
{
    printf("\t\t\nPlacar final\n");
    printf("Vitórias: %d\tEmpates: %d\tDerrotas: %d\t\n", match->victories, match->draws, match->defeats);
    printf("Total de Rodadas: %d\n", match->rounds);
    tempo(duracao);
    if (match->head != NULL)
    {
        Node *temp = match->head;
        while (temp != NULL)
        {
            printf("Rodada: %d\t", temp->round);
            printf("%s\n", (temp->result == VICTORY) ? cor2 "vitória!" reset : (temp->result == DRAW) ? cor3 " empate!" reset: cor1 "derrota!" reset);
            temp = temp->next;
        }
    }
    finish(match);
}

void finish(Match *match)
{
    printf("Pressione qualquer tecla para finalizar o jogo!\n");
    sleep(delay);
    getchar();
    while(match->head != NULL){
        Node *temp = match->head;
        match->head = match->head->next;
        free(temp);
    }
    printf("Finalizando");
    for (int i = 0; i < 3; i++)
    {
        printf(" .");
        sleep(delay);
    }
    printf("\n");
    printf("Obrigado por jogar!\n");
}
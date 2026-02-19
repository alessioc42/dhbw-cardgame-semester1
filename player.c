#include "player.h"

#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <string.h>

unsigned int gebeKarteAnSpieler(struct playerStruct *player, struct playingCard cards[CARDS_LEN], unsigned int currentTopCard) {
    if (currentTopCard == 0 || player->numberOfCards == ROUNDS) {
        wprintf(L"Error: No cards available or player stack full!");
        exit(1);
    }

    player->cards[player->numberOfCards] = &cards[currentTopCard];
    player->numberOfCards++;
    return currentTopCard - 1;
}

void setzeSpielerName(wchar_t name[PLAYER_NAME_MAX_LEN]) {
    wchar_t inputBuffer[PLAYER_NAME_MAX_LEN + 1]; // PLAYER_NAME_MAX_LEN characters + null terminator

    while (1) {
        wprintf(L"* Bitte geben Sie Ihren Namen ein: ");

        if (fgetws(inputBuffer, sizeof(inputBuffer) / sizeof(wchar_t), stdin) == NULL) {
            clearerr(stdin);
            wprintf(L"\033[38;5;124mFehlerhafte Eingabe, bitte versuchen Sie es erneut!\033[0m\n");
            continue;
        }

        size_t len = wcslen(inputBuffer);
        if (len == 0) {
            wprintf(L"\033[38;5;124mName darf nicht leer sein!\033[0m\n");
            continue;
        }

        if (inputBuffer[len - 1] == L'\n') {
            inputBuffer[len - 1] = L'\0';
            len--;
        } else {
            wint_t ch;
            while ((ch = getwchar()) != WEOF && ch != L'\n');
            wprintf(L"\033[38;5;124mName zu lang! Maximal %d Zeichen erlaubt.\033[0m\n", PLAYER_NAME_MAX_LEN - 1);
            continue;
        }

        if (len == 0) {
            wprintf(L"\033[38;5;124mName darf nicht leer sein!\033[0m\n");
            continue;
        }

        if (len > PLAYER_NAME_MAX_LEN - 1) {
            wprintf(L"\033[38;5;124mName zu lang! Maximal %d Zeichen erlaubt.\033[0m\n", PLAYER_NAME_MAX_LEN - 1);
            continue;
        }

        wcscpy(name, inputBuffer);
        break;
    }
}

void setzeSpielerStandardwerte(struct playerStruct *player, struct playerStruct *computer) {
    player->type = PLAYER;
    player->numberOfCards = 0;
    player->points = 0;
    player->choice = (unsigned int)-1;
    setzeSpielerName(player->name);

    computer->type = COMPUTER;
    computer->numberOfCards = 0;
    computer->points = 0;
    computer->choice = (unsigned int)-1;
    wcscpy(computer->name, L"Computer");
}

void druckeSpielerDeck(const struct playerStruct *player) {
    wprintf(L"%-*ls : ", PLAYER_NAME_MAX_LEN, player->name);
    for (int i = 0; i < player->numberOfCards; i++) {
        druckeKarte(*player->cards[i]);
        wprintf(L" ");
    }
    wprintf(L"\n");
}

void setzeSpielerEingabe(struct playerStruct *player) {
    druckeSpielerDeck(player);
    wprintf(L"%-*ls  :", PLAYER_NAME_MAX_LEN -1, L"");
    for (int i = 1; i <= player->numberOfCards; i++) {
        wprintf(L"%3d ", i);
    }
    wprintf(L"\n");

    int userSelection;
    while (1) {
        wprintf(L"Wahl (1 - %d): ", player->numberOfCards);

        wchar_t inputBuffer[16];
        if (fgetws(inputBuffer, sizeof(inputBuffer) / sizeof(wchar_t), stdin) == NULL) {
            clearerr(stdin);
            wprintf(L"\033[38;5;124mFehlerhafte Eingabe, bitte geben Sie eine Zahl ein!\033[0m\n");
            continue;
        }
        size_t len = wcslen(inputBuffer);
        if (len == 0) {
            continue;
        }
        if (inputBuffer[len - 1] == L'\n') {
            inputBuffer[len - 1] = L'\0';
        } else {
            wint_t ch;
            while ((ch = getwchar()) != WEOF && ch != L'\n');
        }

        if (swscanf(inputBuffer, L"%d", &userSelection) != 1) {
            wprintf(L"\033[38;5;124mFehlerhafte Eingabe, bitte geben Sie eine Zahl ein!\033[0m\n");
            continue;
        }

        if (userSelection <= 0 || userSelection > player->numberOfCards) {
            wprintf(L"\033[38;5;124mInvalide Zahl!\033[0m\n");
        } else {
            break;
        }
    }

    player->choice = userSelection - 1;
    wprintf(L"Ihre Auswahl: ");
    druckeKarte(*player->cards[player->choice]);
    wprintf(L"\n");
}

void loescheKarteAusSpielerDeck(struct playerStruct *player) {
    player->cards[player->choice] = NULL;
    player->choice = (unsigned int)-1;
    for (int i = 0; i < (int)player->numberOfCards - 1; i++) {
        if (player->cards[i] == NULL && player->cards[i+1] != NULL) {
            player->cards[i] = player->cards[i+1];
            player->cards[i+1] = NULL;
        }
    }
    player->numberOfCards--;
}

unsigned int alleKartenAusteilen(struct playingCard cards[CARDS_LEN], struct playerStruct *player, struct playerStruct *computer, unsigned int currentTopCard) {
    for (int i = 0; i < ROUNDS; i++) {
        currentTopCard = gebeKarteAnSpieler(player, cards, currentTopCard);
    }
    for (int i = 0; i < ROUNDS; i++) {
        currentTopCard = gebeKarteAnSpieler(computer, cards, currentTopCard);
    }
    return currentTopCard;
}

void spielloop(struct playerStruct *player, struct playerStruct *computer, botFunction botFunction) {
    PlayerType nextPlayer = COMPUTER;

    for (int i = 1; i < ROUNDS*2+1; i++) {
        if (i%2 == 1) {
            wprintf(L"-------------------Runde:%2d--------------------\n", i / 2 +1 );
        }
        if (nextPlayer == PLAYER) {
            setzeSpielerEingabe(player);
        } else {
            botFunction(computer, player->choice != (unsigned int)-1 ? player->cards[player->choice] : NULL);
            wprintf(L"Computer Wählt: ");
            druckeKarte(*computer->cards[computer->choice]);
            wprintf(L"\n");
        }

        if (i % 2 == 0 && i != 0) {
            PlayerType winner = player->cards[player->choice]->value >= computer->cards[computer->choice]->value ? PLAYER : COMPUTER;
            unsigned int pointsToWin = player->cards[player->choice]->pointsValue + computer->cards[computer->choice]->pointsValue;
            if (winner == PLAYER) {
                wprintf(L"\033[33m ---> %ls hat diese Runde gewonnen (+%d)\033[0m\n", player->name, pointsToWin);
                player->points += pointsToWin;
            } else {
                wprintf(L"\033[33m ---> Der Computer hat diese Runde gewonnen (+%d)\033[0m\n", pointsToWin);
                computer->points += pointsToWin;
            }
            loescheKarteAusSpielerDeck(player);
            loescheKarteAusSpielerDeck(computer);

            nextPlayer = winner;
        } else {
            nextPlayer = nextPlayer == PLAYER ? COMPUTER : PLAYER;
        }
    }
}

void druckeGewinner(const struct playerStruct *player, const struct playerStruct *computer) {
    wprintf(L"\n\n-----------------------------------------------\n");
    wprintf(L"*     Ihre Punkte      *      Computer        *\n");
    wprintf(L"* %10d           * %10d           *\n", player->points, computer->points);
    wprintf(L"-----------------------------------------------\n");
    PlayerType finalWinner = player->points >= computer->points ? PLAYER : COMPUTER;
    wprintf(finalWinner == PLAYER ? L"* \033[32m" : L"* \033[31m");
    wprintf(L"Gewinner des Spiels: %ls\033[0m\n", finalWinner == PLAYER ? player->name : computer->name);
    wprintf(L"-----------------------------------------------\n");
    wprintf(L"----------------Ende des Spieles---------------\n");
    wprintf(L"-----------------------------------------------\n");
}

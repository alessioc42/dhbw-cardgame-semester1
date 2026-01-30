#include "bot.h"
#include "player.h"

#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

void botStrategieZufaellig(struct playerStruct *computer, struct playingCard *playerSelection) {
    int botSelection = rand() % computer->numberOfCards;
    computer->choice = botSelection;
}

void botStrategieSpieleErsteKarte(struct playerStruct *computer, struct playingCard *playerSelection) {
    computer->choice = 0;
}

void botStrategieSpieleHoehsteKarte(struct playerStruct *computer, struct playingCard *playerSelection) {
    unsigned int botSelection = (unsigned int)-1;
    unsigned int currentValue = 0;
    int found = 0;

    for (int i = 0; i < (int)computer->numberOfCards; i++) {
        if (computer->cards[i] == NULL) continue;
        if (!found || computer->cards[i]->value > currentValue) {
            currentValue = computer->cards[i]->value;
            botSelection = (unsigned int)i;
            found = 1;
        }
    }

    computer->choice = found ? botSelection : (unsigned int)-1;
}

void botStrategieSpieleNiedrigsteKarte(struct playerStruct *computer, struct playingCard *playerSelection) {
    unsigned int botSelection = 0;
    unsigned int currentValue = 100; // Higher than the highest card value
    for (int i = 0; i < computer->numberOfCards; i++) {
        if (computer->cards[i]->value < currentValue) {
            currentValue = computer->cards[i]->value;
            botSelection = i;
        }
    }
    computer->choice = botSelection;
}

void botStrategieSpieleAbwechselned(struct playerStruct *computer, struct playingCard *playerSelection) {
    if (computer->numberOfCards % 2 == 0) {
        computer->choice = (computer->numberOfCards - 1);
    } else {
        computer->choice = 0;
    }
}

#define OVER_MAX_CARD_DIFFERENCE 20
void botStrategieReaktiv(struct playerStruct *computer, struct playingCard *playerSelection) {
    if (playerSelection == NULL) {
        botStrategieSpieleNiedrigsteKarte(computer, playerSelection);
        return;
    }
    unsigned int botSelection = 0;
    unsigned int selectionCardDifference = OVER_MAX_CARD_DIFFERENCE;
    for (int i = 0; i < computer->numberOfCards; i++) {
        const unsigned int diff = computer->cards[i]->value - playerSelection->value;
        if (diff > 0 && diff < selectionCardDifference) {
            selectionCardDifference = diff;
            botSelection = i;
        }
    }
    if (selectionCardDifference == OVER_MAX_CARD_DIFFERENCE) {
        botStrategieSpieleNiedrigsteKarte(computer, playerSelection);
        return;
    }
    computer->choice = botSelection;
}

botFunction waehleZufaelligenBot(void) {
    void (*bot)(struct playerStruct *computer, struct playingCard *playerSelection);

    unsigned int botIndex = rand() % 5;

    wprintf(L"* ");
    switch (botIndex) {
        case 0:
            bot = &botStrategieZufaellig;
            wprintf(L"Zufälliger Gegner: spielt zufällig eine Karte.");
            break;
        case 1:
            bot = &botStrategieSpieleErsteKarte;
            wprintf(L"Einfacher Gegner: spielt Karten der Reihe nach.");
            break;
        case 2:
            bot = &botStrategieSpieleHoehsteKarte;
            wprintf(L"Strategischer Gegner: spielt von der höchsten zur niedrigsten Karte.");
            break;
        case 3:
            bot = &botStrategieSpieleAbwechselned;
            wprintf(L"Wechselnder Gegner: spielt abwechselnd die kleinste und die größte Karte.");
            break;
        case 4:
            bot = &botStrategieReaktiv;
            wprintf(L"Reaktiver Gegner: versucht, die Karte des Gegners gezielt zu übertrumpfen, sonst spielt er die kleinste Karte.");
            break;
        default:
            bot = &botStrategieZufaellig;
            break;
    }

    return bot;
}

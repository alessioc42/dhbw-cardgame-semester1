#include "cards.h"
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

void druckeKarte(struct playingCard card) {
    const wchar_t *colorChar = L"♠♥♦♣";

    if (card.cardColor == HEARTS || card.cardColor == DIAMONDS) {
        wprintf(L"\033[38;5;124m\033[47m");
    } else {
        wprintf(L"\033[30m\033[47m");
    }

    if (card.value == 10) {
        wprintf(L"%lc10", colorChar[card.cardColor]);
        wprintf(L"\033[0m");
        return;
    }

    unsigned int valueChar;
    if (card.value == 14) {
        valueChar = 'A';
    } else if (card.value >= 11) {
        valueChar = 'B' + card.value - 11;
    } else {
        valueChar = '0' + card.value;
    }

    wprintf(L"%lc%c ", colorChar[card.cardColor], valueChar);
    wprintf(L"\033[0m");
}

void setzeUngemischtesKartenDeckAuf(struct playingCard cards[CARDS_LEN]) {
    int index = 0;
    for (int cardColor = 0; cardColor < 4; cardColor++) {
        for (int card = 0; card < 13; card++) {
            cards[index].cardColor = cardColor;
            cards[index].value = card + 2;
            if (cards[index].value <= 10) {
                cards[index].pointsValue = cards[index].value;
            } else {
                if (card == 12) {
                    cards[index].pointsValue = 11;
                } else {
                    cards[index].pointsValue = cards[index].value - 9;
                }
            }
            index++;
        }
    }
}

#define SHUFFLE_COUNT 1024
void mischeKarten(struct playingCard cards[CARDS_LEN]) {
    for (int i = 0; i < SHUFFLE_COUNT; i++) {
        int cardIndex1 = rand() % CARDS_LEN;
        int cardIndex2 = rand() % CARDS_LEN;
        struct playingCard cachedCard = cards[cardIndex1];
        cards[cardIndex1] = cards[cardIndex2];
        cards[cardIndex2] = cachedCard;
    }
}

void druckeKartenDeck(struct playingCard cards[CARDS_LEN]) {
    for (int i = 0; i < CARDS_LEN; i++) {
        druckeKarte(cards[i]);
        wprintf(L" ");
        if ((i + 1) % 13 == 0) {
            wprintf(L"\n");
        }
    }
}

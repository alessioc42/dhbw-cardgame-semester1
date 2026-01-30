#ifndef CARDS_H
#define CARDS_H

#include <wchar.h>

#define CARDS_LEN 52
#define ROUNDS 10 // MIN = 1; MAX = 25

typedef enum {
    SPADES,
    HEARTS,
    DIAMONDS,
    CLUBS
} CardColor;

struct playingCard {
    CardColor cardColor;
    unsigned int pointsValue;
    unsigned int value;
};

void druckeKarte(struct playingCard card);
void setzeUngemischtesKartenDeckAuf(struct playingCard cards[CARDS_LEN]);
void mischeKarten(struct playingCard cards[CARDS_LEN]);
void druckeKartenDeck(struct playingCard cards[CARDS_LEN]);

#endif // CARDS_H

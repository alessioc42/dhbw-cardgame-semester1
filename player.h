#ifndef PLAYER_H
#define PLAYER_H

#include "cards.h"
#include "bot.h"

#include <wchar.h>

#define PLAYER_NAME_MAX_LEN 17

typedef enum {COMPUTER, PLAYER} PlayerType;

struct playerStruct {
    PlayerType type;
    struct playingCard *cards[ROUNDS];
    wchar_t name[PLAYER_NAME_MAX_LEN];
    unsigned int numberOfCards;
    unsigned int points;
    unsigned int choice;
};

unsigned int gebeKarteAnSpieler(struct playerStruct *player, struct playingCard cards[CARDS_LEN], unsigned int currentTopCard);
void setzeSpielerStandardwerte(struct playerStruct *player, struct playerStruct *computer);
void druckeSpielerDeck(const struct playerStruct *player);
void setzeSpielerEingabe(struct playerStruct *player);
void loescheKarteAusSpielerDeck(struct playerStruct *player);
unsigned int alleKartenAusteilen(struct playingCard cards[CARDS_LEN], struct playerStruct *player, struct playerStruct *computer, unsigned int currentTopCard);
void spielloop(struct playerStruct *player, struct playerStruct *computer, botFunction botFunction);
void druckeGewinner(const struct playerStruct *player, const struct playerStruct *computer);

#endif // PLAYER_H

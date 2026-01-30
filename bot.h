#ifndef BOT_H
#define BOT_H

#include "cards.h"

struct playerStruct;

typedef void (*botFunction)(struct playerStruct *computer, struct playingCard *playerSelection);

void botStrategieZufaellig(struct playerStruct *computer, struct playingCard *playerSelection);
void botStrategieSpieleErsteKarte(struct playerStruct *computer, struct playingCard *playerSelection);
void botStrategieSpieleHoehsteKarte(struct playerStruct *computer, struct playingCard *playerSelection);
void botStrategieSpieleNiedrigsteKarte(struct playerStruct *computer, struct playingCard *playerSelection);
void botStrategieSpieleAbwechselned(struct playerStruct *computer, struct playingCard *playerSelection);
void botStrategieReaktiv(struct playerStruct *computer, struct playingCard *playerSelection);

botFunction waehleZufaelligenBot(void);

#endif // BOT_H

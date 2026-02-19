#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <locale.h>
#include <time.h>

#ifdef _WIN32
#include <io.h>
#include <fcntl.h>
#endif

#include "cards.h"
#include "player.h"
#include "bot.h"

int main() {
    setlocale(LC_ALL, "");

#ifdef _WIN32
    _setmode(_fileno(stdout), _O_U16TEXT);
#endif

    wprintf(L"-----------------------------------------------\n");
    wprintf(L"*      Programmieren I - Programmentwurf      *\n");
    wprintf(L"*      Das DHBW-Kartenspiel                   *\n");
    wprintf(L"-----------------------------------------------\n");

    srand((unsigned int)time(NULL));

    struct playingCard cards[CARDS_LEN];
    unsigned int currentTopCard = CARDS_LEN - 1;
    setzeUngemischtesKartenDeckAuf(cards);
    mischeKarten(cards);

    struct playerStruct player;
    struct playerStruct computer;
    setzeSpielerStandardwerte(&player, &computer);
    alleKartenAusteilen(cards, &player, &computer, currentTopCard);

    const botFunction bot = waehleZufaelligenBot();

    wprintf(L"\n");

    spielloop(&player, &computer, bot);

    druckeGewinner(&player, &computer);

    return 0;
}

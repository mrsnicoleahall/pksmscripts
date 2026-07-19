#include <pksm.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/*
 * Script 10 - All Vivillon Forms (Ultra Moon)
 * ---------------------------------------------------------------------------
 * Vivillon (#666) has 20 patterns:
 *   0 Icy Snow, 1 Polar, 2 Tundra, 3 Continental, 4 Garden, 5 Elegant,
 *   6 Meadow, 7 Modern, 8 Marine, 9 Archipelago, 10 High Plains,
 *   11 Sandstorm, 12 River, 13 Monsoon, 14 Savanna, 15 Sun, 16 Ocean,
 *   17 Jungle, 18 Fancy, 19 Poke Ball.
 * All shiny copies first, then all non-shiny copies.
 *
 *   Trainer (OT) : cole (Female)   IVs: 31   TID/SID: random (one set/run)
 *
 * Legality note: the region patterns (0-17) are transfer-only in Gen VII.
 * Fancy (18) and Poke Ball (19) were event distributions. Verify in PKHeX.
 * ---------------------------------------------------------------------------
 */

#define SPECIES 666
#define FORMS   20

int main(int argc, char** argv)
{
    unsigned int tid, sid;
    int s, f, slot;
    char pkmn[260];

    srand(time(0));
    tid = rand() % 65536;
    sid = rand() % 65536;

    slot = 0;
    sav_box_decrypt();

    for (s = 1; s >= 0; s--)
    {
        for (f = 0; f < FORMS; f++)
        {
            pkx_generate(pkmn, SPECIES);
            pkx_set_value(pkmn, GEN_SEVEN, FORM, f);

            pkx_set_value(pkmn, GEN_SEVEN, OT_NAME, "cole");
            pkx_set_value(pkmn, GEN_SEVEN, OT_GENDER, 1);
            pkx_set_value(pkmn, GEN_SEVEN, TID, tid);
            pkx_set_value(pkmn, GEN_SEVEN, SID, sid);
            pkx_set_value(pkmn, GEN_SEVEN, MET_YEAR, cfg_default_year());
            pkx_set_value(pkmn, GEN_SEVEN, MET_MONTH, cfg_default_month());
            pkx_set_value(pkmn, GEN_SEVEN, MET_DAY, cfg_default_day());

            pkx_set_value(pkmn, GEN_SEVEN, IV_HP, 31);
            pkx_set_value(pkmn, GEN_SEVEN, IV_ATK, 31);
            pkx_set_value(pkmn, GEN_SEVEN, IV_DEF, 31);
            pkx_set_value(pkmn, GEN_SEVEN, IV_SPATK, 31);
            pkx_set_value(pkmn, GEN_SEVEN, IV_SPDEF, 31);
            pkx_set_value(pkmn, GEN_SEVEN, IV_SPEED, 31);

            pkx_set_value(pkmn, GEN_SEVEN, SHINY, s);

            pkx_set_value(pkmn, GEN_SEVEN, NICKNAME, i18n_species(SPECIES));
            pkx_set_value(pkmn, GEN_SEVEN, NICKNAMED, 0);

            sav_inject_pkx(pkmn, GEN_SEVEN, slot / 30, slot % 30, 0);
            slot++;
        }
    }
    sav_box_encrypt();

    gui_warn("All 20 Vivillon patterns injected!\nOT: cole (Female)\nShiny copies first, then non-shiny.");
    return 0;
}

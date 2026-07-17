#include <pksm.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Script 13 - All Oricorio Forms (Ultra Moon)
 * ---------------------------------------------------------------------------
 * Oricorio (#741) has 4 styles:
 *   0 Baile, 1 Pom-Pom, 2 Pa'u, 3 Sensu.
 * All shiny copies first, then all non-shiny copies.
 *
 *   Trainer (OT) : Cole (Female)   IVs: 31   TID/SID: random (one set/run)
 *
 * Legality note: Oricorio is native to this game. Verify in PKHeX.
 * ---------------------------------------------------------------------------
 */

#define SPECIES 741
#define FORMS   4

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

            pkx_set_value(pkmn, GEN_SEVEN, OT_NAME, "Cole");
            pkx_set_value(pkmn, GEN_SEVEN, OT_GENDER, 1);
            pkx_set_value(pkmn, GEN_SEVEN, TID, tid);
            pkx_set_value(pkmn, GEN_SEVEN, SID, sid);

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

    gui_warn("All 4 Oricorio styles injected!\nOT: Cole (Female)\nShiny copies first, then non-shiny.");
    return 0;
}

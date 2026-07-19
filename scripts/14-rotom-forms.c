#include <pksm.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/*
 * Script 14 - All Rotom Forms (Ultra Moon)
 * ---------------------------------------------------------------------------
 * Rotom (#479) has 6 forms:
 *   0 Normal, 1 Heat, 2 Wash, 3 Frost, 4 Fan, 5 Mow.
 * All shiny copies first, then all non-shiny copies.
 *
 *   Trainer (OT) : cole (Female)   IVs: 31   TID/SID: random (one set/run)
 *
 * Legality note: Rotom's appliance forms are set in-game via the Rotom
 * catalogue; the stored data keeps the chosen form. Verify in PKHeX.
 * ---------------------------------------------------------------------------
 */

#define SPECIES 479
#define FORMS   6

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

    gui_warn("All 6 Rotom forms injected!\nOT: cole (Female)\nShiny copies first, then non-shiny.");
    return 0;
}

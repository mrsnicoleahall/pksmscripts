#include <pksm.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/*
 * Script 9 - All Pikachu Cap Forms (Ultra Moon)
 * ---------------------------------------------------------------------------
 * The hat/cap Pikachu (#25) forms in Gen VII:
 *   1 Original Cap, 2 Hoenn Cap, 3 Sinnoh Cap, 4 Unova Cap,
 *   5 Kalos Cap, 6 Alola Cap, 7 Partner Cap.
 * (Form 0 is ordinary Pikachu and is skipped - these are the HAT forms.)
 *
 * These cap Pikachu are SHINY-LOCKED event Pokemon, so only non-shiny copies
 * are made (a shiny cap Pikachu is not legal).
 *
 *   Trainer (OT) : Cole (Female)   IVs: 31   TID/SID: random (one set/run)
 *
 * Legality note: real cap Pikachu are fateful-encounter event distributions
 * with fixed OT/ID/ribbon data. This makes ordinary Cole-owned cap Pikachu,
 * which will read as illegal in a strict checker. For a legal cap Pikachu use
 * the real event file via Script 15. Verify in PKHeX.
 * ---------------------------------------------------------------------------
 */

#define SPECIES 25

int main(int argc, char** argv)
{
    unsigned int tid, sid;
    int f, slot;
    char pkmn[260];

    srand(time(0));
    tid = rand() % 65536;
    sid = rand() % 65536;

    slot = 0;
    sav_box_decrypt();

    /* Non-shiny only (cap Pikachu are shiny-locked). Forms 1..7. */
    for (f = 1; f <= 7; f++)
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

        pkx_set_value(pkmn, GEN_SEVEN, SHINY, 0);

        pkx_set_value(pkmn, GEN_SEVEN, NICKNAME, i18n_species(SPECIES));
        pkx_set_value(pkmn, GEN_SEVEN, NICKNAMED, 0);

        sav_inject_pkx(pkmn, GEN_SEVEN, slot / 30, slot % 30, 0);
        slot++;
    }
    sav_box_encrypt();

    gui_warn("All 7 cap Pikachu forms injected!\nOT: Cole (Female)\nNon-shiny only (shiny-locked).\nUse Script 15 for legal event copies.");
    return 0;
}

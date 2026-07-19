#include <pksm.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/*
 * Script 4 - All Alolan Forms (Ultra Moon)
 * ---------------------------------------------------------------------------
 * Injects every Alolan regional form. All shiny copies first, then all
 * non-shiny copies (Alolan forms have no shiny lock).
 *
 *   Trainer (OT) : cole (Female)   IVs: 31   TID/SID: random (one set/run)
 *
 * Legality note: best-effort legal via PKSM's generator. Alolan forms are
 * native to this game. Verify in PKHeX before online use.
 * ---------------------------------------------------------------------------
 */

int main(int argc, char** argv)
{
    /* Alolan form is form index 1 for each of these species. */
    int species[] = { 19, 20, 26, 27, 28, 37, 38, 50, 51,
                      52, 53, 74, 75, 76, 88, 89, 103, 105 };
    int count = 18;

    unsigned int tid, sid;
    int s, i, slot;
    char pkmn[260];

    srand(time(0));
    tid = rand() % 65536;
    sid = rand() % 65536;

    slot = 0;
    sav_box_decrypt();

    /* s = 1 -> shiny pass, s = 0 -> non-shiny pass */
    for (s = 1; s >= 0; s--)
    {
        for (i = 0; i < count; i++)
        {
            pkx_generate(pkmn, species[i]);
            pkx_set_value(pkmn, GEN_SEVEN, FORM, 1);

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

            pkx_set_value(pkmn, GEN_SEVEN, NICKNAME, i18n_species(species[i]));
            pkx_set_value(pkmn, GEN_SEVEN, NICKNAMED, 0);

            sav_inject_pkx(pkmn, GEN_SEVEN, slot / 30, slot % 30, 0);
            slot++;
        }
    }
    sav_box_encrypt();

    gui_warn("All Alolan forms injected!\nOT: cole (Female)\nShiny copies first, then non-shiny.");
    return 0;
}

#include <pksm.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Script 12 - All Flabebe Line Forms (Ultra Moon)
 * ---------------------------------------------------------------------------
 * The Flabebe evolution line with every flower colour:
 *   Flabebe (#669), Floette (#670), Florges (#671)
 *   Colours (form): 0 Red, 1 Yellow, 2 Orange, 3 Blue, 4 White.
 * All shiny copies first, then all non-shiny copies.
 *
 * Floette's "Eternal Flower" form (AZ's Floette, form 5) is intentionally
 * excluded: it has never been legally obtainable or tradeable.
 *
 *   Trainer (OT) : Cole (Female)   IVs: 31   TID/SID: random (one set/run)
 *
 * Legality note: transfer-only line in Gen VII. Verify in PKHeX.
 * ---------------------------------------------------------------------------
 */

int main(int argc, char** argv)
{
    int line[] = { 669, 670, 671 };
    int lineCount = 3;
    int colours = 5;   /* forms 0-4; Eternal Flower (5) excluded */

    unsigned int tid, sid;
    int s, i, f, slot;
    char pkmn[260];

    srand(time(0));
    tid = rand() % 65536;
    sid = rand() % 65536;

    slot = 0;
    sav_box_decrypt();

    for (s = 1; s >= 0; s--)
    {
        for (i = 0; i < lineCount; i++)
        {
            for (f = 0; f < colours; f++)
            {
                pkx_generate(pkmn, line[i]);
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

                pkx_set_value(pkmn, GEN_SEVEN, NICKNAME, i18n_species(line[i]));
                pkx_set_value(pkmn, GEN_SEVEN, NICKNAMED, 0);

                sav_inject_pkx(pkmn, GEN_SEVEN, slot / 30, slot % 30, 0);
                slot++;
            }
        }
    }
    sav_box_encrypt();

    gui_warn("Flabebe line (5 colours x 3 stages) injected!\nOT: Cole (Female)\nShiny copies first, then non-shiny.\nEternal Flower excluded (illegal).");
    return 0;
}

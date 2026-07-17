#include <pksm.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/*
 * Script 2 - Full Living Dex, best IVs / natures / abilities (Ultra Moon)
 * ---------------------------------------------------------------------------
 * Generates one of every species (National Dex #1-807) ordered by dex number,
 * one per box slot. Non-shiny.
 *
 *   Trainer (OT) : Cole (Female)
 *   Trainer IDs  : randomised once per run (one TID/SID for the whole box)
 *   IVs          : 31 in every stat (best possible)
 *   Nature       : Hardy (a NEUTRAL nature - no stat penalty, always legal).
 *                  "Best nature" is role-dependent and can't be chosen
 *                  per-species automatically, so a no-downside neutral nature
 *                  is used. Change NEUTRAL_NATURE below if you prefer another.
 *   Ability      : each species' standard legal ability (kept from the
 *                  generator). Hidden Abilities are NOT force-set because they
 *                  are not legally obtainable on every species - forcing them
 *                  would break the "completely legal" requirement.
 *
 * Legality note: best-effort legal via PKSM's generator. Verify in PKHeX.
 * ---------------------------------------------------------------------------
 */

#define NEUTRAL_NATURE 0   /* 0 Hardy, 6 Docile, 12 Serious, 18 Bashful, 24 Quirky */

int main(int argc, char** argv)
{
    unsigned char version = *argv[0];
    int maxSpecies;
    unsigned int tid, sid;
    int i, species;
    char pkmn[260];

    if (version != 32 && version != 33)
    {
        if (!gui_choice("Made for Ultra Sun / Ultra Moon.\nBox layout may differ on other games.\nContinue anyway?"))
            return 0;
    }
    maxSpecies = (version == 32 || version == 33) ? 807 : 802;

    srand(time(0) + version);
    tid = rand() % 65536;
    sid = rand() % 65536;

    sav_box_decrypt();
    for (i = 0; i < maxSpecies; i++)
    {
        species = i + 1;

        pkx_generate(pkmn, species);

        pkx_set_value(pkmn, GEN_SEVEN, OT_NAME, "Cole");
        pkx_set_value(pkmn, GEN_SEVEN, OT_GENDER, 1);           /* 1 = Female */
        pkx_set_value(pkmn, GEN_SEVEN, TID, tid);
        pkx_set_value(pkmn, GEN_SEVEN, SID, sid);

        pkx_set_value(pkmn, GEN_SEVEN, IV_HP, 31);
        pkx_set_value(pkmn, GEN_SEVEN, IV_ATK, 31);
        pkx_set_value(pkmn, GEN_SEVEN, IV_DEF, 31);
        pkx_set_value(pkmn, GEN_SEVEN, IV_SPATK, 31);
        pkx_set_value(pkmn, GEN_SEVEN, IV_SPDEF, 31);
        pkx_set_value(pkmn, GEN_SEVEN, IV_SPEED, 31);

        pkx_set_value(pkmn, GEN_SEVEN, NATURE, NEUTRAL_NATURE);
        pkx_set_value(pkmn, GEN_SEVEN, SHINY, 0);

        pkx_set_value(pkmn, GEN_SEVEN, NICKNAME, i18n_species(species));
        pkx_set_value(pkmn, GEN_SEVEN, NICKNAMED, 0);

        sav_inject_pkx(pkmn, GEN_SEVEN, i / 30, i % 30, 0);
    }
    sav_box_encrypt();

    gui_warn("Living Dex generated!\nOT: Cole (Female)\n31 IVs, neutral nature, legal ability.\nVerify in PKHeX before online use.");
    return 0;
}

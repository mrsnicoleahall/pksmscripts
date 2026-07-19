#include <pksm.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/*
 * Script 1 - Shiny Full Living Dex (Ultra Moon)
 * ---------------------------------------------------------------------------
 * Generates one of every species (National Dex #1-807) ordered by dex number,
 * one per box slot. Every species is made shiny EXCEPT those that can never be
 * legally shiny (shiny-locked), which are generated non-shiny instead.
 *
 *   Trainer (OT) : cole (Female)
 *   Trainer IDs  : randomised once per run (one TID/SID for the whole box)
 *   IVs          : 31 in every stat (best possible - always legal)
 *   Ball / moves / met data : PKSM legal defaults for each species
 *
 * Legality note: this uses PKSM's own generator (the same basis as FlagBrew's
 * living-dex script) plus a curated shiny-lock table. It is best-effort legal.
 * Always verify with PKHeX before trading/using online.
 * ---------------------------------------------------------------------------
 */

/* Species that have no legal shiny form as of Generation VII. */
int shiny_locked(int species)
{
    switch (species)
    {
        /* Mythicals with no legal shiny */
        case 151:  /* Mew */
        case 251:  /* Celebi */
        case 491:  /* Darkrai */
        case 492:  /* Shaymin */
        case 493:  /* Arceus */
        case 494:  /* Victini */
        case 647:  /* Keldeo */
        case 648:  /* Meloetta */
        case 649:  /* Genesect */
        case 720:  /* Hoopa */
        case 721:  /* Volcanion */
        case 801:  /* Magearna */
        case 802:  /* Marshadow */
        case 807:  /* Zeraora */
        /* Legendaries that are shiny-locked in every game up to Gen VII */
        case 643:  /* Reshiram */
        case 644:  /* Zekrom */
        case 646:  /* Kyurem */
        case 716:  /* Xerneas */
        case 717:  /* Yveltal */
        case 718:  /* Zygarde */
        case 772:  /* Type: Null */
        case 773:  /* Silvally */
        case 785:  /* Tapu Koko */
        case 786:  /* Tapu Lele */
        case 787:  /* Tapu Bulu */
        case 788:  /* Tapu Fini */
        case 789:  /* Cosmog */
        case 790:  /* Cosmoem */
        case 791:  /* Solgaleo */
        case 792:  /* Lunala */
        case 800:  /* Necrozma */
        case 803:  /* Poipole */
        case 804:  /* Naganadel */
            return 1;
        default:
            return 0;
    }
}

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

        pkx_set_value(pkmn, GEN_SEVEN, OT_NAME, "cole");
        pkx_set_value(pkmn, GEN_SEVEN, OT_GENDER, 1);           /* 1 = Female */
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

        pkx_set_value(pkmn, GEN_SEVEN, SHINY, shiny_locked(species) ? 0 : 1);

        pkx_set_value(pkmn, GEN_SEVEN, NICKNAME, i18n_species(species));
        pkx_set_value(pkmn, GEN_SEVEN, NICKNAMED, 0);

        sav_inject_pkx(pkmn, GEN_SEVEN, i / 30, i % 30, 0);
    }
    sav_box_encrypt();

    gui_warn("Shiny living Dex generated!\nOT: cole (Female)\nShiny-locked species were made non-shiny.\nVerify in PKHeX before online use.");
    return 0;
}

#include <pksm.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Script 16 - Bulk Sanity / Compatibility Check (Ultra Moon)
 * ---------------------------------------------------------------------------
 * Scans EVERY Pokemon in ALL PC boxes and writes a report listing anything
 * suspicious, to:
 *
 *      /3ds/PKSM/legality-report.txt
 *
 * For each Pokemon it checks, using the game's own data tables:
 *   - species  present in Ultra Sun/Ultra Moon      (sav_check_value)
 *   - ability  present in USUM
 *   - held item present in USUM
 *   - Poke Ball present in USUM
 *   - all 4 moves present in USUM
 * plus a few heuristics:
 *   - shiny on a species that is shiny-locked (can't legally be shiny)
 *   - level outside 1-100
 *
 * WHAT THIS IS NOT
 * ---------------------------------------------------------------------------
 * This is NOT a full PKHeX-grade legality analysis. PKSM only exposes the
 * game's data tables to scripts - it does not expose its real legality
 * checker (the one behind the per-Pokemon "Legality" button that uses your
 * configured API URL), and scripts cannot read HTTP responses back from a
 * server, so a script cannot call your legality API and collect verdicts.
 *
 * Use this as a fast BULK PRE-FILTER: it reliably catches cross-generation
 * contamination (moves/abilities/items/species that don't exist in Gen VII)
 * and the shiny-lock mistakes. Then run the built-in "Legality" button (your
 * API) on anything it flags, or export the box and check it in PKHeX.
 * ---------------------------------------------------------------------------
 */

#define REPORT_PATH "/3ds/PKSM/legality-report.txt"
#define SLOTS_PER_BOX 30

/* Species with no legal shiny form as of Generation VII (see Script 1). */
int shiny_locked(int species)
{
    switch (species)
    {
        case 151: case 251: case 491: case 492: case 493: case 494:
        case 647: case 648: case 649: case 720: case 721: case 801:
        case 802: case 807:
        case 643: case 644: case 646: case 716: case 717: case 718:
        case 772: case 773: case 785: case 786: case 787: case 788:
        case 789: case 790: case 791: case 792: case 800: case 803: case 804:
            return 1;
        default:
            return 0;
    }
}

int main(int argc, char** argv)
{
    unsigned char version = *argv[0];
    int slots, i, box, slot;
    int species, ability, item, ball, shiny, level, egg, mv, m;
    int total = 0, flagged = 0;
    char pkmn[260];
    char issues[700];
    char line[160];
    char summary[160];
    FILE* rep;

    if (version != 32 && version != 33)
    {
        if (!gui_choice("Made for Ultra Sun / Ultra Moon.\nRun the scan anyway?"))
            return 0;
    }

    slots = sav_get_max(MAX_SLOTS);

    rep = fopen(REPORT_PATH, "w");
    if (rep != NULL)
    {
        fprintf(rep, "PKSM Bulk Sanity / Compatibility Report (Ultra Moon)\n");
        fprintf(rep, "Flags values that do not exist in Gen VII, plus shiny-lock/level checks.\n");
        fprintf(rep, "NOT a full PKHeX legality analysis - verify flagged mons in PKHeX.\n");
        fprintf(rep, "====================================================================\n\n");
    }

    for (i = 0; i < slots; i++)
    {
        box  = i / SLOTS_PER_BOX;
        slot = i % SLOTS_PER_BOX;

        sav_get_pkx(pkmn, box, slot);

        species = pkx_get_value(pkmn, GEN_SEVEN, SPECIES);
        if (species == 0)
            continue;               /* empty slot */

        total++;
        issues[0] = '\0';

        if (!sav_check_value(SAV_VALUE_SPECIES, species))
        {
            sprintf(line, "  - species #%d not available in USUM\n", species);
            strcat(issues, line);
        }

        ability = pkx_get_value(pkmn, GEN_SEVEN, ABILITY);
        if (!sav_check_value(SAV_VALUE_ABILITY, ability))
        {
            sprintf(line, "  - ability #%d not available in USUM\n", ability);
            strcat(issues, line);
        }

        item = pkx_get_value(pkmn, GEN_SEVEN, ITEM);
        if (item != 0 && !sav_check_value(SAV_VALUE_ITEM, item))
        {
            sprintf(line, "  - held item #%d not available in USUM\n", item);
            strcat(issues, line);
        }

        ball = pkx_get_value(pkmn, GEN_SEVEN, BALL);
        if (!sav_check_value(SAV_VALUE_BALL, ball))
        {
            sprintf(line, "  - Poke Ball #%d not available in USUM\n", ball);
            strcat(issues, line);
        }

        for (m = 0; m < 4; m++)
        {
            mv = pkx_get_value(pkmn, GEN_SEVEN, MOVE, m);
            if (mv != 0 && !sav_check_value(SAV_VALUE_MOVE, mv))
            {
                sprintf(line, "  - move #%d not available in USUM\n", mv);
                strcat(issues, line);
            }
        }

        shiny = pkx_get_value(pkmn, GEN_SEVEN, SHINY);
        if (shiny && shiny_locked(species))
        {
            strcat(issues, "  - shiny, but this species is shiny-locked (illegal shiny)\n");
        }

        level = pkx_get_value(pkmn, GEN_SEVEN, LEVEL);
        if (level < 1 || level > 100)
        {
            sprintf(line, "  - level %d is out of range (1-100)\n", level);
            strcat(issues, line);
        }

        if (issues[0] != '\0')
        {
            flagged++;
            egg = pkx_get_value(pkmn, GEN_SEVEN, EGG);
            if (rep != NULL)
            {
                fprintf(rep, "Box %d, Slot %d: %s (#%d)%s\n%s\n",
                    box + 1, slot + 1, i18n_species(species), species,
                    egg ? " [EGG]" : "", issues);
            }
        }
    }

    if (rep != NULL)
    {
        fprintf(rep, "====================================================================\n");
        fprintf(rep, "Scanned %d Pokemon. Flagged %d.\n", total, flagged);
        fclose(rep);
    }

    if (flagged == 0)
    {
        sprintf(summary, "Scan complete.\n%d Pokemon checked.\nNothing flagged.\n(Not a full legality check.)", total);
    }
    else
    {
        sprintf(summary, "Scan complete.\n%d checked, %d flagged.\nSee %s\nVerify flagged mons in PKHeX.", total, flagged, REPORT_PATH);
    }
    gui_warn(summary);
    return 0;
}

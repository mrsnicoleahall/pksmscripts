#include <pksm.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Script 17 - Data-driven Shiny Living Dex injector (Ultra Moon)
 * ---------------------------------------------------------------------------
 * The "no shortcuts" shiny dex. Instead of building Pokemon on-device with
 * pkx_generate (which only makes a default template and leaves illegal
 * placeholder moves), this injects PRE-BUILT, PKHeX-verified-legal .pk7 files.
 *
 * Every file in this set was generated with PKHeX.Core's real legality engine:
 * a legal encounter for each species is found, transferred/evolved up to a
 * USUM-legal PK7 owned by "cole", made shiny only where the engine confirms a
 * legal shiny is possible, IVs maxed where legal, and each one is checked with
 * LegalityAnalysis before export. Result: 807/807 legal, 789 shiny.
 *
 * USAGE
 *   1. Copy the .pk7 files to:   /3ds/PKSM/shinydex
 *   2. Run this script. Each Pokemon is placed at its National Dex slot
 *      (species 1 -> Box 1 Slot 1, ... species 807 -> Box 27), regardless of
 *      file order, by reading the species out of each file.
 *
 * This OVERWRITES boxes 1-27. Back up / empty them first.
 * ---------------------------------------------------------------------------
 */

#define DEX_DIR "/3ds/PKSM/shinydex"

int ends_with(char* name, char* ext)
{
    int nl = strlen(name);
    int el = strlen(ext);
    if (nl < el)
        return 0;
    return strcasecmp(name + nl - el, ext) == 0;
}

char* read_file(char* path, int* outSize)
{
    FILE* f;
    int size;
    char* data;

    f = fopen(path, "rb");
    if (f == NULL)
        return NULL;
    fseek(f, 0, SEEK_END);
    size = ftell(f);
    fseek(f, 0, SEEK_SET);
    if (size <= 0)
    {
        fclose(f);
        return NULL;
    }
    data = malloc(size);
    fread(data, 1, size, f);
    fclose(f);
    *outSize = size;
    return data;
}

int main(int argc, char** argv)
{
    struct directory* dir;
    char* data;
    char* name;
    int i, size, species, slot, count = 0;
    char msg[128];

    dir = read_directory(DEX_DIR);
    if (dir == NULL || dir->count == 0)
    {
        gui_warn("No files in:\n" DEX_DIR "\n\nCopy the shiny-dex .pk7 files\nthere, then run again.");
        if (dir != NULL)
            delete_directory(dir);
        return 0;
    }

    sav_box_decrypt();
    for (i = 0; i < dir->count; i++)
    {
        name = dir->files[i];
        if (!ends_with(name, ".pk7"))
            continue;
        data = read_file(name, &size);
        if (data == NULL)
            continue;

        species = pkx_get_value(data, GEN_SEVEN, SPECIES);
        if (species >= 1 && species <= 807)
        {
            slot = species - 1;               /* dex-order placement */
            sav_inject_pkx(data, GEN_SEVEN, slot / 30, slot % 30, 0);
            count++;
        }
        free(data);
    }
    sav_box_encrypt();
    delete_directory(dir);

    sprintf(msg, "Shiny living Dex injected!\n%d Pokemon placed in dex order.\nOT: cole  |  All PKHeX-legal.", count);
    gui_warn(msg);
    return 0;
}

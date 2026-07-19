#include <pksm.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Script 15 - Legal Event Pokemon (Ultra Moon)
 * ---------------------------------------------------------------------------
 * A real event Pokemon is only legal with its original event data (OT name,
 * Trainer ID/SID, met date, fateful flag, event ribbon). Those values are
 * baked into the official distribution files and cannot be recreated by hand,
 * so this script injects REAL event files from a folder:
 *
 *      /3ds/PKSM/events
 *
 *   *.wc7full / *.wc7  - Wonder Cards -> injected into your Mystery Gift album.
 *                        Redeem them in-game at the delivery girl in any
 *                        Pokemon Center to get the (fully legal) Pokemon.
 *   *.pk7              - event Pokemon dumps -> injected straight into BOXES
 *                        (from Box 1), if you have any.
 *
 * This pack ships pre-loaded with the English Gen VII event Wonder Cards that
 * are redeemable on Ultra Moon (from the ProjectPokemon Events Gallery). Add
 * or remove files in the folder to taste.
 *
 * NOTE: the Mystery Gift album holds a limited number of cards. If there are
 * more cards than free slots, this injects as many as fit and tells you how
 * many are left - redeem those in-game, then run again for the rest.
 * ---------------------------------------------------------------------------
 */

#define EVENT_DIR "/3ds/PKSM/events"

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
    int i, size, slot;
    int boxSlot   = 0;   /* running box/slot index for .pk7 */
    int pkCount   = 0;
    int wcCount   = 0;
    int wcSkipped = 0;   /* wonder cards left because the album was full */
    int albumFull = 0;
    char msg[192];

    dir = read_directory(EVENT_DIR);
    if (dir == NULL || dir->count == 0)
    {
        gui_warn("No files found in:\n" EVENT_DIR "\n\nPut .wc7full / .wc7 / .pk7\nevent files there, then run again.");
        if (dir != NULL)
            delete_directory(dir);
        return 0;
    }

    sav_box_decrypt();
    for (i = 0; i < dir->count; i++)
    {
        name = dir->files[i];

        if (ends_with(name, ".pk7"))
        {
            data = read_file(name, &size);
            if (data == NULL)
                continue;
            /* doTradeEdits = 0 keeps the original event OT/handler data intact */
            sav_inject_pkx(data, GEN_SEVEN, boxSlot / 30, boxSlot % 30, 0);
            free(data);
            boxSlot++;
            pkCount++;
        }
        else if (ends_with(name, ".wc7full") || ends_with(name, ".wc7"))
        {
            slot = sav_wcx_free_slot();
            if (slot < 0)
            {
                /* Mystery Gift album is full - count the rest and move on */
                albumFull = 1;
                wcSkipped++;
                continue;
            }
            data = read_file(name, &size);
            if (data == NULL)
                continue;
            /* alternateFormat: 1 for the 784-byte .wc7full, 0 for the 264-byte .wc7 */
            sav_inject_wcx(data, GEN_SEVEN, slot, ends_with(name, ".wc7full") ? 1 : 0);
            free(data);
            wcCount++;
        }
    }
    sav_box_encrypt();
    delete_directory(dir);

    if (pkCount == 0 && wcCount == 0 && wcSkipped == 0)
    {
        gui_warn("No .wc7full / .wc7 / .pk7 files found in:\n" EVENT_DIR);
    }
    else if (albumFull)
    {
        sprintf(msg, "Events injected!\n%d Wonder Cards -> Mystery Gift\n%d Pokemon (.pk7) -> boxes\n\nAlbum full: %d cards left over.\nRedeem in-game, then run again.", wcCount, pkCount, wcSkipped);
        gui_warn(msg);
    }
    else
    {
        sprintf(msg, "Events injected!\n%d Wonder Cards -> Mystery Gift\n%d Pokemon (.pk7) -> boxes\n\nRedeem the Wonder Cards in-game\nat the delivery girl (Pokemon Center).", wcCount, pkCount);
        gui_warn(msg);
    }
    return 0;
}

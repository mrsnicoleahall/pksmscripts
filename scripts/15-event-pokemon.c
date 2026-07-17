#include <pksm.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Script 15 - Legal Event Pokemon (Ultra Moon)
 * ---------------------------------------------------------------------------
 * The ONLY way an event Pokemon is completely legal is with its real,
 * original event data: the correct OT name, Trainer ID/SID, met date,
 * fateful-encounter flag and event ribbon. Those values are baked into the
 * official distribution files - they cannot be re-created by setting fields by
 * hand, and they are NOT owned by "Cole".
 *
 * So this script injects REAL event files that you drop into a folder:
 *
 *   1. Create the folder:            /3ds/PKSM/events
 *   2. Put your Gen VII event files there:
 *        *.pk7  - event Pokemon dumps -> injected into your BOXES (start box 1)
 *        *.wc7  - Wonder Cards        -> injected into your Mystery Gift album
 *      (Get these from a reputable event archive, e.g. the community
 *       "Wondercard Database" / event .pk7 collections.)
 *   3. Run this script.
 *
 * Every injected Pokemon keeps its genuine event data, so it stays legal.
 * Files from other generations should be brought in with PKSM's transfer
 * tools instead; this script only handles Gen VII (.pk7 / .wc7).
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
    int boxSlot = 0;      /* running box/slot index for .pk7 */
    int pkCount = 0;
    int wcCount = 0;

    dir = read_directory(EVENT_DIR);
    if (dir == NULL || dir->count == 0)
    {
        gui_warn("No files found in:\n" EVENT_DIR "\n\nPut legal .pk7 / .wc7 event\nfiles there, then run again.");
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
        else if (ends_with(name, ".wc7"))
        {
            data = read_file(name, &size);
            if (data == NULL)
                continue;
            slot = sav_wcx_free_slot();
            if (slot < 0)
                slot = 0;
            sav_inject_wcx(data, GEN_SEVEN, slot, 0);
            free(data);
            wcCount++;
        }
    }
    sav_box_encrypt();
    delete_directory(dir);

    if (pkCount == 0 && wcCount == 0)
    {
        gui_warn("No .pk7 or .wc7 files found in:\n" EVENT_DIR);
    }
    else
    {
        char msg[128];
        sprintf(msg, "Events injected!\n%d Pokemon (.pk7) -> boxes\n%d Wonder Cards (.wc7)\nOriginal event data kept (legal).", pkCount, wcCount);
        gui_warn(msg);
    }
    return 0;
}

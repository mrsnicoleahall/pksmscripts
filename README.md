# PKSM Ultra Moon Scripts — Cole

A batch of [PKSM](https://github.com/FlagBrew/PKSM) C scripts (picoC) for a
**Pokémon Ultra Moon** save (Generation VII, game version `33`). Every script
sets the trainer to **Cole (Female)** with randomised Trainer IDs.

> ⚠️ **Legality disclaimer.** These use PKSM's own Pokémon generator (the same
> basis as FlagBrew's built-in *Living Dex* script) plus curated shiny-lock /
> form data. That produces **best-effort legal** Pokémon — good enough for
> normal play and storage, but *not guaranteed* to pass a strict legality
> checker in every case (regional forms, transfer-only species, fixed-data
> legendaries, and events all have edge cases). **Always verify in
> [PKHeX](https://github.com/kwsch/PKHeX) before trading or going online.**
> The one exception is Script 15, which injects real event files and therefore
> stays genuinely legal.

## What's included / what was skipped

You asked for 15 scripts and said to skip anything not valid for Gen VII.
Three are **Generation VIII+** content that doesn't exist in Ultra Moon, so
they were dropped:

| # | Script | Status |
|---|--------|--------|
| 1 | Shiny full living Dex (except where shiny is impossible) | ✅ `scripts/01-shiny-living-dex.c` |
| 2 | Full living Dex, best IVs / natures / abilities | ✅ `scripts/02-living-dex-best-ivs.c` |
| 3 | All Galarian forms | ❌ **Skipped — Galar is Gen VIII** |
| 4 | All Alolan forms | ✅ `scripts/04-alolan-forms.c` |
| 5 | All Alcremie forms | ❌ **Skipped — Alcremie is Gen VIII** |
| 6 | All Unown forms | ✅ `scripts/06-unown-forms.c` |
| 7 | All Furfrou forms | ✅ `scripts/07-furfrou-forms.c` |
| 8 | All Deoxys forms | ✅ `scripts/08-deoxys-forms.c` |
| 9 | All Pikachu cap forms | ✅ `scripts/09-pikachu-cap-forms.c` |
| 10 | All Vivillon forms | ✅ `scripts/10-vivillon-forms.c` |
| 11 | All Hisuian forms | ❌ **Skipped — Hisui/Legends Arceus is Gen VIII** |
| 12 | All Flabébé line forms | ✅ `scripts/12-flabebe-line-forms.c` |
| 13 | All Oricorio forms | ✅ `scripts/13-oricorio-forms.c` |
| 14 | All Rotom forms | ✅ `scripts/14-rotom-forms.c` |
| 15 | All event Pokémon | ✅ `scripts/15-event-pokemon.c` |
| 16 | Bulk sanity / compatibility check | ✅ `scripts/16-bulk-legality-check.c` |

## Script 16 — Bulk sanity / compatibility check

Scans **every Pokémon in all boxes** and writes a report to
`/3ds/PKSM/legality-report.txt`, flagging anything whose species, ability,
held item, Poké Ball, or moves **don't exist in Ultra Moon** (catches
cross-gen contamination), plus shiny-on-shiny-locked and out-of-range levels.

It is **not** a full PKHeX-grade legality check. PKSM does not expose its real
legality checker (the per-Pokémon "Legality" button that uses your configured
API URL) to scripts, and scripts can't read HTTP responses back, so a script
can't call your legality API and collect verdicts. Use this as a fast bulk
pre-filter, then run the built-in Legality button (your API) on flagged mons,
or check them in PKHeX.

## Common behaviour (Scripts 1–14)

- **OT name:** `Cole`, **OT gender:** Female.
- **Trainer IDs:** one random TID + SID generated per run and applied to the
  whole batch (a single trainer's Pokémon). Re-run for a different pair.
- **IVs:** 31 in every stat (best possible — always legal).
- **Injection:** written into your PC boxes starting at **Box 1, slot 1**, in
  order. This **overwrites** whatever is in those slots, so back up / empty the
  target boxes first.
- **Ball, level, moves and met data:** left at PKSM's legal defaults for each
  species.

### Per-script notes

- **Script 1 — Shiny living Dex.** #1–807 in dex order. Made shiny except for
  the ~34 species that can never be legally shiny (Mew, Celebi, the shiny-locked
  legendaries, Tapus, Type: Null line, Cosmog line, UB gifts like Poipole,
  etc.), which are generated non-shiny. The exclusion list is at the top of the
  file — edit it if you disagree with a call.
- **Script 2 — Living Dex, best stats.** #1–807, non-shiny. IVs 31; nature set
  to a **neutral** nature (Hardy — no stat penalty; "best nature" is
  role-dependent so a no-downside default is used, configurable via
  `NEUTRAL_NATURE`); ability kept at each species' standard **legal** ability
  (Hidden Abilities are *not* force-set, since they aren't legal on every
  species).
- **Scripts 4–14 (forms).** As you asked, each script injects **all shiny
  copies first, then all non-shiny copies**. Cap Pikachu (Script 9) are
  shiny-locked event Pokémon, so only non-shiny copies are made. Floette's
  Eternal Flower is excluded from Script 12 (never legally obtainable).

## Script 15 — Legal event Pokémon (read this)

"All event Pokémon, completely legal" **cannot** also be Cole-owned: a real
event mon is only legal with its original OT/ID/date/ribbon/fateful data, which
is fixed in the official distribution file. So this script keeps the genuine
data instead of overwriting it with Cole.

**How to use it:**

1. Create the folder `/3ds/PKSM/events` on your SD card.
2. Drop your Gen VII event files in there:
   - `*.pk7` — event Pokémon dumps → injected into your **boxes** (from Box 1).
   - `*.wc7` — Wonder Cards → injected into your **Mystery Gift album**.
   Get these from a reputable event archive (e.g. community *Wondercard
   Database* / event `.pk7` collections).
3. Run the script. Each file keeps its real event data, so it stays legal.

Files from other generations should be brought over with PKSM's transfer tools
rather than this script.

## Installing / running on the 3DS

**There is nothing to compile.** These are **C (picoC) scripts** and PKSM runs
them directly with its built-in interpreter — you just copy the `.c` files onto
the SD card. They require **PKSM v6.0.0+** on CFW (needed for C scripts).

1. Copy the `.c` files from `scripts/` into `/3ds/PKSM/scripts/usum/` on your SD
   card. (The `usum` folder is what PKSM reads when an Ultra Sun / Ultra Moon
   save is loaded.) For Script 15, also create `/3ds/PKSM/events/`.
2. Open PKSM → load your Ultra Moon save → **Scripts**. The menu opens on
   PKSM's **built-in** scripts by default — **press `X` to switch to the SD
   card**, otherwise you won't see these. Then pick a script and press `A`.
   (`Y` toggles the `usum`/`universal` folders; `B` goes up a level.)
3. **Save** in PKSM to write changes back.
4. Before running: **back up your save** and empty the boxes you're filling —
   scripts write from Box 1 and overwrite those slots.
5. Verify results in [PKHeX](https://github.com/kwsch/PKHeX) before online use.

## Sources

- [PKSM Scripts Development wiki](https://github.com/FlagBrew/PKSM/wiki/Scripts-Development)
- [FlagBrew/PKSM-Scripts](https://github.com/FlagBrew/PKSM-Scripts)
- [PKHeX](https://github.com/kwsch/PKHeX) (legality verification)

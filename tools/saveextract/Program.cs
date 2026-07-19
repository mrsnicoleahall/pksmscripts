// Extracts a cole-owned shiny living Dex from an existing legal USUM save.
// Usage: dotnet run -c Release -- <save file> <outDir>
// For each species #1-807 it picks the best (shiny) legal box entry, re-OTs it
// to "cole" (female) when that stays PKHeX-legal, else keeps the original OT
// (event mons), and writes <species>.pk7. Requires .NET 10 + PKHeX.Core.
using PKHeX.Core; using System; using System.IO; using System.Linq; using System.Collections.Generic;

string savePath = args.Length > 0 ? args[0] : "save.bin";
string outDir   = args.Length > 1 ? args[1] : "coledex";
Directory.CreateDirectory(outDir);
var sav = SaveUtil.GetSaveFile(savePath);
if (sav == null) { Console.WriteLine("Unrecognized save."); return; }
bool Legal(PKM p) => new LegalityAnalysis(p).Valid;

var bySpecies = new Dictionary<int, List<PKM>>();
for (int b = 0; b < sav.BoxCount; b++) for (int s = 0; s < sav.BoxSlotCount; s++)
{
    var pk = sav.GetBoxSlotAtIndex(b, s);
    if (pk == null || pk.Species == 0) continue;
    if (!bySpecies.TryGetValue(pk.Species, out var l)) { l = new(); bySpecies[pk.Species] = l; }
    l.Add(pk);
}
PKM? ReOT(PKM src)
{
    var pk = src.Clone();
    pk.OriginalTrainerName = "cole";
    pk.OriginalTrainerGender = 1;
    pk.RefreshChecksum();
    return Legal(pk) ? pk : null;
}
int shiny = 0, cole = 0, written = 0; var missing = new List<int>();
for (int sp = 1; sp <= 807; sp++)
{
    if (!bySpecies.TryGetValue(sp, out var cands)) { missing.Add(sp); continue; }
    PKM? best = null; int bestScore = -1; bool bestCole = false, bestShiny = false;
    foreach (var c in cands)
    {
        if (!Legal(c)) continue;
        var re = ReOT(c);
        if (re != null) { int sc = (re.IsShiny ? 4 : 0) + 3; if (sc > bestScore) { bestScore = sc; best = re; bestCole = true; bestShiny = re.IsShiny; } }
        { int sc = (c.IsShiny ? 4 : 0) + 1; if (sc > bestScore) { bestScore = sc; best = c.Clone(); bestCole = false; bestShiny = c.IsShiny; } }
    }
    if (best == null) { missing.Add(sp); continue; }
    if (bestShiny) shiny++; if (bestCole) cole++;
    var bytes = best.Data.ToArray(); if (bytes.Length > 232) bytes = bytes[..232];
    File.WriteAllBytes(Path.Combine(outDir, $"{sp:000}.pk7"), bytes); written++;
}
Console.WriteLine($"written={written}/807 shiny={shiny} coleOT={cole} missing={missing.Count}");

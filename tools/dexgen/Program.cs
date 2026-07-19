using PKHeX.Core; using System; using System.Linq; using System.Reflection;
var u = EncounterCriteria.Unrestricted;
foreach(var p in typeof(EncounterCriteria).GetProperties())
  Console.WriteLine($"{p.Name} = {p.GetValue(u)}  canWrite={p.CanWrite}");
// test constructing shiny criteria & converting a Gen4 species (Bidoof/Unown)
EncounterCriteria Crit(Shiny s){
  var c=new EncounterCriteria();
  foreach(var p in typeof(EncounterCriteria).GetProperties().Where(p=>p.CanWrite)) p.SetValue(c,p.GetValue(u));
  typeof(EncounterCriteria).GetProperty("Shiny").SetValue(c,s);
  return c;
}
var tr=new SimpleTrainerInfo(GameVersion.UM){OT="cole",Gender=1,TID16=54321,SID16=12345,Language=2};
string[] gn={"D","P","Pt","HG","SS","B","W","B2","W2","X","Y","OR","AS","SN","MN","US","UM","R","S","E","FR","LG"};
var versions=gn.Select(n=>Enum.TryParse<GameVersion>(n,out var v)?v:default).Where(v=>(int)v>0).ToArray();
foreach(ushort sp in new ushort[]{264,201,415}){ // Bibarel, Unown, Combee(->Vespiquen f)
  var tmpl=new PK7{Species=sp,Form=0,Gender=1};
  var encs=EncounterMovesetGenerator.GenerateEncounters(tmpl,tr,default,versions).Take(20).ToList();
  bool got=false;
  foreach(var enc in encs){
    PKM raw; try{raw=((IEncounterConvertible)enc).ConvertToPKM(tr,Crit(Shiny.Always));}catch{continue;}
    if(raw.IsShiny){ var conv = raw is PK7?raw:EntityConverter.ConvertToType(raw,typeof(PK7),out _); if(conv!=null){Console.WriteLine($"{sp} shiny via {enc.GetType().Name} rawSpecies={(Species)raw.Species}");got=true;break;}}
  }
  if(!got)Console.WriteLine($"{sp}: no shiny candidate");
}

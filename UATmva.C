#include "src/UATmvaConfig.cc"
#include "src/UATmvaTree.cc"
#include "src/UATmvaClassification.cc"
#include "src/figureOfMerit.cc"
#include "src/UATmvaTools.cc"
#include "src/UATmvaReader.cc"
#include "src/UATmvaSummary.cc"


void UATmva(TString Cfg = "Config.cfg" , TString Steps = "TRS" ){

  Steps.ToUpper();

  UATmvaConfig Config;
  Config.ReadCfg(Cfg);
  Config.Print();

  // Connect inout TTrees if needed (Training & Reading)
  UATmvaTree Tree;
  if ( Steps.Contains ('T') || Steps.Contains ('R') ) Tree.Open(Config);

  // MVA Training
  if ( Steps.Contains ('T') ) { 
    UATmvaClassification UATmva;
    UATmva.Do(Config,Tree);
  }

  // MVA Reading (Evaluation)
  if ( Steps.Contains ('R') ) {
    UATmvaReader Reader;
    Reader.SetNbin(44); 
    Reader.SetNbin(1);
    Reader.Do(Config,Tree); 
  }  

  // Summary & Plots
  if ( Steps.Contains ('S') || Steps.Contains ('C') || Steps.Contains ('P') ) {
    UATmvaSummary Summary;
    Summary.Init(Config);
    if ( Steps.Contains ('S') ) Summary.Print();
    if ( Steps.Contains ('C') ) Summary.CPlots();
    if ( Steps.Contains ('P') ) Summary.Plots();
  }

}

#include "src/UATmvaConfig.cc"
#include "src/UATmvaTree.cc"
#include "src/UATmvaClassification.cc"
#include "src/figureOfMerit.cc"
#include "src/UATmvaTools.cc"
#include "src/UATmvaReader.cc"
#include "src/UATmvaSummary.cc"

/*

  T = Train all MVA
  R = Read back all MVA and evaluate best one and cut value
  S = Print all MVA summary (significance , limit, ... )
  Y = Print all MVA Yields (for best cut)
  B = Print MVA summary and Yields for best one
  P = Inpect MVA plots
  C = Plot Input Variables Control Plots 

*/



void UATmva(TString Cfg = "Config.cfg" , TString Steps = "TRSB" ){

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
    Reader.Do(Config,Tree); 
  }  

  // Summary & Plots
  if (    Steps.Contains ('S') 
       || Steps.Contains ('Y') 
       || Steps.Contains ('B') 
       || Steps.Contains ('C') 
       || Steps.Contains ('P') ) {
    UATmvaSummary Summary;
    Summary.Init(Config);
    if ( Steps.Contains ('S') ) Summary.Print();
    if ( Steps.Contains ('Y') ) Summary.Yields();
    if ( Steps.Contains ('B') ) Summary.BestMVA();
    if ( Steps.Contains ('C') ) Summary.CPlots();
    if ( Steps.Contains ('P') ) Summary.Plots();
  }

}

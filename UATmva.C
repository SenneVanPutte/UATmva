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
  Y = Print all MVA Yields (for best Limit)
  B = Print MVA summary and Yields for best one
  P = Inpect MVA plots
  C = Plot Input Variables Control Plots 
  L = Prepare Limit Input Cards

*/



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
    Reader.Do(Config,Tree); 
  }  

  // Summary & Plots
  if (    Steps.Contains ('S') 
       || Steps.Contains ('Y') 
       || Steps.Contains ('B') 
       || Steps.Contains ('C') 
       || Steps.Contains ('P') 
       || Steps.Contains ('M') 
       || Steps.Contains ('L') ) {
    UATmvaSummary Summary;
    Summary.Init(Config);
    if ( Steps.Contains ('S') ) Summary.Print();
    if ( Steps.Contains ('Y') ) Summary.Yields();
    if ( Steps.Contains ('B') ) Summary.BestMVA();
    if ( Steps.Contains ('C') ) {
       if      ( Steps.Contains ('0') ) Summary.CPlots(Config,0);
       else if ( Steps.Contains ('1') ) Summary.CPlots(Config,1);
       else if ( Steps.Contains ('2') ) Summary.CPlots(Config,2);
       else                             Summary.CPlots(Config,0);   
    }
    if ( Steps.Contains ('P') ) Summary.Plots(Config,0);
    if ( Steps.Contains ('M') ) { 
       if      ( Steps.Contains ('0') )  Summary.Plots(Config,1); 
       else if ( Steps.Contains ('3') )  Summary.Plots(Config,1,3);
       else if ( Steps.Contains ('4') )  Summary.Plots(Config,1,4);
       else                              Summary.Plots(Config,1);
    }
    if ( Steps.Contains ('L') ) Summary.LimitCard(Config);
  }

}

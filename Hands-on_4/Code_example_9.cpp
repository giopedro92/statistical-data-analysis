// Text file format (available types: ’F’ and ’I’)
// var1/F:var2/F:var3/F:var4/F
// 0.21293 -0.49200 -0.58425 -0.70591
// ...
TString sigFile = "signal.txt";     // text file for signal
TString bkgFile = "background.txt"; // text file for background

Double_t sigWeight = 1.0; // overall weight for all signal events
Double_t bkgWeight = 1.0; // overall weight for all background events

factory->SetInputTrees( sigFile, bkgFile, sigWeight, bkgWeight );
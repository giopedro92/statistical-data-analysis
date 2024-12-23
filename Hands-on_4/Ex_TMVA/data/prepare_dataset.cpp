//  allows to analyse data stored in TTrees (CSVs) with a high level interface.

#include <iostream>

void prepare_dataset() {
  auto df = ROOT::RDF::FromCSV("atlas-higgs-challenge-2014-v2.csv");

  // Get the number of entries

  auto nevents = *df.Count(); 
  std::cout << "n. dati " << nevents << "\n"; 
  // auto h1 = df.Histo1D("totale_casi");

  // Returns the names of the available columns.

  auto colNames = df.GetColumnNames();
  for (auto &&colName : colNames)  {
    std::cout << colName << ", " ;  
  }
  std::cout << '\n';

  // auto colNames = df.GetColumnNames();
  // for (auto &&colName : colNames)  {
  //   std::cout << "  loader.AddVariable(\"" << colName << "\", 'F');  \t// "
  //     << colName << " (" << df.GetColumnType(colName) << ")\n";
  // }

  // Save selected columns to disk, in a new TTree in new file.
  df.Filter("Label == \"s\" ")
    .Snapshot("tree", "atlas-higgs-challenge-2014-v2-sig.root");  

  df.Filter("Label == \"b\" ")
    .Snapshot("tree", "atlas-higgs-challenge-2014-v2-bkg.root");
}
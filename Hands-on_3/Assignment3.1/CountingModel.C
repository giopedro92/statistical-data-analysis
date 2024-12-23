using namespace RooFit;
using namespace RooStats;

void CountingModel(  int nobs = 3,           // number of observed events
                     double b = 1,           // number of background events
                     double sigmab = 0.2 )   // relative uncertainty in b
{
   RooWorkspace w("w");
   
// make Poisson model * Gaussian constraint
   w.factory("sum:nexp(s[3,0,15],b[1,0,10])");
// Poisson of (n | s+b)
   w.factory("Poisson:pdf(nobs[0,50],nexp)");
   w.factory("Gaussian:constraint(b0[0,10],b,sigmab[1])");
   w.factory("PROD:model(pdf,constraint)");


   w.var("b0")->setVal(b);
   w.var("b0")->setConstant(true); // needed for being treated as global observables
   w.var("sigmab")->setVal(sigmab*b);  
   

   ModelConfig mc("ModelConfig",&w);
   mc.SetPdf(*w.pdf("model"));
   mc.SetParametersOfInterest(*w.var("s"));
   mc.SetObservables(*w.var("nobs"));
   mc.SetNuisanceParameters(*w.var("b"));

   // these are needed for the hypothesis tests
   mc.SetSnapshot(*w.var("s"));
   mc.SetGlobalObservables(*w.var("b0"));

   mc.Print();
   // import model in the workspace 
   w.import(mc);

   // make data set with the namber of observed events
   RooDataSet data("data","", *w.var("nobs"));
   w.var("nobs")->setVal(3);
   data.add(*w.var("nobs") );
   // import data set in workspace and save it in a file
   w.import(data);

   w.Print();

   TString fileName = "CountingModel.root"; 

   // write workspace in the file (recreate file if already existing)
   w.writeToFile(fileName, true);
}
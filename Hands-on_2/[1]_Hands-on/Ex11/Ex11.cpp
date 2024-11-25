#include "RooRealVar.h"
#include "RooAbsArg.h"
#include "RooAbsReal.h"
#include "RooDataSet.h"
#include "RooGaussian.h"
#include "RooProdPdf.h"
#include "RooAddPdf.h"
#include "RooMinimizer.h"
#include "RooFitResult.h"
#include "RooPlot.h"
#include "RooWorkspace.h"
#include "TCanvas.h"
#include "TAxis.h"
#include "TFile.h"
#include "TH1.h"

using namespace RooFit;

void Ex11() {
    RooWorkspace w{"Ex11workspace"};
    w.factory("x[-20, 20]"); // x
    w.factory("f[0.5, 0, 1]"); // coefficient f
    w.factory("Gaussian::s1(x,mean[0],width[3.9])");   // 2 gaussians
    w.factory("Gaussian::s2(x,mean[0],width[4,3,6])"); // same mean <-> strong correlation
    w.factory("SUM::model(f * s1, (1 - f) * s2)"); // model

    auto model = w.pdf("model"); // Returns as RooAbsPdf*
    auto x = w.var("x");         // Returns as RooRealVar*

// Tasks
// 1. Generate a Dataset with 1000 events
    // Generate a toy MC sample from composite PDF
    RooDataSet* data = model->generate(*x, 1000); // use *x because x is a pointer
    w.import(*data);

// 2. Save the Workspace (model + data) to a file
    w.writeToFile("Ex11workspace.root");

// 3. Minimize the Likelihood
    // Construct function object representing –log(L)
    RooAbsReal* nll = model->createNLL(*data);

    // Create a MINUIT interface (a RooMinuit object) to this  likelihood function
    RooMinimizer m(*nll);
    
    // Enable verbose mode to display the MINUIT steps (use the RooMinuit::setVerbose(kTRUE))
    m.setVerbose(kTRUE);

    // MIGRAD likelihood minimization
    m.migrad();
    
    model->getParameters(RooArgSet(*x))->Print("s");
}
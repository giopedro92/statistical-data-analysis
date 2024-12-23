// Organisation and simultaneous fits: creating and writing a workspace

#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooGaussian.h"
#include "RooChebychev.h"
#include "RooAddPdf.h"
#include "RooWorkspace.h"
#include "RooPlot.h"
#include "TCanvas.h"
#include "TAxis.h"
#include "TFile.h"
#include "TH1.h"
#include <memory>

using namespace RooFit;
 
void rf502_wspacewrite()
{
   // C r e a t e   m o d e l   a n d   d a t a s e t
   // -----------------------------------------------
 
   // Declare observable x
   RooRealVar x("x", "x", 0, 10);
 
   // Create two Gaussian PDFs sig1(x,mean,sigma1) and sig2(x,mean,sigma2) and their parameters
   RooRealVar mean("mean", "mean of gaussians", 5, 0, 10);
   RooRealVar sigma1("sigma1", "width of gaussians", 0.5);
   RooRealVar sigma2("sigma2", "width of gaussians", 1);
 
   RooGaussian sig1("sig1", "Signal component 1", x, mean, sigma1);
   RooGaussian sig2("sig2", "Signal component 2", x, mean, sigma2);
 
   // Build Chebychev polynomial pdf for background
   RooRealVar a0("a0", "a0", 0.5, 0., 1.);
   RooRealVar a1("a1", "a1", 0.2, 0, 1.);
   RooChebychev bkg("bkg", "Background", x, RooArgSet(a0, a1));
 
   // Sum the signal components into a composite signal pdf in different fractions
   RooRealVar sig1frac("sig1frac", "fraction of component 1 in signal", 0.8, 0., 1.);
   RooAddPdf sig("sig", "Signal", RooArgList(sig1, sig2), sig1frac);
 
   // Sum the composite signal and background
   RooRealVar bkgfrac("bkgfrac", "fraction of background", 0.5, 0., 1.);
   RooAddPdf model("model", "g1 + g2 + a", RooArgList(bkg, sig), bkgfrac);
 
   // Generate a data sample of 1000 events in x from model
   std::unique_ptr<RooDataSet> data{model.generate(x, 1000)};
 
   // C r e a t e   w o r k s p a c e ,   i m p o r t   d a t a   a n d   m o d e l
   // -----------------------------------------------------------------------------
 
   // Create a new empty workspace
   RooWorkspace *w = new RooWorkspace("w", "workspace");

   std::cout << "\n import(model) \n" << endl;

   // Import model and all its components into the workspace
   w->import(model);

   std::cout << "\n import(*data) \n" << endl;

   // Import data into the workspace
   w->import(*data);

   std::cout << "\n Print workspace contents \n" << endl;

   // Print workspace contents
   w->Print();
 
   // S a v e   w o r k s p a c e   i n   f i l e
   // -------------------------------------------

   // Save the workspace into a ROOT file
   w->writeToFile("rf502_workspace.root");
 
   // Workspace will remain in memory after macro finishes
   gDirectory->Add(w);
}
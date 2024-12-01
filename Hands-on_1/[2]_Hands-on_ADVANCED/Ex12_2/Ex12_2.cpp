#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooGaussian.h"
#include "TCanvas.h"
#include "RooPlot.h"
#include "TAxis.h"

#include "TFile"

using namespace RooFit;

void Ex12_2() {

   TFile* root_file = new TFile::Open("B0sInvariantMass.root", "READ");



   RooRealVar x{"x", "energy",           5, 6};
   RooRealVar mean_b{"mean_b", "Mean_b", 5.4,  5, 6};
   RooRealVar gamma{"gamma", "Gamma", 1, 5, 6};

   RooBreitWigner bw_model(x, mean_b, gamma);

   bw_model.fitTo(data);

   RooRealVar mean_g{"mean_g", "Mean_g", 5.3, 5, 6};
   RooRealVar sigma_g{"sigma_g", "Sigma", 0.3, 0, 3};

   RooGaussian gaus_model(x, mean_g, sigma_g);

   gaus_model.fitTo(data);

   TCanvas *c = new TCanvas("");
   auto frame = x.frame();
   data.plotOn()
   gaus_model.plotOn
   frame->Draw();
}
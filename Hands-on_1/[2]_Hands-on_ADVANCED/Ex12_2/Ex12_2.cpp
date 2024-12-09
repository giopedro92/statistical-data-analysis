// 1. open the “B0sInvariantMass.root”
// 2. import the corresponding binned dataset
// 3. Create a Breit-Wigner model
// 4. Fit the model to the binned dataset
// 5. Create a Gaussian function
// 6. fit to the data
// 7. plot the data and the BW and Gaussian distribution to the same canvas
// 8. Compare the fitted value with the particle mass reported in the Particle Data Group (B0s = B^0_s meson (s anti-b))

#include "RooRealVar.h"
#include "RooAbsArg.h"
#include "RooAbsReal.h"
#include "RooDataSet.h"
#include "RooGaussian.h"
#include "RooProdPdf.h"
#include "RooAddPdf.h"
#include "RooMinimizer.h"
#include "RooFitResult.h"
#include "RooHistFunc.h"
#include "RooPlot.h"
#include "RooWorkspace.h"
#include "TCanvas.h"
#include "TAxis.h"
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "RooFormulaVar.h"
#include "RooDataHist.h"
#include "RooGenericPdf.h"
#include "RooAbsData.h"
#include "RooAbsPdf.h"
#include "TStyle.h"
#include "RooConstVar.h"
#include "RooCategory.h"
#include "RooBreitWigner.h"
#include "TTree.h"
#include "TH1D.h"
#include "TRandom.h"
#include <iostream>

using namespace RooFit;

void Ex12_2() {
// 1. open the “B0sInvariantMass.root”
   TFile* root_file = TFile::Open("B0sInvariantMass.root");

   if (!root_file || root_file->IsZombie()) {
      std::cerr << "Errore nell'aprire il file" << std::endl;
      return;
   }


// 2. import the corresponding binned dataset
   TH1* hist = dynamic_cast<TH1*>(root_file->Get("massaB0"));
   
   if (!hist) {
      std::cerr << "\n Errore nel recuperare l'istogramma \n" << std::endl;
      return;
   }

   RooRealVar inv_mass("inv_mass", "Invariant mass", hist->GetXaxis()->GetXmin(),
                                                     hist->GetXaxis()->GetXmax(), "GeV");

   RooDataHist hist_B0mass("hist_B0mass", "Histogram B0 mass", RooArgList(inv_mass), Import(*hist));


// 3. Create a Breit-Wigner model
   RooRealVar mean_bw{"mean_bw", "Mean_bw", 5.36693, 5, 5.7, "GeV"};
   RooRealVar gamma_bw{"gamma_bw", "gamma_bw", 0, 1};

   RooBreitWigner bw_model{"bw_model", "Breit-Wigner", inv_mass, mean_bw, gamma_bw};


// 4. Fit the model to the binned dataset   
   bw_model.fitTo(hist_B0mass);


// 5. Create a Gaussian function
   RooRealVar mean_g{"mean_g", "Mean_g", 5.36693, 5, 5.7, "GeV"};
   RooRealVar sigma_g{"sigma_g", "Sigma", 0, 1};

   RooGaussian gaus_model{"gaus_model", "Gaussian", inv_mass, mean_g, sigma_g};


// 6. fit to the data
   gaus_model.fitTo(hist_B0mass);


// 7. plot the data and the BW and Gaussian distribution to the same canvas
   TCanvas *c = new TCanvas("c", "B^{0} mass", 2490, 1400);

   RooPlot *frame = inv_mass.frame(Title("B^{0} mass"));

   hist_B0mass.plotOn(frame);

   bw_model.plotOn(frame);
   gaus_model.plotOn(frame, LineColor(kRed));

   frame->Draw();

   gPad->BuildLegend(0.6,0.7, 0.9,0.9);

   c->Print("B0mass.png");


// 8. Compare the fitted value with the particle mass reported in the Particle Data Group
   std::cout << "\n Breit-Wigner mean" << endl;

   mean_bw.Print();
   
   std::cout << "\n Gaussian mean" << endl;

   mean_g.Print();
}
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
#include <iostream>

using namespace RooFit;

void Ex13_1() {
// 1. build a Gaussian p.d.f. with mean 0 and sigma 3.
//    Called this Gaussian p.d.f. "sig"
    RooRealVar x{"x", "observable", -20, 20};

    RooRealVar mean{"mean", "gaussian mean", 0};
    RooRealVar sigma{"sigma", "gaussian sigma", 3};

    RooGaussian sig{"sig", "Gaussian signal", x, mean, sigma};


// 2. add an exponential background component "bkg" to the model,
//    expressed as a function of the parameter tau, exp(-x/tau).
//    Set the initial value of tau to 10
    RooRealVar tau{"tau", "#tau", 10};
    RooFormulaVar rate{"rate", "1/#tau", "- 1. / tau", RooArgList(tau)};
    
    RooExponential bkg{"bkg", "Exponential background", x, rate};


// 3. Define a parameter "fsig" to represent the signal-to-background ratio
    RooRealVar fsig{"fsig", "signal-to-background ratio", 0.8, 0, 1};


// 4. Build a composite model in the following form:
//    model(x)=fsig×sig(x)+(1−fsig)×bkg(x)
    RooAddPdf model{"model", "Composite model signal + background", RooArgList(sig, bkg), fsig};


// 6. Generate a binned dataset of 1000 events (bin width = 0.5).
    //x.setBins(200);

    RooDataHist *data = model.generateBinned(x, 1000);


// 7. Make a Fit with Maximum Likelihood
    model.fitTo(*data);


// 5. & 8. Visualize the p.d.f.
    TCanvas *c1 = new TCanvas("c1", "Composite model signal + background", 2800, 1650);
    c1->SetCanvasSize(2790, 1600);

    c1->Divide(2);


    auto frame1 = x.frame(Title("Composite model signal + background"));

    sig.plotOn(frame1, LineColor(kRed));
    bkg.plotOn(frame1, LineColor(kGreen));
    model.plotOn(frame1, LineColor(43));

    std::cout << "\n" << endl;
    sig.Print("t");

    c1->cd(1);
    frame1->GetYaxis()->SetTitle("sig, bkg, sig+bkg");
    gPad->SetLeftMargin(0.15);
    frame1->Draw();
    gPad->BuildLegend(0.6,0.7, 0.9,0.9);


    auto frame2 = x.frame(Title("Composite model signal + background and fit"));
    
    data->plotOn(frame2);
    model.plotOn(frame2);

    std::cout << "\n" << endl;
    model.Print("t");

    c1->cd(2);
    gPad->SetLeftMargin(0.15);
    frame2->Draw();
    gPad->BuildLegend(0.6,0.7, 0.9,0.9);


    c1->Print("sig_bkg_model_fit.png");
}
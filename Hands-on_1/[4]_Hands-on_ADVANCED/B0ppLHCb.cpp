// 1. Load the unbinned dataset from the file rarest_b0_decay.dat
// define an extended composite model for invariant mass
// 2. background (you’re free to choose the model: flat, polynomial, exponential, ...)
// 3. Gaussian peak around the B0 mass
// 4. Gaussian peak around the Bs0 mass
// 5. Fit the model to the data using a maximum likelihood fit
// 6. Plot data and model
// 7. Superimpose each single component with different color
// Make histogram of residual and pull distributions
// 8. Construct a histogram with the residuals of the data w.r.t. the curveù
// 9. Construct a histogram with the pulls of the data w.r.t the curve
// 10. Create a new frame to draw the residual distribution and add the distribution to the frame
// 11. Create a new frame to draw the pull distribution and add the distribution to the frame

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
#include "RooPolynomial.h"
#include <iostream>

using namespace RooFit;

void B0ppLHCb() {
    RooRealVar inv_mass{"inv_mass", "Invariant mass", 5080, 5565, "MeV/c^{2}"};


// 1. Load the unbinned dataset from the file rarest_b0_decay.dat
    RooDataSet data = *RooDataSet::read("rarest_b0_decay.dat", inv_mass, "v");


// define an extended composite model for invariant mass
// 2. background (you’re free to choose the model: flat, polynomial, exponential, ...)
    //RooRealVar coeff1{"coeff1", "coeff1", 1};
    //RooRealVar c{"c", "c", 30};

    //RooPolynomial bkg{"bkg", "Polinomial background", inv_mass, RooArgList(coeff1, c)};

    RooRealVar tau{"tau", "#tau", 0, 1000};
    RooFormulaVar rate{"rate", "1/#tau", "- 1. / tau", RooArgList(tau)};
    
    RooExponential bkg{"bkg", "Exponential background", inv_mass, rate};

    RooRealVar Nbkg("Nbkg", "Number of background events", 132, 0., 10000);

    RooExtendPdf Ebkg("Ebkg", "Extended Exponential background", bkg, Nbkg);


// 3. Gaussian peak around the B0 mass
    RooRealVar meanB0{"meanB0", "Mean B^{0}mass", 5279.72, 5260, 5300, "MeV/c^{2}"};
    RooRealVar sigmaB0{"sigmaB0", "#sigma_{B^{0}}", 0, 20};

    RooGaussian B0mass{"B0mass", "B^{0} mass", inv_mass, meanB0, sigmaB0};

    RooRealVar NB0mass("NB0mass", "Number of B^{0} mass events", 33, 0., 10000);

    RooExtendPdf EB0mass("EB0mass", "Extended B^{0} mass", B0mass, NB0mass);


// 4. Gaussian peak around the Bs0 mass
    RooRealVar meanB0s{"meanB0s", "Mean B^{0}_{s}mass", 5366.93, 5340, 5390, "MeV/c^{2}"};
    //RooRealVar sigmaB0s{"sigmaB0s", "#sigma_{B^{0}_{s}}", 0, 20};

    RooGaussian B0smass{"B0smass", "B^{0}_{s} mass", inv_mass, meanB0s, sigmaB0}; // STESSA SIGMA ANCHE PER B0s, STESSA AMPIEZZA PER ENTRAMBI I PICCHI

    RooRealVar NB0smass("NB0smass", "Number of B^{0}_{s} mass events", 1, 0., 10000);

    RooExtendPdf EB0smass("EB0smass", "Extended B^{0}_{s} mass", B0smass, NB0smass);


// Build a composite model in the following form (with extended pdfs)
   //   Emodel = Ebkg + (Esig1 + Esig2)
    RooAddPdf Emodel{"Emodel", "Extended model B^{0} + B^{0}_{s} + bkg", RooArgList(Ebkg, EB0mass, EB0smass)};


// 6. Plot data and model
// 7. Superimpose each single component with different color
    TCanvas *c1 = new TCanvas("c1", "(E)Components, Emodel", 2800, 1700);
    c1->SetCanvasSize(2790, 1600);

    c1->Divide(2);


// B0, B0s and bkg NOT extended
    auto frame1 = inv_mass.frame(Title("Components and Extended B^{0}, B^{0}_{s} and background"));

    bkg.plotOn(frame1, LineColor(kGreen));
    B0mass.plotOn(frame1, LineColor(kRed));
    B0smass.plotOn(frame1, LineColor(kGray));
    
    Ebkg.plotOn(frame1, LineColor(kGreen), LineStyle(2));
    EB0mass.plotOn(frame1, LineColor(kRed), LineStyle(2));
    EB0smass.plotOn(frame1, LineColor(kGray), LineStyle(2));

    c1->cd(1);
    frame1->GetYaxis()->SetTitle("(E)B^{0}, (E)B^{0}_{s}, (E)bkg");
    gPad->SetLeftMargin(0.15);
    frame1->Draw();
    gPad->BuildLegend(0.65,0.7, 0.97,0.9);


// B0, B0s and bkg EXTENDED
    auto frame2 = inv_mass.frame(Title("Extendeds and Composite Extended model B^{0} + B^{0}_{s} + background"));

    Ebkg.plotOn(frame2, LineColor(kGreen));
    EB0mass.plotOn(frame2, LineColor(kRed));
    EB0smass.plotOn(frame2, LineColor(kGray));

    Emodel.plotOn(frame2, LineColor(kBlue));

    c1->cd(2);
    frame2->GetYaxis()->SetTitle("EB^{0}, EB^{0}_{s}, Ebkg, Emodel");
    gPad->SetLeftMargin(0.15);
    frame2->Draw();
    gPad->BuildLegend(0.65,0.7, 0.97,0.9);


    c1->Print("Ecomponents_Emodel.png");


// data and fit (EXTENDED)
    TCanvas *c2 = new TCanvas("c2", "Data and Fits", 2800, 1700);
    c2->SetCanvasSize(2790, 1600);

    inv_mass.setBins(25); //(bin width = 0.5) --> x_width / bin_width


    auto frame3 = inv_mass.frame(Title("Data and Fits"));


    data.plotOn(frame3, Name("data"));


// FIT A F T E R PLOTTING DATA !!!!!!
// 5. Fit the model to the data using a maximum likelihood fit
    bkg.fitTo(data);
    
    EB0mass.fitTo(data);
    EB0smass.fitTo(data);
    
    Emodel.fitTo(data);

    
// QUESTO STAMPA LE PDF S E N Z A NORMALIZZARLE O USARE QUELLE CHE SONO LE COMPONENTI DEL MODELLO !!!!!
// PER QUESTO NON LE RISCALA
    //Ebkg.plotOn(frame3, LineColor(kGreen));
    //EB0mass.plotOn(frame3, LineColor(kRed));
    //EB0smass.plotOn(frame3, LineColor(kGray));

// COSÌ INVECE STAMPO LE PDF CHE COMPONGONO IL MODELLO
    Emodel.plotOn(frame3, Components(RooArgSet(Ebkg)), LineColor(kGreen));
    
    Emodel.plotOn(frame3, Components(RooArgSet(EB0mass)), LineColor(kRed), LineStyle(7));
    Emodel.plotOn(frame3, Components(RooArgSet(EB0smass)), LineColor(kGray), LineStyle(7));

    Emodel.plotOn(frame3, LineStyle(kBlue), Name("Emodel"));


    frame3->GetYaxis()->SetTitle("Data and Fits");
    gPad->SetLeftMargin(0.15);
    frame3->Draw();
    gPad->BuildLegend(0.56,0.7, 0.9,0.9);

    c2->Print("data_Ecomponents_Emodel_fits.png");


// Make histogram of residual and pull distributions
    TCanvas *c3 = new TCanvas("c3", "Residual and pull distributions", 2800, 1700);
    c3->SetCanvasSize(2790, 1600);
    c3->Divide(2);


// 8. Construct a histogram with the residuals of the data w.r.t. the curve
    RooHist *hresid = frame3->residHist();


// 9. Construct a histogram with the pulls of the data w.r.t the curve
    RooHist *hpull = frame3->pullHist();


// 10. Create a new frame to draw the residual distribution and add the distribution to the frame
    auto frame4 = inv_mass.frame(Title("Residual Distribution"));
    frame4->addPlotable(hresid, "P");

    c3->cd(1);
    //frame4->GetYaxis()->SetTitle("");
    gPad->SetLeftMargin(0.15);
    frame4->Draw();


// 11. Create a new frame to draw the pull distribution and add the distribution to the frame
    auto frame5 = inv_mass.frame(Title("Pull Distribution"));
    frame5->addPlotable(hpull, "P");

    c3->cd(2);
    //frame5->GetYaxis()->SetTitle("");
    gPad->SetLeftMargin(0.15);
    frame5->Draw();

    c3->Print("residual_pull_distributions.png");
}
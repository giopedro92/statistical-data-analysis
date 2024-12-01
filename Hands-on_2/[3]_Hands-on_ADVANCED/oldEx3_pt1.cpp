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
#include "RooFormulaVar.h"
#include "RooDataHist.h"
#include "RooGenericPdf.h"
#include "RooAbsData.h"
#include "RooAbsPdf.h"
#include <iostream>

using namespace RooFit;

void Ex3_pt1() {
// PART 1------------------------------------------------

// OBSERVABLE: reconstructed neutrino energy
    RooRealVar energy{"energy", "Neutrino Energy", 0.5, 14, "GeV"};
    
// Load datasets
    // events observed
    RooDataSet data = *RooDataSet::read("minos_2013_data.dat", energy, "v");
    // MC simulation
    RooDataSet mc_noosc = *RooDataSet::read("minos_2013_mc.dat", energy, "v");
    
// BUILD MODEL
    // create an histogram-based function which represents the shape 
    // of the energy distribution for non-oscillated neutrinos
    RooDataSet *dd = (RooDataSet*) mc_noosc.reduce(RooArgSet(energy));
    RooDataHist *dh_mc_noosc = dd->binnedClone();

    // a real-valued function sampled from a multidimensional histogram
    // RooHistFunc::RooHistFunc (const char * name,
    //                           const char * title,
    //                           const RooArgSet & vars,
    //                           const RooDataHist & dhist,
    //                           Int_t intOrder = 0 
    //                           )
    RooHistFunc func_noosc{"func_noosc", "No oscillation", energy, *dh_mc_noosc, 2};

    // SHAPE
    // initial parameters from the paper (but bigger to make the fit work)
    RooRealVar mixing{"mixing", "mixing", 0.95, 0, 1};
    RooRealVar dm2{"dm2", "dm2", 2.41e-3, 1.8e-3, 3e-3, "eV^{2}"};
    RooRealVar L{"L", "L", 730, "km"};

    RooFormulaVar prob_osc{"prob_osc",
                           "Oscillation Probability",
                           "1 - @0 * (sin(1.267 * @1 * @2 / @3))**2",
                           RooArgList(mixing, dm2, L, energy)};

    // shape is the product func_noosc * prob_osc
    RooGenericPdf model{"model", "model", "@0 * @1", RooArgSet(prob_osc, func_noosc)};

// FIT, PLOT and SAVE
    // Fit
    model.fitTo(data);

    // Plot
    // Create an empty plot frame
    //RooPlot* frame = energy.frame();
    TCanvas *c = new TCanvas("c", "Neutrino energy");
    auto frame = energy.frame(Title("Neutrino energy"));

    c->SetCanvasSize(2490, 1400);
    c->SetWindowSize(2500, 1500);

    data.plotOn(frame); // Plot data on frame
    model.plotOn(frame); // Plot model on frame

    // Draw frame on canvas
    frame->Draw();

    // Save
    c->Print("minos_data.png");
}
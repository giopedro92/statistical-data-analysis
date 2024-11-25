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

using namespace RooFit;

void Ex3_pt1() {
// PART 1
    RooRealVar energy{"energy", "Neutrino Energy", 0.5, 14, "GeV"};
    
    // Load data
    RooDataSet data = *RooDataSet::read("minos_2013_data.dat", energy, "v"); // events observed
    RooDataSet mc_noosc = *RooDataSet::read("minos_2013_mc.dat", energy, "v"); // MC simulation

    // create an histogram-based function which represents the shape 
    // of the energy distribution for non-oscillated neutrinos
    RooDataSet *dd = (RooDataSet*) mc_noosc.reduce(RooArgSet(energy));
    RooDataHist *dh_mc_noosc = dd->binnedClone();

    RooHistFunc func_noosc{ "func_mc_noosc", "No oscillation", energy, *dh_mc_noosc, 2};

// -
    // initial parameters from the paper
    RooRealVar sin2_2theta{"sin2_2theta", "sin2_2theta", 0.95, 0, 1};
    RooRealVar dm2{"dm2", "dm2", 2.41e-3, 1.8e-3, 3e-3};
    RooRealVar L{"L", "L", 730, "km"};

    RooFormulaVar prob_osc{"prob_osc", "Oscillation Probability", "1 - @0 * sin(1.267 * @1 * @2 / @3)**2", RooArgList(sin2_2theta, dm2, L, energy)};

    RooGenericPdf model{"model", "model", "@0*@1", RooArgSet(prob_osc, func_noosc)};

    model.fitTo(data);

    // DRAWING
    // Create an empty plot frame
    //RooPlot* frame = energy.frame();
    TCanvas *c = new TCanvas();
    auto frame = energy.frame(Title("Neutrino energy"));

    data.plotOn(frame); // Plot data on frame
    model.plotOn(frame); // Plot model on frame

    // Draw frame on canvas
    frame->Draw();
    c->Print("minos_data.png");
}
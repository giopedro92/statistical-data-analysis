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

void Ex3_pt2() {
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



// PART 2
    std::cout << "\n PART 2 \n";
    RooAbsReal *nll = model.createNLL(data);
    RooMinimizer m(*nll);
    m.setVerbose(kTRUE);
    m.migrad();
    sin2_2theta.Print();
    dm2.Print();

// -
    std::cout << "\n Hesse \n";
    m.setVerbose(kFALSE);
    m.hesse();
    sin2_2theta.Print();
    dm2.Print();

// -
    std::cout << "\n Hesse \n";
    m.minos(dm2);
    dm2.Print();

    RooFitResult *res = m.save();
    res->Print("v");

    // Contour
    RooPlot *frame1 = m.contour(dm2, sin2_2theta, 1, 2, 3);
    // values calculated from the other paper

    frame1->SetTitle("Minuit contour plot");
    frame1->Draw();
}
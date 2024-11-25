#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooGaussian.h"
#include "TCanvas.h"
#include "RooPlot.h"
#include "TAxis.h"
using namespace RooFit;

void example1() {
// OBJECTS
// Observable with lower and upper bound:
RooRealVar mes{"mes", "m_{ES}", 5.26, 5.30, "GeV"};

// Parameters with starting value, lower bound, upper bound:
RooRealVar sigmean{"sigmean",   "B^{#pm} mass", 5.28, 5.20, 5.30};
RooRealVar sigwidth{"sigwidth", "B^{#pm} width", 0.0027, 0.001, 1.};

// Build a Gaussian PDF:
RooGaussian signal{"signal", "signal PDF", mes, sigmean, sigwidth};

// PLOT
// Create an empty plot frame
RooPlot* xframe = mes.frame();

// Plot model on frame
signal.plotOn(xframe);

// Draw frame on canvas
xframe->Draw();
}
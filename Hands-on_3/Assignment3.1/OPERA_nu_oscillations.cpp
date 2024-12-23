// Discovery of tau neutrino appearance
// in the CNGS neutrino beam
// with the OPERA experiment

// In this analysis, tau candidates are selected looking into 4 channels
// corresponding to 4 different tau decay modes.

// observed number of ντ candidates n_i
// for each individual τ decay channel i = 1,2,3,4
// is an independent Poisson process with expectation μ*s_i + β_i
// s_i (constant) expected signal events
// β_i (parameter) expected background events
// signal strength μ (parameter) continuous multiplicative parameter for the expected signal
// background-only hypothesis μ = 0
// nominal signal hypothesis  μ = 1.

// likelihood Gaussian terms to account for the background uncertainties:
// σ_bi background uncertainty for channel i (from Table 3)
// β_i  background parameters Gaussian modelled.

#include "RooRealVar.h"
#include "RooAbsArg.h"
#include "RooAbsReal.h"
#include "RooDataSet.h"
#include "RooGaussian.h"
#include "RooPoisson.h"
#include "RooArgSet.h"
#include "RooWorkspace.h"
#include "RooStats/HypoTestResult.h"
#include "RooStats/ProfileLikelihoodCalculator.h"
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
using namespace RooStats;

// consider only one (overall) channel defined as the sum of events
// neglecting which decay mode they are associated with


// ------------------------------------------------------

// take values from the row “Total” of Table 3
// observed events (observable),
// nominal number of signal events (constant, neglect the error)
// expected number of background events (parameter)
// uncertainty of the background events (constant).



void OPERA_nu_oscillations() {
// - Create a counting model based on Poisson Statistics
    RooRealVar nobs("nobs", "Observed events", 0., 10.);

    // s nominal number of signal events
    RooRealVar s ("s", "Nominal Signal", 2.64); 

    // μ signal strength
    // μ parameter we want to estimate or set a limit (parameter of interest)
    RooRealVar mu("mu", "Signal strength", 0.5, 0., 1.);

    // b number of background events
    RooRealVar b("b", "Background events", 0., 1.); //nuisance parameter

    RooRealVar b0("b0", "Background mean", 0.2, 0.3);

    RooRealVar sigma_b("sigma_b", "Background uncertainty", 0.05);

    // expected events nexp = μ s+b
    RooFormulaVar nexp("nexp",  "mu * s + b", RooArgSet(mu, s, b));

    // PDFs
    // expected distribution for nexp Poisson(nobs | nexp = μ s +b)
    RooPoisson poisson("poisson", "Poisson PDF", nobs, nexp);
    // b is the nuisance parameter
    // real value of b is unknown
    // best estimate is “b0” with uncertainty sigma_b
    // To express this uncertainty we add in the model a Gaussian constraint
    // We can interpret this term as having
    // an additional measurement b0 with an uncertainty sigmab
    // i.e. we have a likelihood for that measurement Gaussian( b0 | b, sigmab)
    RooGaussian gauss("gauss", "Gaussian constraint", b0, b, sigma_b);

    // Final model (Poisson * Gaussian)
    RooProdPdf model("model", "Counting model", poisson, gauss);


// - generate a hypothetical observed data set
    // we have a counting model
    // the data set will contain only one event and
    // the observable nobs will have our desired value
    
    // make data set with the number of observed events; nobs is a RooRealVar
    RooDataSet data("data", "Observed data", nobs);
    nobs.setVal(5.);
    data.add(nobs);


// - Import the model and the data in a RooFit RooWorkspace and save to file
    // import data set in workspace and save it in a file
    // Creazione del workspace
    RooWorkspace w("w");
    w.import(model);
    w.import(data);


// - Create the ModelConfig object and import in the workspace
    ModelConfig mc("mc", &w);
    
    mc.SetPdf(*w.pdf("model"));
    
    mc.SetParametersOfInterest(*w.var("mu"));
    
    mc.SetObservables(*w.var("nobs"));
    
    // We need to add in the ModelConfig also b0 as a “global observable”
    // The reason is that b0 needs to be treated as anauxiliary observable
    // in case of frequentist statistics and varied when tossing pseudo-experiments.
    mc.SetGlobalObservables(*w.var("b0"));
    
    mc.SetNuisanceParameters(*w.var("b"));
    
    w.import(mc);


    w.var("b0")->setVal(0.25);
    // As b0 is the global observable, b0 needs to have a range
    // However, it is a constant variable (this is needed when we fit the model).
    w.var("b0")->setConstant(true); // needed for being treated as global observables
    
    
    // write workspace in the file (recreate file if already existing)
    w.writeToFile("CountingModel_OPERA.root", true);


    mc.Print();
    // import model in the workspace 
    w.import(mc);


    w.Print();
}
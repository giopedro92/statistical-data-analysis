#include "RooRealVar.h"
#include "RooGaussian.h"
#include "RooPoisson.h"
#include "RooFormulaVar.h"
#include "RooAddPdf.h"
#include "RooArgSet.h"
#include "RooDataSet.h"
#include "RooWorkspace.h"
#include "RooProdPdf.h"
#include "RooStats/HypoTestResult.h"
#include "RooStats/ProfileLikelihoodCalculator.h"


using namespace RooFit;
using namespace RooStats;


void ex1_1()  
   
{
    RooRealVar nobs("nobs", "Observed events", 0.,10.);
    RooRealVar s ("s","nominal signal",2.64); 
    RooRealVar mu("mu", "Signal strength",0.5, 0., 1.); //parameter of interest
    RooRealVar b("b", "Background events", 0., 1.); //nuisance parameter
    RooRealVar b0("b0", "Background mean",0.2,0.3);
    RooRealVar sigma_b("sigma_b", "Background uncertainty", 0.05);

    // Formula per il numero atteso
    RooFormulaVar nexp("nexp",  "mu * s + b", RooArgSet(mu,s, b));

    // PDFs
    RooPoisson poisson("poisson", "Poisson PDF", nobs, nexp);
    RooGaussian gauss("gauss", "Gaussian constraint", b0, b, sigma_b);

    // Modello finale (Poisson * Gaussian)
    RooProdPdf model("model", "Counting model", poisson, gauss);

    // Set di dati osservati
    RooDataSet data("data", "Observed data", nobs);
    nobs.setVal(5.);
    data.add(nobs);


    // Creazione del workspace
    RooWorkspace w("w");
    w.import(model);
    w.import(data);
    

    // Configurazione per il test d'ipotesi
    ModelConfig mc("mc", &w);
    mc.SetPdf(*w.pdf("model"));
    mc.SetParametersOfInterest(*w.var("mu"));
    mc.SetObservables(*w.var("nobs"));
    mc.SetGlobalObservables(*w.var("b0"));
    mc.SetNuisanceParameters(*w.var("b"));
    w.import(mc);

    w.var("b0")->setVal(0.25);
    w.var("b0")->setConstant(true);
    
    w.writeToFile("CountingModel.root", true);
    
    
    
}

void ex1_2()
{
    RooRealVar nobs1("nobs1", "Observed events1", 0.,10.);
    RooRealVar nobs2("nobs2", "Observed events2", 0.,10.);
    RooRealVar nobs3("nobs3", "Observed events3", 0.,10.);
    RooRealVar nobs4("nobs4", "Observed events4", 0.,10.);

    RooRealVar s1 ("s1","nominal signal1",0.52); 
    RooRealVar s2 ("s2","nominal signal2",0.73); 
    RooRealVar s3 ("s3","nominal signal3",0.61); 
    RooRealVar s4 ("s4","nominal signal4",0.78); 

    RooRealVar mu("mu", "Signal strength",0.5, 0., 1.); 

    RooRealVar b1("b1", "Background events1", 0., 1.); 
    RooRealVar b2("b2", "Background events2", 0., 1.);
    RooRealVar b3("b3", "Background events3", 0., 1.);
    RooRealVar b4("b4", "Background events4", 0., 1.);

    RooRealVar b01("b01", "Background mean1",0.2,0.3);
    RooRealVar b02("b02", "Background mean2",0.2,0.3);
    RooRealVar b03("b03", "Background mean3",0.2,0.3);
    RooRealVar b04("b04", "Background mean4",0.2,0.3);

    RooRealVar sigma_b1("sigma_b1", "Background uncertainty1", 0.01);
    RooRealVar sigma_b2("sigma_b2", "Background uncertainty2", 0.03);
    RooRealVar sigma_b3("sigma_b3", "Background uncertainty3", 0.001);
    RooRealVar sigma_b4("sigma_b4", "Background uncertainty4", 0.01);
    

    // Formula per il numero atteso
    RooFormulaVar nexp1("nexp1",  "mu * s1 + b1", RooArgSet(mu,s1, b1));
    RooFormulaVar nexp2("nexp2",  "mu * s2 + b2", RooArgSet(mu,s2, b2));
    RooFormulaVar nexp3("nexp3",  "mu * s3 + b3", RooArgSet(mu,s3, b3));
    RooFormulaVar nexp4("nexp4",  "mu * s4 + b4", RooArgSet(mu,s4, b4));

    // PDFs
    RooPoisson poisson1("poisson1", "Poisson PDF1", nobs1, nexp1);
    RooPoisson poisson2("poisson2", "Poisson PDF2", nobs2, nexp2);
    RooPoisson poisson3("poisson3", "Poisson PDF3", nobs3, nexp3);
    RooPoisson poisson4("poisson4", "Poisson PDF4", nobs4, nexp4);

    RooGaussian gauss1("gauss1", "Gaussian constraint1", b01, b1, sigma_b1);
    RooGaussian gauss2("gauss2", "Gaussian constraint2", b02, b2, sigma_b2);
    RooGaussian gauss3("gauss3", "Gaussian constraint3", b03, b3, sigma_b3);
    RooGaussian gauss4("gauss4", "Gaussian constraint4", b04, b4, sigma_b4);

    // Modello finale (Poisson * Gaussian)
    RooProdPdf model1("model1", "Counting model1", poisson1, gauss1);
    RooProdPdf model2("model2", "Counting model2", poisson2, gauss2);
    RooProdPdf model3("model3", "Counting model3", poisson3, gauss3);
    RooProdPdf model4("model4", "Counting model4", poisson4, gauss4);

    RooProdPdf model_tot("model_tot", "Counting model_tot", RooArgSet(model1,model2,model3,model4));

    // Set di dati osservati
    RooDataSet data1("data1", "Observed data1", RooArgSet(nobs1,nobs2,nobs3,nobs4));
    nobs1.setVal(3.);
    nobs2.setVal(1.);
    nobs3.setVal(1.);
    nobs4.setVal(0.);
    data1.add(nobs1);
    data1.add(nobs2);
    data1.add(nobs3);
    data1.add(nobs4);


    // Creazione del workspace
    RooWorkspace w1("w1");
    w1.import(model_tot);
    w1.import(data1);
    

    // Configurazione per il test d'ipotesi
    ModelConfig mc1("mc1", &w1);
    mc1.SetPdf(*w1.pdf("model_tot"));
    mc1.SetParametersOfInterest(*w1.var("mu"));
    mc1.SetObservables(RooArgSet(*w1.var("nobs1"),*w1.var("nobs2"),*w1.var("nobs3"),*w1.var("nobs4")));
    mc1.SetGlobalObservables(RooArgSet(*w1.var("b01"),*w1.var("b02"),*w1.var("b03"),*w1.var("b04")));
    mc1.SetNuisanceParameters(RooArgSet(*w1.var("b1"),*w1.var("b2"),*w1.var("b3"),*w1.var("b4")));
    w1.import(mc1);

    w1.var("b01")->setVal(0.04);
    w1.var("b01")->setConstant(true);
    w1.var("b02")->setVal(0.17);
    w1.var("b02")->setConstant(true);
    w1.var("b03")->setVal(0.004);
    w1.var("b03")->setConstant(true);
    w1.var("b04")->setVal(0.03);
    w1.var("b04")->setConstant(true);
    
    w1.writeToFile("CountingModel1.root", true);
}
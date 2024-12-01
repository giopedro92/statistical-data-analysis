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

void Ex11() {
    RooWorkspace w{"Ex11workspace"};
    
    // Observable: A variable named “x” defined in the range [−20,20]
    //w.factory("x[-20,20]");
    
    // Both Gaussians share the same mean, “mean”, fixed at 0 (constant)
    //w.factory("mean[0]");   // mean constant 0
    
    // The standard deviation (σ) of the first Gaussian, “s1”, is 3.K,
    // where K is the last digit of yourmatriculation number. This is a constant.
    w.factory("Gaussian::gaus1(x[-20,20],mean[0],s1[3.9])");
    
    // The standard deviation (σ) of the second Gaussian, “s2”,
    // is defined within the interval [3,6] and has an initial value of 4
    w.factory("Gaussian::gaus2(x,mean,s2[4,3,6])"); // same mean = strong correlation
    
    // Model: The sum of two Gaussians: f⋅gaus1(..)+(1−f)⋅gaus2(..)
    // The coefficient “f” has an initial value of 0.5 and is not constant
    w.factory("SUM::model(f[0.5,0,1] * gaus1, gaus2)"); // model

    std::cout << "\n w.Print() \n" << endl;
    w.Print();

    // Retrive from the workspace
    auto gaus1 = w.pdf("gaus1");
    auto gaus2 = w.pdf("gaus2");

    auto model = w.pdf("model"); // Returns as RooAbsPdf*
    auto x     = w.var("x");     // Returns as RooRealVar*

    // Visualize and save the gaussians we are working with
    TCanvas *c1 = new TCanvas("c1", "c1");
    c1->SetCanvasSize(2490, 1400);
    c1->SetWindowSize(2500, 1500);

    auto frame = x->frame(Title("gaus1, gaus2, model"));

    gaus1->plotOn(frame, LineStyle(1));
    gaus2->plotOn(frame, LineStyle(2));
    model->plotOn(frame, LineStyle(10));

    frame->GetXaxis()->SetTitle("x");
    frame->GetYaxis()->SetTitle("gaus1, gaus2, model");

    frame->Draw();

    c1->Print("gaus1_gaus2_model.png");


// Tasks
// 1. Generate a Dataset with 1000 events
    // Generate a toy MC sample from composite PDF
    auto data = model->generate(*x, 1000); // use *x because x is a pointer

    // importing data to workspace
    w.import(*data);


// 2. Save the Workspace (model + data) to a file
    w.writeToFile("Ex11workspace.root");

    std::cout << "\n w.Print() \n" << endl;
    w.Print();

// 3. Minimize the Likelihood
    // Construct function object representing –log(L)
    RooAbsReal* nll = model->createNLL(*data);

    // Create a MINUIT interface to this  likelihood function
    RooMinimizer m(*nll);
    
    // Enable verbose mode to display the MINUIT steps
    m.setVerbose(kTRUE);

    // Use Minuit as minimizer instead of Minuit 2
    m.setMinimizerType("Minuit");

    // MIGRAD likelihood minimization
    std::cout << "\n MIGRAD \n" << endl;
    
    // Call MIGRAD to minimize the likelihood
    m.migrad();

    // Display the values of the parameters f, mean, s1, and s2
    std::cout << "\n Parameters MIGRAD \n" << endl;
    model->getParameters(RooArgSet(*x))->Print("s");
    // also std::unique_ptr<RooArgSet> { model->getParameters(*x) } -> Print("s");


// 4. HESSE Error Calculation
    std::cout << "\n HESSE \n" << endl;

    // Disable verbose mode
    m.setVerbose(kFALSE);

    // Call HESSE to compute errors using the second derivative of the
    m.hesse();

    // Display the values of the parameters f, mean, s1, and s2
    std::cout << "\n Parameters HESSE \n" << endl;
    model->getParameters(RooArgSet(*x))->Print("s");
    // also std::unique_ptr<RooArgSet> { model->getParameters(*x) } -> Print("s");


// 5. MINOS Error Calculation for “s2”
    std::cout << "\n MINOS \n" << endl;
    
    // retrive from the workspace
    auto s2 = w.var("s2");
    
    // Call MINOS for the parameter “s2”
    m.minos(*s2);
    // also m.minos(*w.var("s2"));

    // Display the values of the parameters f, mean, s1, and s2
    std::cout << "\n Parameters MINOS \n" << endl;
    model->getParameters(RooArgSet(*x))->Print("s");
    // also std::unique_ptr<RooArgSet> { sum->getParameters(*x) } -> Print("s");


// 6. Save Results
    std::cout << "\n SAVE RESULTS \n" << endl;
    
    // Take a snapshot of the fit result
    RooFitResult *res = m.save();
    
    // Print the result in verbose mode
    res->Print("v");

    // Visualize the correlation matrix from the fit result
    TCanvas *c2 = new TCanvas("c2", "c2");
    c2->SetCanvasSize(2490, 1400);
    c2->SetWindowSize(2500, 1500);

    gStyle->SetPalette(1);
    res->correlationHist()->Draw("colz");

    c2->Print("correlation_matrix.png");


// 7. Contour Plot
    std::cout << "\n CONTOUR PLOT \n" << endl;

    // Create a contour plot for “f” vs “s2” at 68%, 95.45%, 99.73% confidence levels
    // using the method RooMinuit::contour(var1, var2, n1, n2, n3)
    TCanvas *c3 = new TCanvas("c3", "c3");
    c3->SetCanvasSize(2490, 1400);
    c3->SetWindowSize(2500, 1500);

    // retrive from the workspace
    auto f = w.var("f");
    
    RooPlot *plot = m.contour(*f, *s2, 1, 2, 3); // 1, 2, 3 sigma

    plot->GetXaxis()->SetRangeUser(-1, 2);
    plot->GetYaxis()->SetRangeUser(2.5, 6.5);

    plot->SetTitle("Contour plot of f vs s2");
    plot->GetXaxis()->SetTitle("v2");
    plot->GetYaxis()->SetTitle("f");

    // Draw the plot frame and save it to a file
    plot->Draw();

    c3->Print("f_vs_s2.png");
}
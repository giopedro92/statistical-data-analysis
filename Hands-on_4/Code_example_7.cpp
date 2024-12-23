TTree* inputTree = (TTree*)source->Get( "<YourTreeName>" );

TCut signalCut = ...; // how to identify signal events
TCut backgrCut = ...; // how to identify background events

factory->SetInputTrees( inputTree, signalCut, backgrCut );
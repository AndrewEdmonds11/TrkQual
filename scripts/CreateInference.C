/// \file
/// \ingroup tutorial_tmva
/// \notebook -nodraw
/// This macro parses a .onnx file
/// into RModel object and further generating the .hxx header files for inference.
///
/// \macro_code
/// \macro_output
/// \author Sanjiban Sengupta
/// modified by A. Edmonds (2025)

#include "root/TMVA/RModel.hxx"
#include "root/TMVA/RModelParser_ONNX.hxx"
#include <iostream>

using namespace TMVA::Experimental;

//bname is the base name the model saved as
void CreateInference(const char* bname,const char* suffix=""){
  std::string modelname = std::string("model/") + std::string(bname) + std::string(".onnx");
  std::string infername = std::string("code/") + std::string(bname) +std::string(suffix) + std::string(".hxx");

  SOFIE::RModelParser_ONNX parser;
  SOFIE::RModel model = parser.Parse(modelname, true);

  //Generating inference code
  model.Generate();
  // write the code in a file (by default Linear_16.hxx and Linear_16.dat
  model.OutputGenerated(infername);
}

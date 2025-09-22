/// \file
/// \ingroup tutorial_tmva
/// \notebook -nodraw
/// This macro parses a Keras .h5 file
/// into RModel object and further generating the .hxx header files for inference.
///
/// \macro_code
/// \macro_output
/// \author Sanjiban Sengupta

#include "root/TMVA/RModel.hxx"
#include "root/TMVA/RModelParser_ONNX.hxx"
#include <iostream>

using namespace TMVA::Experimental;

//bname is the base name the model saved as
void CreateInference(const char* bname,const char* suffix=""){
  //  string modelname = string("model/") + string(bname) + string(".h5");
  std::string modelname = std::string("model/") + std::string(bname) + std::string(".onnx");
  std::string infername = std::string("code/") + std::string(bname) +std::string(suffix) + std::string(".hxx");
  //  //Parsing the saved Keras .h5 file into RModel object
  //  SOFIE::RModel model = SOFIE::PyKeras::Parse(modelname);

  //  //Generating inference code
  //  model.Generate();
  //  cout << "Saving file " << infername << endl;
  //  model.OutputGenerated(infername);

  SOFIE::RModelParser_ONNX parser;
  SOFIE::RModel model = parser.Parse(modelname, true);
  //  std::cout << model.UseSession() << std::endl;
  //  model.PrintRequiredInputTensors();
  //  model.PrintIntermediateTensors();
  //  model.PrintDynamicTensors();
  std::cout << "AE: " << std::endl;
  // auto outputTensors = model.GetOutputTensorNames();
  // for (auto& name : outputTensors) {
  //   model.GetTensorShape(name);
  //   //    std::cout << name << ": " << SOFIE::ConvertDynamicShapeToString() << std::endl;
  // }
  //  model.PrintRequiredInputTensors();
  // auto inputTensors = model.GetInputTensorNames();
  // for (auto& name : inputTensors) {
  // //   //    std::cout << model.IsInputTensor(name) << std::endl;
  //   model.GetTensorShape(name);
  // //   //    std::cout << name << ": " << SOFIE::ConvertDynamicShapeToString() << std::endl;
  // }
    //  model.PrintOutputTensors();
  
  //Generating inference code
  model.Generate();
  // write the code in a file (by default Linear_16.hxx and Linear_16.dat
  model.OutputGenerated(infername);
}

int main() {
  CreateInference("TrkQual_ANN1_v2.0");
  return 0;
}

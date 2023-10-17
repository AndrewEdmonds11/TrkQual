/// \file
/// \ingroup tutorial_tmva
/// \notebook -nodraw
/// This macro parses a Keras .h5 file
/// into RModel object and further generating the .hxx header files for inference.
///
/// \macro_code
/// \macro_output
/// \author Sanjiban Sengupta

using namespace TMVA::Experimental;

//bname is the base name the model saved as
void CreateInference(const char* bname,const char* suffix=""){
  string modelname = string("model/") + string(bname) + string(".h5");
  string infername = string("code/") + string(bname) +string(suffix) + string(".hxx");
  //Parsing the saved Keras .h5 file into RModel object
  SOFIE::RModel model = SOFIE::PyKeras::Parse(modelname);

  //Generating inference code
  model.Generate();
  cout << "Saving file " << infername << endl;
  model.OutputGenerated(infername);
}

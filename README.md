# TrkQual

## Introduction
The TrkQual algorithm is trained to classify tracks as either "high quality" or "low quality". At the moment, the model implemented in Offline is an Artificial Neural Network (see [here](https://github.com/Mu2e/Offline/blob/main/TrkDiag/src/TrackQuality_module.cc))

This README covers:
* where an analyzer can find things they might like to know (e.g. the definition of high quality and low quality), and
* instructions for those interested in retraining or improving on the model

## For the Interested Analyzer
The [jupyter notebook](TrkQualTrain.ipynb) contains lots of information close to the top of the file includings:
* EventNtuple datasets used (search for ```training_dataset```),
* definitions of high quality and low quality (search for ```high_qual``` and ```low_qual```),
* the features trained on (search for ```feature```), and
* the model structure (search for "Model Definitions")

## For the Interested (Re)Trainer
For those who are interested in either (a) retraining the current algorithm (e.g. we have updated reconstruction), or (b) investigating or updating an old model

### General Overview
There are two steps to releasing an updated TrkQual algorithm:

1. Train the algorithm and save the model as an ONNX file, and
1. Convert the trained model into C++ inference code to copy into Offline

Each of these will be done in a different environment.

### General Setup
You will need to create your own fork of the repository:

* go to www.github.com/AndrewEdmonds11/TrkQual and click "fork"
* then in your terminal:
```
cd /path/to/your/work/area/

# only need to do this once
git clone https://www.github.com/YourGitHubUsername/TrkQual.git
cd TrkQual/
git remote add -f andy https://www.github.com/AndrewEdmonds11/TrkQual.git

# do these whenever you are doing new development
git fetch andy main # get the latest and greatest
git checkout --no-track -b your-new-branchname andy/main
```

### Training a Model
For training, you need to ssh into a mu2egpvm machine with a port forwarded, and setup the correct python environment:

```
ssh -L XXXX:localhost:XXXX username@mu2egpvmYY.fnal.gov # XXXX is any port number, and YY is the gpvm number
cd /path/to/your/work/area/
mu2einit
pyenv rootana 2.0.0
```

You can start a jupyter notebook like so:

```
jupyter-notebook --no-browser --port=XXXX # XXXX is the same port that you forwarded when you ssh'd in
```

and copy and paste the URL to your browser to open it.

You will see a directory listing of the TrkQual directory. Click the TrkQualTrain.ipynb to open the notebook in your browser.

Make any changes that you want to make:
* if this is just a retraining with updated datasets, you can just change the dataset names in section "Common Definitions", and the ```training_version_numbers``` in "Model Definitions"
* if you want to add or remove features, you can do that in "Common Definitions" too
* if you want to add a new model, then you can do that in the cell that says ```A new model can go in this cell```
   * if you want to modify the ANN1 model (e.g. change structure, or activation functioon), then I would copy it into this new cell and call it ANN2
   * if you want to try a brandh new model (e.g. a BDT), then you may need to write a new ```save_func``` etc.

Once ready, click "Kernel->Restart & Run All". You will see a bunch of plots, including some comparisons to previous models. Your model will be saved in the model/ directory along with a ```*plots.root``` file containing histograms.


### Converting a Model for Use in Offline
There are two things we need to get the model running in Offline:
* a ```.hxx``` file containing code, and
* a ```.dat``` file containing parameters

For creating inference code, we use a different environment than for training:

```
ssh username@mu2egpvmYY.fnal.gov
cd /path/to/your/work/area/
mu2einit
muse setup EventNtuple
cd TrkQual/
```

You can then generate the inference code like so:

```
root -l -b scripts/CreateInference.C\(\"TrkQual_ANN1_v2\"\)
```

If you did not change the model, then you (should) just need to copy the .dat file to Offline (from experience this isn't always the case)

```
cp code/TrkQual_ANN1_v2.dat ../Offline/TrkDiag/data/
```

and make sure that the new .dat file is used in the TrackQuality module.

If you modified the ANN model, then you need to copy both the .hxx and .dat file

```
cp code/TrkQual_ANN2_v1.hxx ../Offline/TrkDiag/inc/
cp code/TrkQual_ANN2_v1.dat ../Offline/TrkDiag/data/
```

and make sure that the new model is implemented correctly the TrackQuality module.

If you trained a different model, then you are entering new territory and should discuss with experts how best to implement. Either:
* we make the ```TrackQuality``` module model agnostic, or
* we write separate ```TrackQuality``` modules for different models...

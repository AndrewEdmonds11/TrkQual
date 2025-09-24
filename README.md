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

## How to (Re)Train
There are two steps to releasing an updated TrkQual algorithm:

1. Train the algorithm and save the model as an ONNX file, and
1. Convert the trained model into C++ inference code to copy into Offline

Each of these will be done in a different environment.

### Training the Model
This is done using the Jupyter notebook

To train TrkQual we create a "TrkQualTree" from the EventNtuple using RooUtil. Then we use a jupyter notebook to perform the training

## Set Up
Log onto a mu2egpvm machine with a port forwarded:

```
ssh -L XXXX:localhost:XXXX username@mu2egpvmYY.fnal.gov
```

where XXXX is a port number. You will need to do this every time you log in.

Assuming you have a work area ```/path/to/your/work/area/```, do the following:

```
cd /path/to/your/work/area/
git clone https://www.github.com/AndrewEdmonds11/TrkQual.git # (or git clone your fork)
```

Then every time you log in do:
```
cd /path/to/your/work/area/
mu2einit
pyenv rootana 2.0.0
cd TrkQual/
```

Now start a jupyter notebook:

```
jupyter-notebook --no-browser --port=XXXX
```

and copy and paste the URL to your browser to open it.

You will see a directory listing of the TrkQual directory. Click the TrkQualTrain.ipynb to open the notebook in your browser. Make any changes that you want to the trainings (e.g. new variables, train on a different dataset) and then click "Kernel->Restart & Run All". Your model will be saved in the model/ directory.

## Exporting a saved model to be used in Offline

Create the ```.dat``` and ```.hxx``` file that we need in Offline

```
root -l -b scripts/CreateInference.C\(\"TrkQual_ANN1\"\)
```

If you did not change the model, then you just need to copy the .dat file to Offline with an updated version number

```
cp code/TrkQual_ANN1.dat ../Offline/TrkDiag/data/TrkQual_ANN1_vX.dat
```

and make sure that the new .dat file is used in the TrackQuality module.

If you did change the model, then you need to copy both the .hxx and .dat file

```
cp code/TrkQual_ANN2.hxx ../Offline/TrkDiag/inc/TrkQual_ANN2.hxx
cp code/TrkQual_ANN2.dat ../Offline/TrkDiag/data/TrkQual_ANN2_v1.dat
```

and make sure that the new model is implemented correctly the TrackQuality module.

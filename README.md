# TrkQual
Scripts for training TrkQual on EventNtuple (v6)

## Overview
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
source /cvmfs/mu2e.opensciencegrid.org/env/ana/current/bin/activate
muse setup EventNtuple
cd TrkQual/
```

## Training TrkQual
First choose an EventNtuple dataset that you want to train on and create a filelist e.g.

```
setup mu2efiletools
mkdir filelist/
mu2eDatasetFileList nts.mu2e.CeEndpointMix1BBTriggered.MDC2020an_v06_01_01_best_v1_3.root > filelists/nts.mu2e.CeEndpointMix1BBTriggered.MDC2020an_v06_01_01_best_v1_3.root.list
```

Then create a copy of ```scripts/TrkQualTree_v2_0.C``` and edit it so that ```input_filename``` points to your filelist and ```output_filename``` points to somewhere you can write to. To run the script:

```
root -l -b scripts/TrkQualTree_vX_X.C+
```

You will now have a TrkQualTree in the file given by ```output_filename```.

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

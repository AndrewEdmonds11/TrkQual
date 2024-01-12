# TrkQual
Scripts for training TrkQual on TrkAna (v4) trees

## Set Up
Log onto a mu2egpvm machine with a port forwarded:

    ssh -L XXXX:localhost:XXXX username@mu2egpvmYY.fnal.gov

where XXXX is a port number. You will need to do this every time you log in.

Then change directory to a working area and copy the repositry:

    cd /to/your/work/area/
    git clone https://www.github.com/AndrewEdmonds11/TrkQual.git

If you want to commit any changes you make, then you will need to make your own fork and do:

    git clone git@github.com:YourGitHubUsername/TrkQual.git

instead.

Then change directory into TrkQual and setup the environment:

    cd TrkQual/
    setup mu2e
    setup pyana v1_04_00

where you will need to do these setup commands every time you log in.

## Training and saving a model
To train you should have logged in with port XXXX forwarded (see above), and then you can do:

    cd /to/your/work/area/TrkQual
    setup mu2e
    setup pyana
    jupyter-notebook --no-browser --port=XXXX

then copy and paste the URL that prints out to your browser.

You will see a directory listing of the TrkQual directory. Click the TrkQualTrain.ipynb to open the notebook in your browser. Make any changes that you want to the trainings (e.g. new variables, train on a different dataset) and then click "Kernel->Restart & Run All". Your model will be saved in the model/ directory.

## Exporting a saved model to be used in Offline

Create the ```.dat``` and ```.hxx``` file that we need in Offline

 root -l -b CreateInference.C\(\"TrkQual_ANN1\"\)


If you did not change the model, then you just need to copy the .dat file to Offline with an updated version number

 cp code/TrkQual_ANN1.dat ../Offline/TrkDiag/data/TrkQual_ANN1_vX.dat

and make sure that the new .dat file is used in the TrackQuality module.

If you did change the model, then you need to copy both the .hxx and .dat file

 cp code/TrkQual_ANN2.hxx ../Offline/TrkDiag/inc/TrkQual_ANN2.hxx
 cp code/TrkQual_ANN2.dat ../Offline/TrkDiag/data/TrkQual_ANN2_v1.dat
 
and make sure that the new model is implemented correctly the TrackQuality module.

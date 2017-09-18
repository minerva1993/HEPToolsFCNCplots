#!/usr/bin/env python
import sys, os
import google.protobuf
from ROOT import *
from keras.models import Model, Sequential
from keras.layers import Input, Dense, Activation, Dropout, add, concatenate
#from keras.layers.core import Dropout
from keras.regularizers import l2
from keras.optimizers import SGD, Adam, Adadelta
from keras.utils import plot_model

TMVA.Tools.Instance()
TMVA.PyMethodBase.PyInitialize()

fout = TFile("output_keras.root","recreate")

factory = TMVA.Factory("TMVAClassification", fout,
                       "!V:!Silent:Color:DrawProgressBar:Transformations=I;D;P;G;D:AnalysisType=Classification" )

loader = TMVA.DataLoader("keras_res2")
loader.AddVariable("njets", "I")
loader.AddVariable("nbjets_m",'I')
loader.AddVariable("ncjets_m",'I')
loader.AddVariable("lepDPhi",'F')
loader.AddVariable("missingET",'F')
loader.AddVariable("bjetmDR",'F')
loader.AddVariable("bjetmDEta",'F')
loader.AddVariable("bjetmDPhi",'F')
loader.AddVariable("dibjetsMass",'F')
loader.AddVariable("bjetPt_dibjetsm",'F')
loader.AddVariable("cjetPt",'F')
loader.AddVariable("jet1pt",'F')
loader.AddVariable("jet2pt",'F')
loader.AddVariable("jet3pt",'F')
loader.AddVariable("jet4pt",'F')
loader.AddVariable("jet1eta",'F')
loader.AddVariable("jet2eta",'F')
loader.AddVariable("jet3eta",'F')
loader.AddVariable("jet4eta",'F')
#loader.AddVariable("jet1phi",'F')
#loader.AddVariable("jet2phi",'F')
#loader.AddVariable("jet3phi",'F')
#loader.AddVariable("jet4phi",'F')
loader.AddVariable("jet1m",'F')
loader.AddVariable("jet2m",'F')
loader.AddVariable("jet3m",'F')
loader.AddVariable("jet4m",'F')
loader.AddVariable("jet1csv",'F')
loader.AddVariable("jet2csv",'F')
loader.AddVariable("jet3csv",'F')
loader.AddVariable("jet4csv",'F')
loader.AddVariable("jet1cvsl",'F')
loader.AddVariable("jet2cvsl",'F')
loader.AddVariable("jet3cvsl",'F')
loader.AddVariable("jet4cvsl",'F')
loader.AddVariable("jet1cvsb",'F')
loader.AddVariable("jet2cvsb",'F')
loader.AddVariable("jet3cvsb",'F')
loader.AddVariable("jet4cvsb",'F')
loader.AddVariable("DRlepWeta",'F')#
#loader.AddVariable("DRlepWphi",'F')
loader.AddVariable("DRlepWm",'F')#
loader.AddVariable("DRjet0pt",'F')
loader.AddVariable("DRjet0eta",'F')
#loader.AddVariable("DRjet0phi",'F')
loader.AddVariable("DRjet0m",'F')
loader.AddVariable("DRjet0csv",'F')
loader.AddVariable("DRjet0cvsl",'F')
loader.AddVariable("DRjet0cvsb",'F')
loader.AddVariable("DRjet1pt",'F')
loader.AddVariable("DRjet1eta",'F')
#loader.AddVariable("DRjet1phi",'F')
loader.AddVariable("DRjet1m",'F')
loader.AddVariable("DRjet1csv",'F')
loader.AddVariable("DRjet1cvsl",'F')
loader.AddVariable("DRjet1cvsb",'F')
loader.AddVariable("DRjet2pt",'F')
loader.AddVariable("DRjet2eta",'F')
#loader.AddVariable("DRjet2phi",'F')
loader.AddVariable("DRjet2m",'F')
loader.AddVariable("DRjet2csv",'F')
loader.AddVariable("DRjet2cvsl",'F')
loader.AddVariable("DRjet2cvsb",'F')
loader.AddVariable("DRjet3pt",'F')
loader.AddVariable("DRjet3eta",'F')
#loader.AddVariable("DRjet3phi",'F')
loader.AddVariable("DRjet3m",'F')
loader.AddVariable("DRjet3csv",'F')
loader.AddVariable("DRjet3cvsl",'F')
loader.AddVariable("DRjet3cvsb",'F')
loader.AddVariable("DRjet12pt",'F')
loader.AddVariable("DRjet12eta",'F')
#loader.AddVariable("DRjet12phi",'F')
loader.AddVariable("DRjet12m",'F')
loader.AddVariable("DRjet12DR",'F')
loader.AddVariable("DRjet23pt",'F')
loader.AddVariable("DRjet23eta",'F')
#loader.AddVariable("DRjet23phi",'F')
loader.AddVariable("DRjet23m",'F')
loader.AddVariable("DRjet31pt",'F')
loader.AddVariable("DRjet31eta",'F')
#loader.AddVariable("DRjet31phi",'F')
loader.AddVariable("DRjet31m",'F')
loader.AddVariable("DRlepTpt",'F')
loader.AddVariable("DRlepTeta",'F')
#loader.AddVariable("DRlepTphi",'F')
loader.AddVariable("DRlepTm",'F')
loader.AddVariable("DRhadTpt",'F')
loader.AddVariable("DRhadTeta",'F')
#loader.AddVariable("DRhadTphi",'F')
loader.AddVariable("DRhadTm",'F')


## Load input files
fsig1 = TFile("/home/minerva1993/fcnc/analysis_jw/tmva/v3/mkNtuple_v3/tmva_Top_Hct.root")
fsig2 = TFile("/home/minerva1993/fcnc/analysis_jw/tmva/v3/mkNtuple_v3/tmva_AntiTop_Hct.root")
fbkg1 = TFile("/home/minerva1993/fcnc/analysis_jw/tmva/v3/mkNtuple_v3/tmva_ttLF.root")
fbkg2 = TFile("/home/minerva1993/fcnc/analysis_jw/tmva/v3/mkNtuple_v3/tmva_ttbb.root")

tsig1 = fsig1.Get("tmva_tree")
tsig2 = fsig2.Get("tmva_tree")
tbkg1 = fbkg1.Get("tmva_tree")
tbkg2 = fbkg2.Get("tmva_tree")

loader.AddSignalTree(tsig1, 0.156137331574)
loader.AddSignalTree(tsig2, 0.156137331574)
loader.AddBackgroundTree(tbkg1, 0.0910581123792)
loader.AddBackgroundTree(tbkg2, 0.0910581123792)

sigCut = TCut("missingET > 0 && cjetPt > 0 && jet1csv > 0 &&  jet2csv > 0 &&  jet3csv > 0 && jet4csv > 0 && DRlepWpt > 0 && DRjet0csv > 0 && DRjet1csv > 0 && DRjet2csv > 0 && DRjet3csv > 0")

bkgCut = TCut("missingET > 0 && cjetPt > 0 && jet1csv > 0 &&  jet2csv > 0 &&  jet3csv > 0 && jet4csv > 0 && DRlepWpt > 0 && DRjet0csv > 0 && DRjet1csv > 0 && DRjet2csv > 0 && DRjet3csv > 0")

loader.PrepareTrainingAndTestTree(
    sigCut, bkgCut,
    "nTrain_Signal=30000:nTrain_Background=40000:nTest_Signal=5000:nTest_Background=5000:SplitMode=Random:NormMode=NumEvents:!V"
)

#factory.BookMethod(loader, TMVA.Types.kBDT, "BDT", "!H:!V:NTrees=850:MinNodeSize=2.5%:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=0.5:UseBaggedBoost:BaggedSampleFraction=0.5:SeparationType=GiniIndex:nCuts=20")

#factory.BookMethod(loader, TMVA.Types.kDNN, "DNN", '!H:V:ErrorStrategy=CROSSENTROPY:VarTransform=N:WeightInitialization=XAVIERUNIFORM:Architecture=CPU:Layout=ReLU|300,ReLU|500,ReLU|700,ReLU|700,ReLU|700,ReLU|700,ReLU|700,ReLU|700,ReLU|700,ReLU|500,ReLU|300,ReLU|100,LINEAR:TrainingStrategy=LearningRate=1e-2,Repetitions=1,ConvergenceSteps=20,Multithreading=True,Regularization=L2,WeightDecay=1e-4,BatchSize=100,TestRepetitions=5,DropConfig=0.2+0.0+0.0+0.0+0.2+0.0+0.0+0.0+0.2+0.0+0.0+0.0+0.0,Momentum=0.7')

#Keras
a = 700

inputs = Input(shape=(77,))
x = Dense(a)(inputs)
x = Dropout(0.3)(x)

branch_point1 = Dense(a, name='branch_point1')(x)
#branch_layer1 = Dense(500, name='branch_layer1')(branch_point1)

x = Dense(a, activation='relu')(x)
x = Dropout(0.3)(x)

branch_point3 = Dense(a, name='branch_point3')(x)

x = Dense(a, activation='relu')(x)
x = Dropout(0.3)(x)
x = Dense(a, activation='relu')(x)
x = Dropout(0.3)(x)
x = Dense(a, activation='relu')(x)
x = Dropout(0.3)(x)
x = Dense(a, activation='relu')(x)
x = Dropout(0.3)(x)#

x = add([x, branch_point1])
#x = concatenate([x, branch_layer1], axis=-1)

x = Dense(a, activation='relu')(x)

branch_point2 = Dense(a, name='branch_point2')(x)

x = Dense(a, activation='relu')(x)
x = Dropout(0.3)(x)
x = Dense(a, activation='relu')(x)
x = Dropout(0.3)(x)
x = Dense(a, activation='relu')(x)
x = Dropout(0.3)(x)
x = Dense(a, activation='relu')(x)#
x = Dropout(0.3)(x)

x = add([x, branch_point2])

x = Dense(a, activation='relu')(x)
x = Dropout(0.3)(x)

x = add([x, branch_point3])

predictions = Dense(2, activation='softmax')(x)

model = Model(inputs=inputs, outputs=predictions)

#model.compile(loss='categorical_crossentropy', optimizer=SGD(lr=0.01), metrics=['accuracy',])
model.compile(loss='binary_crossentropy', optimizer=Adam(lr=0.001, beta_1=0.9, beta_2=0.999, epsilon=1e-08, decay=2E-3), metrics=['accuracy'])
#model.compile(loss='categorical_crossentropy', optimizer=Adadelta(lr=1.0, rho=0.95, epsilon=1e-08, decay=0.0), metrics=['accuracy'])
model.save('model.h5')
model.summary()
plot_model(model, to_file='model.png')

factory.BookMethod(loader, TMVA.Types.kPyKeras, 'PyKeras',"H:!V:VarTransform=D,G:FilenameModel=model.h5:NumEpochs=10:BatchSize=200")

factory.TrainAllMethods()
factory.TestAllMethods()
factory.EvaluateAllMethods()
fout.Close()

TMVA.TMVAGui("output_keras.root")

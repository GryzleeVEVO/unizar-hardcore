disp('---------  Búsqueda exhaustiva, k = 5 ---------');

tic

[bestModel, bestError] = KFoldExhaustiva(XTrain, YTrain, 5);

XExp = expandir(XTrain, bestModel);

[XNorm, mu, sig] = normalizar(XExp);

thetaNorm = XNorm \ YTrain;

theta = desnormalizar(thetaNorm, mu, sig);

yPred = XExp * theta;

XExpTest = expandir(XTest, bestModel);
yPredTest = XExpTest * theta;

toc

bestModel

bestError

errorTrain = rmse(yPred, YTrain)
errorTest = rmse(yPredTest, YTest)

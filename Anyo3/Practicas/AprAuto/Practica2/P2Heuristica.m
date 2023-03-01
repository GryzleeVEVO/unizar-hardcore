disp('---------  Búsqueda heurística, k = 5 ---------');

tic

[bestModel, trainHistory, cvHistory] = KFoldHeuristica(XTrain, YTrain, 5);

XExp = expandir(XTrain, bestModel);

[XNorm, mu, sig] = normalizar(XExp);

thetaNorm = XNorm \ YTrain;

theta = desnormalizar(thetaNorm, mu, sig);

yPred = XExp * theta;

XExpTest = expandir(XTest, bestModel);
yPredTest = XExpTest * theta;

toc

graficaError(trainHistory(1, :), cvHistory(1, :), '1', 5);
graficaError(trainHistory(2, :), cvHistory(2, :), '2', 5);
graficaError(trainHistory(3, :), cvHistory(3, :), '3', 5);

bestModel

errorTrain = rmse(yPred, YTrain)
errorTest = rmse(yPredTest, YTest)

trainHistory
cvHistory

disp('---------  Búsqueda heurística, k = 10 ---------');

[bestModel, trainHistory, cvHistory] = KFoldHeuristica(XTrain, YTrain, 10);

XExp = expandir(XTrain, bestModel);

[XNorm, mu, sig] = normalizar(XExp);

thetaNorm = XNorm \ YTrain;

theta = desnormalizar(thetaNorm, mu, sig);

yPred = XExp * theta;

XExpTest = expandir(XTest, bestModel);
yPredTest = XExpTest * theta;

%graficaError(trainHistory(1, :), cvHistory(1, :), '1', 10);
%graficaError(trainHistory(2, :), cvHistory(2, :), '2', 10);
%graficaError(trainHistory(3, :), cvHistory(3, :), '3', 10);

bestModel

errorTrain = rmse(yPred, YTrain)
errorTest = rmse(yPredTest, YTest)

trainHistory
cvHistory

disp('---------  Búsqueda heurística, k = N ---------');

[bestModel, trainHistory, cvHistory] = KFoldHeuristica(XTrain, YTrain, size(XTrain, 1));

XExp = expandir(XTrain, bestModel);

[XNorm, mu, sig] = normalizar(XExp);

thetaNorm = XNorm \ YTrain;

theta = desnormalizar(thetaNorm, mu, sig);

yPred = XExp * theta;

XExpTest = expandir(XTest, bestModel);
yPredTest = XExpTest * theta;

bestModel

errorTrain = rmse(yPred, YTrain)
errorTest = rmse(yPredTest, YTest)

trainHistory
cvHistory
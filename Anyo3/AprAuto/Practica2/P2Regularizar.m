disp('---------  Regularización, k = 5 ---------');

[bestLambda, trainHistory, cvHistory] = KFoldRegularizar(XTrain, YTrain, 5);

XExp = expandir(XTrain, [10,10,10]);

[XNorm, mu, sig] = normalizar(XExp);

H = XNorm' * XNorm + bestLambda * diag([0 ones(1, size(XExp, 2)-1)]);

% Entrena el modelo
thetaNorm = H \ (XNorm' * YTrain);
theta = desnormalizar(thetaNorm, mu, sig);

yPred = XExp * theta;

XExpTest = expandir(XTest, [10,10,10]);
yPredTest = XExpTest * theta;

bestLambda

errorTrain = rmse(yPred, YTrain)
errorTest = rmse(yPredTest, YTest)

trainHistory
cvHistory

graficaErrorReg(trainHistory, cvHistory);
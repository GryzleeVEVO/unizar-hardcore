% Ambos atributos se expanden a grado 6
XExp = expandir2(xTrain1, xTrain2, 6);
XExpTest = expandir2(xTest1, xTest2, 6);

% Obtiene mejor modelo junto a la historia de modelos probados
[bestLambda, trainHistory, cvHistory] = ...
KFoldRegularizarLogistica(XExp, yTrain, 5, options);

graficaF1(trainHistory, cvHistory);

% Entrena
theta0 = rand(size(XExp, 2), 1);
thetaBest = minFunc(@CosteLogReg, theta0, options, XExp, yTrain, bestLambda);

yPredTrainBest = (XExp * thetaBest) >= 0;
yPredTestBest = (XExpTest * thetaBest) >= 0;

% Reentrena con lambda = 0
theta0 = rand(size(XExp, 2), 1);
thetaL0 = minFunc(@CosteLogReg, theta0, options, XExp, yTrain, 0);

yPredTrainL0 = (XExp * thetaL0) >= 0;
yPredTestL0 = (XExpTest * thetaL0) >= 0;

% Superficie de separación entrenamiento
plot_wines(xDataTrain, yDataTrain, f1, f2);
plotDecisionBoundary(thetaBest, XTrain);

plot_wines(xDataTrain, yDataTrain, f1, f2);
plotDecisionBoundary(thetaL0, XTrain);

% Superficie de separación test
plot_wines(xDataTest, yDataTest, f1, f2);
plotDecisionBoundary(thetaBest, XTrain);

plot_wines(xDataTest, yDataTest, f1, f2);
plotDecisionBoundary(thetaL0, XTrain);

% Métricas de error
accuracyTrainBest = accuracy(yPredTrainBest, yTrain)
precisionTrainBest = precision(yPredTrainBest, yTrain)
recallTrainBest = recall(yPredTrainBest, yTrain)

accuracyTestBest = accuracy(yPredTestBest, yTest)
precisionTestBest = precision(yPredTestBest, yTest)
recallTestBest = recall(yPredTestBest, yTest)

accuracyTrainL0 = accuracy(yPredTrainL0, yTrain)
precisionTrainL0 = precision(yPredTrainL0, yTrain)
recallTrainL0 = recall(yPredTrainL0, yTrain)

accuracyTestL0 = accuracy(yPredTestL0, yTest)
precisionTestL0 = precision(yPredTestL0, yTest)
recallTestL0 = recall(yPredTestL0, yTest)

% Probabilidad de ser clase 1 si Atrib. 6 = 0.6
xTrain1Const = repmat(0.6, size(xTrain1, 1), 1);
XProb = expandir2(xTrain1Const, xTrain2, 6);
h = 1 ./ (1 + exp(- (XProb * thetaBest)));
graficaProbabilidad(xTrain2, h, yTrain);

% Entrena
theta0 = rand(size(XTrain, 2), 1);
theta = minFunc(@CosteLogistico, theta0, options, XTrain, yTrain);

% Obtiene predicciones con conjunto de entrenamiento
yPredTrain = (XTrain * theta) >= 0;

% Obtiene predicciones con conjunto de prueba
yPredTest = (XTest * theta) >= 0;

% Superficie de separación entrenamiento
plot_wines(xDataTrain, yDataTrain, f1, f2);
plotDecisionBoundary(theta, XTrain);

% Superficie de separación test
plot_wines(xDataTest, yDataTest, f1, f2);
plotDecisionBoundary(theta, XTest);

% Métricas de error
accuracyTrain = accuracy(yPredTrain, yTrain)
precisionTrain = precision(yPredTrain, yTrain)
recallTrain = recall(yPredTrain, yTrain)

accuracyTest = accuracy(yPredTest, yTest)
precisionTest = precision(yPredTest, yTest)
recallTest = recall(yPredTest, yTest)

% Probabilidad de ser clase 1 si Atrib. 6 = 0.6
xTrain1Const = repmat(0.6, size(xTrain1, 1), 1);
XProb = [ones(length(yDataTrain), 1) xTrain1Const, xTrain2];
h = 1 ./ (1 + exp(- (XProb * theta)));
graficaProbabilidad(xTrain2, h, yTrain);

clear; close all;

%% Carga de datos
load('MNISTdata2.mat');
rand('state', 0);
p = randperm(length(y));
X = X(p, :);
y = y(p);

%% Bayes ingénuo

tic
[lambda, trHist, cvHist] = kFoldBayesiano(X, y, 5, 1);
toc

% Entrenamiento
modelo = entrenarGaussianas(X, y, 10, 1, lambda);

yPred = clasificacionBayesiana(modelo, X);
yPredTest = clasificacionBayesiana(modelo, Xtest);

% Historial lambdas
graficaHistoria(trHist, cvHist);

% Accuracy
accuracyTrain = accuracy(yPred, y)
accuracyTest = accuracy(yPredTest, ytest)

% Matriz confusión
matrizConfusion(yPred, y);
matrizConfusion(yPredTest, ytest);

% Confusiones visualizadas
verConfusiones(X, y, yPred);
verConfusiones(Xtest, ytest, yPredTest);

%% Covarianzas completas

tic
[lambda, trHist, cvHist] = kFoldBayesiano(X, y, 5, 0);
toc

% Entrenamiento
modelo = entrenarGaussianas(X, y, 10, 0, lambda);

yPred = clasificacionBayesiana(modelo, X);
yPredTest = clasificacionBayesiana(modelo, Xtest);

% Historial lambdas
graficaHistoria(trHist, cvHist);

% Accuracy
accuracyTrain = accuracy(yPred, y)
accuracyTest = accuracy(yPredTest, ytest)

% Matriz confusión
matrizConfusion(yPred, y);
matrizConfusion(yPredTest, ytest);

% Confusiones visualizadas
verConfusiones(X, y, yPred);
verConfusiones(Xtest, ytest, yPredTest);

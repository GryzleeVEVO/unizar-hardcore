clear; close all;

%% Carga de datos

clase = 1; f1 = 6; f2 = 10;

options.useMex = 1;
options.display = 'none';
options.method = 'newton';

wine_train = load('wine_train.txt');

xDataTrain = wine_train(:, 2:end);
yDataTrain = wine_train(:, 1);

xTrain1 = xDataTrain(:, f1);
xTrain2 = xDataTrain(:, f2);

XTrain = [ones(length(yDataTrain), 1) xTrain1, xTrain2];
yTrain = yDataTrain == clase;

wine_test = load('wine_test.txt');

xDataTest = wine_test(:, 2:end);
yDataTest = wine_test(:, 1);
xTest1 = xDataTest(:, f1);
xTest2 = xDataTest(:, f2);

XTest = [ones(length(yDataTest), 1) xTest1, xTest2];
yTest = yDataTest == clase;

%% Regresión logística básica

run("P3LogBasica.m");

%% Regresión con regularización

run("P3Regularizar.m");

%% Curvas precision/recall

run("P3PrecisionRecall.m");

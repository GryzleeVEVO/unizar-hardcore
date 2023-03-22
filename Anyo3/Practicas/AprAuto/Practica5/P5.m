clear; close all;

%% Carga de datos
load('MNISTdata2.mat');
rand('state', 0);
p = randperm(length(y));
X = X(p, :);
y = y(p);

%% Modelos gaussianos regularizados

modelo = entrenarGaussianas(X, y, 10, 0, 0);
yhat = clasificacionBayesiana(modelo, X);


%% Bayes ingénuo


%% Covarianzas completas


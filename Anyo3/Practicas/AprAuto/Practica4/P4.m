clear; close all;

%% Carga de datos

options.useMex = 1;
options.display = 'none';
options.method = 'lbfgs';

load('MNISTdata2.mat');
rand('state', 0);
p = randperm(length(y));
X = X(p, :);
y = y(p);

%% Entrenamiento

% Añade una columna de unos
XTr = [ones(size(y, 1), 1) X];
XTestExp = [ones(size(ytest, 1), 1), Xtest];

[lambda, trainHist, cvHist] = entrenamientoMulticlase(XTr, y, 5, options);

graficaHistoria(trainHist, cvHist);

%% Modelo final

Theta = zeros(size(XTr, 2), 10);

% Obtiene un modelo para cada clase (one-vs-rest)
for C = 1:10
   % Etiqueta soluciones para el caso
    y_c = y;
    y_c(y_c ~= C) = 0;
    y_c(y_c == C) = 1;
    
    theta0 = zeros(size(XTr, 2), 1);

    % Calcula pesos
    theta = minFunc(@CosteLogReg, theta0, options, XTr, y_c, lambda);
    Theta(:, C) = theta;
end

% Obtiene predicciones
h = 1 ./ (1 + exp(- (XTr * Theta)));
hTest = 1 ./ (1 + exp(- (XTestExp * Theta)));

% Obtiene la mejor predicción por cada fila y su columna
[~, yPred] = max(h, [], 2);
[~, yPredTest] = max(hTest, [], 2);

accuracyTrain = accuracy(yPred, y)

accuracyTest = accuracy(yPredTest, ytest)

CurvaPrecisionRecall(hTest, ytest);

verConfusiones(X, y, yPred);
verConfusiones(Xtest, ytest, yPredTest);

close all;

%% Carga de datos
datosTrain = load('CochesTrain.txt');
datosTest = load('CochesTest.txt');

XTrain = datosTrain(:, 2:4); % Años/Km/CV
YTrain = datosTrain(:, 1); % Precio (€)

NTrain = length(YTrain);

XTest = datosTest(:, 2:4);
YTest = datosTest(:, 1);
NTest = length(YTest);

%% Búsqueda heurística

run('P2Heuristica.m');

%% Búsqueda exhaustiva

run('P2Exhaustiva.m');

%% Regulzarización

run('P2Regularizar.m');
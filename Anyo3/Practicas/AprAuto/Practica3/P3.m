

%% Carga de datos




clase = 1; f1 = 6; f2 = 10;

wine_train = load('wine_train.txt');

xDataTrain = wine_train(:, 2:end);
yDataTest = wine_train(:, 1);
xTrain1 = xDataTrain(:, f1); 
xTrain2 = xDataTest(:, f2);
yTrain = yDataTrain == clase;

wine_test = load('wine_test.txt');

xDataTest = wine_test(:, 2:end);
yDataTest = wine_test(:, 1);
xTest1 = xDataTest(:, f1);
xTest2 = xDataTest(:, f2);
yTest = yDataTest == clase;

%% Regresión logística básica

%% Regresión con regularización

%% Curvas precision/recall
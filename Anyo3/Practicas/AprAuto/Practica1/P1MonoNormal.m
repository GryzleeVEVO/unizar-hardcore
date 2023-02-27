% Aprendizaje Automático
% Práctica 1: Regresión
% Autor: Dorian Boleslaw Wozniak (817570@unizar.es)

%% Regresión monovariable, ecuación normal

% Obtiene atributos
X = [ones(N, 1), superficie];

tic;

% Obtiene pesos
theta = X \ precio;

toc

% Obtiene predicción
precioPredicho = X * theta;

graficaMonovariable(superficie, precio, theta, precioPredicho);

error = mean(abs(precioPredicho - precio) ./ precio)

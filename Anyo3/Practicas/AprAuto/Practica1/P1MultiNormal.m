% Aprendizaje Automático
% Práctica 1: Regresión
% Autor: Dorian Boleslaw Wozniak (817570@unizar.es)

%% Regresión multivariable, ecuación normal

X = [ones(N, 1), superficie, habitaciones];

tic

% Obtiene pesos
theta = X \ precio;

toc

% Obtiene predicción
precioPredicho = X * theta;

graficaMultivariable(superficie, habitaciones, precio, theta, precioPredicho);

error = mean(abs(precioPredicho - precio) ./ precio)

% Aprendizaje Automático
% Práctica 1: Regresión
% Autor: Dorian Boleslaw Wozniak (817570@unizar.es)

%% Regresión multivariable, descenso de gradiente con error cuadratico

X = [ones(N, 1), superficie, habitaciones];

alpha = 0.0029;

% Normalización
[XNorm, mu, sigma] = normalizar(X, N);

tic

% Obtiene pesos
[theta, thetaHist, iter] = descensoCuadratico(XNorm, precio, alpha);

toc

% Desnormalización
theta = desnormalizar(theta, mu, sigma);

% Obtiene predicción
precioPredicho = X * theta;

alpha
iter
error = mean(abs(precioPredicho - precio) ./ precio)

graficaMultivariable(superficie, habitaciones, precio, theta, precioPredicho);
histDescenso(thetaHist);

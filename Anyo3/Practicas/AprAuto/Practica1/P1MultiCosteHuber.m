% Aprendizaje Automático
% Práctica 1: Regresión
% Autor: Dorian Boleslaw Wozniak (817570@unizar.es)

%% Regresión robusta, coste de Huber

X = [ones(N, 1), superficie, habitaciones];

alpha = 1.35;
delta = 214.4;

% Normalización
[XNorm, mu, sigma] = normalizar(X, N);

tic

% Obtiene pesos
[theta, thetaHist, iter] = descensoHuber(XNorm, precio, alpha, delta);

toc

% Desnormalización
theta = desnormalizar(theta, mu, sigma);

% Obtiene predicción
precioPredicho = X * theta;

alpha
delta
iter
error = mean(abs(precioPredicho - precio) ./ precio)

graficaMultivariable(superficie, habitaciones, precio, theta, precioPredicho);
histDescenso(thetaHist);

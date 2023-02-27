% Aprendizaje Automático
% Práctica 1: Regresión
% Autor: Dorian Boleslaw Wozniak (817570@unizar.es)

%% Regresión monovariable, descenso de gradiente con error cuadratico

X = [ones(N, 1), superficie];

alpha = 0.0031;

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

graficaMonovariable(superficie, precio, theta, precioPredicho);
histDescenso(thetaHist);

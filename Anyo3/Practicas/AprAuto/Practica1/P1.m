close all;
%% Carga de datos
datosEntrenamiento = load('PisosTrain.txt');
datosValidacion = load('PisosTest.txt');

superficie = datosEntrenamiento(:,1);
habitaciones = datosEntrenamiento(:,2);
precio = datosEntrenamiento(:,3);
N = length(precio);

superficieTest = datosValidacion(:,1);
habitacionesTest = datosValidacion(:,2);
precioTest = datosValidacion(:,3);
NTest = length(precioTest);
%% Regresión monovariable, ecuación normal

X = [ones(N, 1), superficie];

% Normalización
%[XNorm, mu, sigma] = normalizar(X, N);

% Obtiene pesos
theta = XNorm \ precio;

% Desnormalización
%theta = desnormalizar(theta, mu, sigma);

% Obtiene predicción
precioPredicho = X * theta; 

%graficaMonovariable(superficie, precio, X, precioPredicho);

mean(abs(precioPredicho - precio)./precio)

%% Regresión multivariable, ecuación normal

X = [ones(N, 1), superficie, habitaciones];

% Normalización
[XNorm, mu, sigma] = normalizar(X, N);

% Obtiene pesos
theta = XNorm \ precio;

% Desnormalización
theta = desnormalizar(theta, mu, sigma);

% Obtiene predicción
precioPredicho = X * theta; 

%graficaMultivariable(superficie, habitaciones, precio, precioPredicho, theta);

mean(abs(precioPredicho - precio)./precio)

%% Regresión monovariable, descenso de gradiente

X = [ones(N, 1), superficie];

% Normalización
[XNorm, mu, sigma] = normalizar(X, N);

% Obtiene pesos
theta = descensoGradiente(XNorm, precio, 5 * 10^-3);

% Desnormalización
theta = desnormalizar(theta, mu, sigma);

% Obtiene predicción
precioPredicho = X * theta; 

%graficaMonovariable(superficie, precio, X, precioPredicho);

mean(abs(precioPredicho - precio)./precio)

%% Regresión multivariable, descenso de gradiente

X = [ones(N, 1), superficie, habitaciones];

% Normalización
[XNorm, mu, sigma] = normalizar(X, N);

% Obtiene pesos
theta = descensoGradiente(XNorm, precio, 3 * 10^-3);

% Desnormalización
theta = desnormalizar(theta, mu, sigma);

% Obtiene predicción
precioPredicho = X * theta; 

graficaMultivariable(superficie, habitaciones, precio, precioPredicho, theta);

mean(abs(precioPredicho - precio)./precio)

%% Regresión robusta, coste de Huber

X = [ones(N, 1), superficie, habitaciones];

% Normalización
[XNorm, mu, sigma] = normalizar(X, N);

% Obtiene pesos
theta = descensoHuber(XNorm, precio, 3.5, 5);

% Desnormalización
theta = desnormalizar(theta, mu, sigma);

% Obtiene predicción
precioPredicho = X * theta; 

%graficaMultivariable(superficie, habitaciones, precio, precioPredicho, theta);

mean(abs(precioPredicho - precio)./precio)






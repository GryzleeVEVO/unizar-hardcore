close all;
%% Carga de datos de entrenamiento y prueba
datos = load('PisosTrain.txt');
datosTest = load('PisosTest.txt');

superficie = datos(:,1);
habitaciones = datos(:,2);
precio = datos(:,3);
N = length(precio);

superficieTest = datosTest(:,1);
habitacionesTest = datosTest(:,2);
precioTest = datosTest(:,3);
NTest = length(precioTest);
%% Regresión monovariable - ecuación normal

% Entrenamiento
X = [ones(N,1) superficie];
theta = X \ precio;
precioEstimado = X * theta;

% Evaluación

% Gráfica
figure;
plot(superficie, precio, 'bx');
%plot(superficie, precioEstimado, 'cx');

title('Precio de los pisos')
xlabel('Superficie (m^2)'); ylabel('Precio (€)');
grid on; hold on;

plot(superficie, precioEstimado, 'r-'); % Dibujo la recta de predicción
legend('Datos Entrenamiento', 'Prediccion')



%% Regresión multivariable - ecuación normal

X = [ones(N,1) superficie habitaciones];
theta = X \ precio;
precioEstimado = X * theta;


%% Regresión monovariable - descenso de gradiente

%theta = descensoGradiente(X, Y, 0.1);

%% Regrasión multivariable - descenso de gradiente



%% Regresión robusta - descenso de gradiente + coste de Huber

%theta = descensoGradiente(X, Y, 0.1);

%% Definición de funciones

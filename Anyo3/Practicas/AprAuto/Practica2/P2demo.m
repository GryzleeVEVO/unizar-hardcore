close all;
%% Cargar los datos
datos = load('CochesTrain.txt');
ydatos = datos(:, 1); % Precio en Euros
Xdatos = datos(:, 2:4); % A�os, Km, CV
x1dibu = linspace(min(Xdatos(:, 1)), max(Xdatos(:, 1)), 100)'; %para dibujar

datos2 = load('CochesTest.txt');
ytest = datos2(:, 1); % Precio en Euros
Xtest = datos2(:, 2:4); % A�os, Km, CV
Ntest = length(ytest);

%% Dibujo de un Ajuste Parab�lico Monovariable

disp('********************Ajuste polinomico multivariable*******************');
grado = 10;
X = expandir(Xdatos, [grado 0 0]);
[Xcv, ycv, Xtr, ytr] = particion(1, 5, X, ydatos);
w = Xtr \ ytr; % Solucion con la Ecuaci�n Normal

figure;
grid on; hold on;
plot(Xtr(:, 2), ytr, 'bx');
plot(Xcv(:, 2), ycv, 'ro');
title(sprintf('Polinomio grado %d', grado));
ylabel('Precio Coches (Euros)'); xlabel('A�os');
Xd = expandir(x1dibu, grado);
plot(x1dibu, Xd * w, 'r-'); % Dibujo la recta de predicci�n
legend('Datos Entrenamiento', 'Datos validacion', 'Prediccion')

RMSEtr = RMSE(w, Xtr, ytr)
RMSEcv = RMSE(w, Xcv, ycv)

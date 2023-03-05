%% P3: Cargar y mostrar datos de clasificación de vinos

clear ; close all;

% Optiones para minFunc
options.useMex = 1 ;
options.display = 'none';
options.method = 'newton';

% Cargar y preparar datos
wine_train = load('wine_train.txt');
wine_test  = load('wine_test.txt');

clase = 1;
f1 = 6; f2 = 10; 
disp(sprintf('----- Clasificación de la clase %g con las features %g y %g ----', clase, f1, f2));

ydata = wine_train(:,1);
Xdata = wine_train(:,2:end);
x1 = Xdata(:,f1);
x2 = Xdata(:,f2);
y = ydata==clase;
X = [ones(length(y),1) x1 x2];

yt = wine_test(:,1);
Xt = wine_test(:,2:end);
xt1 = Xt(:,f1);
xt2 = Xt(:,f2);
ytest = yt==clase;
Xtest = [ones(length(ytest),1) xt1 xt2];

%% Ejemplo de dibujo de una solución mala 

theta = [0 1.1 -0.5]'; 
plot_wines(Xdata, ydata, f1, f2);
plotDecisionBoundary(theta, X);




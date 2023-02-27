% Aprendizaje Automático
% Práctica 1: Regresión
% Autor: Dorian Boleslaw Wozniak (817570@unizar.es)

function graficaMonovariable(x1, y, theta, yPred)
    %GRAFICAMONOVARIABLE Dibuja una gŕafica que representa un conjunto de datos y la recta de regresión obtenida
    figure;
    title('Precio de los pisos');
    grid on; hold on;

    plot(x1, y, 'bx');
    ylabel('Euros'); xlabel('Superficie (m^2)');

    plot(x1, yPred, 'r-');
    legend('Datos Entrenamiento', ['Predicción (ŷ = ' num2str(theta(1)), ...
                   ' + ', num2str(theta(2)), 'x_1)']);
end

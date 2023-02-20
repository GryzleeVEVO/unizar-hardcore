function graficaMonovariable(x1, y, X, yPred)
    %GRAFICAMONOVARIABLE
    figure;
    title('Precio de los pisos');
    grid on; hold on;

    plot(x1, y, 'bx');
    ylabel('Euros'); xlabel('Superficie (m^2)');

    plot(X(:, 2), yPred, 'r-');
    legend('Datos Entrenamiento', 'Predicción')
end

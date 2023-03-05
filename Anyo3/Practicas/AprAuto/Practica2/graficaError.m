function graficaError(historyTrain, historyTest, atributo, k)
    %GRAFICAERROR Curva de evolución de errores para el algoritmo 

    figure;
    title(sprintf('Evolución RMSE: Atributo %s, k = %d', atributo, k));
    grid on; hold on;

    plot(historyTrain);
    plot(historyTest);

    ylabel('RMSE'); xlabel('Grado atributo');
    legend('Error de los datos de entrenamiento', 'Error de los datos de validación');
end

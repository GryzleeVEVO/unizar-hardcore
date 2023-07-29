function graficaF1(historyTrain, historyTest)
    %GRAFICAF1 Dibuja la gráfica con la evolución de f1 de test y validación para lambda

    figure;
    title('Evolución f1');
    semilogx(historyTrain(:,1), historyTrain(:,4));
    grid on; hold on;
    semilogx(historyTest(:,1), historyTest(:,4));
    ylabel('f1'); xlabel('Lambda');
    legend('Datos de entrenamiento', 'Datos de validación');
end

function graficaHistoria(historyTrain, historyTest)
    %GRAFICAHISTORIA Dibuja la gráfica con la evolución de f1 de test y validación para lambda

    figure;
    title('Evolución accuracy');
    semilogx(historyTrain(:,1), historyTrain(:,2));
    grid on; hold on;
    semilogx(historyTest(:,1), historyTest(:,2));
    ylabel('accuracy'); xlabel('Lambda');
    legend('Datos de entrenamiento', 'Datos de validación');
end

function graficaErrorReg(historyTrain, historyTest)
    %GRAFICAERROR 

    figure;
    title('Evolución RMSE: Valor lambda');
    semilogx(historyTrain(:,1), historyTrain(:,2));
    grid on; hold on;
    semilogx(historyTest(:,1), historyTest(:,2));
    ylabel('RMSE'); xlabel('Lambda');
    legend('Error de los datos de entrenamiento', 'Error de los datos de validación');
end

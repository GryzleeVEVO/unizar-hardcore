% Aprendizaje Automático
% Práctica 1: Regresión
% Autor: Dorian Boleslaw Wozniak (817570@unizar.es)

function histDescenso(thetaHist)
    %HISTDESCENSO Dibuja el historial de valores de cada theta a lo largo de un descenso de gradiente
    figure;
    title('Historial de descenso de gradiente en funcion de theta');
    grid on; hold on;

    for i = 1:size(thetaHist, 2)
        plot(thetaHist(:, i));
    end

end

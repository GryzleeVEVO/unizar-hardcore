function [p] = precision(yPred, y)
    %PRECISION Devuelve la precisón entre la preddicción y el resultado esperado
    tp = sum((yPred == 1) & (y == 1)); % Positivos verdaderos
    fp = sum((yPred == 1) & (y == 0)); % Falsos positivos

    p = tp / (tp + fp);
end

function [r] = recall(yPred, y)
    %RECALL Devuelve el recall entre la preddicción y el resultado esperado
    tp = sum((yPred == 1) & (y == 1)); % Positivos verdaderos
    fn = sum((yPred == 0) & (y == 1)); % Falsos negativos

    r = tp / (tp + fn);
end

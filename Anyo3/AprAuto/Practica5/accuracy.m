function [score] = accuracy(yPred, y)
    %ACCURACY Calcula la proporción de muestras clasificadas correctamente sobre el total
    score = sum(yPred == y) / size(y, 1);
end

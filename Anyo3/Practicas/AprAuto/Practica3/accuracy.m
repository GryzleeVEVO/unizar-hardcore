function [score] = accuracy(yPred, y)
    %ACCURACY Summary of this function goes here
    %   Detailed explanation goes here
    tp = sum((yPred == 1) & (y == 1)); % Positivos verdaderos
    tn = sum((yPred == 0) & (y == 0)); % Positivos falsoss

    score = (tp + tn) / (size(y, 1));
end

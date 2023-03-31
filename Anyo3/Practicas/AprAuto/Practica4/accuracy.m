function [score] = accuracy(yPred, y)
    %ACCURACY Summary of this function goes here

    score = sum(yPred == y) / size(y, 1);
end

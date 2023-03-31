function [score] = f1Score(yPred, y)
    %F1SCORE Devuelve el f1-score entre la preddicción y el resultado esperado

    p = precision(yPred, y);
    r = recall(yPred, y);
    score = 2 * p * r / (p + r);
end

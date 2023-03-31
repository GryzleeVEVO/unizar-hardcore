pr1 = []; pr2 = []; pr3 = [];

% Genera curva precision-recall
for i = 0:0.01:1
    hP1 = 1 ./ (1 + exp(- (XTest * theta))) >= i;
    hP2 = 1 ./ (1 + exp(- (XExpTest * thetaBest))) >= i;
    hP3 = 1 ./ (1 + exp(- (XExpTest * thetaL0))) >= i;

    pr1 = cat(1, pr1, [recall(hP1, yTest), precision(hP1, yTest)]);
    pr2 = cat(1, pr2, [recall(hP2, yTest), precision(hP2, yTest)]);
    pr3 = cat(1, pr3, [recall(hP3, yTest), precision(hP3, yTest)]);
end

CurvaPrecisionRecall(pr1, pr2, pr3);

function [bestLambda, trainHist, cvHist] = kFoldBayesiano(X, y, numParticiones, naiveBayes)
    %KFOLDBAYESIANO Entrenamiento mediante k-fold para una clasificación bayesiana con regularización

    bestLambda = 0;
    bestError = -Inf;

    trainHist = [];
    cvHist = [];

    nc = 10;

    % Para cada lambda
    for lambda = logspace(-9, 2, 30)
        accuracyTrain = 0; accuracyCV = 0;

        % Repite con 5 particiones
        for fold = 1:numParticiones
            [XCv, yCv, XTr, yTr] = particion(fold, numParticiones, X, y);

            % Obtiene un modelo gaussiano para cada clase
            modelo = entrenarGaussianas(XTr, yTr, nc, naiveBayes, lambda);

            % Obtiene la predicción
            yPred = clasificacionBayesiana(modelo, XTr);
            yPredCv = clasificacionBayesiana(modelo, XCv);

            % Calcula métricas
            accuracyTrain = accuracyTrain + accuracy(yPred, yTr);
            accuracyCV = accuracyCV + accuracy(yPredCv, yCv);
        end

        % Calcula media de métricas
        accuracyTrain = accuracyTrain / numParticiones;
        accuracyCV = accuracyCV / numParticiones;

        trainHist = cat(1, trainHist, [lambda, accuracyTrain]);
        cvHist = cat(1, cvHist, [lambda, accuracyCV]);

        % Comprueba si el modelo es mejor
        if (accuracyCV > bestError)
            bestLambda = lambda;
            bestError = accuracyCV;
        end

    end

end

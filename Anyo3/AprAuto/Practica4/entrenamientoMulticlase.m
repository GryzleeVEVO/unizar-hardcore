function [bestLambda, trainHist, cvHist] = entrenamientoMulticlase(X, y, numParticiones, options)
    %ENTRENAMIENTOMULTICLASE

    bestLambda = 0;
    bestError = -Inf;

    trainHist = [];
    cvHist = [];

    % Para cada lambda
    for lambda = logspace(-9, 2, 30)
        accuracyTrain = 0; accuracyCV = 0;

        % Repite con 5 particiones
        for fold = 1:numParticiones
            [XCv, yCv, XTr, yTr] = particion(fold, numParticiones, X, y);

            Theta = zeros(size(XTr, 2), 10);

            % Obtiene un modelo para cada clase (one-vs-rest)
            for C = 1:10
                % Etiqueta soluciones para el caso
                yTr_c = yTr;
                yTr_c(yTr_c ~= C) = 0;
                yTr_c(yTr_c == C) = 1;

                theta0 = zeros(size(XTr, 2), 1);

                % Calcula pesos
                theta = minFunc(@CosteLogReg, theta0, options, XTr, yTr_c, lambda);
                Theta(:, C) = theta;
            end

            h = 1 ./ (1 + exp(- (XTr * Theta)));
            hCv = 1 ./ (1 + exp(- (XCv * Theta)));

            % Obtiene la mejor predicción por cada fila y su columna
            [~, yPred] = max(h, [], 2);
            [~, yPredCv] = max(hCv, [], 2);

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

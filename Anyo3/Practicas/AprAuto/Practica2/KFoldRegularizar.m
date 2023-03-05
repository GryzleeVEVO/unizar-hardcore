function [bestLambda, errorTrainHistory, errorValidHistory] = KFoldRegularizar(X, y, numParticiones)
    %KFOLDHEURISTICA

    % Modelo seleccionado por el algoritmo (grado/atributo)
    bestLambda = 0;

    % Mejor RMSE medio obtenido
    bestError = Inf;

    % Historial de evolución de errores para cada atributo
    errorTrainHistory = [];
    errorValidHistory = [];

    % Prueba para cada atributo en orden
    XExp = expandir(X, [10, 10, 10]);

    for lambda = logspace(-8, 2, 50)
        % Expande los atributos con el modelo a probar

        errorTrain = 0;
        errorValid = 0;

        % Bucle interno prueba con varios folds de los datos
        for fold = 1:numParticiones
            % Obtiene las particiones
            [XCv, yCv, XTr, yTr] = particion(fold, numParticiones, XExp, y);

            % Normaliza
            [XTrNorm, mu, sig] = normalizar(XTr);

            H = XTrNorm' * XTrNorm + lambda * diag([0 ones(1, size(XTr, 2)-1)]);

            % Entrena el modelo
            thetaNorm = H \ (XTrNorm' * yTr);

            % Desnormaliza
            theta = desnormalizar(thetaNorm, mu, sig);

            %  Obtiene predicciones
            yPred = XTr * theta;
            yPredCv = XCv * theta;

            % Calcula errores
            errorTrain = errorTrain + rmse(yPred, yTr);
            errorValid = errorValid + rmse(yPredCv, yCv);
        end

        % Calcula error medio de los folds
        errorTrain = errorTrain / numParticiones;
        errorValid = errorValid / numParticiones;

        errorTrainHistory = cat(1, errorTrainHistory, [lambda, errorTrain]);
        errorValidHistory = cat(1, errorValidHistory, [lambda, errorValid]);

        if (errorValid < bestError)
            bestLambda = lambda;
            bestError = errorValid;
        end
    end

end

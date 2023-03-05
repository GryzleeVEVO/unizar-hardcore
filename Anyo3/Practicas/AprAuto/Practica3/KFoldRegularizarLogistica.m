function [bestLambda, errorTrainHistory, errorValidHistory] = KFoldRegularizarLogistica(X, y, numParticiones, options)
    %

    % Mejor parámetro regularización
    bestLambda = 0;

    % Mejor RMSE medio obtenido
    bestError = Inf;

    % Historial de evolución de errores para cada atributo
    % errorTrainHistory = []; errorValidHistory = [];

    % Prueba para cada atributo en orden
    %XExp = expandir(X, [10, 10, 10]);

    for lambda = logspace(-8, 2, 50)
        % Expande los atributos con el modelo a probar

        errorTrain = 0;
        errorValid = 0;

        % Bucle interno prueba con varios folds de los datos
        for fold = 1:numParticiones
            % Obtiene las particiones
            [XCv, yCv, XTr, yTr] = particion(fold, numParticiones, XExp, y);
            
            % Realiza predicción
            theta0 = rand(y);
            theta = minFunc(@CosteLogReg, theta0, options, xTr, yTr, lambda);

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

        %errorTrainHistory = cat(1, errorTrainHistory, [lambda, errorTrain]);
        %errorValidHistory = cat(1, errorValidHistory, [lambda, errorValid]);

        if (errorValid < bestError)
            bestLambda = lambda;
            bestError = errorValid;
        end
    end

end

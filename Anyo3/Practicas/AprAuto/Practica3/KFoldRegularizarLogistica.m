function [bestLambda, trainHistory, cvHistory] = KFoldRegularizarLogistica(X, y, numParticiones, options)
    %KFOLDREGULARIZARHEURISTICA Algoritmo k-fold que devuelve una lambda para una regresión logística regularizada

    % Mejor parámetro regularización
    bestLambda = 0;

    % Mejor RMSE medio obtenido
    bestError = -Inf;

    % Historial de evolución de errores para cada atributo
    trainHistory = []; cvHistory = [];

    for lambda = logspace(-8, 2, 50)
        % Expande los atributos con el modelo a probar
        precisionTrain = 0; precisionCV = 0;
        recallTrain = 0; recallCV = 0;
        f1Train = 0; f1CV = 0;

        % Bucle interno prueba con varios folds de los datos
        for fold = 1:numParticiones
            % Obtiene las particiones
            [XCv, yCv, XTr, yTr] = particion(fold, numParticiones, X, y);

            % Realiza predicción
            theta0 = rand(size(XTr, 2), 1);
            theta = minFunc(@CosteLogReg, theta0, options, XTr, yTr, lambda);

            %  Obtiene predicciones
            yPred = (XTr * theta) >= 0;
            yPredCv = (XCv * theta) >= 0;
            
            % Calcula precision/recall y f1
            precisionTrain = precisionTrain + precision(yPred, yTr);
            recallTrain = recallTrain + recall(yPred, yTr);
            f1Train = f1Train + f1Score(yPred, yTr);

            precisionCV = precisionCV + precision(yPredCv, yCv);
            recallCV = recallCV + recall(yPredCv, yCv);
            f1CV = f1CV + f1Score(yPredCv, yCv);

        end

        % Calcula error medio de los folds
        precisionTrain = precisionTrain / numParticiones;
        recallTrain = recallTrain / numParticiones;
        f1Train = f1Train / numParticiones;

        precisionCV = precisionCV / numParticiones;
        recallCV = recallCV / numParticiones;
        f1CV = f1CV / numParticiones;

        trainHistory = cat(1, trainHistory, [lambda, precisionTrain, recallTrain, f1Train]);
        cvHistory = cat(1, cvHistory, [lambda, precisionCV, recallCV, f1CV]);

        % Actualiza lambda si f1 mejor
        if (f1CV > bestError)
            bestLambda = lambda;
            bestError = f1CV;
        end

    end

end

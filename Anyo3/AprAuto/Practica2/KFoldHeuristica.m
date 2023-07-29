function [bestModel, errorTrainHistory, errorValidHistory] = KFoldHeuristica(X, y, numParticiones)
    %KFOLDHEURISTICA Algoritmo K-Fold para buscar el mejor modelo mediante búsqueda heurística

    % Modelo seleccionado por el algoritmo (grado/atributo)
    bestModel = ones([1, size(X, 2)]);

    % Mejor RMSE medio obtenido
    bestError = Inf;

    % Historial de evolución dde errores para cada atributo
    errorTrainHistory = zeros(size(X, 2), 10);
    errorValidHistory = zeros(size(X, 2), 10);

    % Prueba para cada atributo en orden
    for atributo = 1:size(X, 2)
        % Obtiene el mejor modelo hasta ahora
        modelo = bestModel;

        % Bucle externo evalúa los modelos
        for grado = 1:10
            modelo(atributo) = grado;

            % Expande los atributos con el modelo a probar
            XExp = expandir(X, modelo);

            errorTrain = 0;
            errorValid = 0;

            % Bucle interno prueba con varios folds de los datos
            for fold = 1:numParticiones
                % Obtiene las particiones
                [XCv, yCv, XTr, yTr] = particion(fold, numParticiones, XExp, y);

                % Normaliza
                [XTrNorm, mu, sig] = normalizar(XTr);

                % Entrena el modelo
                thetaNorm = XTrNorm \ yTr;

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

            errorTrainHistory(atributo, grado) = errorTrain;
            errorValidHistory(atributo, grado) = errorValid;

            if (errorValid < bestError)
                bestModel = modelo;
                bestError = errorValid;
            end

        end

    end

end

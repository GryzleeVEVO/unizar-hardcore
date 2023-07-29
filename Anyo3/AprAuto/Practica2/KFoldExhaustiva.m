function [bestModel, bestError] = KFoldExhaustiva(X, y, numParticiones)
    %KFOLDHEURISTICA Algoritmo K-Fold para buscar el mejor modelo mediante búsqueda exhaustiva

    % Modelo seleccionado por el algoritmo (grado/atributo)
    bestModel = ones([1, size(X, 2)]);

    % Mejor RMSE medio obtenido
    bestError = Inf;

    % Prueba todas las combinaciones de atributos

    for atrib1 = 1:10

        for atrib2 = 1:10

            for atrib3 = 1:10
                modelo = [atrib1, atrib2, atrib3];

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

                if (errorValid < bestError)
                    bestModel = modelo;
                    bestError = errorValid;
                end

            end

        end

    end

end

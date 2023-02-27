function [bestModel, bestError] = KFoldHeuristica(X, y, numParticiones)
    %KFOLDHEURISTICA Summary of this function goes here

    % Modelo seleccionado por el algoritmo (grado/atributo)
    bestModel = ones(size(X, 2))

    % Mejor RMSE medio obtenido
    bestError = Inf;

    % Prueba para cada atributo en orden
    for atributo = size(X, 2)
        % Obtiene el mejor modelo hasta ahora
        modelo = bestModel;
        
        % Bucle externo evalúa los modelos
        for grado = 1:10
            modelo(atributo) = grado;

            % Expande los atributos con el modelo a probar
            XExp = expandir(X, modelo);
        
            % Bucle interno prueba con varios folds de los datos
            for fold = 1:numParticiones
                % Obtiene las particiones
                [XCv, yCv, XTr, yTr] = particion(fold, numParticiones, XExp, y);
        
                % Entrena el modelo y obtiene predicciones
                theta = XTr \ yTr;
                yPred = theta * XTr;

                % Calcula errores
                errorTrain = errorTrain + RMSE();
                errorValid = errorValid + RMSE();
            end

            % Calcula error medio de los folds
            errorTrain = errorTrain / k;
            errorValid = errorValid / k;

            if (errorValid < bestError)
                bestModel = model;
                bestError = errorValid;
            end
        end
    end
end


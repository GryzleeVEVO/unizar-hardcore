function yhat = clasificacionBayesiana(modelo, X)
    % Con los modelos entrenados, predice la clase para cada muestra X

    nc = size(modelo, 2);
    N = size(X, 1);

    % Matriz de predicciones para cada atributo, para cada clase
    p = zeros(size(X, 1), nc);

    % Obtiene predicciones para cada clase
    for i = 1:nc
        % Obtiene las probabilidades (convertidas a logaritmos)
        pX_y = gaussLog(modelo(i).mu, modelo(i).Sigma, X);
        py = log(modelo(i).N / N);

        % Obtiene la predicción
        p(:, i) = pX_y + py;
    end

    % Selecciona las clases con las mejores predicciones
    [~, yhat] = max(p, [], 2);

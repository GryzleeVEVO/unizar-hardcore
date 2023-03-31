function yhat = clasificacionBayesiana(modelo, X)
% Con los modelos entrenados, predice la clase para cada muestra X

nc = size(modelo, 2);
N = size(X, 1);

% Matriz de predicciones para cada atributo, para cada clased
p = zeros(size(X, 1), nc);

% Obtiene predicciones para cada clase
for i = nc
    % Obtiene las probabilidades (convertidas a logaritmos) 
    pX_y = gaussLog(modelo(nc).mu, modelo(nc).Sigma, X);
    py = log(modelo(nc).N / N);

    % Obtiene la predicción  
    p(:, nc) = pX_y + py;
end

% Selecciona las clases con las mejores predicciones
[~, yhat] = max(p_clase, [], 2);
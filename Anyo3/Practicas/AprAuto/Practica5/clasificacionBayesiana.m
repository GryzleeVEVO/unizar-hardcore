function yhat = clasificacionBayesiana(modelo, X)
% Con los modelos entrenados, predice la clase para cada muestra X

nc = size(modelo, 2); 
p = zeros(size(X, 1), nc);


% Obtiene predicciones para cada clase
for i = nc
    p_clase = -(1/2 * log(abs(modelo(i).Sigma))) ...
        - (1/2 * (X - modelo(i).mu)' * (modelo(i).Sigma^-1) * (X - modelo(i).mu)) ...
        + ln (modelo(i).N );

    p(:, nc) = p_clase;
end

[~, yhat] = max(p_clase, [], 2);
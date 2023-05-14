function c = updateClusters(D, mu)
    % D(m,n), m datapoints, n dimensions
    % mu(K,n) final centroids
    %
    % c(m) assignment of each datapoint to a class
    %
    % Asigna un cluster a cada dato respecto a la distancia con los
    % centroides de estos

    J = zeros(size(D, 1), size(mu, 1));

    % Obtiene distancias de los datos para cada centroide
    for k = 1:size(mu, 1)
        J(:, k) = vecnorm(D - mu(k, :), 2, 2);
    end

    % Obtiene el índice del centroide a menor distancia de cada dato
    [~, c] = min(J, [], 2);

end

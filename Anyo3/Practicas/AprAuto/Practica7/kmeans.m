function [mu, c] = kmeans(D, mu0)
    % D(m,n), m datapoints, n dimensions
    % mu0(K,n) K initial centroids
    %
    % mu(K,n) final centroids
    % c(m) assignment of each datapoint to a class

    mu = mu0;
    c0 = zeros(size(D, 1), 1);
    c = c0;

    for i = 1:100

        % Actualiza centroides y cluters
        c = updateClusters(D, mu);
        mu = updateCentroids(D, c, size(mu, 1));

        % Si los clusters han cambiado, parar
        if isequal(c, c0)
            break;
        else
            c0 = c;
        end

    end

end

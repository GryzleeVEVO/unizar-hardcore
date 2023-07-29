function munew = updateCentroids(D, c, K)
    % D((m,n), m datapoints, n dimensions
    % c(m) assignment of each datapoint to a class
    %
    % munew(K,n) new centroids
    %
    % Escoge el centroide que esté lo mas cerca de media a
    % los miembros de su cluster

    munew = zeros(K, size(D, 2));

    % Calcula el valor medio de cada componente de los datos del cluster
    for k = 1:K
        munew(k, :) = mean(D(c == k, :), 1);
    end

end

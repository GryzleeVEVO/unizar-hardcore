function J = distorsion(D, c, mu)
    % Dado un conjunto de datos D, las agrupaciones a las que pertenece
    % cada dato c, y los valores medios de cada agrupación, calcula el
    % valor de la función de distorsión

    m = size(D, 1);
    J = 0;


    for i=1:m
        J = J + vecnorm(D(i, :) - mu(c(i)));
    end

    J = 1/m * J;
end


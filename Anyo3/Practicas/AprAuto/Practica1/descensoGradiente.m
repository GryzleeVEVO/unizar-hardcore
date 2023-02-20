function [theta] = descensoGradiente(X, Y, alpha)
    %DESCENSOGRADIENTE

    iter = 10 ^ 3; tol = 10 ^ -12;

    % Se inicializan los pesos de forma aleatoria
    theta = rand(size(X, 2), 1);

    for i = 1:iter
        % Calcula el gradiente dados los pesos
        grad = X' * (X * theta - Y);

        % Si el gradiente converge o está lo suficientemene cerca, para
        if abs(grad) < tol; break; end

        % Si el gradiente ha aumentado, alpha es demasiado grande
        if (i ~= 1); if(gradPrevio < grad); break; end; end

        % Actualiza pesos restandole el gradiente
        theta = theta - alpha * grad;
        gradPrevio = grad;
    end

end

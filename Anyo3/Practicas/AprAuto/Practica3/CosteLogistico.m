function [J, grad, Hess] = CosteLogistico(theta, X, y)
    N = size(X,2);

    % Hipótesis
    h = 1 ./ (1 + exp(-X* theta)); 

    % Función coste (máxima verisimilitud)
    J = (-y' * log(h) - (1-y') * log(1-h))/N; 
    
    if nargout > 1
        % Gradiente
        grad = X' * (h-y) / N;
    end

    if nargout > 2
        % Hessiano
        R = diag(h.*(1-h));
        Hess = X' * R*X/N;
    end
end


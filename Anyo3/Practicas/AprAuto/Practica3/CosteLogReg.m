function [J, grad, Hess] = CosteLogReg(theta, X, y, lambda)
    [N ,D] = size(X);

    % Hipótesis
    h = 1 ./ (1 + exp(-X* theta)); 

    % th(0) no se penaliza
    th = theta; th(1) = 0;
    
    % Función coste (máxima verisimilitud)
    J = (-y' * log(h) - (1-y') * log(1-h))/N + (lambda/2) * (th' * th); 
    
    if nargout > 1
        % Gradiente
        grad = X' * (h-y) / N + lambda * th;
    end

    if nargout > 2
        % Hessiano
        R = diag(h.*(1-h));
        Hess = X' * R*X/N + diag([0 lambda * ones(1, D-1)]);
    end
end


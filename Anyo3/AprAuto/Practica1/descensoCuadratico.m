% Aprendizaje Automático
% Práctica 1: Regresión
% Autor: Dorian Boleslaw Wozniak (817570@unizar.es)

function [theta, thetaHist, i] = descensoCuadratico(X, Y, alpha)
    %DESCENSOGRADIENTE Algoritmo de descenso de gradiente para la función de coste cuardrática

    iter = 10 ^ 3; tol = 10 ^ -3;

    % Se inicializan los pesos de forma aleatoria
    theta = rand(size(X, 2), 1);

    if nargout > 1
        thetaHist = theta';
    end

    for i = 1:iter
        % Calcula el gradiente dados los pesos
        grad = X' * (X * theta - Y);

        % Para si el gradiente aumenta o si el cambio es pequeño
        if (i ~= 1)
            %if (gradPrevio > grad)
            %    break;
            %end
            if (abs(gradPrevio - grad) < tol)
                break;
            end

        end

        % Actualiza pesos restandole el gradiente
        theta = theta - alpha * grad;
        gradPrevio = grad;

        if nargout > 1
            thetaHist = cat(1, thetaHist, theta');
        end

    end

end

% Aprendizaje Automático
% Práctica 1: Regresión
% Autor: Dorian Boleslaw Wozniak (817570@unizar.es)

function [theta, thetaHist, i] = descensoHuber(X, Y, alpha, delta)
    %DESCENSOHUBER Algoritmo de descenso de gradiente para la función de coste de Huber

    iter = 10 ^ 3; tol = 10 ^ -3;

    % Se inicializan los pesos de forma aleatoria
    theta = rand(size(X, 2), 1);

    if nargout > 1
        thetaHist = theta';
    end

    for i = 1:iter
        r = X * theta - Y;
        good = abs(r) <= delta;

        % Calcula el gradiente dados los pesos
        grad = X(good, :)' * r(good) + delta * X(~good, :)' * sign(r(~good));

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

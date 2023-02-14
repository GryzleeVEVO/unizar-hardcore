function [theta] = descensoGradiente(X, Y, alpha)
    %DESCENSOGRADIENTE Algoritmo de descenso de gradiente en lotes
    %   Devuelve, a partir de una muestra de entrenamiento formada por una
    %   matriz de atributos X y un vector de salidas esperadas Y, y un factor
    %   de aprendizaje alpha, devuelve una serie de pesos que mejor se ajustan
    %   a los datos

    % Tamaño de los datos de entrenamiento (se asumen correctos)
    N = size(X, 1);

    % Media y desviación tipica de los atributos (salvo la primera columna)
    mu = mean(X(:, 2:end));
    sigma = std(X(:, 2:end));

    % Normalización (x'_i = (x_i - µ_i) / s_i)
    X(:, 2:end) = (X(:, 2:end) - repmat(mu, N, 1)) ./ repmat(sigma, N, 1);

    % ---
    % Iteraciones y tolerancia de solución
    iter = 100;
    tol = 0.01;

    % Se inicializan los pesos a un valor aleatorio entre 0 y 1
    theta = rand(N);

    % Realiza el descenso de gradiente
    for i = 1:iter
        % Calcula gradiente
        grad = X' * (X * theta - y);
        
        % Una vez el gradiente sea menor a la tolerancia, se admite
        % la solución
        if abs(grad) < tol; break; end

        % Actualiza pesos
        theta = theta - alpha * grad;
    end
    % ---

    % Desnormalización
    theta(2:end) = theta(2:end) ./ sigma;
    theta(1) = theta(1) - (mu * theta(2:end));

end

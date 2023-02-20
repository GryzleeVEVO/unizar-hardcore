function [XNorm, mu, sigma] = normalizar(X, N)
    %NORMALIZAR
    mu = mean(X(:, 2:end));
    sigma = std(X(:, 2:end));

    XNorm = X;
    XNorm(:, 2:end) = (X(:, 2:end) - repmat(mu, N, 1)) ./ repmat(sigma, N, 1);
end

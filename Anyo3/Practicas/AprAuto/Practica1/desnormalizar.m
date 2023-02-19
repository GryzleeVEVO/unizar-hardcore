function theta = desnormalizar(theta, mu, sigma)
    %DESNORMALIZAR

    theta(2:end) = theta(2:end) ./ sigma';
    theta(1) = theta(1) - (mu * theta(2:end));
end

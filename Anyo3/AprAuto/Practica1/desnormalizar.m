% Aprendizaje Automático
% Práctica 1: Regresión
% Autor: Dorian Boleslaw Wozniak (817570@unizar.es)

function theta = desnormalizar(theta, mu, sigma)
    %DESNORMALIZAR Desnormalizar los pesos calculados a partir de un conjunto de atributos normalizados

    theta(2:end) = theta(2:end) ./ sigma';
    theta(1) = theta(1) - (mu * theta(2:end));
end

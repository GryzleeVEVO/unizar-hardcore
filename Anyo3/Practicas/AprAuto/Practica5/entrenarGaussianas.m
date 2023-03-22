function modelo = entrenarGaussianas( Xtr, ytr, nc, NaiveBayes, landa )
% Entrena una Gaussana para cada clase y devuelve:
% modelo{i}.N     : Numero de muestras de la clase i
% modelo{i}.mu    : Media de la clase i
% modelo{i}.Sigma : Covarianza de la clase i
% Si NaiveBayes = 1, las matrices de Covarianza serán diagonales
% Se regularizarán las covarianzas mediante: Sigma = Sigma + landa*eye(D)

modelo = struct('N', {}, 'mu', {}, 'Sigma', {});

for clase = 1:nc
    % Obtiene el número de muestras para la clase
    N_clase = sum(ytr(:,1) == clase);

    % Obtiene las muestras de la clase seleccionada
    XTr_clase = Xtr((ytr == clase), :);

    % Media
    mu_clase = mean(XTr_clase, 2);

    % Covarianza
    Sigma_clase = cov(XTr_clase);

    if NaiveBayes 
        % Devuelve la covarianza diagonal si se usa Bayes ingénuo
        Sigma_clase = diag(diag(Sigma_clase));
    end

    % Aplica regularización al modelo
    Sigma_clase = Sigma_clase + landa * eye(size(Xtr, 2));

    % Guarda los datos
    modelo(clase).N = N_clase;
    modelo(clase).mu = mu_clase;
    modelo(clase).Sigma = Sigma_clase;
end
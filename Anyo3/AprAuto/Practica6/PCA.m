function [z, k, U] = PCA(X, k)
    % Obtiene la covarianza
    S = cov(X);

    % Obtiene vector/valores propios
    [U, L] = eig(S);

    % Reordena vector propio usando valores propios
    [l, ind] = sort(diag(L), 'descend');
    U_ord = U(:, ind);

    % Obtiene un valor k

    sum_eigval = sum(l);

    if nargin <= 1

        for k = 1:400
            sum_eigval_k = sum(l(1:k));

            if ((sum_eigval_k / sum_eigval) > 0.99)
                break;
            end

        end

    end

    z = X * U_ord(:, 1:k);
    U = U_ord;
end

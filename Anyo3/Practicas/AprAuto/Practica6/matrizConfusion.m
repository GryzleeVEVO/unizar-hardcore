function confusion = matrizConfusion(yPred, y)
    %MATRIZCONFUSION Summary of this function goes here
    confusion = zeros(10);

    % Obtiene
    for claseReal = 1:10
        yHat_clase = yPred(y == claseReal, :);

        for clasePredicha = min(y):max(y)
            clasificados = yHat_clase;
            clasificados(yHat_clase == clasePredicha, :) = 1;
            clasificados(yHat_clase ~= clasePredicha, :) = 0;

            confusion(claseReal, clasePredicha) = sum(clasificados);
        end

    end

    %confusion = cat(1, confusion, sum(confusion, 1));
    %confusion = cat(2, confusion, sum(confusion, 2));

    figure;

    x = repmat(1:10, 10, 1);
    y = x';
    t = num2cell(confusion);
    t = cellfun(@num2str, t, 'UniformOutput', false); % convert to string

    imagesc(confusion);
    text(x(:), y(:), t, 'HorizontalAlignment', 'Center')
    ylabel("Clase real"); xlabel("Clase Predicha");

end

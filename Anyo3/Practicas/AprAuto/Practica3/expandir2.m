function X = expandir2(x1, x2, grado)
% Expansión polinómica de dos atributos, con sus productos cruzados.
%   Devuelve una matriz de atributos con: 
%   1, x1, x2, x1.^2, x2.^2, x1*x2, x1.^3, x1.^2*x2, etc..
%   Ya añade la columna de unos

X = ones(size(x1(:,1)));
for i = 1:grado
    for j = 0:i
        X(:, end+1) = (x1.^(i-j)).*(x2.^j);
    end
end

end

% Calcula el coste cuadrático, gradiente y Hessiano de una entrada
function [J ,grad, Hess] = CosteL2(theta, X, y)
r = X*theta - y;
J = (1/2)*sum(r.^2);
if nargout > 1; grad = X'*r; end
if nargout > 2; Hess = X'*X; end


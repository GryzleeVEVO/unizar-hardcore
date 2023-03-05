function plotDecisionBoundary(theta, X)
% Dibuja la superficie de separación definida por theta
% Antes se tienen que haber dibujado las muestras
% Credit: Adapted from code by Andrew Ng 

hold on
% adivina el grado de expansion
nf = length(theta);
grado = (sqrt(1+8*nf)-3)/2; 

% Calcula los límites del dibujo
delta = 0.1;
x1 = min(X(:,2)); x2 = max(X(:,2));
y1 = min(X(:,3)); y2 = max(X(:,3));
xmin = x1 - delta*(x2-x1); xmax = x2 + delta*(x2-x1); 
ymin = y1 - delta*(y2-y1); ymax = y2 + delta*(y2-y1); 

if grado <= 1  % Si no hay expansion de atributos

    % Calculate the decision boundary line
    plot_x = [xmin,  xmax];
    plot_y = (-1./theta(3)).*(theta(2).*plot_x + theta(1));

    % Plot, and adjust axes for better viewing
    plot(plot_x, plot_y, 'm-', 'LineWidth', 1)
    axis([xmin xmax ymin ymax]);
else

    % Here is the grid range
    x = linspace(xmin,  xmax, 100);
    y = linspace(ymin,  ymax, 100);
    z = zeros(length(y), length(x));

    % Evaluate z = theta*x over the grid
    for i = 1:length(x)
        for j = 1:length(y)
            z(j,i) = expandir2(x(i), y(j), grado)*theta;
        end
    end
    % Plot z = 0
    contour(x, y, z, [0, 0], 'm-', 'LineWidth', 1)

end

hold off

end

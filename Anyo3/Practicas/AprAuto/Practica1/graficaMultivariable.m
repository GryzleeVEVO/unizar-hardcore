function graficaMultivariable(x1, x2, y, yest, th)
    %GRAFICAMULTIVARIABLE

    % Dibujar los puntos de entrenamiento y su valor estimado
    figure;
    plot3(x1, x2, y, '.r', 'markersize', 20);
    axis vis3d; hold on;
    plot3([x1 x1]', [x2 x2]', [y yest]', '-b');

    % Generar una retícula de np x np puntos para dibujar la superficie
    np = 20;
    ejex1 = linspace(min(x1), max(x1), np)';
    ejex2 = linspace(min(x2), max(x2), np)';
    [x1g, x2g] = meshgrid(ejex1, ejex2);
    x1g = x1g(:); %Los pasa a vectores verticales
    x2g = x2g(:);

    % Calcula la salida estimada para cada punto de la retícula
    Xg = [ones(size(x1g)), x1g, x2g];
    yg = Xg * th;

    % Dibujar la superficie estimada
    surf(ejex1, ejex2, reshape(yg, np, np)); grid on;
    title('Precio de los Pisos')
    zlabel('Euros'); xlabel('Superficie (m^2)'); ylabel('Habitaciones');
end

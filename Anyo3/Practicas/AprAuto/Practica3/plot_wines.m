function plot_wines(X, y, f1, f2)
% Dibuja las tres clases de vinos, en el plano f1, f2

figure
plotsymbol = {'kd', 'ro', 'b*'};
for c=1:3
    plot(X(y==c,f1),X(y==c,f2),plotsymbol{c});
    hold on
end
grid
%title('Clases de vinos')
legend('Clase 1', 'Clase 2', 'Clase 3');
xlabel(sprintf('Atributo %d', f1));
ylabel(sprintf('Atributo %d', f2));

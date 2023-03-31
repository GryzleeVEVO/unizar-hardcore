function graficaProbabilidad(val, pred, esp)
    %GRAFICAPROBABILIDAD Genera una gráfica con la probabilidad de un atribito sea clasifiado de cierta manera

    figure;
    title("Preddición dado Atributo 6 = 0.6");
    grid on;
    plot(val, pred, 'x');
    hold on;
    plot(val, esp, 'x');
    axis padded;

    xlabel("Valor de atributo 10");
    ylabel("Probabilidad");

    yline(0.5);
    legend("Valor predicho", "Valor esperado", "Umbral (clase 1 si >= 0.5)")
end

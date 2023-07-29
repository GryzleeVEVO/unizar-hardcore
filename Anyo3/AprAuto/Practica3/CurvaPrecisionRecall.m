function CurvaPrecisionRecall(pr1, pr2, pr3)
    %CURVAPRECISIONRECALL Dibuja tres curvas precision-recall

    figure;
    grid on;
    plot(pr1(:, 1), pr1(:, 2), '-');
    hold on;
    plot(pr2(:, 1), pr3(:, 2), '-');
    plot(pr3(:, 1), pr3(:, 2), '-');
    axis ([0 1 0 1])
    ylabel('Precision'); xlabel('Recall');
    legend('Regresión básica', 'Regresión con mejor lambda', 'Regresión con lambda = 0');

end

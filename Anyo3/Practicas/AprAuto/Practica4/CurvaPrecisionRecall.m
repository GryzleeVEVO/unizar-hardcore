function CurvaPrecisionRecall(h, y)
    %CURVAPRECISIONRECALL Dibuja curvas precision-recall
   
    figure;
    grid on;
    hold on;

    for clase = 1:10
        pr = [];
        
        y_C = y;
        y_C(y ~= clase) = 0; y_C(y == clase) = 1;
        for umbral = 0:0.1:1
            yPred = h(:,clase) >= umbral;

            pr = cat(1, pr, [recall(yPred, y_C), precision(yPred, y_C)]);
        end
        
        plot(pr(:,1), pr(:,2), '-', 'LineWidth', 1.5);
    end


    axis ([0 1 0 1]);
    ylabel('Precision'); xlabel('Recall');
    legend('1', '2', '3', '4', '5', '6', '7', '8', '9', '0');

end

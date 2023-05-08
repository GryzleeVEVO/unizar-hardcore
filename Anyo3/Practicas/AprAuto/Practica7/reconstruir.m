function reconstruir(im, mu, c, K)

    qIM = zeros(length(c), 3);

    for h = 1:K
        ind = find(c == h);
        qIM(ind, :) = repmat(mu(h, :), length(ind), 1);
    end

    qIM = reshape(qIM, size(im, 1), size(im, 2), size(im, 3));
    figure(2)
    imshow(uint8(qIM));

end

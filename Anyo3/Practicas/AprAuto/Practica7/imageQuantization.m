figure(1)
im = imread('peppers.jpg');
imshow(im)

D = double(reshape(im, size(im, 1) * size(im, 2), 3));
m = size(D, 1);
n = size(D, 2);

%% Obtiene la K adecuada

stats = zeros(16, 2);

for K = 1:16
    time = zeros(10, 1);
    K_best_J = Inf;

    for n = 1:10
        %mu0 = randi(255, K, 3);

        %mu0 = randi(255, K, 3) * rand();

        p = randperm(size(unique(D, 'rows'), 1))';
        mu0 = D(p(1:K, :), :);

        %p = randperm(size(unique(D, 'rows'), 1))';
        %mu0 = D(p(1:K, :), :) * rand();

        tic
        [mu, c] = kmeans(D, mu0);
        time(n) = toc;

        J = distorsion(D, c, mu);

    end

    stats(K, 1) = K_best_J;
    stats(K, 2) = mean(time);
end

figure;

plot(stats(:, 1));
xlabel('K');
ylabel('J');

%% Loro
figure(1)
im = imread('smallparrot.jpg');
imshow(im)

D = double(reshape(im, size(im, 1) * size(im, 2), 3));
m = size(D, 1);
n = size(D, 2);

%% K=6
K = 6;

p = randperm(size(unique(D, 'rows'), 1))';
mu0 = D(p(1:K, :), :);

[mu, c] = kmeans(D, mu0);

reconstruir(im, mu, c, K);

%% K=16

K = 6;

p = randperm(size(unique(D, 'rows'), 1))';
mu0 = D(p(1:K, :), :);

[mu, c] = kmeans(D, mu0);

reconstruir(im, mu, c, K);

%% Pimientos

figure(1)
im = imread('peppers.jpg');
imshow(im)

D = double(reshape(im, size(im, 1) * size(im, 2), 3));
m = size(D, 1);
n = size(D, 2);

%% K=6
K = 6;

p = randperm(size(unique(D, 'rows'), 1))';
mu0 = D(p(1:K, :), :);
[mu, c] = kmeans(D, mu0);

reconstruir(im, mu, c, K);

%% K=16

K = 6;

p = randperm(size(unique(D, 'rows'), 1))';
mu0 = D(p(1:K, :), :);
[mu, c] = kmeans(D, mu0);

reconstruir(im, mu, c, K);

%% Mandril

figure(1)
im = imread('mandril.jpg');
imshow(im)

D = double(reshape(im, size(im, 1) * size(im, 2), 3));
m = size(D, 1);
n = size(D, 2);

%% K=6
K = 6;

p = randperm(size(unique(D, 'rows'), 1))';
mu0 = D(p(1:K, :), :);
[mu, c] = kmeans(D, mu0);

reconstruir(im, mu, c, K);

%% K=16

K = 6;

p = randperm(size(unique(D, 'rows'), 1))';
mu0 = D(p(1:K, :), :);
[mu, c] = kmeans(D, mu0);

reconstruir(im, mu, c, K);

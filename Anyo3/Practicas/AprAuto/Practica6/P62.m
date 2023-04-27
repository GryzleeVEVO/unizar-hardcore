%% Lab 6.2: PCA

clear all
close all

% load images
% images size is 20x20.

load('MNISTdata2.mat');
rand('state', 0);
p = randperm(length(y));
X = X(p, :);
y = y(p);

nrows = 20;
ncols = 20;

nimages = size(X, 1);
nimagestest = size(Xtest, 1);

% This parameter controls the amount of noise injected per pixel
noise_std = 0.3;
X_noise = X + randn(nimages, nrows * ncols) * noise_std;
Xtest_noise = Xtest + randn(nimagestest, nrows * ncols) * noise_std;

%Show the noisy images
for I = 1:40:nimagestest
    subplot(1, 2, 1)
    imshow(reshape(Xtest(I, :), nrows, ncols))
    subplot(1, 2, 2)
    imshow(reshape(Xtest_noise(I, :), nrows, ncols))
    pause(0.1)
end

%% Perform PCA (use your code from previous exercise)
X_norm = X - mean(X_noise);
Xtest_norm = Xtest - mean(Xtest_noise);

[z, k, U] = PCA(X_norm);
z_test = Xtest_norm * U(:, 1:k);

% Show two prinicpal components with labels
figure(100)
clf, hold on
plotwithcolor(z(:,1:2), y);

%% Denoise test images using k components

X_noise_norm = X_noise - mean(X_noise);
Xtest_noise_norm = Xtest_noise - mean(Xtest_noise);

z_noise = X_noise * U(:, 1:k);
z_noise_test = Xtest_noise_norm * U(:, 1:k);

% Reconstruct images to visualize denoising results
X_reconstructed = z_noise * U(:, 1:k)' + mean(X_noise);
Xtest_reconstructed = z_noise_test * U(:, 1:k)' + mean(Xtest_noise);

% Display original and reconstructed images
figure;

for B = 0:40:nimagestest - 10

    for I = 1:10
        subplot(2, 10, I);
        imshow(reshape(X_noise(B + I, :), nrows, ncols));
        title('Original');
        subplot(2, 10, I + 10);
        imshow(reshape(X_reconstructed(B + I, :), nrows, ncols));
        title('Reconstructed');
    end

    pause(0.1)
    clf
end

%% Use the classifier from previous labs (P5 and P61) to classify noise
%  test images both directly and on the PCA space

% Noise

lambda0 = kFoldBayesiano(X, y, 5, 0);

modelo = entrenarGaussianas(X, y, 10, 0, lambda0);

yPred = clasificacionBayesiana(modelo, X_noise);
yPred_Test = clasificacionBayesiana(modelo, Xtest_noise);

accuracyTrain = accuracy(yPred, y)
accuracyTest = accuracy(yPred_Test, ytest)

matrizConfusion(yPred, y);
matrizConfusion(yPred_Test, ytest);

verConfusiones(X, y, yPred);
verConfusiones(X, ytest, yPred_Test);

% PCA

lambda1 = kFoldBayesiano(z, y, 5, 0);

modelo = entrenarGaussianas(z, y, 10, 0, lambda1);

yPred = clasificacionBayesiana(modelo, z_noise);
yPred_Test = clasificacionBayesiana(modelo, z_noise_test);

accuracyTrain = accuracy(yPred, y)
accuracyTest = accuracy(yPred_Test, ytest)

matrizConfusion(yPred, y);
matrizConfusion(yPred_Test, ytest);

verConfusiones(X, y, yPred);
verConfusiones(X, ytest, yPred_Test);

%% Optional You can try and analyse:
% 1- Propose a strategy to select the # of components (hint1)
% 2- Training with denoised train images. Remember not to use test images
% during training.

[z_noise, k, U] = PCA(X_noise, 153);
z_noise_test = Xtest_noise_norm * U(:, 1:k);

% Reconstruct images to visualize denoising results
X_reconstructed = z_noise * U(:, 1:k)' + mean(X_noise);
Xtest_reconstructed = z_noise_test * U(:, 1:k)' + mean(Xtest_noise);

for B = 0:40:nimagestest - 10

    for I = 1:10
        subplot(2, 10, I);
        imshow(reshape(X_noise(B + I, :), nrows, ncols));
        title('Original');
        subplot(2, 10, I + 10);
        imshow(reshape(X_reconstructed(B + I, :), nrows, ncols));
        title('Reconstructed');
    end

    pause(0.1)
    clf
end

% Denoised

lambda2 = kFoldBayesiano(X_reconstructed, y, 5, 0);

modelo = entrenarGaussianas(X_reconstructed, y, 10, 0, lambda2);

yPred = clasificacionBayesiana(modelo, X_noise);
yPred_Test = clasificacionBayesiana(modelo, Xtest_noise);

accuracyTrain = accuracy(yPred, y)
accuracyTest = accuracy(yPred_Test, ytest)

matrizConfusion(yPred, y);
matrizConfusion(yPred_Test, ytest);

verConfusiones(X, y, yPred);
verConfusiones(X, ytest, yPred_Test);

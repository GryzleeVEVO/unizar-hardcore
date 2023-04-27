%% Lab 6.1: PCA

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

%Show the images
for I=1:40:nimages
    imshow(reshape(X(I,:),nrows,ncols))
    pause(0.1)
end

%% Perform PCA following the instructions of the lab
X_norm = X - mean(X);

[z, k, U] = PCA(X_norm);
zTest = Xtest * U(:, 1:k);

% Show two prinicpal components with labels
figure(100)
clf, hold on
plotwithcolor(z(:,1:2), y);

%% Use the classifier from previous labs on the projected space

lambda = kFoldBayesiano(z, y, 5, 0);

modelo = entrenarGaussianas(z, y, 10, 0, lambda);

yPred = clasificacionBayesiana(modelo, z);
yPred_Test = clasificacionBayesiana(modelo, zTest);

accuracyTrain = accuracy(yPred, y)
accuracyTest = accuracy(yPred_Test, ytest)

matrizConfusion(yPred, y);
matrizConfusion(yPred_Test, ytest);

verConfusiones(X, y, yPred);
verConfusiones(X, ytest, yPred_Test);

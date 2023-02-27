% Aprendizaje Automático
% Práctica 1: Regresión
% Autor: Dorian Boleslaw Wozniak (817570@unizar.es)

close all;
%% Carga de datos
datosEntrenamiento = load('PisosTrain.txt');
datosValidacion = load('PisosTest.txt');

superficie = datosEntrenamiento(:, 1);
habitaciones = datosEntrenamiento(:, 2);
precio = datosEntrenamiento(:, 3);
N = length(precio);

superficieTest = datosValidacion(:, 1);
habitacionesTest = datosValidacion(:, 2);
precioTest = datosValidacion(:, 3);
NTest = length(precioTest);

%% Pruebas
run("P1MonoNormal.m");

run("P1MultiNormal.m");

run("P1MonoErrorCuad.m");

run("P1MultiErrorCuad.m");

run("P1MultiCosteHuber.m");

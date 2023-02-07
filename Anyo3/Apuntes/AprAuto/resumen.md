---
mainfont: Liberation Serif
monofont: JetBrains Mono

title: Aprendizaje automático
---

# REGRESIÓN

Nomenclatura                 Expresión                                                  
---------------------------  ---------------------------------------------------------- 
Muestra de entrenamiento     $D = \{(x^i,y^i)\}^N_{i=1}$                                
Variables de entrada         $X = (x_0,...x_D)^T$                                       
Variable de salida/objetivo  $y$                                                        
Parámetros/pesos             $\theta=(\theta _0,...,\theta _D)^T$ o $W=(w_0,...,w_D)^T$ 
Hipótesis                    $h_{\theta}=\theta^TX=\sum^D_{i=0}\theta _ix_i$            


## Regresión monovariable y multivariable

**Regresión**: Predicción del valor de la salida a partir de las entradas

Regresión      Expresión                                                               
-------------  ----------------------------------------------------------------------- 
Monovariable   $\hat{y}=h_\theta(x)=\theta _0+\theta _1x$                              
Multivariable  $\hat{y}=h_\theta(x)=\theta _0+\theta _1x_1+\theta _2x_2+...=\theta^TX$ 

**Función de coste**: Cuanto se desvía la predicción de la realidad.

Función de coste                  Expresión                                                    
--------------------------------  -------------------------------------------------------------
Suma de errores cuadráticos       $J(\theta)=\frac{1}{2}\sum^N_{i=1}(h_\theta(X^i)-y^i)^2$     
Estimación por mínimos cuadrados  $\hat{\theta}=arg\space \min\limits_{\theta}\space J(\theta)$

## Algoritmo de descenso de gradiente

**Gradiente**: Derivación de la función de coste respecto a los pesos.

- $g(\theta)=\frac{\partial}{\partial\theta}J(\theta)=X^T(X\theta-y)$ (Tras simplificar)

El descenso de gradiente consiste en minimizar el error restándose a los parámetros su gradiente.

- $\theta_{k+1} = \theta _k - \alpha g(\theta _k)$, donde $\alpha$ es un factor de aprendizaje

---

```matlab
function [j, grad, Hess] = CosteL2(theta, X, y)
% Calcula el coste cuadrático, y si se piden, su gradiente y su hessiano
r = (X*theta)-y % Residuo
J = (1/2)*sum(r.^2) % Coste cuadrático
if nargout > 1
    grad = X'*r; % Gradiente -> Multiplica la entrada traspuesta por el residuo
end
if nargout > 2
    Hess = X'*X; % Hessiano
end
```

## Ecuación normal

## Convergencia y normalizado de atributos

## Regresión polinómica

## Regresión robusta

## Métricas de error

# REGULARIZACIÓN Y SELECCIÓN DE MODELOS

# REGRESIÓN LOGÍSTICA

# REDUCCIÓN DE LA DIMENSIÓN

# AGRUPAMIENTO

# RECOMENDADORES

# PROCESOS GAUSSIANOS

# ¿¿¿¿PROBLEMAS????
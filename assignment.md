# Assignment

## 1. What is the maximum height of a projectile launched upward with an initial velocity of 50 m/s?

Given:

```
y_0 = 1000 m (from provided code)
v_{0y} = 50 m/s
a_y = -g = -9.8 m/s^2
```

Recover time `t`:

```
v_y = v_{0y} + a_y*t
v_y = 0 m/s at apex
t = -v_{0y}/a_y
  = -(50 m/s)/(-9.8 m/s^2)
t = 5.1020 s
```

Recover `y`:

```
y = y_0 + v_{0y}*t + (1/2)*a_y*t^2
  = (1000 m) + (50 m/s)*(5.1020 s) + (1/2)*(-9.8 m/s^2)*(5.1020 s)^2
  = 1127.5510 m
```

Alternatively:

```
v_y^2 = v_{0y}^2 + 2*a_y*deltay
v_y = 0 m/s at apex
deltay = -v_{0y}^2/(2*a_y)
       = -(50 m/s)^2/(2*(-9.8 m/s^2))
       = 127.5510 m
y = y_0 + deltay
  = (1000 m) + (127.5510 m)
  = 1127.5510 m
```

The maximum height reached is `y_{max, no drag} = 1127.5510 m`.

## 2. How long does it take to reach maximum height?

From part 1, `t = 5.1020 s`.

## 3. How long does it take to hit the ground? (use tvec in the command terminal)

```
y = y_0 + v_{0y}*t + (1/2)*a_y*t^2
y = 0 m
0 = y_0 + v_{0y}*t + (1/2)*a_y*t^2
```

Use the quadratic formula, where:

```
a = (1/2)*a_y = (1/2)*(-9.8 m/s^2) = -4.9 m/s^2
b = v_{0y} = 50 m/s
c = y_0 = 1000 m
```

```
t = (-b +- sqrt(b^2 - 4ac)) / (2a)
t = ((-50 m/s) +- sqrt((50 m/s)^2 - 4*(-4.9 m/s^2)*(1000 m))) / (2*(-4.9 m/s^2))
t = -10.0674 s, 20.2715 s
```

Assume `t` is non-negative: `t = 20.2715 s`.

## 4. Modify the code I provided to include the effect of drag force

- Relevant source: [./src/simulation.cpp](./src/simulation.cpp)
- Hosted program: https://juniperspring.org/projects/sdl-drag/

## 5. What is the maximum height of a projectile launched upward with an initial velocity of 50 m/s and `C/m = 5 × 10^(-4)`?

Given:

```
y_0 = 1000 m
v_{0y} = 50 m/s
C/m = 5*10^(-4) [kg/m]/[kg] = [1/m]
```

Choose `C = 0.025 kg/m` (arbitrary so long as `C > 0`). Then:

```
m = C / (5*10^(-4) 1/m)
  = (0.025 kg/m) / (5*10^(-4) 1/m)
  = 50 kg
```

According to the simulation, the maximum height reached is `y_{max, drag} = 1120.0283 m`.

This gives a delta:

```
y_{max, drag} - y_{max, no drag}
= (1120.0283 m) - (1127.5510 m)
= -7.5227 m
```

The object experiencing drag does not travel as high.

## 6. How long does it take to reach this maximum height?

According to the simulation, `t = 4.9000 s`. We note that our `dt` is `0.01 s`, so `t = 4.90 s`.

This gives a delta:

```
t_{drag} - t_{no drag}
= (4.90 s) - (5.1020 s)
= -0.20 s
```

The object experiencing drag reaches its maximum height in less time.

## 7. How long does it take to hit the ground?

According to the simulation, `t = 21.4500 s`.

## 8. Modify your code with drag to work in two dimensions

- Relevant source: [./src/simulation.cpp](./src/simulation.cpp)
- Hosted program: https://juniperspring.org/projects/sdl-drag/

Our implementation treats all simulations as three dimensional; simulations of trajectories in one and two dimensions are modeled as special cases of trajectories in three dimensions.

## 9. What is the maximum height of a projectile launched upward with an initial velocity `v_0 = 50 m/s i + 50 m/s j` and `C/m = 5 × 10^(-4)`?

According to the simulation, the maximum height reached is `y_{max, drag} = 1115.6448 m`.

## 10. How long does it take to reach this maximum height?

According to the simulation, `t = 4.7600 s`.

## 11. What is the horizontal displacement when the projectile reaches maximum height?

According to the simulation, `x = 222.1294 m`.

## 12. How long does it take to hit the ground?

According to the simulation, `t = 21.4900 s`.

## 13. What is the range of the projectile?

Let range be denoted by `r`, in meters. According to the simulation, `r = 792.7075 m`.

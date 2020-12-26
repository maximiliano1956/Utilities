#!/bin/env wolframscript
#
(* Triple integral approximation by riemann sums *)

(* INPUT:  fn function of x,y,z to approximate triple integral *)
(*         (ax,bx) range of X variable (ax<bx) (rangex) *)
(*         (ay,by) range of Y variable (ay<by) (rangey) *) 
(*         (az,bz) range of Z variable (az<bz) (rangez) *)
(*         n defines the approximation of dx*dy*dz as rangex*rangey*rangez/2^n DEFAULT: n=9 *)
(*         v verbose (True or False) DEFAULT: v=False *)

(* OUTPUT: Triple integral approximation by riemann sums *)

TripleRiemannSum[fn_, ax_, bx_, ay_, by_, az_, bz_, n_ : 9, v_ : False] :=
 (SeedRandom[];
  rangex = bx - ax;
  rangey = by - ay;
  rangez = bz - az;
  srange = Abs[rangex] + Abs[rangey] + Abs[rangez];
  nx = Ceiling[(n*Abs[rangex])/srange];
  ny = Ceiling[(n*Abs[rangey])/srange];
  nz = Ceiling[(n*Abs[rangez])/srange];
  dx = N[rangex/2^nx];
  dy = N[rangey/2^ny];
  dz = N[rangez/2^nz];
  dxdydz = dx dy dz;
  If[v, Print["rangex=", rangex, "\trangey=", rangey, "\trangez=", 
    rangez]];
  If[v, Print["nx=", nx, "\tny=", ny, "\tnz=", nz]];
  If[v, Print["dx=", dx, "\tdy=", dy, "\tdz=", dz]];
  Somma = 0;
  For[kx = 0; x = ax, kx < 2^nx, kx++,
   For[ky = 0; y = ay, ky < 2^ny, ky++,
    For[kz = 0; z = az, kz < 2^nz, kz++,
     Somma += 
      fn[x + dx RandomReal[], y + dy RandomReal[], 
       z + dz RandomReal[]];
     If[v, 
      Print["x=", kx, "\ty=", ky, "\tz=", kz, "\tSomma=", 
       Somma dxdydz]];
     z += dz
     ]; y += dy
    ]; x += dx
   ];
  Somma *= dxdydz;
  Return[N[Somma]])

(* Double integral approximation by riemann sums *)

(* INPUT:  fn function of x,y to approximate double integral *)
(*         (ax,bx) range of X variable (ax<bx) (rangex) *)
(*         (ay,by) range of Y variable (ay<by) (rangey) *) 
(*         n defines the approximation of dx*dy as rangex*rangey/2^n DEFAULT: n=9 *)
(*         v verbose (True or False) DEFAULT: v=False *)

(* OUTPUT: Double integral approximation by riemann sums *)

DoubleRiemannSum[fn_, ax_, bx_, ay_, by_, n_ : 9, v_ : False] :=
 (SeedRandom[];
  rangex = bx - ax;
  rangey = by - ay;
  srange = Abs[rangex] + Abs[rangey];
  nx = Ceiling[(n*Abs[rangex])/srange];
  ny = Ceiling[(n*Abs[rangey])/srange];
  dx = N[rangex/2^nx];
  dy = N[rangey/2^ny];
  dxdy = dx dy;
  If[v, Print["rangex=", rangex, "\trangey=", rangey]];
  If[v, Print["nx=", nx, "\tny=", ny]];
  If[v, Print["dx=", dx, "\tdy=", dy]];
  Somma = 0;
  For[kx = 0; x = ax, kx < 2^nx, kx++,
   For[ky = 0; y = ay, ky < 2^ny, ky++,
    Somma += fn[x + dx RandomReal[], y + dy RandomReal[]];
    If[v, Print["x=", kx, "\ty=", ky, "\tSomma=", Somma dxdy]];
    y += dy
    ]; x += dx
   ];
  Somma *= dxdy;
  Return[N[Somma]])

(* Simple integral approximation by riemann sums *)

(* INPUT:  fn function of x to approximate double integral *)
(*         (a,b) range of X variable (ax<bx) (rangex) *)
(*         n defines the approximation of dx as range/2^n DEFAULT: n=9 *)
(*         v verbose (True or False) DEFAULT: v=False *)

(* OUTPUT: Simple integral approximation by riemann sums *)

SimpleRiemannSum[fn_, a_, b_, n_ : 9, v_ : False] :=
 (SeedRandom[];
  range = b - a;
  dx = N[range/2^n];
  If[v, Print["range=", range]];
  If[v, Print["n=", n]];
  If[v, Print["dx=", dx]];
  Somma = 0;
  For[k = 0; x = a, k < 2^n, k++,
   Somma += fn[x + dx RandomReal[]];
   If[v, Print["x=", k, "\tSomma=", Somma dx]];
   x += dx
   ];
  Somma *= dx;
  Return[N[Somma]])

(* Contour integral approximation by riemann sums *)

(* INPUT:  fn   function of z to approximate double integral *)
(*         fcnt contour parametric function describing the contour *)
(*         (ta,tb) range of t variable in fcnt parametric function *)
(*         n defines the approximation of dt as range/2^n DEFAULT: n=9 *)
(*         v verbose (True or False) DEFAULT: v=False *)

(* OUTPUT: Contour integral approximation by riemann sums *)

ContourRiemannSum[fn_, fcnt_, ta_, tb_, n_ : 9, v_ : False] :=
 (SeedRandom[];
  range = tb - ta;
  dt = N[range/2^n];
  If[v, Print["range=", range]];
  If[v, Print["n=", n]];
  If[v, Print["dt=", dt]];
  Somma = 0;
  t = ta;
  za = fcnt[t + dt RandomReal[]];
  For[k = 0; t = ta, k < 2^n, k++,
   t += dt;
   zb = fcnt[t + dt RandomReal[]];
   Somma += fn[za]*(zb - za);
   za = zb;
   ];
  Return[N[Somma]])

Print["\n\nAPPROXIMATION OF VARIOUS INTEGRALS BY RIEMANN SUM\n\n\n"]

f[x_, y_, z_] := 1/(1 + (x y z)^2)
apprx=TripleRiemannSum[f, 0, 1, 0, 1, 0, 1, 9, False];
nint=NIntegrate[f[x, y, z], {x, 0, 1}, {y, 0, 1}, {z, 0, 1}];
theo=N[Pi^3/32];
Print["Riemann Sums: Triple Integration of f[x,y,z]=1/(1 + (x y z)^2) ="]
Print["\t\t",apprx]
Print["By NIntegrate =\t",nint]
Print["Theoric value =\t",theo]
Print["Error =\t\t",Abs[apprx-nint]]
Print["\n\n"]

g[x_, y_] := 1/(1 + (x y)^2)
apprx=DoubleRiemannSum[g, 0, 1, 0, 1, 9, False];
nint=NIntegrate[g[x, y], {x, 0, 1}, {y, 0, 1}];
Print["Riemann Sums: Double Integration of f[x,y]=1/(1 + (x y)^2) ="]
Print["\t\t",apprx]
Print["By NIntegrate =\t",nint]
Print["Error =\t\t",Abs[apprx-nint]]
Print["\n\n"]

h[x_] := 1/(1 + x ^2)
apprx=SimpleRiemannSum[h, 0, 1, 9, False];
nint=NIntegrate[h[x], {x, 0, 1}];
Print["Riemann Sums: Simple Integration of f[x]=1/(1 + x^2) ="]
Print["\t\t",apprx]
Print["By NIntegrate = ",nint]
Print["Error =\t\t",Abs[apprx-nint]]
Print["\n\n"]

cntr[t_] := 10 (Cos[t] + I Sin[t])
fz[z_] := 1/z
apprx=ContourRiemannSum[fz, cntr, 2 Pi, 0, 10, False];
nint=NIntegrate[fz[cntr[t]] cntr'[t], {t, 2 Pi, 0}];
Print["Contour Riemann Sums: Integration of f[z]=1/z on a circular contour ="]
Print["\t\t",apprx]
Print["By NIntegrate = ",nint]
Print["Error =\t\t",Abs[apprx-nint]]
Print["\n\n"]

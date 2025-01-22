#!/usr/bin/sage

#
# equazione diofantina nelle variabili a,b,c
#
# a/(b+c)+b/(a+c)+c/(a+b)-n
#

#
# definizione di un anello di polinomi
# a coefficienti razionali nei simboli a,b,c
#
R.<a,b,c>=QQ[]

#
# parametro da scegliere
#
n=4

#
# equazione diofantea:
#
# a/(b+c)+b/(a+c)+c/(a+b)=n
#
# bisogna trovare le terne a,b,c
# di interi positivi che soddisfa F=0
#
F=a/(b+c)+b/(a+c)+c/(a+b)-n

#
# espressione simbolica razionalizzata
#
F=F.numerator()

#
# genera curva ellittica in x,y da cubica
#
E=EllipticCurve_from_cubic(F,morphism=False)

#
# genera trasformazione birational (morfismo)
# che trasforma F(a,b,c) in E(x,y)
#
f=EllipticCurve_from_cubic(F,morphism=True)

#
# servira' la trasformazione inversa per passare
# da (x,y) a (a,b,c)
#
finv=f.inverse()

#
# trasforma in Weierstrass short form
# y^2=x^3+ax+b
#
Eq=E.short_weierstrass_model()
print(Eq)

#
# genera equazioni isomorfismo E<-->Eq
#
w=E.isomorphism_to(Eq)

#
# esplicita le 2 componenti della trasformazione
# inversa che converte il punto dalla curva Eq(x,y)
# alla curva E(X,Y)
#
# X=f3(x,y)  Y=f4(x,y)
#
(f3,f4)=w.dual().rational_maps()

#
# generatori razionali della curva
#
GG=Eq.gens()

#
# primo generatore
#
E1=GG[0]

#
# numero tentativi
#
nt=100

#
# init accumulatore somma punti
#
E2=0

#
# trasforma E1 su Eq in P1 su E
#
P1=E(f3(E1.xy()),f4(E1.xy()))

#
# loop nt tentativi
#
for nt in range(1,nt+1):
#
# aggiunge P1 ad ogni loop
# ottenendo multipli successivi di P1
#
    E2=E2+P1
#
# converte il punto da E a F
#
    P=finv(E2)
#
# ricava il denominatore
# (essendo P a coordinate razionali)
#
    Q=P[0].denominator()
    
#
# ottiene i valori interi a,b,c
# essendo F omogenea viene soddisfatta
# da qualunque terna (k*a,k*b,k*c)
#
    x=Q*P[0]
    y=Q*P[1]
    z=Q*P[2]
    
#
# accetta solo le soluzioni positive
#
    if x>0 and y>0 and z>0:
        
#
# stampa numero iterazione e terna a,b,c positiva
#
        print(nt)
        print(x)
        print(y)
        print(z)
        
#
# stampa F(a,b,c) che deve essere zero
# 
        print(F(a=x,b=y,c=z))


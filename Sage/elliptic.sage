#!/usr/bin/sage

def somma(pr,par1,par2):
    return (par1+par2)%pr

def moltip(pr,par1,par2):
    return (par1*par2)%pr

def s_opposite(pr,pa):
    if pa==0:
        return 0
    else:
        return pr-pa

def m_opposite(pr,par):
    return pow(par,-1,pr)

def divis(pr,dvdnd,dvsr):
    return(moltip(pr,dvdnd,m_opposite(pr,dvsr)))

def sottraz(pr,mnd,sttrnd):
    return(somma(pr,mnd,s_opposite(pr,sttrnd)))

def sqr(pr,a):
    for i in range(0,pr):
        if pow(i,2,pr)==a:
            return (i,s_opposite(pr,i))
    return (pr,pr)

def y_point(pr,p1,p2,x0):
    t=pow(x0,3,pr)
    u=moltip(pr,p1,x0)
    s=somma(pr,t,u)
    s=somma(pr,s,p2)
    return sqr(pr,s)

def p_on_curve(pr,p1,p2,x0):
    (y00,y01)=y_point(pr,p1,p2,x0)
    if y00==pr:
        return (False,(0,0))
    else:
        return (True,(y00,y01))

def lmbd(pr,x0,y0,x1,y1):
    if x0==x1 and y0==y1:
        dy=somma(pr,moltip(pr,3,pow(x0,2,pr)),a)
        dx=moltip(pr,2,y0)
    else:
        dx=sottraz(pr,x1,x0)
        dy=sottraz(pr,y1,y0)
    if dx==0:
        return "inf"
    lm=divis(pr,dy,dx)
    return lm

#
# (x0,y0) + (x1,y1)
#
# se P0==P1
# l=(3*x0^2+a)/(2*y0)
# se P0!=P1
# l=(y1-y0)/(x1-x0)
# xr=l^2-x0-x1
# yr=l*(x0-xr)-y0

def psumc(pr,x0,y0,z0,x1,y1,z1):
    if z0==0:
        return (x1,y1,z1)
    if z1==0:
        return (x0,y0,z0)
    l=lmbd(pr,x0,y0,x1,y1)
    if l=="inf":
        return (0,1,0)
    xr=sottraz(pr,pow(l,2,pr),x0)
    xr=sottraz(pr,xr,x1)
    yr=moltip(pr,l,sottraz(pr,x0,xr))
    yr=sottraz(pr,yr,y0)
    return (xr,yr,1)

def psum(pr,x0,y0,z0,x1,y1,z1):
    (xr,yr,zr) = psumc(pr,x0,y0,z0,x1,y1,z1)
    return point(xr,yr,zr)

def point(x,y,z):
    return "("+str(x)+" : "+str(y)+" : "+str(z)+")"

def double_and_add(pr,m,Ix,Iy,Iz):
    (Ox,Oy,Oz)=(0,0,0)
    pv=True
    m = list(bin(m)[2:])
    m.reverse()
    for d in m:
        if pv==True:
            (Dx,Dy,Dz)=(Ix,Iy,Iz)
            pv=False
        else:
            (Dx,Dy,Dz)=psumc(p,Dx,Dy,Dz,Dx,Dy,Dz)
        if d=='1':
            (Ox,Oy,Oz)=psumc(p,Ox,Oy,Oz,Dx,Dy,Dz)
    return (Ox,Oy,Oz)
																																									          
p=2**31-1
#p=31
p=2**13-1
if is_prime(p):
    print("p =",p,"è primo")
else:
    print("p =",p,"NON è primo")
																																								      
a=2
b=7

E=EllipticCurve(GF(p),[a,b])
print(E)
G=E.abelian_group()
print(G)
np_sage=G.order()
print(np_sage)
for g in G:
    print(g)
print(G.ngens())
P=G.gens()[0]
print(P)

print("is abelian:",G.is_abelian())
print("is cyclic:",G.is_cyclic())
print("===============")

np=1
print(np,point(0,1,0))

for x in range(0,p):
    (tf,(y00,y01))=p_on_curve(p,a,b,x)
    if tf==True:
        np=np+1
        print(np,point(x,y00,1))
        if np==2:
            (x1,y1,z1)=(x,y00,1)
        if y01!=y00:
            np=np+1
            print(np,point(x,y01,1))
																																											         
print(np,"points on ec curve")

if is_prime(np):
    print(np,"è primo")
else:
    print(np,"NON è primo")
print("np_sage=",np_sage)

print(100.0*np/p**2,"%")

(px,py,pz)=double_and_add(p,np,x1,y1,z1)
print(point(px,py,pz))

from __future__ import print_function, division
from fenics import *
import collections

def linear_elasticity(mesh, bc, E=1, nu=0.3):

    #Create vector space of mesh
    V = VectorFunctionSpace(mesh, "P", 1)
    
    #Mark boundary subdomians, edges / set to 0
    boundary_parts = MeshFunction("size_t", mesh, mesh.topology().dim() - 1)
    boundary_parts.set_all(0)

    #Get fixed subdomains / use as Dirichlet b.c. / set to 0
    fixed_subdomains = bc.get_fixed()
    fixed_bcs = [DirichletBC(V, Constant((0.0, 0.0)), fixed)
        for fixed in fixed_subdomains]

    #Force subdomains
    forces, Ts = bc.get_forces()
    for i, force in enumerate(forces):
        force.mark(boundary_parts, len(fixed_subdomains) + i)
    B = bc.get_body_force() # Body force per unit volume

    #Variational Formulation
    u = TrialFunction(V)
    d = u.geometric_dimension()
    v = TestFunction(V)
    mu = E / (2. * (1 + nu))
    lmbda = E * nu / ((1. + nu) * (1. - 2. * nu))
    eps = 0.5 * (nabla_grad(v) + nabla_grad(v).T)
    sig = lmbda * nabla_div(u) * Identity(d) + 2 * mu * 0.5 * (nabla_grad(u) + nabla_grad(u).T)
    K = inner(sig, eps) * dx

    #Boundary conditions on the subdomains
    dss = ds(domain=mesh, subdomain_data=boundary_parts)
    f = dot(B, v) * dx + sum([dot(T, v) * dss(len(fixed_subdomains) + i)
        for i, T in enumerate(Ts)])

    #Compute solution
    u = Function(V)
    solve(K == f, u, fixed_bcs)
    return u







from __future__ import print_function, division
from fenics import *

def von_Mises_stress(mesh, displacements):
    """ Computes the stress on a mesh given the displacements. """
    u = displacements
    mu=1
    lmbda=1.25
    d = displacements.geometric_dimension()
    sig = lmbda * nabla_div(u) * Identity(d) + 2 * mu * 0.5 * (nabla_grad(u) + nabla_grad(u).T)
    V = FunctionSpace(mesh, "P", 1)
    s = sig - (1. / 3) * tr(sig) * Identity(d)
    von_Mises = sqrt(3. / 2 * inner(s, s))
    von_Mises = project(von_Mises, V)
    return von_Mises

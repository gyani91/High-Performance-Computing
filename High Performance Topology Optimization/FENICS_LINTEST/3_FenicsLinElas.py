from __future__ import print_function, division
from fenics import *
import mshr

from boundary_conditions import BoundaryConditions
from linear_elasticity import linear_elasticity
from von_Mises_stress import von_Mises_stress


#Boundary Conditions / Forces
class MBBBoundaryConditions(BoundaryConditions):
    def get_fixed(self):
        width, height, tol = self.width, self.height, self.tol

        class BottomBoundary(SubDomain):
            """ Constrain the bottom to not move. """
            def inside(self, x, on_boundary):
                return ((near(x[0], 0.1, 2e1) or
                    near(x[0], width - 0.1, 2e1)) and near(x[1], 0, tol))
        return [BottomBoundary()]

    def get_forces(self):
        width, height, tol = self.width, self.height, self.tol

        class PointLoad(SubDomain):
            """ Add a point load to the top center. """
            def inside(self, x, on_boundary):
                return (near(x[0], width / 2.0, width / 50) and
                    near(x[1], height, tol))
        return [PointLoad()], [Constant((0.0, -10))]

if __name__ == "__main__":
    
    #size of mesh must match simulation
    width, height, tol = 320, 80, 5e-2
    
    #compute boundary conditions
    bc = MBBBoundaryConditions(width, height, tol)

    #Import mesh and scale coordinates to be (0,0) to (width x height).
    mesh = Mesh("bridge.xml")
    coords = mesh.coordinates()
    x = coords[:, 0]
    y = coords[:, 1]
    x[:] = (x - min(x)) / (max(x) - min(x)) * width
    y[:] = (y - min(y)) / (max(y) - min(y)) * height
    
    #compute displacements (linear elasticity) using fenics
    E, nu = 1e2, 0.3
    u = linear_elasticity(mesh, bc, E=E)
    
    #calculate magnitudes
    V = FunctionSpace(mesh, "P", 1)
    u_magnitude = sqrt(dot(u, u))
    u_magnitude = project(u_magnitude, V)
    print("min/max u: {:g}, {:g}".format(
         u_magnitude.vector().get_local().min(),
         u_magnitude.vector().get_local().max()))
        
    #compute von mises stresses on domain
    von_Mises = von_Mises_stress(mesh, u)

    # Save solution to file in VTK format
    File("output/bridge-displacement.pvd") << u
    File("output/bridge-magnitude.pvd") << u_magnitude
    File("output/bridge-von_mises.pvd") << von_Mises

    #output mesh
    File("my_mesh.xml.gz") << mesh











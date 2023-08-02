#include "cctk.h"
#include "cctk_Arguments.h"
#include "cctk_Functions.h"
#include "cctk_Parameters.h"



subroutine qlm_set_coordinates (CCTK_ARGUMENTS, hn)
  use cctk
  implicit none
  DECLARE_CCTK_ARGUMENTS
  DECLARE_CCTK_FUNCTIONS
  DECLARE_CCTK_PARAMETERS
  integer :: hn
  
  CCTK_REAL :: theta, phi
  CCTK_REAL :: sin_theta, cos_theta, sin_phi, cos_phi
  
  integer :: i, j
  
  ! Calculate the coordinates
  do j = 1, qlm_nphi(hn)
     do i = 1, qlm_ntheta(hn)
        theta = qlm_origin_theta(hn) + (i-1)*qlm_delta_theta(hn)
        phi   = qlm_origin_phi(hn)   + (j-1)*qlm_delta_phi(hn)
        
        sin_theta = sin(theta)
        cos_theta = cos(theta)
        sin_phi   = sin(phi)
        cos_phi   = cos(phi)
        
        qlm_x(i,j,hn) = qlm_origin_x(hn) + qlm_shape(i,j,hn) * sin_theta * cos_phi
        qlm_y(i,j,hn) = qlm_origin_y(hn) + qlm_shape(i,j,hn) * sin_theta * sin_phi
        qlm_z(i,j,hn) = qlm_origin_z(hn) + qlm_shape(i,j,hn) * cos_theta
        
        qlm_xp(i,j,hn) = qlm_origin_xp(hn) + qlm_shapep(i,j,hn) * sin_theta * cos_phi
        qlm_yp(i,j,hn) = qlm_origin_yp(hn) + qlm_shapep(i,j,hn) * sin_theta * sin_phi
        qlm_zp(i,j,hn) = qlm_origin_zp(hn) + qlm_shapep(i,j,hn) * cos_theta
        
        qlm_xpp(i,j,hn) = qlm_origin_xpp(hn) + qlm_shapepp(i,j,hn) * sin_theta * cos_phi
        qlm_ypp(i,j,hn) = qlm_origin_ypp(hn) + qlm_shapepp(i,j,hn) * sin_theta * sin_phi
        qlm_zpp(i,j,hn) = qlm_origin_zpp(hn) + qlm_shapepp(i,j,hn) * cos_theta
     end do
  end do
  
end subroutine qlm_set_coordinates
